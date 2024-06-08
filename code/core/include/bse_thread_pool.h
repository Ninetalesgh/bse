#pragma once

#include "bse_thread.h"


namespace bse
{

  namespace thread
  {
    struct Task
    {
      using fn = void( void* );
      fn* function;
      void* parameter;
    };

    enum class alignas(4) TaskState : u32
    {
      INVALID     = 0x0,
      IN_QUEUE    = 0x1,
      IN_PROGRESS = 0x2,
      COMPLETED   = 0x4,
    };

    struct alignas(8) TaskQueue
    {
      struct Entry
      {
        Task task;
        TaskState volatile* state;
      } *tasks;

      void* semaphore;
      s32 volatile pushIndex;
      s32 volatile popIndex;
      s32 volatile pushLockObject;
    };

    struct Pool
    {

      //TODO
      TaskQueue taskQueue;
    };

    thread::ID create( thread::Pool* threadPool, thread::entry_fn* entry, void* parameter );




    #if 0

    bool push_task( TaskQueue* queue, bs::Task const& task, bs::TaskState volatile* out_taskState )
    {
      bool successfullyPushedTask = false;
      {
        LOCK_SCOPE( queue->pushBarrierObject );
        s32 pushIndex = queue->pushIndex;
        s32 nextPushIndex = (pushIndex + 1) % MAX_TASK_COUNT;
        if ( nextPushIndex != queue->popIndex )
        {
          TaskQueueEntry& newEntry = queue->tasks[pushIndex];
          newEntry.task = task;
          if ( out_taskState )
          {
            newEntry.taskState = out_taskState;
            *newEntry.taskState = bs::TaskState::IN_QUEUE;
          }
          else
          {
            newEntry.taskState = nullptr;
          }

          successfullyPushedTask = true;
        }

        s32 check = interlocked_compare_exchange( &queue->pushIndex, nextPushIndex, pushIndex );
        assert( check == pushIndex );
      }

      if ( successfullyPushedTask )
      {
        ReleaseSemaphore( queue->semaphore, 1, 0 );
      }

      return successfullyPushedTask;
    }

    u32 pop_task( TaskQueue* queue )
    {
      //returns 0 if the queue was empty and no task could be fetched
      //returns 1 if the task was fetched and executed
      u32 result = false;
      for ( ;;)
      {
        s32 expectedIndex = queue->popIndex;
        if ( expectedIndex != queue->pushIndex )
        {
          TaskQueueEntry taskQueueEntry = queue->tasks[expectedIndex];
          s32 nextPopIndex = (expectedIndex + 1) % MAX_TASK_COUNT;
          s32 popIndex = interlocked_compare_exchange( &queue->popIndex, nextPopIndex, expectedIndex );
          if ( popIndex == expectedIndex )
          {
            //we're good to go to work
            bs::TaskState volatile* state = taskQueueEntry.taskState;
            if ( state )
            {
              interlocked_compare_exchange( (volatile s32*) state, (s32) bs::TaskState::IN_PROGRESS, (s32) bs::TaskState::IN_QUEUE );
              taskQueueEntry.task.function( taskQueueEntry.task.parameter );
              interlocked_compare_exchange( (volatile s32*) state, (s32) bs::TaskState::COMPLETED, (s32) bs::TaskState::IN_PROGRESS );
            }
            else
            {
              BREAK; //tasks state was null, what do we do then? just call the function anyways?
            }

            result = true;
            break;
          }
          else
          {
            //we try again
            continue;
          }
        }
        else
        {
          //queue is empty
          break;
        }
      }

      return result;
    }

    INLINE bool push_async_task( bs::Task const& task, bs::TaskState volatile* out_taskState )
    {
      return push_task( &global::asyncTaskQueue, task, out_taskState );
    }

    INLINE bool push_synced_task( bs::Task const& task, bs::TaskState volatile* out_taskState )
    {
      return push_task( &global::syncedTaskQueue, task, out_taskState );
    }

    void complete_synced_tasks()
    {
      while ( pop_task( &global::syncedTaskQueue ) ) {}
      win32::for_all_synced_worker_threads( &thread::wait_for_thread_to_pause );
    }

    struct PrmWorkerThreadEntry
    {
      thread::ThreadInfo* threadInfo;
      TaskQueue* taskQueue;
    };
    DWORD WINAPI thread_worker( LPVOID void_parameter )
    {
      PrmWorkerThreadEntry* parameter = (PrmWorkerThreadEntry*) void_parameter;
      thread::ThreadInfo* threadInfo = parameter->threadInfo;
      TaskQueue* queue = parameter->taskQueue;

      thread::write_barrier();
      parameter->threadInfo = nullptr;

      for ( ;;)
      {
        thread::pause_thread_if_requested( threadInfo );

        if ( !pop_task( queue ) )
        {
          threadInfo->isPaused.compare_exchange( 1, 0 );
          WaitForSingleObjectEx( queue->semaphore, INFINITE, FALSE );
          threadInfo->isPaused.compare_exchange( 0, 1 );
        }
      }
    }

    void wait_for_synced_tasks()
    {
    }

    u32 init_worker_threads()
    {
      global::asyncTaskQueue.semaphore = CreateSemaphoreEx( 0, 0/*initialCount*/, global::ASYNC_THREAD_COUNT, 0, 0, SEMAPHORE_ALL_ACCESS );
      global::syncedTaskQueue.semaphore = CreateSemaphoreEx( 0, 0/*initialCount*/, global::SYNCED_THREAD_COUNT, 0, 0, SEMAPHORE_ALL_ACCESS );

      char const* const syncedThreadNames[] =
      {
        "high_priority_worker_0",
        "high_priority_worker_1",
        "high_priority_worker_2",
        "high_priority_worker_3",
        "high_priority_worker_4",
        "high_priority_worker_5",
        "high_priority_worker_6",
        "high_priority_worker_7",
      };
      //static_assert( array_count( syncedThreadNames ) == global::SYNCED_THREAD_COUNT );

      for ( s32 i = 0; i < global::SYNCED_THREAD_COUNT; ++i )
      {
        thread::ThreadInfo& new_worker_thread = global::syncedThreads[i];
        new_worker_thread.name = syncedThreadNames[i];

        win32::PrmWorkerThreadEntry workerThreadParameter = {};
        workerThreadParameter.threadInfo = &new_worker_thread;
        workerThreadParameter.taskQueue = &global::syncedTaskQueue;
        CloseHandle( CreateThread( 0, 0, win32::thread_worker, &workerThreadParameter, 0, (LPDWORD) &workerThreadParameter.threadInfo->id ) );

        //wait until the thread is done launching to overwrite the stack again
        while ( workerThreadParameter.threadInfo != nullptr )
        {
          thread::sleep( 0 );
        }
      }

      char const* const asyncThreadNames[] =
      {
        "low_priority_worker_0",
        "low_priority_worker_1",
      };
      //static_assert( array_count( asyncThreadNames ) == global::ASYNC_THREAD_COUNT );

      for ( s32 i = 0; i < global::ASYNC_THREAD_COUNT; ++i )
      {
        thread::ThreadInfo& new_worker_thread = global::asyncThreads[i];
        new_worker_thread.name = asyncThreadNames[i];

        win32::PrmWorkerThreadEntry workerThreadParameter = {};
        workerThreadParameter.threadInfo = &new_worker_thread;
        workerThreadParameter.taskQueue = &global::asyncTaskQueue;
        CloseHandle( CreateThread( 0, 0, win32::thread_worker, &workerThreadParameter, 0, (LPDWORD) &workerThreadParameter.threadInfo->id ) );

        //wait until the thread is done launching to overwrite the stack again
        while ( workerThreadParameter.threadInfo != nullptr )
        {
          thread::sleep( 0 );
        }
      }

      return 1;
    }

    INLINE void for_all_async_worker_threads( threadfn* fn )
    {
      for ( u32 i = 0; i < global::ASYNC_THREAD_COUNT; ++i )
      {
        if ( global::asyncThreads[i].id ) fn( &global::asyncThreads[i] );
      }
    }

    INLINE void for_all_synced_worker_threads( threadfn* fn )
    {
      for ( u32 i = 0; i < global::SYNCED_THREAD_COUNT; ++i )
      {
        if ( global::syncedThreads[i].id ) fn( &global::syncedThreads[i] );
      }
    }




    #endif 





  };
};
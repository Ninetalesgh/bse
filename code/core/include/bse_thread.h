#pragma once

#pragma once

#include "bse_common.h"

#define LOCK_SCOPE(lockAtomic) thread::LockingObject prevent_locks_in_locks_tmp_object { &lockAtomic }

INLINE s16   interlocked_increment16( s16 volatile* value );
INLINE s16   interlocked_decrement16( s16 volatile* value );
INLINE s32   interlocked_increment( s32 volatile* value );
INLINE s32   interlocked_decrement( s32 volatile* value );
INLINE s32   interlocked_compare_exchange( s32 volatile* value, s32 new_value, s32 comparand );
INLINE void* interlocked_compare_exchange_ptr( void* volatile* value, void* new_value, void* comparand );

namespace bse
{
  struct alignas(4) atomic32
  {
    atomic32() : value( 0 ) {}
    atomic32( s32 value ) : value( value ) {}
    INLINE operator s32 volatile() const { return value; }
    INLINE s32 increment() { return interlocked_increment( &value ); }
    INLINE s32 increment_unsafe() { return ++value; }
    INLINE s32 decrement() { return interlocked_decrement( &value ); }
    INLINE s32 decrement_unsafe() { return --value; }
    INLINE s32 compare_exchange( s32 new_value, s32 comparand ) { return interlocked_compare_exchange( &value, new_value, comparand ); }
  private:
    s32 volatile value;
  };

  namespace thread
  {
    struct Context
    {
      char const* name;

      u32 id;
      atomic32 requestPause;
      atomic32 isPaused;
    };

    struct ThreadInfo
    {
      char const* name;
      u32 id;
      atomic32 requestPause;
      atomic32 isPaused;
    };

    struct LockingObject
    {
      LockingObject( atomic32* lock );
      ~LockingObject();
    private:
      atomic32* lock;
    };

    void sleep( s32 milliseconds );

    u32 is_current_thread( ThreadInfo const* );
    u32 get_current_thread_id();

    void request_pause( ThreadInfo* );
    void request_unpause( ThreadInfo* );
    void wait_for_thread_to_pause( ThreadInfo* );
    void pause_thread_if_requested( ThreadInfo* );
    void pause_thread_if_requested( ThreadInfo*, s32 millisecondsSleepPerPoll );

    void write_barrier();
  };
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////inl//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

namespace bse
{
  namespace thread
  {
    INLINE LockingObject::LockingObject( atomic32* lock ) : lock( lock )
    {
      while ( lock->compare_exchange( 1, 0 ) )
      {
        thread::sleep( 0 );
      }
    }

    INLINE LockingObject::~LockingObject()
    {
      assert( *lock );
      lock->compare_exchange( 0, 1 );
    }

    INLINE void request_pause( ThreadInfo* threadInfo )
    {
      threadInfo->requestPause.compare_exchange( 1, 0 );
    }

    INLINE void request_unpause( ThreadInfo* threadInfo )
    {
      threadInfo->requestPause.compare_exchange( 0, 1 );
    }

    INLINE void wait_for_thread_to_pause( ThreadInfo* threadInfo )
    {
      while ( !threadInfo->isPaused )
      {
        thread::sleep( 0 );
      }
    }

    void pause_thread_if_requested( ThreadInfo* threadInfo, s32 millisecondsSleepPerPoll )
    {
      if ( threadInfo->requestPause )
      {
        threadInfo->isPaused.compare_exchange( 1, 0 );

        while ( threadInfo->requestPause )
        {
          thread::sleep( millisecondsSleepPerPoll );
        }
        threadInfo->isPaused.compare_exchange( 0, 1 );
      }
    }

    INLINE void pause_thread_if_requested( ThreadInfo* threadInfo )
    {
      pause_thread_if_requested( threadInfo, 0 );
    }
  };
};

#if defined(_WIN32)
#include <xthreads.h>
#include <intrin.h>

INLINE s16   interlocked_increment16( s16 volatile* value ) { return _InterlockedIncrement16( (s16*) value ); }
INLINE s16   interlocked_decrement16( s16 volatile* value ) { return _InterlockedDecrement16( (s16*) value ); }
INLINE s32   interlocked_increment( s32 volatile* value ) { return _InterlockedIncrement( (long*) value ); }
INLINE s32   interlocked_decrement( s32 volatile* value ) { return _InterlockedDecrement( (long*) value ); }
INLINE s32   interlocked_exchange( s32 volatile* target, s32 value ) { return _InterlockedExchange( (long*) target, (long) value ); }
INLINE s32   interlocked_compare_exchange( s32 volatile* value, s32 new_value, s32 comparand ) { return _InterlockedCompareExchange( (long*) value, new_value, comparand ); }
INLINE void* interlocked_compare_exchange_ptr( void* volatile* value, void* new_value, void* comparand ) { return _InterlockedCompareExchangePointer( value, new_value, comparand ); }

namespace bse
{
  namespace thread
  {
    INLINE void sleep( s32 milliseconds )
    {
      xtime timer {};
      s64 nanosecondsTarget = s64( _Xtime_get_ticks() ) * 100LL + s64( milliseconds ) * 1000000LL;
      timer.sec  = nanosecondsTarget / 1000000000LL;
      timer.nsec = nanosecondsTarget % 1000000000LL;
      _Thrd_sleep( &timer );
    }

    INLINE u32 is_current_thread( ThreadInfo const* threadInfo )
    {
      return threadInfo->id == _Thrd_id();
    }

    INLINE u32 get_current_thread_id()
    {
      return _Thrd_id();
    }

    INLINE void write_barrier()
    {
      __faststorefence();
    }
  };
};
#endif

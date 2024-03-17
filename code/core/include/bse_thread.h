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
    INLINE operator s32() const { return value; }
    INLINE s32 increment() { return interlocked_increment( &value ); }
    INLINE s32 increment_unsafe() { value = value + 1; return value; }
    INLINE s32 decrement() { return interlocked_decrement( &value ); }
    INLINE s32 decrement_unsafe() { value = value - 1; return value; }
    INLINE s32 compare_exchange( s32 new_value, s32 comparand ) { return interlocked_compare_exchange( &value, new_value, comparand ); }
  private:
    s32 volatile value;
  };

  namespace thread
  {
    using ID = u32;
    using EntryFunctionReturnType = unsigned long;
    using entry_fn = EntryFunctionReturnType BSEAPI( void* );

    struct Context
    {
      char const* name;

      thread::ID id;
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

    void lock( atomic32& lock );
    void unlock( atomic32& lock );

    INLINE void sleep( s32 milliseconds );

    INLINE bool is_current_thread( thread::Context const* );
    INLINE u32 get_current_thread_id();

    void request_pause( thread::Context* );
    void request_unpause( thread::Context* );
    void wait_for_thread_to_pause( thread::Context* );
    void pause_thread_if_requested( thread::Context* );
    void pause_thread_if_requested( thread::Context*, s32 millisecondsSleepPerPoll );

    INLINE void write_barrier();
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
    void lock_atomic( atomic32& lock ) { while ( lock.compare_exchange( 1, 0 ) ) { thread::sleep( 0 ); } }
    void unlock_atomic( atomic32& lock ) { assert( lock ); lock.compare_exchange( 0, 1 ); }

    INLINE LockingObject::LockingObject( atomic32* lock ) : lock( lock ) { lock_atomic( *lock ); }
    INLINE LockingObject::~LockingObject() { unlock_atomic( *lock ); }

    INLINE void request_pause( thread::Context* threadContext )
    {
      threadContext->requestPause.compare_exchange( 1, 0 );
    }

    INLINE void request_unpause( thread::Context* threadContext )
    {
      threadContext->requestPause.compare_exchange( 0, 1 );
    }

    INLINE void wait_for_thread_to_pause( thread::Context* threadContext )
    {
      while ( !threadContext->isPaused )
      {
        thread::sleep( 0 );
      }
    }

    void pause_thread_if_requested( thread::Context* threadContext, s32 millisecondsSleepPerPoll )
    {
      if ( threadContext->requestPause )
      {
        threadContext->isPaused.compare_exchange( 1, 0 );

        while ( threadContext->requestPause )
        {
          thread::sleep( millisecondsSleepPerPoll );
        }
        threadContext->isPaused.compare_exchange( 0, 1 );
      }
    }

    INLINE void pause_thread_if_requested( thread::Context* threadContext )
    {
      pause_thread_if_requested( threadContext, 0 );
    }
  };
};

#if defined(_WIN32)
#include <xthreads.h>
#include <intrin.h>
#include <thread>

s16   interlocked_increment16( s16 volatile* value ) { return _InterlockedIncrement16( (s16*) value ); }
s16   interlocked_decrement16( s16 volatile* value ) { return _InterlockedDecrement16( (s16*) value ); }
s32   interlocked_increment( s32 volatile* value ) { return _InterlockedIncrement( (long*) value ); }
s32   interlocked_decrement( s32 volatile* value ) { return _InterlockedDecrement( (long*) value ); }
s32   interlocked_exchange( s32 volatile* target, s32 value ) { return _InterlockedExchange( (long*) target, (long) value ); }
s32   interlocked_compare_exchange( s32 volatile* value, s32 new_value, s32 comparand ) { return _InterlockedCompareExchange( (long*) value, new_value, comparand ); }
void* interlocked_compare_exchange_ptr( void* volatile* value, void* new_value, void* comparand ) { return _InterlockedCompareExchangePointer( value, new_value, comparand ); }


namespace bse
{
  namespace thread
  {
    void sleep( s32 milliseconds )
    {
      xtime timer {};
      s64 nanosecondsTarget = s64( _Xtime_get_ticks() ) * 100LL + s64( milliseconds ) * 1000000LL;
      timer.sec  = nanosecondsTarget / 1000000000LL;
      timer.nsec = nanosecondsTarget % 1000000000LL;
      _Thrd_sleep( &timer );
    }

    bool is_current_thread( thread::Context const* threadContext )
    {
      return threadContext->id == _Thrd_id();
    }

    u32 get_current_thread_id()
    {
      return _Thrd_id();
    }

    void write_barrier()
    {
      __faststorefence();
    }
  };
};

#elif defined (__clang__)
#include <threads.h>

s32   interlocked_increment( s32 volatile* value )
{
  return __c11_atomic_fetch_add( (_Atomic(int) *) value, 1, 5 );
}
s32   interlocked_decrement( s32 volatile* value )
{
  return __c11_atomic_fetch_sub( (_Atomic(int) *) value, 1, 5 );
}
s32 interlocked_compare_exchange( s32 volatile* value, s32 new_value, s32 comparand )
{
  return __c11_atomic_compare_exchange_strong( (_Atomic(int) *) (value), &comparand, new_value, 4, 0 );
}

namespace bse
{
  namespace thread
  {
    void sleep( s32 milliseconds )
    {
      timespec remaining {};
      timespec timer {};
      s64 nanosecondsTarget = s64( milliseconds ) * 1000000LL;
      timer.tv_sec  = nanosecondsTarget / 1000000000LL;
      timer.tv_nsec = nanosecondsTarget % 1000000000LL;

      thrd_sleep( &timer, &remaining );
    }

    bool is_current_thread( thread::Context const* threadContext )
    {
      return true;
      return threadContext->id == thread::ID( thrd_current() );
    }

    u32 get_current_thread_id()
    {
      return u32( thrd_current() );
    }

    void write_barrier()
    {
      //TODO
      BREAK;
    }
  };
};



#endif

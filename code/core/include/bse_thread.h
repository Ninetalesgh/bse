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

    struct LockingObject
    {
      LockingObject( atomic32* lock );
      ~LockingObject();
    private:
      atomic32* lock;
    };

    void lock( atomic32& lock );
    void unlock( atomic32& lock );

    void sleep( s32 milliseconds );

    u32 is_current_thread( thread::Context const* );
    u32 get_current_thread_id();

    void request_pause( thread::Context* );
    void request_unpause( thread::Context* );
    void wait_for_thread_to_pause( thread::Context* );
    void pause_thread_if_requested( thread::Context* );
    void pause_thread_if_requested( thread::Context*, s32 millisecondsSleepPerPoll );

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

    INLINE u32 is_current_thread( thread::Context const* threadContext )
    {
      return threadContext->id == _Thrd_id();
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

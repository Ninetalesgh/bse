#include "include/bse_allocator.h"

#include <memory>

namespace bse
{
  namespace memory
  {
    void* allocate( s64 size )
    {
      return allocate( platform->default.allocator, size );
    }
    void free( void* ptr, s64 size )
    {
      free( platform->default.allocator, ptr );
    }
  };


  struct ThreadSafeLinearAllocator
  {
    struct Entry
    {
      char* begin;
      char* end;
    };

    char* buffer;
    Entry* entries;
    Entry* lastEntry;
    atomic32 threadGuard;
  };

  INLINE char* align_pointer_forward( char* ptr, s32 byteAlignment ) { return (char*) (((u64( ptr ) - 1) | (byteAlignment - 1)) + 1); }

  void* allocate_to_zero( ThreadSafeLinearAllocator* allocator, s64 size )
  {
    void* result = allocate( allocator, size );
    memset( result, 0, size );
    return result;
  }
  void* allocate( ThreadSafeLinearAllocator* allocator, s64 size )
  {
    LOCK_SCOPE( allocator->threadGuard ); //This isn't meant to be called often, not necessary to optimize now

    //make sure there's space for entry
    if ( allocator->entries->end + sizeof( ThreadSafeLinearAllocator::Entry ) > (char*) allocator->entries )
    {
      BREAK; //TODO more space
      return nullptr;
    }

    s64 bestSlotSize = s64( ((char*) (allocator->entries - 1)) - allocator->entries->end );
    ThreadSafeLinearAllocator::Entry* bestSlot = allocator->entries;
    ThreadSafeLinearAllocator::Entry* search = allocator->lastEntry;

    //linear find smallest space that fits O(n)
    while ( search > allocator->entries )
    {
      s64 slotSize = search[-1].begin - search[0].end;
      if ( slotSize > size && slotSize < bestSlotSize )
      {
        bestSlotSize = slotSize;
        bestSlot = search;
      }
      --search;
    }

    if ( bestSlotSize < size )
    {
      BREAK; //TODO more space
      return nullptr;
    }

    size_t bytes = (bestSlot - allocator->entries) * sizeof( ThreadSafeLinearAllocator::Entry );
    memmove( allocator->entries - 1, allocator->entries, bytes );
    --allocator->entries;
    --bestSlot;
    bestSlot->begin = bestSlot[1].end;
    bestSlot->end = align_pointer_forward( bestSlot->begin + size, 64 );

    return bestSlot->begin;
  }

  void free( ThreadSafeLinearAllocator* allocator, void* address )
  {
    LOCK_SCOPE( allocator->threadGuard );

    ThreadSafeLinearAllocator::Entry* search = allocator->entries;

    while ( search < allocator->lastEntry )
    {
      if ( search->begin == (char*) address )
      {
        size_t bytes = (search - allocator->entries) * sizeof( ThreadSafeLinearAllocator::Entry );
        memmove( allocator->entries + 1, allocator->entries, bytes );
        ++allocator->entries;
        return;
      }

      ++search;
    }
    BREAK; //TODO address wasn't in this allocator? 
  }

  ThreadSafeLinearAllocator* create_thread_safe_linear_allocator( s64 size )
  {
    char* allocation = (char*) bse::platform->allocate_memory( size );
    ThreadSafeLinearAllocator* result = (ThreadSafeLinearAllocator*) allocation;

    if ( allocation )
    {
      result->buffer = align_pointer_forward( allocation + sizeof( ThreadSafeLinearAllocator ), 64 );

      result->entries = (ThreadSafeLinearAllocator::Entry*) (allocation + size - sizeof( ThreadSafeLinearAllocator::Entry ));
      result->entries[0].end = (char*) result->buffer;
      result->entries[0].begin = nullptr;
      result->lastEntry = result->entries;
      result->threadGuard = atomic32();
    }
    else
    {
      BREAK;
    }

    return result;
  }

  void destroy_thread_safe_linear_allocator( ThreadSafeLinearAllocator* allocator )
  {
    bse::platform->free_memory( allocator );
  }
};

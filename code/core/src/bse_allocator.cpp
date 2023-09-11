#include "core/include/bse_allocator.h"

#include <memory>

namespace bse
{
  namespace memory
  {
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// General ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate( Allocator* allocator, s64 size )
    {
      if ( allocator == nullptr )
      {
        return platform->allocate_virtual_memory( size );
      }

      switch ( allocator->type )
      {
        case Allocator::Type::Arena:
        {
          return allocate( (Arena*) allocator, size );
        }
        case Allocator::Type::Multipool:
        {
          return allocate( (Multipool*) allocator, size );
        }
        default:
        {
          BREAK;
          return nullptr;
        }
      }
    }

    void* reallocate( Allocator* allocator, void* ptr, s64 oldSize, s64 newSize )
    {
      if ( allocator == nullptr )
      {
        void* newPtr = platform->allocate_virtual_memory( newSize );
        memmove( newPtr, ptr, min( oldSize, newSize ) );
        platform->free_virtual_memory( ptr );
        return newPtr;
      }

      switch ( allocator->type )
      {
        case Allocator::Type::Arena:
        {
          return reallocate( (Arena*) allocator, ptr, oldSize, newSize );
        }
        case Allocator::Type::Multipool:
        {
          return reallocate( (Multipool*) allocator, ptr, oldSize, newSize );
        }
        default:
        {
          BREAK;
          return nullptr;
        }
      }
    }

    void free( Allocator* allocator, void* ptr, s64 size )
    {
      if ( allocator == nullptr )
      {
        platform->free_virtual_memory( ptr );
      }

      switch ( allocator->type )
      {
        case Allocator::Type::Arena:
        {
          free( (Arena*) allocator, ptr );
          break;
        }
        case Allocator::Type::Multipool:
        {
          free( (Multipool*) allocator, ptr, size );
          break;
        }
        default:
        {
          BREAK;
        }
      }
    }

    void free( Allocator* allocator, void* ptr )
    {
      if ( allocator == nullptr )
      {
        platform->free_virtual_memory( ptr );
      }

      switch ( allocator->type )
      {
        case Allocator::Type::Arena:
        {
          free( (Arena*) allocator, ptr );
          break;
        }
        case Allocator::Type::Multipool:
        {
          //multipool can't free without size?
          //TODO probably can do
          BREAK;
          break;
        }
        default:
        {
          BREAK;
        }
      }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Arena /////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate( Arena* arena, s64 size )
    {
      constexpr u32 ALIGN = 16;
      //TODO this alignment maybe a bit much?
      u32 alignment = size > ALIGN ? ALIGN : round_up_to_next_power_of_two( u32( size ) );
      char* result = align_pointer_forward( arena->ptr, alignment );
      if ( result + size > arena->endPtr )
      {
        //TODO something here.. just ignore the allocation? this is a crazy scenario for repeatedly flushed big arenas
        BREAK;
        return nullptr;
      }
      arena->ptr = result + size;
      arena->highestCommitPtr = (char*) max( (char*) arena->ptr, arena->highestCommitPtr );
      return result;
    }

    void* reallocate( Arena* arena, void* ptr, s64 oldSize, s64 newSize )
    {
      if ( (char*) ptr + oldSize == arena->ptr )
      {
        arena->ptr += (newSize - oldSize);
        return ptr;
      }
      else
      {
        void* newPtr = allocate( arena, newSize );
        if ( newPtr )
        {
          memcpy( newPtr, ptr, min( newSize, oldSize ) );
        }
        return newPtr;
      }
    }

    void free( Arena* arena, void* ptr ) { arena->ptr = (char*) ptr; }
    void free( Arena* arena, void* ptr, s64 size )
    {
      if ( (char*) ptr + size != arena->ptr )
      {
        BREAK;
      }
      return free( arena, ptr );
    }

    Arena create_arena( s64 size )
    {
      Arena result;

      char* ptr = (char*) platform->allocate_virtual_memory( size );
      result.beginPtr = ptr;
      result.ptr = ptr;
      result.highestCommitPtr = ptr;
      result.endPtr = ptr + size;

      return result;
    }

    void release_arena( Arena* arena )
    {
      if ( (char*) arena != arena->ptr - sizeof( Arena ) )
      {
        platform->free_virtual_memory( arena->ptr );
      }
      else
      {
        //don't try to release arenas that aren't directly hooked to memory pages
        BREAK;
      }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Multipool /////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    MonotonicPool* create_monotonic_pool( Allocator* parent, s32 elementSize, s32 elementCount )
    {
      s64 allocationSize;

      //take page size into consideration here?

     // platform->info.virtualMemoryPageSize;


      char* allocation = (char*) allocate( parent, allocationSize );

      MonotonicPool result;
      result.next = nullptr;
      result.begin = allocation; //?
      result.parent = parent;
      result.nextPool = nullptr;
      result.elementSize = elementSize;
      result.writeIndex = 0;
      result.elementCount = elementCount; //?
      return (MonotonicPool*) allocation;
    }

    void* allocate( MonotonicPool* pool )
    {

      if ( MonotonicPool::Slot* slot = pool->next )
      {
        pool->next = slot->next;
        return slot;
      }
      else if ( pool->writeIndex < pool->elementCount )
      {
        return pool->begin + (pool->writeIndex++ * pool->elementSize);
      }
      else
      {
        if ( !pool->nextPool )
        {
          //TODO allocate pool
        }

        return allocate( pool->nextPool );
      }
    }

    void free( MonotonicPool* pool, void* ptr )
    {
      MonotonicPool::Slot* slot = (MonotonicPool::Slot*) ptr;
      slot->next = pool->next;
      pool->next = slot;
    }

    void free( MonotonicPool* pool, void* ptr, s64 size ) { free( pool, ptr ); }

    void* allocate( Multipool* multipool, s64 size )
    {
      if ( size > multipool->poolSizeMax )
      {
        //TODO return forward to other allocator/general shit
      }
      s64 const poolIndex = (size - 1) / multipool->poolSizeGranularity;

      return allocate( &multipool->pools[poolIndex], size );
    }

    void* reallocate( Multipool* multipool, void* ptr, s64 oldSize, s64 newSize )
    {
      return nullptr;
    }

    void free( Multipool* multipool, void* ptr, s64 size )
    {
      BREAK;
    }

    void free( Multipool* multipool, void* ptr )
    {
      BREAK;
    }




  };
};


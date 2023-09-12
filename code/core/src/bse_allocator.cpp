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
        #if defined(BS_BUILD_DEBUG_DEVELOPMENT)
        s64 fill = size % platform->info.virtualMemoryPageSize;
        if ( fill < 1024 )
        {
          log_warning( "Allocation only uses ", fill, " bytes of virtual page size "
          , platform->info.virtualMemoryPageSize, ". Consider using a dedicated allocator instead of virtual memory." );
        }
        #endif
        return platform->allocate_virtual_memory( size );
      }

      switch ( allocator->type )
      {
        case AllocatorType::Arena:
        {
          return allocate( (Arena*) allocator, size );
        }
        case AllocatorType::MonotonicPool:
        {
          //size irrelevant for monotonic allocators
          return allocate( (MonotonicPool*) allocator );
          break;
        }
        case AllocatorType::Multipool:
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
        void* newPtr = allocate( allocator, newSize );
        memmove( newPtr, ptr, min( oldSize, newSize ) );
        free( allocator, ptr );
        return newPtr;
      }

      switch ( allocator->type )
      {
        case AllocatorType::Arena:
        {
          return reallocate( (Arena*) allocator, ptr, oldSize, newSize );
        }
        case AllocatorType::MonotonicPool:
        {
          BREAK; //monotonic allocators might have useful reallocation?
          break;
        }
        case AllocatorType::Multipool:
        {
          return reallocate( (Multipool*) allocator, ptr, oldSize, newSize );
        }
        default:
        {
          BREAK;
          break;
        }
      }

      return nullptr;
    }

    void free( Allocator* allocator, void* ptr, s64 size )
    {
      if ( allocator == nullptr )
      {
        platform->free_virtual_memory( ptr );
      }

      switch ( allocator->type )
      {
        case AllocatorType::Arena:
        {
          free( (Arena*) allocator, ptr );
          break;
        }
        case AllocatorType::MonotonicPool:
        {
          free( (MonotonicPool*) allocator, ptr );
          break;
        }
        case AllocatorType::Multipool:
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
        case AllocatorType::Arena:
        {
          free( (Arena*) allocator, ptr );
          break;
        }
        case AllocatorType::MonotonicPool:
        {
          free( (MonotonicPool*) allocator, ptr );
          break;
        }
        case AllocatorType::Multipool:
        {
          //multipool can't free without size?
          //TODO probably can do with pointer tricks?
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
      u32 alignment = size > Allocator::ALIGNMENT ? Allocator::ALIGNMENT : round_up_to_next_power_of_two( u32( size ) );
      char* result = align_pointer_forward( arena->ptr, alignment );

      if ( result + size > arena->begin + arena->size )
      {
        if ( enum_contains( arena->policy, AllocatorPolicy::WhenFullAllocateFromParent ) )
        {
          s64 nextSize = enum_contains( arena->policy, AllocatorPolicy::GeometricGrowth ) ? 2 * arena->size : arena->size;
          arena->nextArena = new_arena( arena->parent, nextSize, arena->policy );
        }

        return arena->nextArena ? allocate( arena->nextArena, size ) : nullptr;
      }
      arena->ptr = result + size;
      return result;
    }

    void* reallocate( Arena* arena, void* ptr, s64 oldSize, s64 newSize )
    {
      if ( (char*) ptr + oldSize == arena->ptr )
      {
        if ( (char*) ptr + newSize <= arena->begin + arena->size )
        {
          arena->ptr += (newSize - oldSize);
          return ptr;
        }
      }

      void* newPtr = allocate( arena, newSize );
      if ( newPtr )
      {
        memcpy( newPtr, ptr, min( newSize, oldSize ) );
      }
      return newPtr;
    }

    void free( Arena* arena, void* ptr )
    {
      if ( arena->begin <= ptr && ptr < arena->begin + arena->size )
      {
        arena->ptr = (char*) ptr;
      }
      else if ( arena->nextArena )
      {
        free( arena->nextArena, ptr );
      }
      else
      {
        log_warning( "free called on memory not belonging to this arena." );
        BREAK;
      }
    }

    Arena* new_arena( Allocator* parent, s64 size, AllocatorPolicy const& policy )
    {
      s64 allocationSize = sizeof( Arena ) + size;
      if ( parent == nullptr )
      {
        //round up to page size, since it would be wasted otherwise
        s64 roundUp = platform->info.virtualMemoryPageSize - (allocationSize % platform->info.virtualMemoryPageSize);
        allocationSize += roundUp;
        size += roundUp;
      }

      //Arena struct sits at the beginning of the allocation
      Arena* result = (Arena*) allocate( parent, allocationSize );
      if ( result )
      {
        result->type      = AllocatorType::Arena;
        result->policy    = policy;
        result->parent    = parent;
        result->begin     = (char*) (result + 1);
        result->ptr       = result->begin;
        result->size      = size;
        result->nextArena = nullptr;
      }
      return result;
    }

    void delete_arena( Arena* arena )
    {
      if ( arena->nextArena ) { delete_arena( arena->nextArena ); }
      free( arena->parent, arena );
    }

    void clear_arena( Arena* arena )
    {
      arena->ptr = arena->begin;
      if ( arena->nextArena ) clear_arena( arena->nextArena );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// MonotonicPool /////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate( MonotonicPool* pool )
    {


      if ( MonotonicPool::Slot* slot = pool->next )
      {
        //TODO do the math again here
        pool->next = slot->next;

        if ( !pool->next && (char*) slot < pool->begin + pool->size )
        {
          pool->next = (MonotonicPool::Slot*) (((char*) slot) + pool->granularity);
          pool->next->next = nullptr;
        }
        return slot;
      }
      else if ( enum_contains( pool->policy, AllocatorPolicy::WhenFullAllocateFromParent ) && !pool->nextPool )
      {
        s64 nextSize = enum_contains( pool->policy, AllocatorPolicy::GeometricGrowth ) ? 2 * pool->size : pool->size;
        pool->nextPool = new_monotonic_pool( pool->parent, nextSize, pool->granularity, pool->policy );
      }

      return pool->nextPool ? allocate( pool->nextPool ) : nullptr;
    }

    void free( MonotonicPool* pool, void* ptr )
    {
      if ( pool->begin <= ptr && ptr < pool->begin + pool->size )
      {
        MonotonicPool::Slot* slot = (MonotonicPool::Slot*) ptr;
        slot->next = pool->next;
        pool->next = slot;
      }
      else if ( pool->nextPool )
      {
        free( pool->nextPool, ptr );
      }
      else
      {
        log_warning( "free called on memory not belonging to this monotonic pool." );
        BREAK;
      }
    }

    MonotonicPool* new_monotonic_pool( Allocator* parent, s64 size, s64 granularity, AllocatorPolicy const& policy )
    {
      s64 allocationSize = sizeof( MonotonicPool ) + size;
      if ( parent == nullptr )
      {
        //round up to page size, since it would be wasted otherwise
        s64 roundUp = platform->info.virtualMemoryPageSize - (allocationSize % platform->info.virtualMemoryPageSize);
        allocationSize += roundUp;
        size += roundUp;
      }

      //round down to granularity or to pointer size, 
      //so the monotonic pool sitting at the end of the allocation is at least a little aligned itself 
      s64 roundDown = size % max( granularity, s64( sizeof( char* ) ) );
      size -= roundDown;
      allocationSize -= roundDown;

      //we stick monotonicpools at the end so the allocated blocks are easier to align, 
      //especially when they're big and/or coming from virtual memory
      char* allocation = (char*) allocate( parent, allocationSize );

      if ( u64( allocation ) & 0xffff ) //??
      {
        BREAK;
      }

      MonotonicPool* result = (MonotonicPool*) (allocation + size);
      if ( result )
      {
        result->type        = AllocatorType::MonotonicPool;
        result->policy      = policy;
        result->parent      = parent;
        result->begin       = allocation;
        result->next        = (MonotonicPool::Slot*) result->begin;
        result->nextPool    = nullptr;
        result->size        = size;
        result->granularity = granularity;
      }

      return result;
    }

    void delete_monotonic_pool( MonotonicPool* pool )
    {
      if ( pool->nextPool ) { delete_monotonic_pool( pool->nextPool ); }
      free( pool->parent, pool );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Multipool /////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

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
      void* newPtr = allocate( multipool, newSize );
      if ( newPtr )
      {
        memcpy( newPtr, ptr, min( newSize, oldSize ) );
        free( multipool, ptr, oldSize );
      }
      return newPtr;
    }

    void free( Multipool* multipool, void* ptr, s64 size )
    {
      s64 const poolIndex = (size - 1) / multipool->poolSizeGranularity;
      free( &multipool->pools[poolIndex], ptr );
    }

    MonotonicPool* new_multipool( Allocator* parent, s64 maxPoolSize, s64 poolSizeGranularity, AllocatorPolicy const& policy )
    {
      //TODODODODODODO
      return nullptr;
    }

  };
};


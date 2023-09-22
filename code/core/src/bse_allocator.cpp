#include "core/include/bse_allocator.h"

#include <memory>

namespace bse
{
  namespace memory
  {
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// In-Engine Use /////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate_temporary( s64 size ) { return allocate( platform->allocators.temporary[platform->thisFrame.frameIndex % array_count( platform->allocators.temporary )], size ); }

    void* allocate_main_thread( s64 size ) { return allocate( platform->allocators.mainThread, size ); }
    void* reallocate_main_thread( void* ptr, s64 oldSize, s64 newSize ) { return reallocate( platform->allocators.mainThread, ptr, oldSize, newSize ); }
    void free_main_thread( void* ptr, s64 size ) { return free( platform->allocators.mainThread, ptr, size ); }

    void* allocate_thread_safe( s64 size ) { return allocate( platform->allocators.threadSafe, size ); }
    void* reallocate_thread_safe( void* ptr, s64 oldSize, s64 newSize ) { return reallocate( platform->allocators.threadSafe, ptr, oldSize, newSize ); }
    void free_thread_safe( void* ptr, s64 size ) { return free( platform->allocators.threadSafe, ptr, size ); }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// General ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate( Allocator* allocator, s64 size )
    {
      if ( allocator == nullptr )
      {
        return allocate_thread_safe( size );
      }

      switch ( allocator->type )
      {
        case AllocatorType::Arena:
        {
          return allocate( (Arena*) allocator, size );
        }
        case AllocatorType::Multipool:
        {
          return allocate( (Multipool*) allocator, size );
          break;
        }
        case AllocatorType::MonotonicPool:
        {
          return allocate( (MonotonicPool*) allocator );
          break;
        }
        case AllocatorType::VirtualMemory:
        {
          return allocate_virtual_memory( size );
          break;
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
        return reallocate_thread_safe( ptr, oldSize, newSize );
      }

      switch ( allocator->type )
      {
        case AllocatorType::Arena:
        {
          return reallocate( (Arena*) allocator, ptr, oldSize, newSize );
        }
        case AllocatorType::Multipool:
        {
          return reallocate( (Multipool*) allocator, ptr, oldSize, newSize );
        }
        case AllocatorType::VirtualMemory:
        {
          return reallocate_virtual_memory( ptr, oldSize, newSize );
          break;
        }
        case AllocatorType::MonotonicPool:
        {
          BREAK; //monotonic allocators might have useful reallocation?
          break;
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
        free_thread_safe( ptr, size );
      }
      else
      {
        switch ( allocator->type )
        {
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
          case AllocatorType::Arena:
          {
            free( (Arena*) allocator, ptr );
            break;
          }
          case AllocatorType::VirtualMemory:
          {
            free_virtual_memory( ptr );
            break;
          }
          default:
          {
            BREAK;
          }
        }
      }
    }

    void free( Allocator* allocator, void* ptr )
    {
      if ( allocator == nullptr )
      {
        BREAK; // general allocator needs size to free
      }
      else
      {
        switch ( allocator->type )
        {
          case AllocatorType::MonotonicPool:
          {
            free( (MonotonicPool*) allocator, ptr );
            break;
          }
          case AllocatorType::Arena:
          {
            free( (Arena*) allocator, ptr );
            break;
          }
          case AllocatorType::VirtualMemory:
          {
            free_virtual_memory( ptr );
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
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Virtual Memory Page ///////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate_virtual_memory( s64 size )
    {
      #if defined(BS_BUILD_DEBUG_DEVELOPMENT)
      s64 fill = size % platform->info.virtualMemoryPageSize;
      if ( size < platform->info.virtualMemoryPageSize * 2 && fill < 1024 )
      {
        log_warning( "Allocation of ", size, " bytes only uses ", fill, " bytes of virtual page size ", platform->info.virtualMemoryPageSize, "." );
      }
      #endif
      return platform->allocate_virtual_memory( nullptr, size );
    }

    void* reallocate_virtual_memory( void* ptr, s64 oldSize, s64 newSize )
    {
      void* newPtr = allocate_virtual_memory( newSize );
      if ( oldSize )
      {
        memmove( newPtr, ptr, min( oldSize, newSize ) );
        free_virtual_memory( ptr );
      }
      return newPtr;
    }

    void free_virtual_memory( void* ptr )
    {
      platform->free_virtual_memory( ptr );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Arena /////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate( Arena* arena, s64 size )
    {
      bool const locked = flags_contain( arena->policy, AllocatorPolicyFlags::ThreadSafe );
      if ( locked ) thread::lock_atomic( arena->allocatorLock );

      u32 alignment = size > Allocator::ALIGNMENT ? Allocator::ALIGNMENT : round_up_to_next_power_of_two( u32( size ) );
      char* result = align_pointer_forward( arena->ptr, alignment );

      if ( result + size > arena->begin + arena->size )
      {
        if ( flags_contain( arena->policy, AllocatorPolicyFlags::AllowGrowth ) )
        {
          s64 nextSize = flags_contain( arena->policy, AllocatorPolicyFlags::GeometricGrowth ) ? 2 * arena->size : arena->size;
          arena->nextArena = new_arena( arena->parent, nextSize, arena->policy );
        }
        else
        {
          BREAK;
        }

        return arena->nextArena ? allocate( arena->nextArena, size ) : nullptr;
      }
      arena->ptr = result + size;

      if ( locked ) thread::unlock_atomic( arena->allocatorLock );
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
      if ( newPtr && oldSize )
      {
        memcpy( newPtr, ptr, min( newSize, oldSize ) );
      }
      return newPtr;
    }

    void free( Arena* arena, void* ptr )
    {
      bool const locked = flags_contain( arena->policy, AllocatorPolicyFlags::ThreadSafe );
      if ( locked ) thread::lock_atomic( arena->allocatorLock );

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

      if ( locked ) thread::unlock_atomic( arena->allocatorLock );
    }

    Arena* new_arena( Allocator* parent, s64 size, AllocatorPolicyFlags const& policy )
    {
      s64 allocationSize = sizeof( Arena ) + size;
      if ( !parent || parent->type == AllocatorType::VirtualMemory )
      {
        parent = &platform->allocators.virtualMemory;
        //round up to page size, since it would be wasted otherwise
        s64 roundUp = difference_to_multiple_of( allocationSize, platform->info.virtualMemoryPageSize );
        allocationSize += roundUp;
        size += roundUp;
      }

      //Arena struct sits at the beginning of the allocation
      Arena* result = (Arena*) allocate( parent, allocationSize );
      if ( result )
      {
        result->allocatorLock = atomic32 {};
        result->type      = AllocatorType::Arena;
        result->policy    = policy;
        result->parent    = parent;
        result->begin     = (char*) (result + 1);
        result->ptr       = result->begin;
        result->size      = size;
        result->nextArena = nullptr;
      }
      else
      {
        BREAK;
      }
      return result;
    }

    Arena* new_arena( Allocator* parent, void* existingBuffer, s64 bufferSize, AllocatorPolicyFlags const& policy )
    {
      //Arena struct sits at the beginning of the allocation
      Arena* result = (Arena*) existingBuffer;
      if ( result )
      {
        result->allocatorLock = atomic32 {};
        result->type      = AllocatorType::Arena;
        result->policy    = policy;
        result->parent    = parent;
        result->begin     = (char*) (result + 1);
        result->ptr       = result->begin;
        result->size      = bufferSize - sizeof( Arena );
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

    void init_arena( Arena& arena, Allocator* parent, s64 bufferSize, AllocatorPolicyFlags const& policy )
    {
      if ( !parent || parent->type == AllocatorType::VirtualMemory )
      {
        parent = &platform->allocators.virtualMemory;
        //round up to page bufferSize, since it would be wasted otherwise
        s64 roundUp = difference_to_multiple_of( bufferSize, platform->info.virtualMemoryPageSize );
        bufferSize += roundUp;
      }

      arena.allocatorLock = atomic32 {};
      arena.type      = AllocatorType::Arena;
      arena.policy    = policy;
      arena.parent    = parent;
      arena.begin     = (char*) allocate( parent, bufferSize );
      arena.ptr       = arena.begin;
      arena.size      = bufferSize;
      arena.nextArena = nullptr;
    }

    void deinit_arena( Arena& arena )
    {
      if ( arena.nextArena ) { deinit_arena( *arena.nextArena ); }
      free( arena.parent, arena.begin );
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// MonotonicPool /////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate( MonotonicPool* pool )
    {
      bool const locked = flags_contain( pool->policy, AllocatorPolicyFlags::ThreadSafe );
      if ( locked ) thread::lock_atomic( pool->allocatorLock );

      if ( MonotonicPool::Slot* slot = pool->next )
      {
        pool->next = slot->next;

        if ( !pool->next && (char*) slot < pool->begin + pool->size )
        {
          pool->next = (MonotonicPool::Slot*) (((char*) slot) + pool->granularity);
          pool->next->next = nullptr;
        }

        if ( locked ) thread::unlock_atomic( pool->allocatorLock );
        return slot;
      }
      else if ( !pool->nextPool && flags_contain( pool->policy, AllocatorPolicyFlags::AllowGrowth ) )
      {
        s64 nextSize = flags_contain( pool->policy, AllocatorPolicyFlags::GeometricGrowth ) ? 2 * pool->size : pool->size;
        pool->nextPool = new_monotonic_pool( pool->parent, nextSize, pool->granularity, pool->policy );
      }

      if ( locked ) thread::unlock_atomic( pool->allocatorLock );
      return pool->nextPool ? allocate( pool->nextPool ) : nullptr;
    }

    void free( MonotonicPool* pool, void* ptr )
    {
      bool const locked = flags_contain( pool->policy, AllocatorPolicyFlags::ThreadSafe );
      if ( locked ) thread::lock_atomic( pool->allocatorLock );

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

      if ( locked ) thread::unlock_atomic( pool->allocatorLock );
    }

    MonotonicPool* new_monotonic_pool( Allocator* parent, s64 size, s64 granularity, AllocatorPolicyFlags const& policy )
    {
      //64 byte aligned should suffice for any crazy business
      s64 const poolSize = round_up_to_multiple_of( sizeof( MonotonicPool ), 64 );

      s64 allocationSize = poolSize + size;
      if ( !parent || parent->type == AllocatorType::VirtualMemory )
      {
        parent = &platform->allocators.virtualMemory;
        //round up to page size, since it would be wasted otherwise
        s64 roundUp = difference_to_multiple_of( allocationSize, platform->info.virtualMemoryPageSize );
        allocationSize += roundUp;
        size += roundUp;
      }

      //round down to granularity so this won't be taking up unused space
      s64 roundDown = size % granularity;
      size -= roundDown;
      allocationSize -= roundDown;

      MonotonicPool* result = (MonotonicPool*) allocate( parent, allocationSize );
      if ( result )
      {
        result->allocatorLock = atomic32 {};
        result->type        = AllocatorType::MonotonicPool;
        result->policy      = policy;
        result->parent      = parent;
        result->begin       = ((char*) (result)) + poolSize;
        result->next        = (MonotonicPool::Slot*) result->begin;
        result->next->next  = nullptr;
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
        return allocate( multipool->parent, size );
      }
      else
      {
        s64 const poolIndex = (size - 1) / multipool->poolSizeGranularity;
        MonotonicPool*& pool = multipool->pools[poolIndex];
        if ( !pool && flags_contain( multipool->policy, AllocatorPolicyFlags::AllowGrowth ) )
        {
          bool const locked = flags_contain( multipool->policy, AllocatorPolicyFlags::ThreadSafe );
          if ( locked ) thread::lock_atomic( multipool->allocatorLock );

          if ( !pool )
          {
            s64 const poolGranularity = min( multipool->poolSizeMax, (poolIndex + 1) * multipool->poolSizeGranularity );
            s64 const poolSize = multipool->defaultElementCount * poolGranularity;
            pool = new_monotonic_pool( multipool->parent, poolSize, poolGranularity, multipool->policy );
          }

          if ( locked ) thread::unlock_atomic( multipool->allocatorLock );
        }

        return pool ? allocate( pool ) : nullptr;
      }
    }

    void* reallocate( Multipool* multipool, void* ptr, s64 oldSize, s64 newSize )
    {
      void* newPtr = allocate( multipool, newSize );
      if ( newPtr && oldSize )
      {
        memcpy( newPtr, ptr, min( newSize, oldSize ) );
        free( multipool, ptr, oldSize );
      }
      return newPtr;
    }

    void free( Multipool* multipool, void* ptr, s64 size )
    {
      if ( size > multipool->poolSizeMax )
      {
        return free( multipool->parent, ptr, size );
      }
      else
      {
        s64 const poolIndex = (size - 1) / multipool->poolSizeGranularity;
        free( multipool->pools[poolIndex], ptr );
      }
    }

    Multipool* new_multipool( Allocator* parent, s64 maxPoolSize, s64 poolSizeGranularity, AllocatorPolicyFlags const& policy )
    {
      s64 const poolCount = 1 + ((maxPoolSize - 1) / poolSizeGranularity);
      s64 const allocationSize = sizeof( Multipool ) + poolCount * sizeofptr;

      s64 fillerAllocationSize = 0;
      s64 fillerPoolSize = 0;
      if ( !parent || parent->type == AllocatorType::VirtualMemory )
      {
        parent = &platform->allocators.virtualMemory;
        //round up to page size, since it would be wasted otherwise
        fillerAllocationSize = difference_to_multiple_of( allocationSize, platform->info.virtualMemoryPageSize );
        fillerPoolSize = fillerAllocationSize - sizeof( MonotonicPool );
        s64 roundDown = fillerPoolSize % max( poolSizeGranularity, s64( sizeofptr ) );

        fillerAllocationSize -= roundDown;
        fillerPoolSize -= roundDown;

        //if the filler wouldn't even be able to fit one, forget about it
        if ( fillerPoolSize / poolSizeGranularity <= 0 )
        {
          fillerAllocationSize = 0;
        }
      }

      Multipool* result = (Multipool*) allocate( parent, allocationSize + fillerAllocationSize );

      if ( result )
      {
        result->allocatorLock = atomic32 {};
        result->type                = AllocatorType::Multipool;
        result->policy              = policy;
        result->parent              = parent;
        result->pools               = (MonotonicPool**) (result + 1);
        result->poolSizeMax         = maxPoolSize;
        result->poolSizeGranularity = poolSizeGranularity;
        result->defaultElementCount = flags_contain( policy, AllocatorPolicyFlags::AllowGrowth ) ? 16 : 0;
      }
      else
      {
        BREAK;
      }

      //fill the otherwise wasted space with allocator of smallest granularity
      if ( fillerAllocationSize )
      {
        result->pools[0] = (MonotonicPool*) (((char*) result) + allocationSize);
        MonotonicPool*& pool = result->pools[0];
        pool->allocatorLock = atomic32 {};
        pool->type        = AllocatorType::MonotonicPool;
        pool->policy      = policy;
        pool->parent      = parent;
        pool->begin       = (char*) (pool + 1);
        pool->next        = (MonotonicPool::Slot*) pool->begin;
        pool->next->next  = nullptr;
        pool->nextPool    = nullptr;
        pool->size        = fillerPoolSize;
        pool->granularity = poolSizeGranularity;
      }

      return result;
    }

    void delete_multipool( Multipool* multipool )
    {
      s64 const poolCount = 1 + ((multipool->poolSizeMax - 1) / multipool->poolSizeGranularity);

      MonotonicPool* pool = multipool->pools[0];
      if ( pool != (MonotonicPool*) &multipool->pools[poolCount] )
      {
        //free this separately, it wasn't part of the multipool allocation deal
        delete_monotonic_pool( pool );
      }

      for ( s64 i = 1; i < poolCount; ++i )
      {
        pool = multipool->pools[i];
        if ( pool )
        {
          delete_monotonic_pool( pool );
        }
      }

      free( multipool->parent, multipool );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Virtual Memory Layout /////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    void init_arena_for_virtual_memory_layout( Arena& arena, Allocator* parent, void* address, s64 size, AllocatorPolicyFlags const& policy )
    {
      arena.allocatorLock = atomic32 {};
      arena.type      = AllocatorType::Arena;
      arena.policy    = policy;
      arena.parent    = parent;
      arena.begin     = (char*) address;
      arena.ptr       = arena.begin;
      arena.size      = size;
      arena.nextArena = nullptr;
    }

    void init_virtual_memory_layout( VirtualMemoryLayout& layout )
    {
      layout.allocatorLock = atomic32 {};
      layout.type        = AllocatorType::VirtualMemory;
      layout.policy      = AllocatorPolicyFlags::None;
      layout.parent      = nullptr;

      //hacked for now, probably won't matter for a lot of platforms
      constexpr s64 networkSize = GigaBytes( 4 );
      constexpr s64 temporarySize = GigaBytes( 16 );
      constexpr s64 generalSize = 0;

      AllocatorPolicyFlags policy = AllocatorPolicyFlags::ThreadSafe | AllocatorPolicyFlags::AllowGrowth;

      s64 total = networkSize + temporarySize + generalSize;
      s64 end = total / s64( platform->info.virtualMemoryPageSize );

      char* pointer = nullptr;
      assert( platform->info.virtualMemoryAddressEnd - platform->info.virtualMemoryAddressBegin > networkSize + temporarySize + generalSize );

      pointer = (char*) platform->allocate_virtual_memory( 0, networkSize );
      init_arena_for_virtual_memory_layout( layout.network, &layout, pointer, networkSize, policy );

      pointer = (char*) platform->allocate_virtual_memory( 0, temporarySize );
      init_arena_for_virtual_memory_layout( layout.temporary, &layout, pointer, temporarySize, policy );

      //pointer = (char*) platform->allocate_virtual_memory( 0, generalSize );
      //init_arena_for_virtual_memory_layout( layout.general, &layout, pointer, generalSize, policy );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Pointer Info //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    bool is_part_of( Arena* arena, void* ptr )
    {
      if ( ptr > arena->begin && ptr < arena->begin + arena->size )
      {
        return true;
      }
      else if ( arena->nextArena )
      {
        return is_part_of( arena->nextArena, ptr );
      }

      return false;
    }

    memory::Type get_memory_type( void* ptr )
    {
      return is_temporary( ptr ) ? Type::Temporary : is_network( ptr ) ? Type::Network : Type::General;
    }

    bool is_network( void* ptr )
    {
      return is_part_of( &platform->allocators.virtualMemory.network, ptr );
    }

    bool is_temporary( void* ptr )
    {
      return is_part_of( &platform->allocators.virtualMemory.temporary, ptr );
    }

    bool is_general( void* ptr )
    {
      return !is_temporary( ptr ) && !is_network( ptr );
    }
  };
};


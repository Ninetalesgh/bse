#pragma once

#include "bse_debug.h"
#include "bse_thread.h"
#include "bse_bit.h"

namespace bse
{
  namespace memory
  {
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Allocators ////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// allocate_temporary lives only a couple of frames,            ///////////////////////////////////
    ///// so use as often as you like but cautiously!                  ///////////////////////////////////
    /////                                                              ///////////////////////////////////
    ///// Allocations in the non-volatile allocators expect a size     ///////////////////////////////////
    ///// for freeing. If you don't want to remember the allocation    ///////////////////////////////////
    ///// size then you can use the given functions.                   ///////////////////////////////////
    ///// Only use allocations and frees associated with each other.   ///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //allocate in the default frame memory
    // !!! WARNING !!! 
    //anything allocated will only live for a couple of frames!
    //TODO this is now only main thread, I think I'll fetch these by threadID ? 
    [[nodiscard]] void* allocate_frame( s64 size );

    //allocate temporary memory that should be cleared entirely every now and then, freeing individual allocations not necessary
    [[nodiscard]] void* allocate_temporary( s64 size );
    void clear_temporary();

    [[nodiscard]] void* allocate_main_thread( s64 size );
    [[nodiscard]] void* reallocate_main_thread( void* ptr, s64 oldSize, s64 newSize );
    void free_main_thread( void* ptr, s64 size );

    [[nodiscard]] void* allocate_thread_safe( s64 size );
    [[nodiscard]] void* reallocate_thread_safe( void* ptr, s64 oldSize, s64 newSize );
    void free_thread_safe( void* ptr, s64 size );

    [[nodiscard]] void* allocate_thread_safe_dont_remember_size( s64 size );
    [[nodiscard]] void* reallocate_thread_safe_dont_remember_size( void* ptr, s64 newSize );
    void free_thread_safe_dont_remember_size( void* ptr );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// General ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class AllocatorType : u16
    {
      VirtualMemory = 0,
      Arena,
      Multipool,
      MonotonicPool
    };
    enum class AllocatorPolicyFlags : u16
    {
      None = 0b0,
      //
      //If set the container will either grow or allocate memory from parent allocator.
      //If unset overflowing allocations return nullptr.
      AllowGrowth = 0b1,
      //
      //Only relevant if container is allowed to grow.
      //If set the container will grow exponentially.
      //If unset container will grow linearly.
      GeometricGrowth = 0b10,
      //
      //If set container is thread safe
      //If unset container is faster, but should be used with that in mind
      ThreadSafe = 0b100,
    };
    BSE_DEFINE_ENUM_OPERATORS_U16( AllocatorPolicyFlags );

    struct Allocator;
    [[nodiscard]] void* allocate( Allocator* allocator, s64 size );
    [[nodiscard]] void* reallocate( Allocator* allocator, void* ptr, s64 oldSize, s64 newSize );
    void free( Allocator* allocator, void* ptr, s64 size );
    void free( Allocator* allocator, void* ptr );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Virtual Memory ////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// Allocate pages directly from the platform,                   ///////////////////////////////////
    ///// platform->info.virtualMemoryPageSize to check the size.      ///////////////////////////////////
    ///// The size parameter is the size of the allocation,            ///////////////////////////////////
    ///// not the page count.                                          ///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] void* allocate_virtual_memory( s64 size );
    [[nodiscard]] void* reallocate_virtual_memory( void* ptr, s64 oldSize, s64 newSize );
    void free_virtual_memory( void* ptr, s64 size );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Arena /////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// This behaves basically like a stack.                         ///////////////////////////////////
    ///// free(arena, ptr) is ruthless and will pop back to ptr,       ///////////////////////////////////
    ///// so unless you are aware of that only ever LIFO like a stack. ///////////////////////////////////
    ///// If reallocate is called on the last allocation it won't      ///////////////////////////////////
    ///// have to move any data around, it will simply add more space. ///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Arena;
    [[nodiscard]] void* allocate( Arena* arena, s64 size );
    [[nodiscard]] void* reallocate( Arena* arena, void* ptr, s64 oldSize, s64 newSize );
    void free( Arena* arena, void* ptr );

    //this does not release memory, just resets the write pointer
    void clear_arena( Arena* arena );

    //in-place arenas
    [[nodiscard]] Arena* new_arena( Allocator* parent, s64 size, AllocatorPolicyFlags const& policy );
    [[nodiscard]] Arena* new_arena( Allocator* parent, s64 size ) { return new_arena( parent, size, AllocatorPolicyFlags::AllowGrowth | AllocatorPolicyFlags::GeometricGrowth ); }
    [[nodiscard]] Arena* new_arena( Allocator* parent, void* existingBuffer, s64 bufferSize, AllocatorPolicyFlags const& policy );
    void delete_arena( Arena* arena );

    //out-of-place arenas
    void init_arena( Arena& arena, Allocator* parent, s64 size, AllocatorPolicyFlags const& policy );
    void deinit_arena( Arena& arena );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// MonotonicPool /////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// Monotonic pool allocations are of one fixed size only,       ///////////////////////////////////
    ///// specified at allocator creation.                             ///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct MonotonicPool;
    [[nodiscard]] void* allocate( MonotonicPool* pool );
    [[nodiscard]] void* reallocate( MonotonicPool* pool, void* ptr, s64 oldSize, s64 newSize );
    void free( MonotonicPool* pool, void* ptr );

    [[nodiscard]] MonotonicPool* new_monotonic_pool( Allocator* parent, s64 size, s64 granularity, AllocatorPolicyFlags const& policy );
    [[nodiscard]] MonotonicPool* new_monotonic_pool( Allocator* parent, s64 size, s64 granularity ) { return new_monotonic_pool( parent, size, granularity, AllocatorPolicyFlags::AllowGrowth | AllocatorPolicyFlags::GeometricGrowth ); }
    void delete_monotonic_pool( MonotonicPool* pool );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Multipool /////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// This is a meta structure that holds monotonic allocators            ////////////////////////////
    ///// of different granularity and will allocate into the best fit.       ////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Multipool;
    [[nodiscard]] void* allocate( Multipool* multipool, s64 size );
    [[nodiscard]] void* reallocate( Multipool* multipool, void* ptr, s64 oldSize, s64 newSize );
    void free( Multipool* multipool, void* ptr, s64 size );

    [[nodiscard]] Multipool* new_multipool( Allocator* parent, s64 maxPoolSize, s64 poolSizeGranularity, AllocatorPolicyFlags const& policy );
    [[nodiscard]] Multipool* new_multipool( Allocator* parent, s64 maxPoolSize, s64 poolSizeGranularity ) { return new_multipool( parent, maxPoolSize, poolSizeGranularity, AllocatorPolicyFlags::AllowGrowth | AllocatorPolicyFlags::GeometricGrowth ); }
    void delete_multipool( Multipool* multipool );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Virtual Memory Layout /////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// This is the memory layout of brewing station,                       ////////////////////////////
    ///// made up of memory for networking, temporary and general allocations ////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct VirtualMemoryLayout;
    void init_virtual_memory_layout( VirtualMemoryLayout& layout );

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Pointer Info //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// Helper functions for pointers.                                      ////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum Type : u32
    {
      Network,
      Temporary,
      General,
    };

    bool is_part_of( Arena* arena, void* ptr );
    memory::Type get_memory_type( void* ptr );
    bool is_network( void* ptr );
    bool is_temporary( void* ptr );
    bool is_general( void* ptr );
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////// Inline //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace bse
{
  namespace memory
  {
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// General ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Allocator
    {
      constexpr static u32 ALIGNMENT = 64;
      Allocator* parent;
      atomic<s32> allocatorLock;
      AllocatorType type;
      AllocatorPolicyFlags policy;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Arena /////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Arena : Allocator
    {
      char* begin;
      char* ptr;
      s64 size;
      Arena* nextArena;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// MonotonicPool /////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct MonotonicPool : Allocator
    {
      struct Slot
      {
        Slot* next;
      };
      Slot* next;
      char* begin;
      MonotonicPool* nextPool;
      s64 size;
      s64 granularity;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Multipool /////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Multipool : Allocator
    {
      MonotonicPool** pools;
      s64 poolSizeMax;
      s64 poolSizeGranularity;
      s64 defaultElementCount;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Virtual Memory Layout /////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct VirtualMemoryLayout : Allocator
    {
      Arena network;
      Arena temporary;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// Allocator proxy for STD Container Wrappers ////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename T>
    struct AllocatorProxy
    {
      using value_type = T;
      using propagate_on_container_move_assignment = std::true_type;
      using is_always_equal = std::true_type;
      using difference_type = std::ptrdiff_t;

      constexpr AllocatorProxy() noexcept {}
      constexpr AllocatorProxy( AllocatorProxy const& ) = default;
      template<typename U> constexpr AllocatorProxy( AllocatorProxy<U> const& ) noexcept {}
      ~AllocatorProxy() = default;
      constexpr AllocatorProxy& operator=( const AllocatorProxy& ) = default;

      void deallocate( T* ptr, s64 size )
      {
        free_thread_safe( ptr, size * sizeof( T ) );
      }
      T* allocate( s64 size )
      {
        return (T*) allocate_thread_safe( size * sizeof( T ) );
      }
    };

    template<typename T, typename U> bool operator ==( AllocatorProxy<T> const&, AllocatorProxy<U> const& ) { return true; }
    template<typename T, typename U> bool operator !=( AllocatorProxy<T> const&, AllocatorProxy<U> const& ) { return false; }

    template<typename T>
    struct TempAllocatorProxy
    {
      using value_type = T;
      using propagate_on_container_move_assignment = std::true_type;
      using is_always_equal = std::true_type;
      using difference_type = std::ptrdiff_t;

      constexpr TempAllocatorProxy() noexcept {}
      constexpr TempAllocatorProxy( TempAllocatorProxy const& ) = default;
      template<typename U> constexpr TempAllocatorProxy( TempAllocatorProxy<U> const& ) noexcept {}
      ~TempAllocatorProxy() = default;
      constexpr TempAllocatorProxy& operator=( const TempAllocatorProxy& ) = default;

      void deallocate( T* ptr, s64 size ) {}
      T* allocate( s64 size )
      {
        return (T*) allocate_temporary( size * sizeof( T ) );
      }
    };

    template<typename T, typename U> bool operator ==( TempAllocatorProxy<T> const&, TempAllocatorProxy<U> const& ) { return true; }
    template<typename T, typename U> bool operator !=( TempAllocatorProxy<T> const&, TempAllocatorProxy<U> const& ) { return false; }

  };
};
#pragma once

#include "bse_debug.h"
#include "bse_thread.h"
namespace bse
{
  namespace memory
  {
    [[nodiscard]]
    void* allocate( s64 size );
    [[nodiscard]]
    void* reallocate( void* ptr, s64 oldSize, s64 newSize );
    void free( void* ptr, s64 size );

    //using allocate_fn = void* (s64 size);
    //using reallocate_fn = void* (void*, s64 oldSize, s64 newSize);
    //using free_fn = void( void*, s64 size );
  };



  struct ThreadSafeLinearAllocator;

  [[nodiscard]]
  /// @brief 
  /// @param size will be the total size of the allocation including the allocator and overhead, 
  /// this is not to be precisely sized.  
  /// @return 
  ThreadSafeLinearAllocator* create_thread_safe_linear_allocator( s64 size );

  void destroy_thread_safe_linear_allocator( ThreadSafeLinearAllocator* allocator );

  [[nodiscard]]
  /// @brief 
  /// @param allocator 
  /// @param size 
  /// @return 64 byte aligned memory block of asked size
  void* allocate( ThreadSafeLinearAllocator* allocator, s64 size );
  [[nodiscard]]
  void* allocate_to_zero( ThreadSafeLinearAllocator* allocator, s64 size );
  void free( ThreadSafeLinearAllocator* allocator, void* address );
};
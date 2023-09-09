#pragma once

#include "bse_allocator.h"

#if defined (BSE_BUILD_MEMORY_ALLOCATE_OVERRIDE)
# define BSE_ARRAY_MEMORY_ALLOCATE BSE_BUILD_MEMORY_ALLOCATE_OVERRIDE
#else
# define BSE_ARRAY_MEMORY_ALLOCATE(size) memory::allocate(size)
#endif

#if defined (BSE_BUILD_MEMORY_REALLOCATE_OVERRIDE)
# define BSE_ARRAY_MEMORY_REALLOCATE BSE_BUILD_MEMORY_REALLOCATE_OVERRIDE
#else
# define BSE_ARRAY_MEMORY_REALLOCATE(ptr, size, newSize) memory::reallocate(ptr, size, newSize)
#endif

#if defined (BSE_BUILD_MEMORY_FREE_OVERRIDE)
# define BSE_ARRAY_MEMORY_FREE BSE_BUILD_MEMORY_FREE_OVERRIDE
#else
# define BSE_ARRAY_MEMORY_FREE(ptr, size) memory::free(ptr, size)
#endif

namespace bse
{
  template<typename T> struct array
  {
    T& push( T const& _t );
    T const& pop();
    void reserve( s32 capacityToReserve );
    T* data();
    s32 count;

    struct iterator
    {
      using value_type = T;
      using difference_type = s64;
      using pointer = T*;
      using reference = T&;
      //using iterator_category =; is this needed?
      iterator() : ptr( nullptr ) {}
      iterator( pointer _ptr ) : ptr( _ptr ) {}
      reference operator*() const { return *ptr; }
      pointer operator->() const { return ptr; }
      reference operator[]( difference_type n ) const { return *(ptr + n); }
      iterator& operator++() { ++ptr; return *this; }
      iterator operator++( s32 ) { iterator tmp = *this; ++ptr; return tmp; }
      iterator& operator--() { --ptr; return *this; }
      iterator operator--( s32 ) { iterator tmp = *this; --ptr; return tmp; }
      iterator& operator+=( difference_type n ) { ptr += n; return *this; }
      iterator& operator-=( difference_type n ) { ptr -= n; return *this; }
      friend bool operator==( iterator const& a, iterator const& b ) { return a.ptr == b.ptr; }
      friend bool operator!=( iterator const& a, iterator const& b ) { return !(a == b); }
      friend bool operator<( iterator const& a, iterator const& b ) { return a.ptr < b.ptr; }
      friend bool operator>( iterator const& a, iterator const& b ) { return b < a; }
      friend bool operator<=( iterator const& a, iterator const& b ) { return !(b < a); }
      friend bool operator>=( iterator const& a, iterator const& b ) { return !(a < b); }
      friend iterator operator+( iterator const& it, difference_type n ) { iterator tmp = it;tmp += n; return tmp; }
      friend iterator operator+( difference_type n, iterator const& it ) { return it + n; }
      friend iterator operator-( iterator const& it, difference_type n ) { iterator tmp = it;  tmp -= n;  return tmp; }
      friend difference_type operator-( iterator const& a, iterator const& b ) { return difference_type( a.ptr - b.ptr ); }

    private:
      T* ptr;
    };
    iterator begin() { return iterator { t }; }
    iterator end() { return iterator { t + count }; }

    array() : t( nullptr ), count( 0 ), capacity( 0 ) {}
    array( array<T> const& other ) : count( other.count ), capacity( other.capacity ) { t = (T*) BSE_ARRAY_MEMORY_ALLOCATE( other.capacity * sizeof( T ) );memcpy( t, other.t, count * sizeof( T ) ); }
    array( s32 capacity ) : count( 0 ), capacity( capacity ) { t = (T*) BSE_ARRAY_MEMORY_ALLOCATE( capacity * sizeof( T ) ); }
    array( T* raw, s32 count ) : t( raw ), count( count ), capacity( count ) {}
    ~array() { BSE_ARRAY_MEMORY_FREE( t, capacity ); }

  private:
    void reallocate( s32 newCapacity )
    {
      t = BSE_ARRAY_MEMORY_REALLOCATE( t, capacity * sizeof( T ), newCapacity * sizeof( T ) );
      capacity = newCapacity;
    }
    T* t;
    s32 capacity;
  };

  template<typename T> T& array<T>::push( T const& _t ) { if ( count + 1 > capacity ) { reallocate( capacity * 2 ); }  return (t[count++] = _t); }
  template<typename T> T const& array<T>::pop() { return t[--count]; }
  template<typename T> void array<T>::reserve( s32 capacityToReserve ) { count = min( count, capacityToReserve );  reallocate( capacityToReserve ); }
  template<typename T> T* array<T>::data() { return t; }


  template<typename K, typename V> struct llmap
  {
  private:
    struct Node {
      K key;
      V value;
      Node* next;
      Node( K const& k, V const& v ) : key( k ), value( v ), next( nullptr ) {}
    };
    Node* head;
  };

  template<typename V> struct llmap<char const*, V>
  {

  };

};
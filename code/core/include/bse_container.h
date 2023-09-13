#pragma once

#include "bse_allocator.h"
#include "bse_string_format.h"

namespace bse
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// General ///////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  struct Container
  {
    Container( memory::Allocator* _allocator );
    memory::Allocator* allocator;
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Array /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename T> struct Array : Container
  {
    struct iterator;
    T& push( T const& value );
    T const& pop();
    void reserve( s32 newCapacity );

    void remove( iterator const& at );
    T& insert( iterator const& at, T const& value );
    void insert( iterator const& at, std::initializer_list<T> const& items );
    void insert( iterator const& at, Array<T> const& items );
    void remove_range( iterator const& from, iterator const& to );

    Array()
      : Container( nullptr )
      , data( 0 )
      , count( 0 )
      , capacity( 0 )
    {}

    Array( memory::Allocator* _allocator )
      : Container( _allocator )
      , data( 0 )
      , count( 0 )
      , capacity( 0 )
    {}

    Array( memory::Allocator* _allocator, s32 _capacity )
      : Container( _allocator )
      , count( 0 )
      , capacity( _capacity )
    {
      data = (T*) memory::allocate( allocator, _capacity * sizeof( T ) );
    }

    Array( s32 _capacity )
      : Container( nullptr )
      , count( 0 )
      , capacity( _capacity )
    {
      data = (T*) memory::allocate( allocator, _capacity * sizeof( T ) );
    }

    Array( std::initializer_list<T> const& list )
      : Container( nullptr )
      , count( s32( list.end() - list.begin() ) )
      , capacity( count )
    {
      data = (T*) memory::allocate( allocator, capacity * sizeof( T ) );
      memcpy( data, list.begin(), capacity * sizeof( T ) );
    }
    ~Array()
    {
      if ( data )
      {
        //if (allocator.type == AllocatorType::Arena) don't call free?
        memory::free( allocator, data, s64( capacity * sizeof( T ) ) );
      }
    }

    Array<T>& operator =( Array<T> const& array )
    {
      if ( data )
      {
        memory::free( allocator, data, capacity * sizeof( T ) );
      }

      count = capacity = array.count;
      data = (T*) memory::allocate( allocator, capacity * sizeof( T ) );
      memcpy( data, array.data, count * sizeof( T ) );
      return *this;
    }

    INLINE T& operator[]( s32 i ) { return data[i]; }
    INLINE T  operator[]( s32 i ) const { return data[i]; }

    struct iterator
    {
      using value_type = T;
      using difference_type = s64;
      using pointer = T*;
      using reference = T&;
      //iterator_category not needed?
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

    iterator find( T const& value )
    {
      Array<T>::iterator finder = data;
      Array<T>::iterator const last = end();
      while ( finder != last )
      {
        if ( *finder == value ) { break; }
        ++finder;
      }
      return finder;
    }

    iterator begin() { return iterator { data }; }
    iterator end() { return iterator { data + count }; }

    T* data;
    s32 count;
    s32 capacity;
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// String ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  struct String : Array<char>
  {
    //TODO make this standalone and not an array child    
    String()
      : Array<char>( nullptr )
    {}

    String( char const* str )
      : Array<char>( nullptr )
    {
      count = capacity = string_length( str ) + 1;
      data = (char*) memory::allocate( allocator, capacity );
      memcpy( data, str, count * sizeof( char ) );
    }

    String& operator =( char const* str )
    {
      if ( data )
      {
        memory::free( allocator, data, capacity * sizeof( char ) );
      }

      count = capacity = string_length( str ) + 1;
      data = (char*) memory::allocate( allocator, capacity * sizeof( char ) );
      memcpy( data, str, count * sizeof( char ) );
      return *this;
    }

    bool operator ==( String const& other ) { return string_match( this->data, other.data ); }
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Map ///////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename K, typename V> struct Map : Container
  {
    //TODO make map something actually useful
    Map()
      : Container( nullptr )
      , pairs( nullptr )
    {}
    Map( memory::Allocator* _allocator )
      : Container( _allocator )
      , pairs( _allocator )
    {}

    struct KeyValuePair
    {
      K key;
      V value;
    };

    INLINE V& operator[]( K const& k )
    {
      //TODO binary search this
      for ( auto& pair : pairs )
      {
        if ( pair.key == k )
        {
          return pair.value;
        }
      }
      return pairs.push( KeyValuePair { k, V{} } ).value;
    }
    INLINE V operator[]( K const& k ) const
    {
      for ( auto const& pair : pairs )
      {
        if ( pair.key == k )
        {
          return pair.value;
        }
      }
      return pairs.push( KeyValuePair { k, V{} } ).value;
    }

    Array<KeyValuePair> pairs;
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
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Array /////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename T> T& Array<T>::push( T const& value )
  {
    if ( count == capacity )
    {
      s64 newCapacity = s64( capacity ) * 2;
      if ( newCapacity > S32_MAX )
      {
        BREAK;
      }
      data = (T*) memory::reallocate( allocator, data, capacity * sizeof( T ), newCapacity * sizeof( T ) );
      capacity = newCapacity;
    }
    return (data[count++] = value);
  }

  template<typename T> INLINE T const& Array<T>::pop() { return data[--count]; }
  template<typename T> void Array<T>::reserve( s32 newCapacity )
  {
    data = (T*) memory::reallocate( allocator, data, capacity * sizeof( T ), newCapacity * sizeof( T ) );
    count = min( count, newCapacity );
    capacity = newCapacity;
  }


  template<typename T> void Array<T>::remove( iterator const& at )
  {
    BREAK;
  }

  template<typename T> T& Array<T>::insert( iterator const& at, T const& value )
  {
    return at;
  }

  template<typename T> void Array<T>::insert( iterator const& at, std::initializer_list<T> const& items )
  {
    BREAK;
  }

  template<typename T> void Array<T>::insert( iterator const& at, Array<T> const& items )
  {
    BREAK;
  }



};
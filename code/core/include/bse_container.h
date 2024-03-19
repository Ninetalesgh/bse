#pragma once

#include "bse_allocator.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>

namespace bse
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Buffer ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  struct Buffer
  {
    char* data;
    s64 size;
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// STD Container Wrappers ////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ///// Redirecting all allocations for standard containers.                ////////////////////////////
  ///// For now they are all just using thread safe allocations.            ////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Vector ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename T> using Vector = std::vector<T, memory::AllocatorProxy<T>>;

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// String ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  using String = std::basic_string<char, std::char_traits<char>, memory::AllocatorProxy<char>>;

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// List //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename T> using List = std::list<T, memory::AllocatorProxy<T>>;

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Map ///////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename Key, typename Value> using Map = std::map<Key, Value, std::less<Key>, memory::AllocatorProxy<std::pair<Key const, Value>>>;

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// HashMap ///////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename Key, typename Value> using HashMap = std::unordered_map<Key, Value, std::hash<Key>, std::equal_to<Key>, memory::AllocatorProxy<std::pair<Key const, Value>>>;

};


namespace bse
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Vector ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  template<typename T> T swap_to_end_and_pop( Vector<T>& array, u32 index )
  {
    std::swap( array[index], array[array.size() - 1] );
    T result = array[array.size() - 1];
    array.pop_back();
    return result;
  }
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
///// Rest of the file is not in use, maybe at some point I'll come back to this  ////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


// namespace bse
// {
//   //////////////////////////////////////////////////////////////////////////////////////////////////////
//   ////////// General ///////////////////////////////////////////////////////////////////////////////////
//   //////////////////////////////////////////////////////////////////////////////////////////////////////

//   ////

//   //////////////////////////////////////////////////////////////////////////////////////////////////////
//   ////////// Array /////////////////////////////////////////////////////////////////////////////////////
//   //////////////////////////////////////////////////////////////////////////////////////////////////////

//   template <typename T> struct Array
//   {
//     struct iterator;
//     T& push() { return push( T() ); }
//     T& push( T const& value );
//     T const& pop();
//     s32 reserve( s32 newCapacity );
//     s32 resize( s32 newCount );
//     void free_unsafe();
//     s32 set_count_to_capacity() { count = capacity; return count; }

//     void remove( iterator const& at );
//     T& insert( iterator const& at, T const& value );
//     void insert( iterator const& at, std::initializer_list<T> const& items );
//     void insert( iterator const& at, Array<T> const& items );
//     void insert( iterator const& at, T const* items, s32 itemCount );
//     void remove_range( iterator const& from, iterator const& to );

//     Array()
//       : allocator( nullptr )
//       , data( 0 )
//       , count( 0 )
//       , capacity( 0 )
//     {}

//     Array( memory::Allocator* _allocator )
//       : allocator( _allocator )
//       , data( 0 )
//       , count( 0 )
//       , capacity( 0 )
//     {}

//     Array( memory::Allocator* _allocator, s32 _capacity )
//       : allocator( _allocator )
//       , count( 0 )
//       , capacity( _capacity )
//     {
//       _internal_allocate( _capacity );
//     }

//     Array( s32 _capacity )
//       : allocator( nullptr )
//       , count( 0 )
//       , capacity( _capacity )
//     {
//       _internal_allocate( _capacity );
//     }

//     Array( std::initializer_list<T> const& list )
//       : allocator( nullptr )
//       , count( s32( list.end() - list.begin() ) )
//       , capacity( count )
//     {
//       _internal_allocate( capacity );
//       _internal_copy_const_elements( list.begin(), count );
//     }

//     Array( Array<T> const& array )
//       : allocator( array.allocator )
//       , count( array.count )
//       , capacity( array.count )
//     {
//       _internal_allocate( capacity );
//       _internal_copy( array.data, count );
//     }

//     ~Array()
//     {
//       _internal_try_free();
//     }

//     Array<T>& operator =( Array<T> const& array )
//     {
//       _internal_try_free();
//       count = capacity = array.count;
//       _internal_allocate( array.count );
//       _internal_copy( array.data, count );
//       return *this;
//     }

//     INLINE T& operator[]( s32 i ) { return data[i]; }
//     INLINE T  operator[]( s32 i ) const { return data[i]; }

//     struct iterator
//     {
//       using value_type = T;
//       using difference_type = s64;
//       using pointer = T*;
//       using reference = T&;
//       //iterator_category not needed?
//       iterator() : ptr( nullptr ) {}
//       iterator( pointer _ptr ) : ptr( _ptr ) {}
//       reference operator*() const { return *ptr; }
//       pointer operator->() const { return ptr; }
//       reference operator[]( difference_type n ) const { return *(ptr + n); }
//       iterator& operator++() { ++ptr; return *this; }
//       iterator operator++( int ) { iterator tmp = *this; ++ptr; return tmp; }
//       iterator& operator--() { --ptr; return *this; }
//       iterator operator--( int ) { iterator tmp = *this; --ptr; return tmp; }
//       iterator& operator+=( difference_type n ) { ptr += n; return *this; }
//       iterator& operator-=( difference_type n ) { ptr -= n; return *this; }
//       friend bool operator==( iterator const& a, iterator const& b ) { return a.ptr == b.ptr; }
//       friend bool operator!=( iterator const& a, iterator const& b ) { return !(a == b); }
//       friend bool operator<( iterator const& a, iterator const& b ) { return a.ptr < b.ptr; }
//       friend bool operator>( iterator const& a, iterator const& b ) { return b < a; }
//       friend bool operator<=( iterator const& a, iterator const& b ) { return !(b < a); }
//       friend bool operator>=( iterator const& a, iterator const& b ) { return !(a < b); }
//       friend iterator operator+( iterator const& it, difference_type n ) { iterator tmp = it;tmp += n; return tmp; }
//       friend iterator operator+( difference_type n, iterator const& it ) { return it + n; }
//       friend iterator operator-( iterator const& it, difference_type n ) { iterator tmp = it;  tmp -= n;  return tmp; }
//       friend difference_type operator-( iterator const& a, iterator const& b ) { return difference_type( a.ptr - b.ptr ); }
//     private:
//       T* ptr;
//     };

//     iterator find( iterator begin, iterator end, T const& value ) const
//     {
//       while ( begin < end )
//       {
//         if ( *begin == value ) { break; }
//         ++begin;
//       }
//       return begin;
//     }

//     INLINE iterator find( T const& value ) const { return find( begin(), end(), value ); }
//     INLINE iterator begin() const { return iterator { data }; }
//     INLINE iterator end() const { return iterator { data + count }; }

//     memory::Allocator* allocator;
//     T* data;
//     s32 count;
//     s32 capacity;

//   private:
//     INLINE void _internal_try_free()
//     {
//       if ( data )
//       {
//         if ( !std::is_trivially_destructible<T>() )
//         {
//           T* entry = data;
//           T* const last = data + count;
//           while ( entry != last )
//           {
//             entry++->~T();
//           }
//         }

//         //if (allocator.type == AllocatorType::Arena) don't call free?
//         memory::free( allocator, data, s64( capacity * sizeof( T ) ) );
//       }
//     }
//     INLINE void _internal_allocate( s32 newCapacity )
//     {
//       data = (T*) memory::allocate( allocator, newCapacity * sizeof( T ) );
//     }

//     INLINE void _internal_copy_const_elements( T const* from, s32 c )
//     {
//       if ( std::is_trivially_constructible<T>() )
//       {
//         memcpy( data, from, c * sizeof( T ) );
//       }
//       else
//       {
//         T const* const last = from + c;
//         T* to = data;
//         while ( from != last )
//         {
//           *to++ = *from++;
//         }
//       }
//     }
//     INLINE void _internal_copy( T* from, s32 c )
//     {
//       if ( std::is_trivially_constructible<T>() )
//       {
//         memcpy( data, from, c * sizeof( T ) );
//       }
//       else
//       {
//         T* const last = from + c;
//         T* to = data;
//         while ( from != last )
//         {
//           *to++ = *from++;
//         }
//       }
//     }
//   };

//   //////////////////////////////////////////////////////////////////////////////////////////////////////
//   ////////// String ////////////////////////////////////////////////////////////////////////////////////
//   //////////////////////////////////////////////////////////////////////////////////////////////////////

//   struct String
//   {
//   private:
//     INLINE void _internal_reallocate_no_move( s32 newCapacity )
//     {
//       _internal_try_free();
//       capacity = max( newCapacity, MIN_CAPACITY );
//       _internal_allocate( newCapacity );
//     }

//     INLINE void _internal_try_free()
//     {
//       if ( capacity > MIN_CAPACITY )
//       {
//         //if (allocator.type == AllocatorType::Arena) don't call free?
//         memory::free( allocator, dataPtr, s64( capacity ) );
//       }
//     }

//     INLINE void _internal_allocate( s32 newCapacity )
//     {
//       if ( newCapacity > MIN_CAPACITY )
//       {
//         dataPtr = (char*) memory::allocate( allocator, newCapacity );
//       }
//     }

//     INLINE void _internal_copy_const( char const* from, s32 c )
//     {
//       char* ptr = capacity > MIN_CAPACITY ? dataPtr : inlineData;
//       memcpy( ptr, from, c );
//     }

//     INLINE void _internal_copy( char* from, s32 c )
//     {
//       char* ptr = capacity > MIN_CAPACITY ? dataPtr : inlineData;
//       memcpy( ptr, from, c );
//     }
//   public:
//     INLINE char const* cstr() const { return capacity > MIN_CAPACITY ? dataPtr : inlineData; }
//     INLINE char* cstr() { return capacity > MIN_CAPACITY ? dataPtr : inlineData; }

//     constexpr static s32 MIN_CAPACITY = 16;
//     String()
//       : allocator( nullptr )
//       , dataPtr( 0 )
//       , count( 0 )
//       , capacity( MIN_CAPACITY )
//     {}

//     String( memory::Allocator* _allocator )
//       : allocator( _allocator )
//       , dataPtr( 0 )
//       , count( 0 )
//       , capacity( MIN_CAPACITY )
//     {}

//     String( s32 _capacity )
//       : allocator( nullptr )
//       , count( 0 )
//       , capacity( max( _capacity, MIN_CAPACITY ) )
//     {
//       capacity = max( _capacity, MIN_CAPACITY );
//       _internal_allocate( capacity );
//     }

//     String( memory::Allocator* _allocator, s32 _capacity )
//       : allocator( _allocator )
//       , count( 0 )
//     {
//       capacity = max( _capacity, MIN_CAPACITY );
//       _internal_allocate( capacity );
//     }

//     String( char const* str )
//       : allocator( nullptr )
//     {
//       count = string_length( str ) + 1;
//       capacity = max( count, MIN_CAPACITY );
//       _internal_allocate( capacity );
//       _internal_copy_const( str, capacity );
//     }

//     String( memory::Allocator* _allocator, char const* str )
//       : allocator( _allocator )
//     {
//       count = string_length( str ) + 1;
//       capacity = max( count, MIN_CAPACITY );
//       _internal_allocate( capacity );
//       _internal_copy_const( str, capacity );
//     }

//     String( String const& str )
//       : allocator( nullptr )
//     {
//       count = str.count;
//       capacity = max( count, MIN_CAPACITY );
//       _internal_allocate( capacity );
//       _internal_copy_const( str.cstr(), capacity );
//     }

//     String( memory::Allocator* _allocator, String const& str )
//       : allocator( _allocator )
//     {
//       count = str.count;
//       capacity = max( count, MIN_CAPACITY );
//       _internal_allocate( capacity );
//       _internal_copy_const( str.cstr(), capacity );
//     }

//     String& operator=( char const* str )
//     {
//       count = string_length( str ) + 1;
//       _internal_reallocate_no_move( count );
//       _internal_copy_const( str, capacity );
//       return *this;
//     }

//     String& operator=( String const& str )
//     {
//       count = str.count;
//       _internal_reallocate_no_move( str.count );
//       _internal_copy_const( str.cstr(), capacity );
//       return *this;
//     }

//     String& operator+=( char const* str )
//     {
//       BREAK;
//     }

//     String& operator+=( String const& str )
//     {
//       return *this += str.cstr();
//     }

//     ~String()
//     {
//       _internal_try_free();
//     }

//     bool operator ==( String const& other ) const { return string_match( this->cstr(), other.cstr() ); }

//     memory::Allocator* allocator;
//   private:
//     union
//     {
//       char* dataPtr;
//       char inlineData[MIN_CAPACITY];
//     };
//   public:
//     s32 count;
//     s32 capacity;
//     void reserve( s32 newCapacity )
//     {
//       if ( newCapacity > MIN_CAPACITY )
//       {
//         char* newStr = (char*) memory::allocate( allocator, newCapacity );
//         if ( capacity > MIN_CAPACITY )
//         {
//           memcpy( newStr, dataPtr, min( newCapacity, count ) );
//           memory::free( allocator, dataPtr, capacity );
//         }
//         else
//         {
//           memcpy( newStr, inlineData, count );
//         }

//         capacity = newCapacity;
//         dataPtr = newStr;
//       }
//       else
//       {
//         if ( capacity > MIN_CAPACITY )
//         {
//           char* oldData = dataPtr;
//           memcpy( inlineData, oldData, newCapacity );
//           memory::free( allocator, oldData, capacity );
//           capacity = MIN_CAPACITY;
//         }
//       }
//     }
//   };

//   template<> INLINE s32 string_format_internal<String*>( char* destination, s32 capacity, String* value ) { return string_format_internal( destination, capacity, value->cstr() ); }
//   template<> INLINE s32 string_format_internal<String const*>( char* destination, s32 capacity, String const* value ) { return string_format_internal( destination, capacity, value->cstr() ); }

//   //////////////////////////////////////////////////////////////////////////////////////////////////////
//   ////////// Map ///////////////////////////////////////////////////////////////////////////////////////
//   //////////////////////////////////////////////////////////////////////////////////////////////////////

//   template<typename K, typename V> struct Map
//   {
//     //TODO make map something actually useful
//     //TODO implement radix search for strings?
//     Map()
//       : allocator( nullptr )
//       , pairs( nullptr )
//     {}
//     Map( memory::Allocator* _allocator )
//       : allocator( _allocator )
//       , pairs( _allocator )
//     {}

//     Map( Map<K, V> const& map )
//       : allocator( nullptr )
//       , pairs( map.pairs )
//     {}

//     ~Map()
//     {
//       pairs.~Array<KeyValuePair>();
//     }

//     struct KeyValuePair
//     {
//       K key;
//       V value;
//     };

//     INLINE V& operator[]( K const& k )
//     {
//       //TODO binary search this
//       for ( auto& pair : pairs )
//       {
//         if ( pair.key == k )
//         {
//           return pair.value;
//         }
//       }
//       return pairs.push( KeyValuePair { k, V{} } ).value;
//     }
//     INLINE V operator[]( K const& k ) const
//     {
//       for ( auto const& pair : pairs )
//       {
//         if ( pair.key == k )
//         {
//           return pair.value;
//         }
//       }
//       return V {};
//       //return pairs.push( KeyValuePair { k, V{} } ).value;
//     }

//     memory::Allocator* allocator;
//     Array<KeyValuePair> pairs;
//   };
// };

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////// Inline //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// namespace bse
// {
//   //////////////////////////////////////////////////////////////////////////////////////////////////////
//   ////////// Array /////////////////////////////////////////////////////////////////////////////////////
//   //////////////////////////////////////////////////////////////////////////////////////////////////////

//   template<typename T> T& Array<T>::push( T const& value )
//   {
//     if ( count == capacity )
//     {
//       s64 newCapacity = s64( max( 4, capacity ) ) * 2;
//       if ( newCapacity > S32_MAX )
//       {
//         BREAK;
//       }
//       data = (T*) memory::reallocate( allocator, data, capacity * sizeof( T ), newCapacity * sizeof( T ) );
//       capacity = s32( newCapacity );
//     }
//     return (data[count++] = value);
//   }

//   template<typename T> INLINE T const& Array<T>::pop() { return data[--count]; }

//   template<typename T> s32 Array<T>::reserve( s32 newCapacity )
//   {
//     data = (T*) memory::reallocate( allocator, data, capacity * sizeof( T ), newCapacity * sizeof( T ) );
//     count = min( count, newCapacity );
//     capacity = newCapacity;
//   }

//   template<typename T> s32 Array<T>::resize( s32 newCount )
//   {
//     reserve( newCount );
//     count = newCount;
//   }

//   template<typename T> void Array<T>::free_unsafe()
//   {
//     if ( data )
//     {
//       memory::free( allocator, data, s64( capacity * sizeof( T ) ) );
//       data = nullptr;
//       count = 0;
//       capacity = 0;
//     }
//   }

//   template<typename T> void Array<T>::remove( iterator const& at )
//   {
//     BREAK;
//   }

//   template<typename T> T& Array<T>::insert( iterator const& at, T const& value )
//   {
//     return at;
//   }

//   template<typename T> void Array<T>::insert( iterator const& at, std::initializer_list<T> const& items )
//   {
//     BREAK;
//   }

//   template<typename T> void Array<T>::insert( iterator const& at, Array<T> const& items )
//   {
//     BREAK;
//   }

//   template<typename T> void Array<T>::insert( iterator const& at, T const* items, s32 itemCount )
//   {
//     BREAK;
//   }

// };

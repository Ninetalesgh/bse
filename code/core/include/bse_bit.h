#pragma once
#include "bse_common.h"

namespace bse
{
  u16 byte_to_u16( u8 highestByte, u8 byte1 );
  u16 byte_to_u16( char highestByte, char byte1 );
  u64 byte_to_u64( u8 highestByte, u8 byte1, u8 byte2, u8 byte3, u8 byte4, u8 byte5, u8 byte6, u8 byte7 );
  u64 byte_to_u64( char highestByte, char byte1, char byte2, char byte3, char byte4, char byte5, char byte6, char byte7 );

  //these will read data as if it was stored in big-endian 
  u16 byte_to_u16( u8* bytesStartingWithHighestByte );
  u16 byte_to_u16( char* bytesStartingWithHighestByte );
  u64 byte_to_u64( u8* bytesStartingWithHighestByte );
  u64 byte_to_u64( char* bytesStartingWithHighestByte );

  constexpr bool is_system_big_endian();
  char* align_pointer_forward( char* ptr, s32 powerof2 );

  u16 leading_zeroes( u16 value );
  u32 leading_zeroes( u32 value );
  u64 leading_zeroes( u64 value );
  u16 round_up_to_next_power_of_two( u16 value );
  u32 round_up_to_next_power_of_two( u32 value );
  u64 round_up_to_next_power_of_two( u64 value );
  u16 round_down_to_next_power_of_two( u16 value );
  u32 round_down_to_next_power_of_two( u32 value );
  u64 round_down_to_next_power_of_two( u64 value );

  s64 round_up_to_multiple_of( s64 value, s64 dividend );
  s64 round_down_to_multiple_of( s64 value, s64 dividend );
  s64 difference_to_multiple_of( s64 value, s64 dividend );
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////inl///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace bse
{

  u16 byte_to_u16( u8 highestByte, u8 byte1 )
  {
    return (u16( highestByte ) << 8) + u16( byte1 );
  }

  u16 byte_to_u16( char highestByte, char byte1 )
  {
    return byte_to_u16( (u8) highestByte, (u8) byte1 );
  }

  u16 byte_to_u16( u8* bytes )
  {
    return byte_to_u16( bytes[0], bytes[1] );
  }

  u16 byte_to_u16( char* bytes )
  {
    return byte_to_u16( (u8) bytes[0], (u8) bytes[1] );
  }

  u64 byte_to_u64( u8 highestByte, u8 byte1, u8 byte2, u8 byte3, u8 byte4, u8 byte5, u8 byte6, u8 byte7 )
  {
    return (u64( highestByte ) << 56)
      + (u64( byte1 ) << 48)
      + (u64( byte2 ) << 40)
      + (u64( byte3 ) << 32)
      + (u64( byte4 ) << 24)
      + (u64( byte5 ) << 16)
      + (u64( byte6 ) << 8)
      + (u64( byte7 ));
  }

  u64 byte_to_u64( char highestByte, char byte1, char byte2, char byte3, char byte4, char byte5, char byte6, char byte7 )
  {
    return byte_to_u64( (u8) highestByte, (u8) byte1, (u8) byte2, (u8) byte3, (u8) byte4, (u8) byte5, (u8) byte6, (u8) byte7 );
  }

  u64 byte_to_u64( u8* bytes )
  {
    return byte_to_u64( bytes[0]
                      , bytes[1]
                      , bytes[2]
                      , bytes[3]
                      , bytes[4]
                      , bytes[5]
                      , bytes[6]
                      , bytes[7] );
  }

  u64 byte_to_u64( char* bytes )
  {
    return byte_to_u64( (u8) bytes[0]
                      , (u8) bytes[1]
                      , (u8) bytes[2]
                      , (u8) bytes[3]
                      , (u8) bytes[4]
                      , (u8) bytes[5]
                      , (u8) bytes[6]
                      , (u8) bytes[7] );
  }

  constexpr bool is_system_big_endian()
  {
    union {
      u32 i;
      char c[4];
    } bint = { 0x01020304 };

    return bint.c[0] == 1;
  }

  INLINE char* align_pointer_forward( char* ptr, s32 powerof2 )
  {
    return (char*) (((u64( ptr ) - 1) | (powerof2 - 1)) + 1);
  }


  #if defined(_WIN32)

  INLINE u16 leading_zeroes( u16 value ) { return u16( __lzcnt16( value ) ); }
  INLINE u32 leading_zeroes( u32 value ) { return u32( __lzcnt( value ) ); }
  INLINE u64 leading_zeroes( u64 value ) { return u64( __lzcnt64( value ) ); }

  #endif

  INLINE u16 round_up_to_next_power_of_two( u16 value ) { return (u16( 1 ) << (u16( 16 ) - leading_zeroes( u16( value - 1u ) ))); }
  INLINE u32 round_up_to_next_power_of_two( u32 value ) { return (u32( 1 ) << (u32( 32 ) - leading_zeroes( u32( value - 1u ) ))); }
  INLINE u64 round_up_to_next_power_of_two( u64 value ) { return (u64( 1 ) << (u64( 64 ) - leading_zeroes( u64( value - 1u ) ))); }

  INLINE u16 round_down_to_next_power_of_two( u16 value ) { return (u16( 1 ) << (u16( 15 ) - leading_zeroes( u16( value ) ))); }
  INLINE u32 round_down_to_next_power_of_two( u32 value ) { return (u32( 1 ) << (u32( 31 ) - leading_zeroes( u32( value ) ))); }
  INLINE u64 round_down_to_next_power_of_two( u64 value ) { return (u64( 1 ) << (u64( 63 ) - leading_zeroes( u64( value ) ))); }

  INLINE s64 round_up_to_multiple_of( s64 value, s64 dividend ) { return value + ((dividend - (value % dividend)) % dividend); }
  INLINE s64 round_down_to_multiple_of( s64 value, s64 dividend ) { return value - (value % dividend); }
  INLINE s64 difference_to_multiple_of( s64 value, s64 dividend ) { return (dividend - (value % dividend)) % dividend; }

};



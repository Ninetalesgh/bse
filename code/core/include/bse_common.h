#pragma once

#include <memory>

# if defined(_WIN32)
#   define INLINE __forceinline
# else
#   define INLINE inline
# endif

#define array_count(array) (sizeof(array) / (sizeof((array)[0])))

#define _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, basictype)\
INLINE enumtypename  operator |  ( enumtypename a, enumtypename b ) { return enumtypename( basictype( a ) | basictype( b ) ); }\
INLINE enumtypename& operator |= ( enumtypename& a, enumtypename b ) { a = b | a; return a; }\
INLINE enumtypename  operator &  ( enumtypename a, enumtypename b ) { return enumtypename( basictype( a ) & basictype( b ) ); }\
INLINE enumtypename& operator &= ( enumtypename& a, enumtypename b ) { a = b & a; return a; }\
INLINE enumtypename  operator ~  ( enumtypename a ) { return enumtypename( ~basictype( a ) ); }\
INLINE enumtypename  operator ^  ( enumtypename a, enumtypename b ) { return enumtypename( basictype( a ) ^ basictype( b ) ); }\
INLINE enumtypename& operator ^= ( enumtypename& a, enumtypename b ) { a = b ^ a; return a; }

#define DEFINE_ENUM_OPERATORS_U8(enumtypename)  _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u8)
#define DEFINE_ENUM_OPERATORS_U16(enumtypename) _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u16)
#define DEFINE_ENUM_OPERATORS_U32(enumtypename) _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u32)
#define DEFINE_ENUM_OPERATORS_U64(enumtypename) _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u64)

using s8  = signed char;
using s16 = short;
using s32 = int;
using s64 = long long;
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using void_fn = void();

struct int2;
struct float2;
struct float3;
struct float4;

constexpr u8   U8_MAX = 0xff;
constexpr u16 U16_MAX = 0xffff;
constexpr u32 U32_MAX = 0xffffffff;
constexpr u64 U64_MAX = 0xffffffffffffffff;
constexpr s8   S8_MAX = 0x7f;
constexpr s16 S16_MAX = 0x7fff;
constexpr s32 S32_MAX = 0x7fffffff;
constexpr s64 S64_MAX = 0x7fffffffffffffff;
constexpr float FLOAT_E = 0.00000011920928955078125f;


constexpr INLINE s32 min( s32 a, s32 b ) { return a < b ? a : b; }
constexpr INLINE s32 max( s32 a, s32 b ) { return a > b ? a : b; }
constexpr INLINE s32 clamp( s32 value, s32 low, s32 high ) { return max( min( value, high ), low ); }

constexpr INLINE u32 min( u32 a, u32 b ) { return a < b ? a : b; }
constexpr INLINE u32 max( u32 a, u32 b ) { return a > b ? a : b; }
constexpr INLINE u32 clamp( u32 value, u32 low, u32 high ) { return max( min( value, high ), low ); }

constexpr INLINE float min( float a, float b ) { return a < b ? a : b; }
constexpr INLINE float max( float a, float b ) { return a > b ? a : b; }
constexpr INLINE float clamp( float value, float low, float high ) { return max( min( value, high ), low ); }

constexpr INLINE u64 KiloBytes( u64 kiloBytes ) { return kiloBytes * 1024ULL; }
constexpr INLINE u64 MegaBytes( u64 megaBytes ) { return KiloBytes( megaBytes ) * 1024ULL; }
constexpr INLINE u64 GigaBytes( u64 gigaBytes ) { return MegaBytes( gigaBytes ) * 1024ULL; }

constexpr INLINE bool is_negative( float x ) { return (*(u32*) &x) & 0x80000000; }
constexpr INLINE bool is_negative( s32 x ) { return (*(u32*) &x) & 0x80000000; }
constexpr INLINE s32   get_sign( s32 x ) { return is_negative( x ) ? -1 : 1; }
constexpr INLINE float get_sign( float x ) { return is_negative( x ) ? -1.0f : 1.0f; }
constexpr INLINE bool sign_match( s32 a, s32 b ) { return (a ^ b) >= 0; }
constexpr INLINE bool sign_match( float a, float b ) { return ((*(s32*) &a) ^ (*(s32*) &b)) >= 0; }
constexpr INLINE float to_radians( float degrees ) { return degrees * 0.01745329251f; }
constexpr INLINE float to_degrees( float radians ) { return radians * 57.2957795131f; }

float rsqrt( float );

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// inl //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//thanks, quake III
float rsqrt( float number )
{
  union {
    float f;
    s32 i;
  } conv;
  conv.f = number;
  conv.i  = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
  return conv.f;
}

struct int2
{
  int2() {}
  int2( s32 xy ) : x( xy ), y( xy ) {}
  int2( s32 x, s32 y ) : x( x ), y( y ) {}
  union
  {
    s32 elements[2];
    struct
    {
      s32 x;
      s32 y;
    };
  };

  INLINE s32& operator[]( int i ) { return elements[i]; }
  INLINE s32  operator[]( int i ) const { return elements[i]; }

  INLINE int2 friend operator +( int2 const& a, int2 const& b ) { return { a.x + b.x, a.y + b.y }; }
  INLINE int2 friend operator -( int2 const& a, int2 const& b ) { return { a.x - b.x, a.y - b.y }; }
  INLINE int2 friend operator *( s32 f, int2 const& v ) { return { f * v.x, f * v.y }; }
  INLINE int2 friend operator *( int2 const& v, s32 f ) { return { f * v.x, f * v.y }; }

  INLINE int2 const& operator+=( int2 const& other ) { return *this = *this + other; }
  INLINE int2 const& operator-=( int2 const& other ) { return *this = *this - other; }
  INLINE int2 const& operator*=( s32 other ) { return *this = *this * other; }
};
struct int3
{
  int3() {}
  int3( s32 xyz ) : x( xyz ), y( xyz ), z( xyz ) {}
  int3( s32 x, s32 y, s32 z ) : x( x ), y( y ), z( z ) {}
  union
  {
    s32 elements[3];
    struct { s32 x; s32 y; s32 z; };
    int2 xy;
  };

  INLINE s32& operator[]( int i ) { return elements[i]; }
  INLINE s32  operator[]( int i ) const { return elements[i]; }
  INLINE int3	 operator-() { return int3 { -x, -y, -z }; }

  INLINE int3 friend operator +( int3 const& a, int3 const& b ) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
  INLINE int3 friend operator -( int3 const& a, int3 const& b ) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
  INLINE int3 friend operator *( s32 f, int3 const& v ) { return { f * v.x, f * v.y, f * v.z }; }
  INLINE int3 friend operator *( int3 const& v, s32 f ) { return { f * v.x, f * v.y, f * v.z }; }
  INLINE int3 friend operator /( int3 const& v, s32 f ) { return { v.x / f, v.y / f, v.z / f }; }

  INLINE int3 const& operator+=( int3 const& other ) { return *this = *this + other; }
  INLINE int3 const& operator-=( int3 const& other ) { return *this = *this - other; }
  INLINE int3 const& operator*=( s32 other ) { return *this = *this * other; }
};

struct int4
{
  int4() {}
  int4( s32 x, s32 y, s32 z, s32 w ) : x( x ), y( y ), z( z ), w( w ) {}
  int4( int3 const& v3, s32 w ) : x( v3.x ), y( v3.y ), z( v3.z ), w( w ) {}
  int4( s32 xyzw ) : x( xyzw ), y( xyzw ), z( xyzw ), w( xyzw ) {}
  union
  {
    s32 elements[4];
    struct {
      s32 x;
      union { struct { s32 y; s32 z; s32 w; }; int3 yzw; };
    };
    int3 xyz;
  };

  INLINE s32& operator[]( int i ) { return elements[i]; }
  INLINE s32  operator[]( int i ) const { return elements[i]; }

  INLINE int4 friend operator +( int4 const& a, int3 const& b ) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w }; }
  INLINE int4 friend operator +( int4 const& a, int4 const& b ) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
  INLINE int4 friend operator -( int4 const& a, int4 const& b ) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
  INLINE int4 friend operator *( s32 f, int4 const& v ) { return { f * v.x, f * v.y, f * v.z, f * v.w }; }
  INLINE int4 friend operator *( int4 const& v, s32 f ) { return { f * v.x, f * v.y, f * v.z, f * v.w }; }
  INLINE int4 friend operator /( int4 const& v, s32 f ) { return { v.x / f, v.y / f, v.z / f, v.w / f }; }

  INLINE int4 const& operator+=( int3 const& other ) { return *this = *this + other; }
  INLINE int4 const& operator+=( int4 const& other ) { return *this = *this + other; }
  INLINE int4 const& operator-=( int4 const& other ) { return *this = *this - other; }
  INLINE int4 const& operator*=( s32 other ) { return *this = *this * other; }
};

struct float2
{
  float2() {}
  float2( float xy ) : x( xy ), y( xy ) {}
  float2( float x, float y ) : x( x ), y( y ) {}
  float2( s32 x, s32 y ) : x( float( x ) ), y( float( y ) ) {}
  float2( int2 const& other ) : x( float( other.x ) ), y( float( other.y ) ) {}

  union
  {
    float elements[2];
    struct { float x; float y; };
  };

  INLINE float& operator[]( int i ) { return elements[i]; }
  INLINE float  operator[]( int i ) const { return elements[i]; }

  INLINE float2 friend operator +( float2 const& a, float2 const& b ) { return { a.x + b.x, a.y + b.y }; }
  INLINE float2 friend operator -( float2 const& a, float2 const& b ) { return { a.x - b.x, a.y - b.y }; }
  INLINE float2 friend operator *( float f, float2 const& v ) { return { f * v.x, f * v.y }; }
  INLINE float2 friend operator *( float2 const& v, float f ) { return { f * v.x, f * v.y }; }
  INLINE float2 friend operator /( float2 const& v, float f ) { f = 1.0f / f; return v * f; }

  INLINE float2        operator+=( float2 const& other ) { return *this = *this + other; }
  INLINE float2        operator-=( float2 const& other ) { return *this = *this - other; }
  INLINE float2        operator*=( float other ) { return *this = *this * other; }

  explicit operator int2() { return { s32( x ), s32( y ) }; }
};

struct float3
{
  float3() {}
  float3( float xyz ) : x( xyz ), y( xyz ), z( xyz ) {}
  float3( float x, float y, float z ) : x( x ), y( y ), z( z ) {}
  union
  {
    float elements[3];
    struct { float x; float y; float z; };
    struct { float r; float g; float b; };
  };

  INLINE float& operator[]( int i ) { return elements[i]; }
  INLINE float  operator[]( int i ) const { return elements[i]; }
  INLINE float3	 operator-() { return float3 { -x, -y, -z }; }

  INLINE float3 friend operator +( float3 const& a, float3 const& b ) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
  INLINE float3 friend operator -( float3 const& a, float3 const& b ) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
  INLINE float3 friend operator *( float f, float3 const& v ) { return { f * v.x, f * v.y, f * v.z }; }
  INLINE float3 friend operator *( float3 const& v, float f ) { return { f * v.x, f * v.y, f * v.z }; }
  INLINE float3 friend operator /( float3 const& v, float f ) { f = 1.0f / f; return v * f; }

  INLINE float3 const& operator+=( float3 const& other ) { return *this = *this + other; }
  INLINE float3 const& operator-=( float3 const& other ) { return *this = *this - other; }
  INLINE float3 const& operator*=( float other ) { return *this = *this * other; }
};

struct float4
{
  float4() {}
  float4( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) {}
  float4( float3 const& v3, float w ) : x( v3.x ), y( v3.y ), z( v3.z ), w( w ) {}
  float4( float xyzw ) : x( xyzw ), y( xyzw ), z( xyzw ), w( xyzw ) {}
  union
  {
    float elements[4];
    struct { float x; float y; float z; float w; };
    struct { float r; float g; float b; float a; };
    float3 xyz;
  };

  INLINE float& operator[]( int i ) { return elements[i]; }
  INLINE float  operator[]( int i ) const { return elements[i]; }

  INLINE float4 friend operator +( float4 const& a, float3 const& b ) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w }; }
  INLINE float4 friend operator +( float4 const& a, float4 const& b ) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
  INLINE float4 friend operator -( float4 const& a, float4 const& b ) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
  INLINE float4 friend operator *( float f, float4 const& v ) { return { f * v.x, f * v.y, f * v.z, f * v.w }; }
  INLINE float4 friend operator *( float4 const& v, float f ) { return { f * v.x, f * v.y, f * v.z, f * v.w }; }
  INLINE float4 friend operator /( float4 const& v, float f ) { f = 1.0f / f; return v * f; }

  INLINE float4 const& operator+=( float3 const& other ) { return *this = *this + other; }
  INLINE float4 const& operator+=( float4 const& other ) { return *this = *this + other; }
  INLINE float4 const& operator-=( float4 const& other ) { return *this = *this - other; }
  INLINE float4 const& operator*=( float other ) { return *this = *this * other; }
};
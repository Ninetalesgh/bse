#pragma once

#include <memory>
#include <initializer_list>
#include <cmath>

#define BSE_RENDERER_VULKAN
//#define BSE_RENDERER_OPENGL

#if !defined (BSE_PLATFORM_WINDOWS) && !defined (BSE_PLATFORM_ANDROID) && !defined (BSE_PLATFORM_LINUX)
#error "Please define either BSE_PLATFORM_WINDOWS, BSE_PLATFORM_LINUX or BSE_PLATFORM_ANDROID"
#endif

#if defined(BSE_BUILD_DEBUG) || defined(BSE_BUILD_DEVELOPMENT)
#  define BSE_BUILD_DEBUG_DEVELOPMENT
#endif

#if defined(_MSC_VER)
# define BSE_COMPILER_MSVC
#endif

#if defined(__clang__)
# define BSE_COMPILER_CLANG
#endif

#define BSE_FUNCTION_NAME __func__
#define BSE_FUNCTION_NAME_NAMESPACE __FUNCTION__
#if defined(BSE_COMPILER_MSVC)
# define BSE_FUNCTION_SIGNATURE __FUNCSIG__
#else
# define BSE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

#if defined(__x86_64__) || defined(_M_X64)
# define BSE_ARCHITECTURE_X64 
#endif

#if defined(__i386__) || defined(_M_IX86)
# define BSE_ARCHITECTURE_X86 
#endif

#if defined(__arm__)
# define BSE_ARCHITECTURE_ARM_V7
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
# define BSE_ARCHITECTURE_ARM_V8
#endif

#if defined(_M_CEE)
#error "No please. no. Not this again. Don't use CLI."
#endif

#if defined(BSE_ARCHITECTURE_X86)
# if defined (__clang__)
#   define BSE_CALLING_CONVENTION_THISCALL __attribute__((thiscall))
# elif defined(BSE_COMPILER_MSVC)
#   define BSE_CALLING_CONVENTION_THISCALL __thiscall
# endif
#else 
# define BSE_CALLING_CONVENTION_THISCALL
#endif

#if defined(BSE_ARCHITECTURE_X86)
# if defined (__clang__)
#   define BSE_CALLING_CONVENTION_STDCALL __attribute__((stdcall))
# elif defined(BSE_COMPILER_MSVC)
#   define BSE_CALLING_CONVENTION_STDCALL __stdcall
# endif
#else 
# define BSE_CALLING_CONVENTION_STDCALL
#endif

#if !defined(BSE_ARCHITECTURE_ARM_V7)
# define BSE_CALLING_CONVENTION_CDECL __cdecl
#else 
# define BSE_CALLING_CONVENTION_CDECL 
#endif

#if defined(__clang__)
# define BSE_CALLING_CONVENTION_FASTCALL __attribute__((fastcall))
#elif defined (BSE_COMPILER_MSVC)
# define BSE_CALLING_CONVENTION_FASTCALL __fastcall
#endif

#if defined(BSE_COMPILER_MSVC)
# define INLINE __forceinline
#else
# define INLINE __attribute__((always_inline))
#endif

#if defined(BSE_ARCHITECTURE_X86)
# define BSEAPI __stdcall
#else 
# define BSEAPI
#endif 

#define BSE_STRINGIZE(s) _BSE_STRINGIZE_HELPER(s)
#define _BSE_STRINGIZE_HELPER(s) #s

#define array_count(array) (sizeof(array) / (sizeof((array)[0])))

#define BSE_STACK_BUFFER_SMALL 4096
#define BSE_STACK_BUFFER_MEDIUM 8192
#define BSE_STACK_BUFFER_LARGE 16384

#define _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, basictype)\
INLINE enumtypename  operator |  ( enumtypename a, enumtypename b ) { return enumtypename( basictype( a ) | basictype( b ) ); }\
INLINE enumtypename& operator |= ( enumtypename& a, enumtypename b ) { a = b | a; return a; }\
INLINE enumtypename  operator &  ( enumtypename a, enumtypename b ) { return enumtypename( basictype( a ) & basictype( b ) ); }\
INLINE enumtypename& operator &= ( enumtypename& a, enumtypename b ) { a = b & a; return a; }\
INLINE enumtypename  operator ~  ( enumtypename a ) { return enumtypename( ~basictype( a ) ); }\
INLINE enumtypename  operator ^  ( enumtypename a, enumtypename b ) { return enumtypename( basictype( a ) ^ basictype( b ) ); }\
INLINE enumtypename& operator ^= ( enumtypename& a, enumtypename b ) { a = b ^ a; return a; }\
INLINE bool flags_contain(enumtypename a, enumtypename b) { return (basictype(a) & basictype(b)) == basictype(b);}

#define BSE_DEFINE_ENUM_OPERATORS_U8(enumtypename)  _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u8)
#define BSE_DEFINE_ENUM_OPERATORS_U16(enumtypename) _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u16)
#define BSE_DEFINE_ENUM_OPERATORS_U32(enumtypename) _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u32)
#define BSE_DEFINE_ENUM_OPERATORS_U64(enumtypename) _DEFINE_ENUM_OPERATORS_INTERNAL(enumtypename, u64)

template <typename T> struct _BSE_ALWAYS_FALSE { constexpr static bool value = false; };
#define BSE_ALWAYS_FALSE(T) _BSE_ALWAYS_FALSE<T>::value

using s8  = signed char;
using s16 = short;
using s32 = int;
using s64 = long long;
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using void_fn = void();
constexpr s32 sizeofptr = sizeof( void* );

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

constexpr INLINE u8    min( u8 a, u8 b ) { return a < b ? a : b; }
constexpr INLINE u16   min( u16 a, u16 b ) { return a < b ? a : b; }
constexpr INLINE u32   min( u32 a, u32 b ) { return a < b ? a : b; }
constexpr INLINE u64   min( u64 a, u64 b ) { return a < b ? a : b; }
constexpr INLINE s8    min( s8 a, s8 b ) { return a < b ? a : b; }
constexpr INLINE s16   min( s16 a, s16 b ) { return a < b ? a : b; }
constexpr INLINE s32   min( s32 a, s32 b ) { return a < b ? a : b; }
constexpr INLINE s64   min( s64 a, s64 b ) { return a < b ? a : b; }
constexpr INLINE float min( float a, float b ) { return a < b ? a : b; }
constexpr INLINE void* min( void* a, void* b ) { return a < b ? a : b; }

constexpr INLINE u8    max( u8 a, u8 b ) { return a > b ? a : b; }
constexpr INLINE u16   max( u16 a, u16 b ) { return a > b ? a : b; }
constexpr INLINE u32   max( u32 a, u32 b ) { return a > b ? a : b; }
constexpr INLINE u64   max( u64 a, u64 b ) { return a > b ? a : b; }
constexpr INLINE s8    max( s8 a, s8 b ) { return a > b ? a : b; }
constexpr INLINE s16   max( s16 a, s16 b ) { return a > b ? a : b; }
constexpr INLINE s32   max( s32 a, s32 b ) { return a > b ? a : b; }
constexpr INLINE s64   max( s64 a, s64 b ) { return a > b ? a : b; }
constexpr INLINE float max( float a, float b ) { return a > b ? a : b; }
constexpr INLINE void* max( void* a, void* b ) { return a > b ? a : b; }

constexpr INLINE u8    clamp( u8 value, u8 low, u8 high ) { return max( min( value, high ), low ); }
constexpr INLINE u16   clamp( u16 value, u16 low, u16 high ) { return max( min( value, high ), low ); }
constexpr INLINE u32   clamp( u32 value, u32 low, u32 high ) { return max( min( value, high ), low ); }
constexpr INLINE u64   clamp( u64 value, u64 low, u64 high ) { return max( min( value, high ), low ); }
constexpr INLINE s8    clamp( s8 value, s8 low, s8 high ) { return max( min( value, high ), low ); }
constexpr INLINE s16   clamp( s16 value, s16 low, s16 high ) { return max( min( value, high ), low ); }
constexpr INLINE s32   clamp( s32 value, s32 low, s32 high ) { return max( min( value, high ), low ); }
constexpr INLINE s64   clamp( s64 value, s64 low, s64 high ) { return max( min( value, high ), low ); }
constexpr INLINE float clamp( float value, float low, float high ) { return max( min( value, high ), low ); }

constexpr INLINE s64 KiloBytes( s64 kiloBytes ) { return kiloBytes * 1024LL; }
constexpr INLINE s64 MegaBytes( s64 megaBytes ) { return KiloBytes( megaBytes ) * 1024LL; }
constexpr INLINE s64 GigaBytes( s64 gigaBytes ) { return MegaBytes( gigaBytes ) * 1024LL; }

#if defined(BSE_PLATFORM_WINDOWS)
constexpr INLINE bool is_negative( float x ) { return (*(u32*) &x) & 0x80000000; }
constexpr INLINE bool sign_match( float a, float b ) { return ((*(s32*) &a) ^ (*(s32*) &b)) >= 0; }
#elif defined(BSE_PLATFORM_ANDROID) || defined(BSE_PLATFORM_LINUX)
constexpr INLINE bool is_negative( float x ) { return std::signbit( x ); }
constexpr INLINE bool sign_match( float a, float b ) { return std::signbit( a ) == std::signbit( b ); }
#endif

constexpr INLINE bool sign_match( s32 a, s32 b ) { return (a ^ b) >= 0; }
constexpr INLINE bool is_negative( s32 x ) { return x < 0; }
constexpr INLINE s32   get_sign( s32 x ) { return x < 0 ? -1 : 1; }
constexpr INLINE float get_sign( float x ) { return x < 0.0f ? -1.0f : 1.0f; }
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

  INLINE s32& operator[]( s32 i ) { return elements[i]; }
  INLINE s32  operator[]( s32 i ) const { return elements[i]; }

  INLINE int2 friend operator +( int2 const& a, int2 const& b ) { return { a.x + b.x, a.y + b.y }; }
  INLINE int2 friend operator -( int2 const& a, int2 const& b ) { return { a.x - b.x, a.y - b.y }; }
  INLINE int2 friend operator *( int2 const& a, int2 const& b ) { return { a.x * b.x, a.y * b.y }; }
  INLINE int2 friend operator /( int2 const& a, int2 const& b ) { return { a.x / b.x, a.y / b.y }; }
  INLINE int2 friend operator *( s32 f, int2 const& v ) { return { f * v.x, f * v.y }; }
  INLINE int2 friend operator *( int2 const& v, s32 f ) { return { f * v.x, f * v.y }; }
  INLINE int2 friend operator /( int2 const& v, s32 f ) { return { v.x / f, v.y / f }; }

  INLINE int2 const& operator+=( int2 const& other ) { return *this = *this + other; }
  INLINE int2 const& operator-=( int2 const& other ) { return *this = *this - other; }
  INLINE int2 const& operator*=( s32 other ) { return *this = *this * other; }

  INLINE bool friend operator ==( int2 const& a, int2 const& b ) { return (a.x == b.x) && (a.y == b.y); };
  INLINE bool friend operator !=( int2 const& a, int2 const& b ) { return (a.x != b.x) && (a.y != b.y); };
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

  INLINE s32& operator[]( s32 i ) { return elements[i]; }
  INLINE s32  operator[]( s32 i ) const { return elements[i]; }
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

  INLINE s32& operator[]( s32 i ) { return elements[i]; }
  INLINE s32  operator[]( s32 i ) const { return elements[i]; }

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

  INLINE float& operator[]( s32 i ) { return elements[i]; }
  INLINE float  operator[]( s32 i ) const { return elements[i]; }

  INLINE float2 friend operator +( float2 const& a, float2 const& b ) { return { a.x + b.x, a.y + b.y }; }
  INLINE float2 friend operator -( float2 const& a, float2 const& b ) { return { a.x - b.x, a.y - b.y }; }
  INLINE float2 friend operator *( float2 const& a, float2 const& b ) { return { a.x * b.x, a.y * b.y }; }
  INLINE float2 friend operator /( float2 const& a, float2 const& b ) { return { a.x / b.x, a.y / b.y }; }

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

  INLINE float& operator[]( s32 i ) { return elements[i]; }
  INLINE float  operator[]( s32 i ) const { return elements[i]; }
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

  INLINE float& operator[]( s32 i ) { return elements[i]; }
  INLINE float  operator[]( s32 i ) const { return elements[i]; }

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

int2   min( int2 const& a, int2 const& b ) { return int2 { min( a.x,b.x ),min( a.y,b.y ) }; }
float2 min( float2 const& a, float2 const& b ) { return float2 { min( a.x,b.x ),min( a.y,b.y ) }; }
int3   min( int3 const& a, int3 const& b ) { return int3 { min( a.x,b.x ),min( a.y,b.y ),min( a.z,b.z ), }; }
float3 min( float3 const& a, float3 const& b ) { return float3 { min( a.x,b.x ),min( a.y,b.y ),min( a.z,b.z ), }; }
int4   min( int4 const& a, int4 const& b ) { return int4 { min( a.x,b.x ),min( a.y,b.y ),min( a.z,b.z ),min( a.w,b.w ) }; }
float4 min( float4 const& a, float4 const& b ) { return float4 { min( a.x,b.x ),min( a.y,b.y ),min( a.z,b.z ),min( a.w,b.w ) }; }
int2   max( int2 const& a, int2 const& b ) { return int2 { max( a.x,b.x ),max( a.y,b.y ) }; }
float2 max( float2 const& a, float2 const& b ) { return float2 { max( a.x,b.x ),max( a.y,b.y ) }; }
int3   max( int3 const& a, int3 const& b ) { return int3 { max( a.x,b.x ),max( a.y,b.y ),max( a.z,b.z ), }; }
float3 max( float3 const& a, float3 const& b ) { return float3 { max( a.x,b.x ),max( a.y,b.y ),max( a.z,b.z ), }; }
int4   max( int4 const& a, int4 const& b ) { return int4 { max( a.x,b.x ),max( a.y,b.y ),max( a.z,b.z ),max( a.w,b.w ) }; }
float4 max( float4 const& a, float4 const& b ) { return float4 { max( a.x,b.x ),max( a.y,b.y ),max( a.z,b.z ),max( a.w,b.w ) }; }
int2   clamp( int2 const& value, int2 const& low, int2 const& high ) { return max( min( value, high ), low ); }
float2 clamp( float2 const& value, float2 const& low, float2 const& high ) { return max( min( value, high ), low ); }
int3   clamp( int3 const& value, int3 const& low, int3 const& high ) { return max( min( value, high ), low ); }
float3 clamp( float3 const& value, float3 const& low, float3 const& high ) { return max( min( value, high ), low ); }
int4   clamp( int4 const& value, int4 const& low, int4 const& high ) { return max( min( value, high ), low ); }
float4 clamp( float4 const& value, float4 const& low, float4 const& high ) { return max( min( value, high ), low ); }


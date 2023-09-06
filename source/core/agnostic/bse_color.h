#pragma once

#include "bse_common.h"

namespace color
{
  constexpr u32 WHITE      = 0xffffffff;
  constexpr u32 BLACK      = 0xff000000;
  constexpr u32 DARK_GRAY  = 0xff1e1e1e;
  constexpr u32 LIGHT_GRAY = 0xffaaaaaa;
  constexpr u32 RED        = 0xff0000ff;
  constexpr u32 GREEN      = 0xff00ff00;
  constexpr u32 BLUE       = 0xffff0000;
  constexpr u32 TEAL       = 0xffffff00;
  constexpr u32 PURPLE     = 0xffff00ff;
  constexpr u32 YELLOW     = 0xff00ffff;

  constexpr u32 MASK_ALPHA = 0xff000000;
  constexpr u32 MASK_RED   = 0x000000ff;
  constexpr u32 MASK_GREEN = 0x0000ff00;
  constexpr u32 MASK_BLUE  = 0x00ff0000;

  INLINE u32 rgba( u8 r, u8 g, u8 b, u8 a ) { return (u32) (a << 24) | (u32) (b << 16) | (u32) (g << 8) | r; }
  INLINE u32 rgb( u8 r, u8 g, u8 b ) { return (u32) (b << 16) | (u32) (g << 8) | r; }

  INLINE u8 get_red( u32 color ) { return u8( color & MASK_RED ); }
  INLINE u8 get_green( u32 color ) { return u8( (color & MASK_GREEN) >> 8 ); }
  INLINE u8 get_blue( u32 color ) { return u8( (color & MASK_BLUE) >> 16 ); }
  INLINE u8 get_alpha( u32 color ) { return u8( (color & MASK_ALPHA) >> 24 ); }

  INLINE float4 float4_from_rgba( u32 color ) { return float4 { (float) get_red( color ),(float) get_green( color ),(float) get_blue( color ),(float) get_alpha( color ) } / 255.0f; }

  INLINE float get_alpha_float( u32 color ) { return float( (color & MASK_ALPHA) >> 24 ) / 255.0f; }

  INLINE u32 srgba_to_rgba( u32 color )
  {
    float4 v = float4_from_rgba( color );

    u8 r = u8( 255.0f * v.r * v.r );
    u8 g = u8( 255.0f * v.g * v.g );
    u8 b = u8( 255.0f * v.b * v.b );
    u8 a = u8( 255.0f * v.a );
    return (u32) (a << 24) | (u32) (b << 16) | (u32) (g << 8) | r;
  }

  INLINE u32 interpolate( float t, u32 colorA, u32 colorB )
  {
    t = clamp( t, 0.0f, 1.0f );

    float const ra = float( colorA & MASK_RED );
    float const rb = float( colorB & MASK_RED );
    u8 const r = u8( rb * t + ra * (1.0f - t) );

    float const ga = float( (colorA & MASK_GREEN) >> 8 );
    float const gb = float( (colorB & MASK_GREEN) >> 8 );
    u8 const g = u8( gb * t + ga * (1.0f - t) );

    float const ba = float( (colorA & MASK_BLUE) >> 16 );
    float const bb = float( (colorB & MASK_BLUE) >> 16 );
    u8 const b = u8( bb * t + ba * (1.0f - t) );

    float const aa = float( (colorA & MASK_ALPHA) >> 24 );
    float const ab = float( (colorB & MASK_ALPHA) >> 24 );
    u8 const a = u8( ab * t + aa * (1.0f - t) );

    return (u32( a ) << 24) | (u32( b ) << 16) | (u32( g ) << 8) | u32( r );
  }
};
#pragma once

#include "bse_common.h"

namespace bse
{
  struct Bitmap
  {
    Bitmap() :pixel( 0 ), width( 0 ), height( 0 ) {}
    u32* pixel;
    union
    {
      struct
      {
        s32 width;
        s32 height;
      };
      int2 dimensions;
    };
  };

  struct Texture
  {
    Texture() : width( 0 ), height( 0 ), channels( 0 ), gpuHandle( 0 ) {}
    union
    {
      struct
      {
        s32 width;
        s32 height;
      };
      int2 dimensions;
    };
    u32 channels;
    u32 gpuHandle;
  };
};
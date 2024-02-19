#pragma once

#include "bse_opengl.h"
#include "bse_container.h"

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

  using VertexBufferGPUHandle  = u32;
  using UVBufferGPUHandle      = u32;
  using IndexBufferGPUHandle   = u32;
  using ShaderProgramGPUHandle = u32;
  using MeshGPUHandle          = u32;
  using TextureGPUHandle       = u32;

  enum class TextureFormat : u32
  {
    INVALID = 0,
    RGB8 = 0x8051,
    RGBA8 = 0x8058,
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
    TextureGPUHandle gpuHandle;
  };

  struct TextureData
  {
    u8* pixel;
    TextureFormat format;
    s32 width;
    s32 height;
  };

  enum class IndexFormat : u32
  {
    INVALID = 0,
    U16 = 1,
    U32 = 2,
  };

  struct MeshData
  {
    float3* vertices;
    float2* uvs;
    void* indices;
    u32 vertexCount;
    u32 indexCount;
    IndexFormat indexFormat;
  };

  struct Mesh
  {
    VertexBufferGPUHandle vertexBuffer;
    UVBufferGPUHandle uvBuffer;
    IndexBufferGPUHandle indexBuffer;
    u32 indexCount;
    IndexFormat indexFormat;
    MeshGPUHandle id;
  };

  struct ShaderProgram
  {
    ShaderProgramGPUHandle id;
    bse::Map<bse::String, u32> uniforms;
    bse::Map<bse::String, u32> attributes;

    u32 get_attribute_handle( bse::String const& name ) { return attributes[name]; }
    u32 get_uniform_handle( bse::String const& name ) { return uniforms[name]; }
  };

  ShaderProgram load_shader( char const* filePath );
};

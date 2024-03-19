#pragma once

#if defined(BSE_RENDERER_OPENGL)
# include "bse_opengl.h"
#elif defined(BSE_RENDERER_VULKAN)
# include "bse_vulkan.h"
#endif

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
  constexpr u32 INVALID_GPU_HANDLE = U32_MAX;

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

    // template <typename T>
    // void set_uniform( bse::String const& name, T const* elements, u32 elementCount = 1 )
    // {
    //   u32 handle = get_uniform_handle( name );

    //   if ( handle != INVALID_GPU_HANDLE )
    //   {
    //     opengl::set_uniform( handle, elements, elementCount );
    //   }
    //   else
    //   {
    //     //uniform not found TODO
    //   }
    // }

    // u32 get_attribute_handle( bse::String const& name )
    // {
    //   auto result = attributes.find( name );
    //   return result == attributes.end() ? INVALID_GPU_HANDLE : result->second;
    // }

    // u32 get_uniform_handle( bse::String const& name )
    // {
    //   auto result = uniforms.find( name );
    //   return result == uniforms.end() ? INVALID_GPU_HANDLE : result->second;
    // }
  };

  //TODO make this less object oriented probably :S 
  class IndexBuffer
  {
  public:
    u32 handle;
    u32 count;
    IndexFormat format;

    // IndexBuffer() : handle( 0 ), count( 0 ), format( IndexFormat::INVALID ) {}
    // ~IndexBuffer() { if ( handle ) { glDeleteBuffers( 1, &handle ); } }

    // void set_data( u32 const* elements, s32 elementCount )
    // {
    //   count = elementCount;
    //   format = IndexFormat::U32;
    //   constexpr u32 size = sizeof( u32 );
    //   //if ( !handle ) { glCreateBuffers( 1, &handle ); }
    //   //glNamedBufferData( handle, size * count, elements, GL_STREAM_DRAW );

    //   if ( !handle ) { glGenBuffers( 1, &handle ); }
    //   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
    //   glBufferData( GL_ELEMENT_ARRAY_BUFFER, size * count, elements, GL_STREAM_DRAW );
    //   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    // }
    // void set_data( u16 const* elements, s32 elementCount )
    // {
    //   count = elementCount;
    //   format = IndexFormat::U16;
    //   constexpr u32 size = sizeof( u16 );
    //   if ( !handle ) { glGenBuffers( 1, &handle ); }
    //   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
    //   glBufferData( GL_ELEMENT_ARRAY_BUFFER, size * count, elements, GL_STREAM_DRAW );
    //   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    // }
    // void set_data( bse::Vector<u32> const& elements ) { set_data( elements.data(), s32( elements.size() ) ); }
    // void set_data( bse::Vector<u16> const& elements ) { set_data( elements.data(), s32( elements.size() ) ); }
  };

  [[nodiscard]]
  ShaderProgram load_shader( char const* filePath );
};

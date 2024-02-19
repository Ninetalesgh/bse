#include "bse_core.h"


//vertexPosition_modelspace
//vertexColor


template <s32 byteCount>
struct FixedSizeString
{

  char const* operator() { return data; }
  char* operator =( char const* str )
  {
    string_copy( data, str, byteCount - 1 );
    data[byteCount - 1] = '\0';
    return data;
  }

  bool operator ==( FixedSizeString<byteCount> const& other )
  {
    return string_match( data, other.data );
  }

  char data[byteCount];
};

using String16 = FixedSizeString<16>;
using String32 = FixedSizeString<32>;




namespace global
{


};

INLINE u32    get_size_for_index_format( bse::IndexFormat indexFormat ) { return indexFormat == bse::IndexFormat::U16 ? sizeof( u16 ) : sizeof( u32 ); }
INLINE GLenum get_gl_index_format( bse::IndexFormat indexFormat ) { return indexFormat == bse::IndexFormat::U16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT; }

// Mesh allocate_mesh( MeshData const* raw )
// {
//   Mesh resultMesh {};

//   u32 const vertexCount = raw->vertexCount;
//   u32 const indexCount = raw->indexCount;
//   IndexFormat const indexFormat = raw->indexFormat;

//   if ( indexFormat == IndexFormat::INVALID ) BREAK;

//   glCreateBuffers( 1, &resultMesh.vertexBuffer );
//   glNamedBufferData( resultMesh.vertexBuffer, vertexCount * sizeof( float3 ), raw->vertices, GL_STATIC_DRAW );

//   glCreateBuffers( 1, &resultMesh.uvBuffer );
//   glNamedBufferData( resultMesh.uvBuffer, vertexCount * sizeof( float2 ), raw->uvs, GL_STATIC_DRAW );

//   glCreateBuffers( 1, &resultMesh.indexBuffer );
//   glNamedBufferData( resultMesh.indexBuffer, indexCount * get_size_for_index_format( indexFormat ), raw->indices, GL_STATIC_DRAW );

//   resultMesh.indexCount = indexCount;
//   resultMesh.indexFormat = indexFormat;
//   return resultMesh;
// }

static float3 vertexBufferData[] =
{
  {-0.5f,-0.5f, 0.0f},
  { 0.5f,-0.5f, 0.0f},
  { 0.0f, 0.5f, 0.0f},
};

static u16 indexBufferData[] =
{
  0,1,2
};

bse::VertexBufferGPUHandle vb;
bse::IndexBufferGPUHandle ib;

bse::MeshGPUHandle make_square()
{
  using namespace bse;
  // static float3 vertexBufferData[] =
  // {
  //   {-1.0f,-1.0f, 0.4f}, // triangle 1 : begin
  //   {-1.0f, 1.0f, 0.4f},
  //   { 1.0f,-1.0f, 0.4f}, // triangle 1 : end
  //   { 1.0f, 1.0f, 0.4f} // triangle 2 : end
  // };

  // static u16 indexBufferData[] =
  // {
  //   0,2,1,
  //   2,3,1
  // };


  MeshGPUHandle mesh;

  glCreateBuffers( 1, &vb );
  glNamedBufferData( vb, array_count( vertexBufferData ) * sizeof( float3 ), vertexBufferData, GL_STATIC_DRAW );

  opengl::check_gl_error();

  glCreateBuffers( 1, &ib );
  glNamedBufferData( vb, array_count( indexBufferData ) * get_size_for_index_format( IndexFormat::U16 ), indexBufferData, GL_STATIC_DRAW );
  opengl::check_gl_error();

  glGenVertexArrays( 1, &mesh );
  glBindVertexArray( mesh );
  opengl::check_gl_error();

  glBindBuffer( GL_ARRAY_BUFFER, vb );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ib );
  opengl::check_gl_error();

  glBindVertexArray( 0 );
  glDisableVertexAttribArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

  opengl::check_gl_error();

  // MeshData testMeshData {};
  // testMeshData.vertices = vertexBufferData;
  // //testMeshData.uvs = g_uv_buffer_data;
  // testMeshData.indices = (void*) indexBufferData;
  // testMeshData.vertexCount = sizeof( vertexBufferData );
  // testMeshData.indexCount = sizeof( indexBufferData );
  // testMeshData.indexFormat = IndexFormat::U16;

  // Mesh result = opengl::allocate_mesh( &testMeshData );
  // opengl::init_mesh_vao( &result );
  return mesh;
}

struct AppData
{
  AppData() {}

  bse::MeshGPUHandle mesh;
  bse::ShaderProgram shader;
};

AppData& app()
{
  void*& ptr = bse::platform->userData;
  if ( !ptr )
  {
    ptr = bse::memory::allocate_thread_safe( sizeof( AppData ) );
    memset( ptr, 0, sizeof( AppData ) );
    AppData* result = new (ptr) AppData();
  }

  return *((AppData*) ptr);
}


void render()
{

  glUseProgram( app().shader.id );
  //glBindVertexArray( app().mesh );

  glBindBuffer( GL_ARRAY_BUFFER, vb );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ib );

  glDrawElements( GL_TRIANGLES, array_count( indexBufferData ), get_gl_index_format( bse::IndexFormat::U16 ), (void*) 0 );
  // opengl::check_gl_error();

  glBindVertexArray( 0 );

}

void initialize( bse::PlatformInitParams* initParameters )
{
  //your initialization goes here

}

void on_reload()
{
  app().shader = bse::load_shader( "../../../app/pong/basic.glsl" );
  app().mesh = make_square();

  //your on reload goes here, relevant for hot reloading
}

void tick()
{
  render();

  //your per frame update goes here
}


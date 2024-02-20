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


static float3 vertexBufferData[] =
{
  {-0.5f,-0.5f, 0.0f},
  { 0.5f,-0.5f, 0.0f},
  {-0.5f, 0.5f, 0.0f},
  { 0.5f, 0.5f, 0.0f},
};

static u16 indexBufferData[] =
{
  0,1,2,
  2,3,1
};

bse::VertexBufferGPUHandle vb;
bse::IndexBufferGPUHandle ib;

bse::MeshGPUHandle make_square()
{
  using namespace bse;

  MeshGPUHandle mesh;

  glCreateBuffers( 1, &vb );
  glNamedBufferData( vb, array_count( vertexBufferData ) * sizeof( float3 ), vertexBufferData, GL_STATIC_DRAW );

  opengl::check_gl_error();

  glCreateBuffers( 1, &ib );
  glNamedBufferData( ib, array_count( indexBufferData ) * get_size_for_index_format( IndexFormat::U16 ), indexBufferData, GL_STATIC_DRAW );
  opengl::check_gl_error();

  //return mesh;

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

  bse::Mesh cube;
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

  glBindVertexArray( app().mesh );
  glDrawElements( GL_TRIANGLES, array_count( indexBufferData ), get_gl_index_format( bse::IndexFormat::U16 ), (void*) 0 );

  glBindVertexArray( 0 );

  return;
}

void initialize( bse::PlatformInitParams* initParameters )
{
  //your initialization goes here

}

void on_reload()
{
  app().shader = bse::load_shader( "../../../app/pong/basic.glsl" );
  app().mesh = make_square();
  //app().cube = load_cube();

  //your on reload goes here, relevant for hot reloading
}



void tick()
{
  render();

  //your per frame update goes here
}


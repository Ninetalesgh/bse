#include "bse_core.h"


//vertexPosition_modelspace
//vertexColor



INLINE float2 screen_to_ndc( int2 const& screen ) { return float2 { -1.0f, 1.0f } + 2.0f * float2 { (float) screen.x / (float) bse::platform->settings.windowSize.x, -(float) screen.y / (float) bse::platform->settings.windowSize.y }; }
INLINE int2   ndc_to_screen( float2 const& ndc ) { return int2 { (s32) (0.5f + (ndc.x + 1.0f) * (float) bse::platform->settings.windowSize.x * 0.5f), (s32) (0.5f - (ndc.y - 1.0f) * (float) bse::platform->settings.windowSize.y * 0.5f) }; }
INLINE u32    get_size_for_index_format( bse::IndexFormat indexFormat ) { return indexFormat == bse::IndexFormat::U16 ? sizeof( u16 ) : sizeof( u32 ); }
INLINE GLenum get_gl_index_format( bse::IndexFormat indexFormat ) { return indexFormat == bse::IndexFormat::U16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT; }


INLINE float3 ndc_to_world( float3 const& ndc, float4 const& projector )
{
  float const x0 = projector.w / (projector.z - ndc.z);
  float const y0 = x0 * ndc.y / projector.y;
  float const z0 = x0 * ndc.x / projector.x;
  return float3 { x0, y0, z0 };
}

INLINE bse::line ndc_to_world_line( float2 const& ndc, float4 const& projector )
{
  float const f = projector.w * 0.5f * (((projector.z + 1.0f) / (projector.z - 1.0f)) - 1.0f);
  float const n = f * (projector.z - 1.0f) / (projector.z + 1.0f);

  float const y0 = n * ndc.y / projector.y;
  float const z0 = n * ndc.x / projector.x;
  float const y1 = f * ndc.y / projector.y;
  float const z1 = f * ndc.x / projector.x;

  bse::point p0 { n,y0,z0 };
  bse::point p1 { f,y1,z1 };

  return p0 & p1;
}

static float3 vertexBufferData[] =
{
  { 0.0f, 0.0f, 0.0f},
  { 0.0f, 0.0f, 1.0f},
  { 0.0f, 1.0f, 0.0f},
  { 0.0f, 1.0f, 1.0f},
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
  float4 projector;
  float3 translator;
  float4 rotor;
};

extern AppData* appData;

struct Shape
{
  float2 pos;
  float2 scale;
  float4 color;
};

Shape shape1;
Shape shape2;
Shape projectile;

void render_shape( Shape const& shape )
{
  appData->shader.set_uniform( "projector", &appData->projector );
  float3 scale { 1.0f, shape.scale.y, shape.scale.x };
  appData->shader.set_uniform( "scale", &scale );
  float3 pos { 1.0f, shape.pos.y, shape.pos.x };
  appData->shader.set_uniform( "position", &pos );
  appData->shader.set_uniform( "objectColor", &shape.color );
  glDrawElements( GL_TRIANGLES, array_count( indexBufferData ), get_gl_index_format( bse::IndexFormat::U16 ), (void*) 0 );
}

void render()
{
  using namespace bse;
  glUseProgram( appData->shader.id );
  glBindVertexArray( appData->mesh );

  render_shape( shape1 );
  render_shape( shape2 );
  render_shape( projectile );

  glBindVertexArray( 0 );

  return;
}

void initialize_platform( bse::PlatformInitParams* initParameters )
{
  //your initialization goes here
}

void initialize_app_data()
{
  //called once after the core is fully initialized, but not on hot reload
  appData->shader = bse::load_shader( "../../../app/pong/basic.glsl" );
  appData->mesh = make_square();


  shape1.pos = { 0.5f, 0.0f };
  shape1.scale = { 0.1f, 0.5f };
  shape1.color = { 1.0f, 0.5f, 1.0f, 1.0f };

  shape2.pos = { -0.5f, 0.0f };
  shape2.scale = { 0.1f, 0.5f };
  shape2.color = { 0.5f, 1.0f, 0.7f, 1.0f };

  projectile.pos = { -0.0f, 0.0f };
  projectile.scale = { 0.05f, 0.05f };
  projectile.color = { 0.2f, 1.0f, 0.2f, 1.0f };
}

void on_reload()
{
  bse::platform->settings.fpsCap = 60.0f;
}

bool float_compare_equal( float a, float b, float e = FLOAT_E )
{
  return abs( a - b ) < e;
}

void tick()
{
  appData->projector = bse::create_viewport_projector( (float) bse::platform->settings.windowSize.x, (float) bse::platform->settings.windowSize.y, to_radians( 90.0f ), 0.10f, 2.0f );
  render();


  {
    float4 test = bse::project_positive_x_to_viewport( float3 { 1.0f, shape1.pos.y, shape1.pos.x }, appData->projector );

    float3 test2 = ndc_to_world( test.xyz, appData->projector );
    float2 t1 = shape1.pos;
    float2 t2 = float2 { test2.z, test2.y };

    if ( !float_compare_equal( t1.x, t2.x, 10.0f * FLOAT_E ) && !float_compare_equal( t1.y, t2.y, 10.0f * FLOAT_E ) )
    {
      BREAK;
    }
  }

  bse::Input::MousePos mp = bse::platform->thisFrame.input.mousePos[0];

  float2 mpf = screen_to_ndc( mp.start );
  bse::line ray = ndc_to_world_line( mpf, appData->projector );

  bse::plane display { -1.0f, 0.0f, 0.0f, 1.0f };
  bse::point intersection = ray ^ display;

  float3 world = bse::to_float3( intersection );

  shape1.pos = float2 { world.z, world.y };
  int2 what = ndc_to_screen( mpf );




  if ( mp.start != what )
  {
    BREAK;
  }
  log_info( mp.start, " - ", mpf, " - ", what );
  //your per frame update goes here

  //bse::platform->settings.windowSize = int2 { 800,800 };
}


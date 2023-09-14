#include "bse_core.h"

using namespace bse;

struct float4x4
{
  float4x4() {}
  float4x4( float4 const& xAxis, float4 const& yAxis, float4 const& zAxis, float4 const& pos )
    : xAxis( xAxis ), yAxis( yAxis ), zAxis( zAxis ), pos( pos )
  {}

  float4x4( float xAxisx, float xAxisy, float xAxisz, float xAxisw,
            float yAxisx, float yAxisy, float yAxisz, float yAxisw,
            float zAxisx, float zAxisy, float zAxisz, float zAxisw,
            float posx, float posy, float posz, float posw )
    :
    m00( xAxisx ), m01( xAxisy ), m02( xAxisz ), m03( xAxisw ),
    m10( yAxisx ), m11( yAxisy ), m12( yAxisz ), m13( yAxisw ),
    m20( zAxisx ), m21( zAxisy ), m22( zAxisz ), m23( zAxisw ),
    m30( posx ), m31( posy ), m32( posz ), m33( posw )
  {}
  float4x4( float const* floatArray )
    :
    m00( floatArray[0] ), m01( floatArray[1] ), m02( floatArray[2] ), m03( floatArray[3] ),
    m10( floatArray[4] ), m11( floatArray[5] ), m12( floatArray[6] ), m13( floatArray[7] ),
    m20( floatArray[8] ), m21( floatArray[9] ), m22( floatArray[10] ), m23( floatArray[11] ),
    m30( floatArray[12] ), m31( floatArray[13] ), m32( floatArray[14] ), m33( floatArray[15] )
  {}

  union
  {
    struct
    {
      float m00, m01, m02, m03;
      float m10, m11, m12, m13;
      float m20, m21, m22, m23;
      float m30, m31, m32, m33;
    };
    struct { float4 row[4]; };
    struct
    {
      float4 xAxis;
      float4 yAxis;
      float4 zAxis;
      float4 pos;
    };
  };

  INLINE float4  operator[]( s32 i ) const { return row[i]; }
  INLINE float4& operator[]( s32 i ) { return row[i]; }

  INLINE float4x4 operator-() { return float4x4 { -m00, -m01, -m02, -m03, -m10, -m11, -m12, -m13, -m20, -m21, -m22, -m23, -m30, -m31, -m32, -m33 }; }
  INLINE friend float4x4 operator+ ( float4x4 const& a, float4x4 const& b ) { return float4x4( a.xAxis + b.xAxis, a.yAxis + b.yAxis, a.zAxis + b.zAxis, a.pos + b.pos ); }
  INLINE friend float4x4 operator- ( float4x4 const& a, float4x4 const& b ) { return float4x4( a.xAxis - b.xAxis, a.yAxis - b.yAxis, a.zAxis - b.zAxis, a.pos - b.pos ); }
  //INLINE friend float4x4 operator*( float f, float4x4 const& m ) { return float4x4 { m.xAxis * f, m.yAxis * f, m.zAxis * f, m.pos * f }; }
  //INLINE friend float4x4 operator/( float4x4 const& m, float f ) { f = 1.0f / f; return m * f; }

  INLINE friend float4 operator*( float4x4 const& m, float4 const& v )
  {
    return float4
    {
      v.x * m.xAxis.x + v.y * m.yAxis.x + v.z * m.zAxis.x + v.w * m.pos.x,
      v.x * m.xAxis.y + v.y * m.yAxis.y + v.z * m.zAxis.y + v.w * m.pos.y,
      v.x * m.xAxis.z + v.y * m.yAxis.z + v.z * m.zAxis.z + v.w * m.pos.z,
      v.x * m.xAxis.w + v.y * m.yAxis.w + v.z * m.zAxis.w + v.w * m.pos.w
    };
  }
  INLINE friend float4x4 operator*( float4x4 const& m, float const& s )
  {
    return float4x4( m.xAxis * s, m.yAxis * s, m.zAxis * s, m.pos * s );
  }
  INLINE friend float4x4 operator*( float const& s, float4x4 const& m ) { return m * s; }

  INLINE friend float4x4 operator* ( float4x4 const& a, float4x4 const& b )
  {
    return float4x4
    {
      a * b.xAxis,
      a * b.yAxis,
      a * b.zAxis,
      a * b.pos
    };
  }

  INLINE static float4x4 const& identity()
  {
    static float4x4 id = float4x4 { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    return id;
  }
  INLINE static float4x4 const& zero()
  {
    static float4x4 zeroid = float4x4 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    return zeroid;
  }
};


float4x4 get_camera_projection_matrix( float viewportWidth, float viewportHeight, float fovRadians, float zNear, float zFar )
{
  float tanHalfFovx = 1.0f / tanf( fovRadians * 0.5f );
  float aspectRatio = viewportHeight / viewportWidth;
  float z = 1.0f / (zFar - zNear);
  float4x4 m = float4x4::zero();
  m.m02 = zFar * z;
  m.m03 = 1.0f;
  m.m11 = aspectRatio * tanHalfFovx;
  m.m20 = tanHalfFovx;
  m.m32 = -(zFar * zNear) * z;
  m.m33 = 0.0f;

  return m;
}


void initialize( bse::PlatformInitParams* initParameters )
{
  //your initialization goes here


  kln::plane testplane { 0,1,0,0 };
  kln::point testpoint { 1,2,3 };

  testpoint = testplane( testpoint );
  testpoint = testpoint;


  float4 projector = create_viewport_projector( 5.0f, 1.0f, to_radians( 80.0f ), .10f, 1000.0f );
  float4x4 testcam = get_camera_projection_matrix( 5.0f, 1.0f, to_radians( 80.0f ), .10f, 1000.0f );

  float4 p { 6,2,4,1 };
  float4 p2 = project_positive_x_to_viewport( p.xyz, projector );

  float4 p3 = testcam * p;

  float a = p.x;


}

static VFS tester;

void on_reload()
{
  // VFS* test = new_vfs( nullptr );

  // delete_vfs( test );
   //your on reload goes here, relevant for hot reloading
}


#include <vector>
#include <string>
#include <map>

static u64 bsetotal = 0;
static u64 stdtotal = 0;
void tick()
{
  u64 countbse;
  u64 countstd;

  {
    PROFILE_SCOPE( countbse );
    {
      bse::String test = "oh";
      bse::String test1 = "oh";
      bse::String test2 = "oh";
      bse::String test3 = "oh";
      bse::String test4 = "oh";
      bse::String test5 = "oh";
      bse::String test6 = "oh";
      bse::String test7 = "oh";
      bse::String test8 = "oh";
      test = "oh2";
    }

    {
      bse::Map<int, char const*> testmap;
    }

    {
      bse::Array<int> blubb = { 1,2,3 };
    }
  }

  {
    PROFILE_SCOPE( countstd );
    {
      std::map<int, char const*> poop;
    }
    {
      std::vector<int> poop;
    }
    {
      std::string test = "oh";
      bse::String test1 = "oh";
      bse::String test2 = "oh";
      bse::String test3 = "oh";
      bse::String test4 = "oh";
      bse::String test5 = "oh";
      bse::String test6 = "oh";
      bse::String test7 = "oh";
      bse::String test8 = "oh";
      test = "oh2";
    }
  }

  bsetotal += countbse;
  stdtotal += countstd;
}


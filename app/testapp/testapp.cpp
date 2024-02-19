#include "bse_core.h"

using namespace bse;

#if 0
namespace bse
{
  namespace opengl
  {
    //DO THESE WORK????
    float4 create_projector( float viewportWidth, float viewportHeight, float fovRadians, float zNear, float zFar )
    {
      float tanHalfFovx = 1.0f / tanf( fovRadians * 0.5f );
      float aspectRatio = viewportWidth / viewportHeight;
      float z = zFar / (zFar - zNear);

      return float4 { tanHalfFovx, aspectRatio, z, zNear };
    }

    float4 project_to_viewport( float4 p, float4 projector )
    {
      float4 result {};
      result.x = p.z * projector.x;
      result.y = p.y * projector.y * projector.x;
      //p.w useful here? isn't it expected to be 1? 
      result.z = p.x * projector.z - p.w * projector.z * projector.w;
      result.w = p.x;
      return result;
    }

    float4 project_to_viewport( float4 p, float viewportWidth, float viewportHeight, float fovRadians, float zNear, float zFar )
    {
      float tanHalfFovx = 1.0f / tanf( fovRadians * 0.5f );
      float aspectRatio = viewportWidth / viewportHeight;
      float z = zFar / (zFar - zNear);


      float4 result {};
      result.x = p.z * tanHalfFovx;
      result.y = p.y * aspectRatio * tanHalfFovx;
      //p.w useful here? isn't it expected to be 1? 
      result.z = p.x * z - p.w * z * zNear;
      result.w = p.x;

      return result;
    }



    void set_texture( u32 gpuHandle, u32 uniformIndex, u32 textureIndex ) { glActiveTexture( GL_TEXTURE0 + textureIndex ); glBindTexture( GL_TEXTURE_2D, gpuHandle ); glUniform1i( uniformIndex, textureIndex ); }
    void unset_texture( u32 textureIndex ) { glActiveTexture( GL_TEXTURE0 + textureIndex ); glBindTexture( GL_TEXTURE_2D, 0 ); glActiveTexture( GL_TEXTURE0 ); }

    bse::ShaderProgramID create_shader_program( OGLFileData headerFileData, OGLFileData vsFileData, OGLFileData fsFileData )
    {
      // log_info( "[OGL] Compiling shader." );

      GLint shaderCodeLengths[] = { (s32) headerFileData.size, (s32) vsFileData.size };

      ShaderID vsID = glCreateShader( GL_VERTEX_SHADER );
      GLchar const* vsCode[] =
      {
        (char const*) headerFileData.data,
        (char const*) vsFileData.data
      };
      glShaderSource( vsID, array_count( vsCode ), vsCode, shaderCodeLengths );
      glCompileShader( vsID );

      if ( !validate_shader( vsID ) )
      {
        BREAK;
      }

      shaderCodeLengths[1] = (s32) fsFileData.size;

      ShaderID fsID = glCreateShader( GL_FRAGMENT_SHADER );
      GLchar const* fsCode[] =
      {
        (char const*) headerFileData.data,
        (char const*) fsFileData.data
      };
      glShaderSource( fsID, array_count( fsCode ), fsCode, shaderCodeLengths );
      glCompileShader( fsID );

      if ( !validate_shader( fsID ) )
      {
        BREAK;
      }

      //  log_info( "[OGL] Linking program." );

      GLuint programID = glCreateProgram();
      glAttachShader( programID, vsID );
      glAttachShader( programID, fsID );
      glLinkProgram( programID );

      if ( !validate_program( programID ) )
      {
        BREAK;
      }

      glDetachShader( programID, vsID );
      glDetachShader( programID, fsID );

      glDeleteShader( vsID );
      glDeleteShader( fsID );

      //get_uniforms( programID );
      return programID;
    }

    bse::ShaderProgramID create_shader_program( char const* combinedglslData, s32 size )
    {
      char const* reader = (char const*) combinedglslData;
      char const* end = reader + size;
      OGLFileData h;
      OGLFileData vs;
      OGLFileData fs;

      //header
      char const* nextSection = bse::string_contains( reader, "#h" );
      assert( nextSection );
      char const* currentSection = nextSection + 2;

      nextSection = bse::string_contains( currentSection, "#vs" );
      assert( nextSection );

      h.data = currentSection;
      h.size = nextSection - currentSection;

      //vertex shader
      currentSection = nextSection + 3;
      nextSection = bse::string_contains( currentSection, "#fs" );
      assert( nextSection );

      vs.data = currentSection;
      vs.size = nextSection - currentSection;

      //fragment shader
      currentSection = nextSection + 3;
      nextSection = end;

      fs.data = currentSection;
      fs.size = nextSection - currentSection;

      return create_shader_program( h, vs, fs );
    }


    ShaderProgram load_shader( char const* filePath )
    {
      ShaderProgram result {};
      LoadedFile shaderFile = load_file( filePath );

      if ( shaderFile.data )
      {
        result.id = opengl::create_shader_program( (char const*) shaderFile.data, shaderFile.size );
        free( shaderFile.data );

        //load attributes
        {
          s32 count = -1;
          s32 length;
          char name[256];
          s32 size;
          GLenum type;

          glUseProgram( result.id );
          glGetProgramiv( result.id, GL_ACTIVE_ATTRIBUTES, &count );

          for ( s32 i = 0; i < count; ++i )
          {
            memset( name, 0, sizeof( name ) );
            glGetActiveAttrib( result.id, GLuint( i ), sizeof( name ), &length, &size, &type, name );
            s32 attributeLocation = glGetAttribLocation( result.id, name );
            if ( attributeLocation >= 0 )
            {
              result.attributes[name] = attributeLocation;
            }
          }

          count = -1;
          char testName[256];
          glGetProgramiv( result.id, GL_ACTIVE_UNIFORMS, &count );
          for ( s32 i = 0; i < count; ++i )
          {
            memset( name, 0, sizeof( name ) );
            glGetActiveUniform( result.id, GLuint( i ), sizeof( name ), &length, &size, &type, name );
            s32 uniform = glGetUniformLocation( result.id, name );
            if ( uniform >= 0 )
            {
              std::string uniformName = name;
              std::size_t found = uniformName.find( '[' );
              if ( found != std::string::npos )
              {
                uniformName.erase( uniformName.begin() + found, uniformName.end() );
                u32 uniformIndex = 0;

                while ( true )
                {
                  //    string_format( testName, sizeof( testName ), uniformName.c_str(), "[", uniformIndex++, "]" );
                  s32 uniformLocation = glGetUniformLocation( result.id, testName );
                  if ( uniformLocation < 0 )
                  {
                    break;
                  }
                  result.uniforms[testName] = uniformLocation;
                }
              }

              result.uniforms[uniformName] = uniform;
            }
          }
        }
      }

      return std::move( result );
    }
  };
};

#endif

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




//static S s;
void on_reload()
{
  using namespace bse;

  // T1 test;

  {
  }
  threadtest();
  // String t = "hello";
  // t = "ayy";
  // Array<String> strings = { "C:\\Test\\", "D:/bse/" };

  // Array<Map<String, s32>> wha( 4 );
  // wha.push();

  // wha[0]["ok"] = 1;
  // (wha[0])["ok"] = 2;

  // Map<String, s32> bla;
  // wha.push( bla );
  // wha[1]["NO"] = 5;

  // wha[1]["NO"] = 3;

  // wha[0]["Test"] = 3;

   // Array<String> strings = { "ok", "nah" };
   // Array<int> ints = { 3, 4, 5, 6 };

   // Array<int> ints2 = ints;

    // char const* a = "teaparty12345678";
    // String tea = a;
    // tea = "no";
    // u32 si = sizeof( String );

    // Array<String> bla { "c:\\test\\", "d:/test2/" };
    // VirtualFileSystem* blau = new_virtual_file_system( nullptr, bla );
    // VFS* test = new_vfs( nullptr );

    // delete_vfs( test );
     //your on reload goes here, relevant for hot reloading
}


void tick()
{



}


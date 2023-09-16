#pragma once 

#include "external/bse_opengl_ext.h"
#include "bse_common.h"

namespace bse
{
  namespace opengl
  {
    INLINE void set_uniform( u32 slot, float const* elements, u32 elementCount = 1 ) { glUniform1fv( slot, GLsizei( elementCount ), elements ); }
    INLINE void set_uniform( u32 slot, float2 const* elements, u32 elementCount = 1 ) { glUniform2fv( slot, GLsizei( elementCount ), (float*) elements ); }
    INLINE void set_uniform( u32 slot, float3 const* elements, u32 elementCount = 1 ) { glUniform3fv( slot, GLsizei( elementCount ), (float*) elements ); }
    INLINE void set_uniform( u32 slot, float4 const* elements, u32 elementCount = 1 ) { glUniform4fv( slot, GLsizei( elementCount ), (float*) elements ); }
    INLINE void set_uniform( u32 slot, s32 const* elements, u32 elementCount = 1 ) { glUniform1iv( slot, GLsizei( elementCount ), elements ); }
    INLINE void set_uniform( u32 slot, int2 const* elements, u32 elementCount = 1 ) { glUniform2iv( slot, GLsizei( elementCount ), (s32*) elements ); }
    INLINE void set_uniform( u32 slot, int3 const* elements, u32 elementCount = 1 ) { glUniform3iv( slot, GLsizei( elementCount ), (s32*) elements ); }
    INLINE void set_uniform( u32 slot, int4 const* elements, u32 elementCount = 1 ) { glUniform4iv( slot, GLsizei( elementCount ), (s32*) elements ); }
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
#pragma once 

#include "external/bse_opengl_ext.h"
#include "bse_common.h"

namespace bse
{
  namespace opengl
  {
    void check_gl_error();

    INLINE void set_uniform( u32 slot, float const* elements, u32 elementCount = 1 ) { glUniform1fv( slot, GLsizei( elementCount ), elements ); }
    INLINE void set_uniform( u32 slot, float2 const* elements, u32 elementCount = 1 ) { glUniform2fv( slot, GLsizei( elementCount ), (float*) elements ); }
    INLINE void set_uniform( u32 slot, float3 const* elements, u32 elementCount = 1 ) { glUniform3fv( slot, GLsizei( elementCount ), (float*) elements ); }
    INLINE void set_uniform( u32 slot, float4 const* elements, u32 elementCount = 1 ) { glUniform4fv( slot, GLsizei( elementCount ), (float*) elements ); }
    INLINE void set_uniform( u32 slot, int const* elements, u32 elementCount = 1 ) { glUniform1iv( slot, GLsizei( elementCount ), elements ); }
    INLINE void set_uniform( u32 slot, int2 const* elements, u32 elementCount = 1 ) { glUniform2iv( slot, GLsizei( elementCount ), (int*) elements ); }
    INLINE void set_uniform( u32 slot, int3 const* elements, u32 elementCount = 1 ) { glUniform3iv( slot, GLsizei( elementCount ), (int*) elements ); }
    INLINE void set_uniform( u32 slot, int4 const* elements, u32 elementCount = 1 ) { glUniform4iv( slot, GLsizei( elementCount ), (int*) elements ); }
  };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace bse
{
  namespace opengl
  {
    void check_gl_error()
    {
      GLenum err;
      while ( (err = glGetError()) != GL_NO_ERROR )
      {
        switch ( err )
        {
          case GL_INVALID_ENUM:
          {
            BREAK;
            break;
          }
          case GL_INVALID_VALUE:
          {
            BREAK;
            break;
          }
          case GL_INVALID_OPERATION:
          {
            BREAK;
            break;
          }
          case GL_STACK_OVERFLOW:
          {
            BREAK;
            break;
          }
          case GL_STACK_UNDERFLOW:
          {
            BREAK;
            break;
          }
          case GL_OUT_OF_MEMORY:
          {
            BREAK;
            break;
          }
          default:
          {
            //??
            BREAK;
          }
        }
      }
    }

    struct OGLFileData
    {
      char const* data;
      s64 size;
    };

    bool validate_shader( GLuint shaderHandle )
    {
      constexpr s32 MAX_INFO_LOG_LENGTH = 8192;
      char infoLog[MAX_INFO_LOG_LENGTH];
      GLint result = GL_FALSE;
      s32 infoLogLength = 0;
      glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
      if ( result != GL_TRUE )
      {
        glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength );
        glGetShaderInfoLog( shaderHandle, min( infoLogLength, MAX_INFO_LOG_LENGTH ), NULL, infoLog );
        log_error( "[OGL] Errors: \n", infoLog );
        BREAK;
      }
      return result == GL_TRUE;
    }

    bool validate_program( GLuint programHandle )
    {
      constexpr s32 MAX_INFO_LOG_LENGTH = 8192;
      char infoLog[MAX_INFO_LOG_LENGTH];
      GLint result = GL_FALSE;
      s32 infoLogLength = 0;
      glGetProgramiv( programHandle, GL_LINK_STATUS, &result );
      if ( result != GL_TRUE )
      {
        glGetProgramiv( programHandle, GL_INFO_LOG_LENGTH, &infoLogLength );
        glGetProgramInfoLog( programHandle, min( infoLogLength, MAX_INFO_LOG_LENGTH ), NULL, infoLog );
        log_error( "[OGL] Errors: \n", infoLog );
        BREAK;
      }
      return result == GL_TRUE;
    }

    GLuint create_shader_program( OGLFileData headerFileData, OGLFileData vsFileData, OGLFileData fsFileData )
    {
      // log_info( "[OGL] Compiling shader." );

      GLint shaderCodeLengths[] = { (s32) headerFileData.size, (s32) vsFileData.size };

      GLuint vs = glCreateShader( GL_VERTEX_SHADER );
      GLchar const* vsCode[] =
      {
        (char const*) headerFileData.data,
        (char const*) vsFileData.data
      };
      glShaderSource( vs, array_count( vsCode ), vsCode, shaderCodeLengths );
      glCompileShader( vs );

      if ( !validate_shader( vs ) )
      {
        BREAK;
      }

      shaderCodeLengths[1] = (s32) fsFileData.size;

      GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
      GLchar const* fsCode[] =
      {
        (char const*) headerFileData.data,
        (char const*) fsFileData.data
      };
      glShaderSource( fs, array_count( fsCode ), fsCode, shaderCodeLengths );
      glCompileShader( fs );

      if ( !validate_shader( fs ) )
      {
        BREAK;
      }

      //  log_info( "[OGL] Linking program." );

      GLuint program = glCreateProgram();
      glAttachShader( program, vs );
      glAttachShader( program, fs );
      glLinkProgram( program );

      if ( !validate_program( program ) )
      {
        BREAK;
      }

      glDetachShader( program, vs );
      glDetachShader( program, fs );

      glDeleteShader( vs );
      glDeleteShader( fs );

      //get_uniforms( program );
      return program;
    }

    GLuint create_shader_program( char const* combinedglslData, s32 size )
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




  };
};
#include "core/bse_core.h"

namespace bse
{

  ShaderProgram load_shader( char const* filePath )
  {
    ShaderProgram result {};
    Buffer shaderFile = file::load( filePath );

    if ( shaderFile.data )
    {
      result.id = opengl::create_shader_program( (char const*) shaderFile.data, (s32) shaderFile.size );
      file::unload( shaderFile );

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
            String uniformName = name;
            s64 found = uniformName.find( '[' );
            if ( found != String::npos )
            {
              uniformName.erase( uniformName.begin() + found, uniformName.end() );
              u32 uniformIndex = 0;

              while ( true )
              {
                string_format( testName, sizeof( testName ), uniformName.c_str(), "[", uniformIndex++, "]" );
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
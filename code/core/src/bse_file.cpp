#include "core/bse_core.h"

namespace bse
{
  namespace file
  {
    Buffer load( char const* filePath )
    {
      file::Info info;
      Buffer result {};
      if ( platform->get_file_info( filePath, &info ) )
      {
        assert( info.size < U32_MAX );
        result.data = (char*) memory::allocate_thread_safe( info.size );
        result.size = info.size;

        if ( !platform->load_file_part( filePath, 0, result.data, result.size ) )
        {
          free( result.data );
          result.size = 0;
        }
      }

      return result;
    }

    void unload( Buffer& buffer )
    {
      memory::free_thread_safe( buffer.data, buffer.size );
      buffer.data = nullptr;
      buffer.size = 0;
    }
  };
};
#include "core/include/bse_debug.h"
#include "core/bse_core.h"

namespace bse
{
  namespace debug
  {
    //forward trivial messages directly to the output
    template<> void log( LogParameters const& parameters, char* message ) { platform->debug_log( parameters, message, 0 ); }
    template<> void log( LogParameters const& parameters, char const* message ) { platform->debug_log( parameters, message, 0 ); }

    template<typename... Args> void log( LogParameters const& parameters, Args... args )
    {
      char debugBuffer[BSE_STACK_BUFFER_LARGE];
      s32 bytesToWrite = bse::string_format( debugBuffer, BSE_STACK_BUFFER_LARGE, args... ) - 1 /* ommit null */;

      if ( bytesToWrite > 0 )
      {
        if ( debugBuffer[bytesToWrite - 1] != '\n' )
        {
          debugBuffer[bytesToWrite++] = '\n';
          debugBuffer[bytesToWrite] = '\0';
        }

        platform->debug_log( parameters, debugBuffer, bytesToWrite );
      }
    }
  };
};


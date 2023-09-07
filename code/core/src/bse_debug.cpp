#include "include/bse_debug.h"
#include "bse_core.h"

namespace bse
{
  namespace debug
  {
    template<typename... Args> void log( LogParameters const& parameters, Args... args )
    {
      s32 const MAX_DEBUG_MESSAGE_LENGTH = 8192;
      char debugBuffer[MAX_DEBUG_MESSAGE_LENGTH];
      s32 bytesToWrite = bse::string_format( debugBuffer, MAX_DEBUG_MESSAGE_LENGTH, args... ) - 1 /* ommit null */;
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


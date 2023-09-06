#pragma once

#include "agnostic/bse_string_format.h"


#define BSE_LOG( ... ) bse::debug::log()




namespace bse
{
  namespace debug
  {
    enum class LogSeverity : u8
    {
      NONE    = 0x0,
      INFO    = 0x1,
      WARNING = 0x2,
      ERROR   = 0x3,
    };

    enum class LogOutputType : u8
    {
      NONE             = 0x0,
      LOCAL_CONSOLE    = 0x1,
      REMOTE_HOST      = 0x2,
      LOCAL_AND_REMOTE = 0x3,
    };

    struct LogParameters
    {
      LogSeverity severity;
      LogOutputType type;
    };

    template<typename... Args> void log( LogParameters const& parameters, Args... args );

  };
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////inl//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

namespace bse
{
  namespace debug
  {
    template<typename... Args> void log( LogParameters const& parameters, Args... args )
    {
      s32 const MAX_DEBUG_MESSAGE_LENGTH = 8192;
      char debugBuffer[MAX_DEBUG_MESSAGE_LENGTH];
      s32 bytesToWrite = bs::string_format( debugBuffer, MAX_DEBUG_MESSAGE_LENGTH, args... ) - 1 /* ommit null */;
      if ( bytesToWrite > 0 )
      {
        if ( debugBuffer[bytesToWrite - 1] != '\n' )
        {
          debugBuffer[bytesToWrite++] = '\n';
          debugBuffer[bytesToWrite] = '\0';
        }

        log_callback( parameters, debugBuffer, bytesToWrite );
      }
    }
  };
};
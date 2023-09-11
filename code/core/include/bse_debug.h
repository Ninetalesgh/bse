#pragma once

#include "bse_common.h"

#define log_info( ... ) bse::debug::log({bse::debug::LogSeverity::INFO, bse::debug::LogOutputType::LOCAL_CONSOLE}, __VA_ARGS__)
#define log_warning( ... ) bse::debug::log({bse::debug::LogSeverity::WARNING, bse::debug::LogOutputType::LOCAL_CONSOLE}, __VA_ARGS__)
#define log_error( ... ) bse::debug::log({bse::debug::LogSeverity::ERROR, bse::debug::LogOutputType::LOCAL_AND_REMOTE}, __VA_ARGS__, " ", __FILE__, " #", __LINE__ )

#if defined(assert)
# undef assert
#endif

#if defined(BSE_BUILD_DEBUG)
# define BREAK __debugbreak()
# define assert(expression) { if ( !(expression) ) BREAK; }
#elif defined(BSE_BUILD_DEVELOP)
# define BREAK {bse::debug::log({bse::debug::LogSeverity::WARNING, bse::debug::LogOutputType::LOCAL_CONSOLE}, "break in ", __FILE__," #", __LINE__ );}
# define assert(expression) if (!(expression)) { bse::debug::log({bse::debug::LogSeverity::ERROR, bse::debug::LogOutputType::ALL}, "assert in ", __FILE__," #", __LINE__ );}
#else
//# define BREAK {}
//# define assert(expression) {}
# define BREAK {bse::debug::log({bse::debug::LogSeverity::WARNING, bse::debug::LogOutputType::LOCAL_CONSOLE}, "break in ", __FILE__," #", __LINE__ );}
# define assert(expression) if (!(expression)) { bse::debug::log({bse::debug::LogSeverity::ERROR, bse::debug::LogOutputType::ALL}, "assert in ", __FILE__," #", __LINE__ );}
#endif

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
      NONE              = 0x0,
      LOCAL_CONSOLE     = 0x1,
      REMOTE_HOST       = 0x2,
      LOCAL_AND_REMOTE  = LOCAL_CONSOLE | REMOTE_HOST,
      WRITE_TO_LOG_FILE = 0x4,
      LOCAL_AND_FILE    = LOCAL_CONSOLE | WRITE_TO_LOG_FILE,
      REMOTE_AND_FILE   = REMOTE_HOST | WRITE_TO_LOG_FILE,
      ALL               = 0xff
    };
    BSE_DEFINE_ENUM_OPERATORS_U8( LogOutputType );

    struct LogParameters
    {
      LogSeverity severity;
      LogOutputType type;
    };

    template<typename... Args> void log( LogParameters const& parameters, Args... args );
  };
};
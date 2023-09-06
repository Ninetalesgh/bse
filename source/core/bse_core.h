#pragma once

#define BSE_BUILD_NETWORK
#define BSE_BUILD_GRAPHICS
#define BSE_BUILD_AUDIO
#define BSE_BUILD_INPUT


#include "agnostic/bse_core_agnostic.h"

namespace bse
{
  namespace debug
  {
    struct LogParameters;
  };

  using debug_log_fn = void( debug::LogParameters const&, char const* message, s32 messageSize );

  struct Platform
  {
    struct FrameInfo
    {
      u64 currentFrameIndex;
      float deltaTime;
      //input::State input;
      //float time;
    } frame;

    struct CallbackInfo
    {
      debug_log_fn* debug_log;

      //  bse::FileSystem* fileSystem = nullptr;
      //  bse::Font* font = nullptr;
      //  bse::GlyphTable* glyphTable = nullptr;
    } callback;

    struct ConstantInfo
    {
      char const* executablePath;
    } constant;


    // net::NetworkData network;

    //This is to be user defined
   // AppUserData* userData;

  } extern* platform;

  struct PlatformInitParams
  {
    char const* exePath;
    struct
    {
      s32 argumentCount;
      char** arguments;
    } commandLine;

    struct
    {
      bool skipInitMouseAndKeyboard;
      bool skipInitController;
    } input;

    struct
    {
      char const* name;
      int2 size;
      int2 position;
      bool fullscreen;
      bool skipInitWindow;
    } window;

    struct
    {
      s32 syncableCount;
      s32 asyncCount;
    } workerThreads;

    struct
    {
      bool skipInitAudio;
    } audio;

    struct
    {
      bool skipInitNetwork;
    } network;

    struct
    {
      bool skipInitConsole;
    } console;
  };

  using core_initialize_fn = void( PlatformInitParams* );
  extern "C" core_initialize_fn core_initialize_internal;

  using core_on_reload_fn = void( Platform* );
  extern "C" core_on_reload_fn core_on_reload_internal;

  using core_tick_fn = void( Platform* );
  extern "C" core_tick_fn core_tick_internal;
};






#if defined(BSE_APP_TO_BUILD)
//include apps here?
#else
//include devenv here?
#endif

#include "dependent/bse_core_dependent.h"
#include "bse_win64.h"
#include "bse_win64_opengl.h"
#include "bse_win64_platform_callbacks.h"

LRESULT CALLBACK bse_main_window_callback( HWND window, UINT message, WPARAM wParam, LPARAM lParam );

void bse_win64_loop();
int bse_win64_init( int argc, char** argv );

int bse_main( int argc, char** argv )
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  if ( !bse_win64_init( argc, argv ) )
  {
    return 0;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Run Core Loop /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  win64::global::running = true;
  while ( win64::global::running )
  {
    //0) Refresh viewport if necessary
    if ( win64::global::platform.settings.windowSize != win64::global::mainWindow.cachedSize )
    {
      win64::set_window_size( win64::global::platform.settings.windowSize );
    }

    //1) Pause thread if requested?

    //2) Loop
    bse_win64_loop();

    //3) Swap Buffers?
    win64::opengl::swap_buffers( win64::global::mainWindow.deviceContext );

    //4) Increase Frame Index
    ++win64::global::platform.thisFrame.frameIndex;
  }

  return 0;
}

void bse_win64_init_core();
int bse_win64_init( int argc, char** argv )
{
  char stack[BSE_STACK_BUFFER_MEDIUM];
  s32 result = 1;

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Callbacks so we are able to use debug logs and the like //////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  win64::register_platform_callbacks();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Timer ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  result = QueryPerformanceFrequency( (LARGE_INTEGER*) &win64::global::performanceCounterFrequency );
  assert( result );

  result = (s32) timeBeginPeriod( 1 );
  assert( result == TIMERR_NOERROR );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Platform /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    bse::platform = &win64::global::platform;
    SYSTEM_INFO systemInfo {};
    GetSystemInfo( &systemInfo );
    bse::platform->info.processorCount = systemInfo.dwNumberOfProcessors;
    bse::platform->info.virtualMemoryAllocationGranularity = systemInfo.dwAllocationGranularity;
    bse::platform->info.virtualMemoryPageSize = systemInfo.dwPageSize;
    bse::platform->info.virtualMemoryAddressBegin = (char*) systemInfo.lpMinimumApplicationAddress;
    bse::platform->info.virtualMemoryAddressEnd = (char*) systemInfo.lpMaximumApplicationAddress;

    bse::platform->info.processorArchitecture = bse::ProcessorArchitecture( systemInfo.wProcessorArchitecture );

    bse::memory::init_virtual_memory_layout( bse::platform->allocators.virtualMemory );
    bse::platform->allocators.mainThread = bse::memory::new_multipool( &bse::platform->allocators.virtualMemory
      , bse::platform->info.virtualMemoryPageSize - sizeof( bse::memory::Multipool ), 16 );

    bse::platform->allocators.threadSafe = bse::memory::new_multipool( &bse::platform->allocators.virtualMemory
      , bse::platform->info.virtualMemoryPageSize - sizeof( bse::memory::Multipool ), 16
      , bse::memory::AllocatorPolicyFlags::AllowGrowth | bse::memory::AllocatorPolicyFlags::GeometricGrowth | bse::memory::AllocatorPolicyFlags::ThreadSafe );

    constexpr s64 sizePerFrame = MegaBytes( 1 ) - sizeof( bse::memory::Arena );

    for ( s32 i = 0; i < array_count( bse::platform->allocators.temporary ); ++i )
    {
      bse::platform->allocators.temporary[i] = bse::memory::new_arena( &bse::platform->allocators.virtualMemory.temporary, sizePerFrame, bse::memory::AllocatorPolicyFlags::AllowGrowth );
    }

    win64::get_exe_path( stack, BSE_STACK_BUFFER_MEDIUM );
    bse::string_replace_char( stack, '\\', '/' );
    bse::platform->info.executablePath = stack;

    //bse::platform->allocators.vfs;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Core and fetch app specifications for platform, quit right after if desired //////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bse_win64_init_core();

  bse::PlatformInitParams initParams {};
  initParams.programHandle = GetModuleHandle( NULL );
  initParams.platform = &win64::global::platform;
  initParams.commandLine.argumentCount = argc;
  initParams.commandLine.arguments = argv;
  win64::global::bseCore.initialize( &initParams, &win64::global::platform );
  if ( initParams.shutdownAfterInitializing ) return 0;


  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Console (Is this relevant?) //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  if ( !initParams.console.skipInitConsole ) { CoInitializeEx( 0, COINIT_MULTITHREADED ); }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Network //////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if !defined(BSE_BUILD_SKIP_NETWORK)
  if ( !initParams.network.skipInitNetwork )
  {
    WSADATA wsaData;
    result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    assert( result == 0 );
  }
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Input ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if !defined(BSE_BUILD_SKIP_INPUT)
  if ( !initParams.input.skipInitController )
  {
    win64::load_xInput();
  }
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Graphics /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if !defined(BSE_BUILD_SKIP_GRAPHICS)
  win64::global::openglDll = LoadLibraryA( "opengl32.dll" );
  assert( win64::global::openglDll );
  result = win64::opengl::load_extensions();
  assert( result );

  if ( !initParams.window.skipInitWindow )
  {
    win64::WindowInitParameter parameter {};
    parameter.windowName             = initParams.window.name;
    parameter.size                   = initParams.window.size;
    parameter.pos                    = initParams.window.position;
    parameter.wndClass.cbSize        = sizeof( WNDCLASSEX );
    parameter.wndClass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    parameter.wndClass.lpfnWndProc   = bse_main_window_callback;
    parameter.wndClass.hInstance     = (HINSTANCE) initParams.programHandle;
    parameter.wndClass.hCursor       = LoadCursor( (HINSTANCE) NULL, IDC_ARROW );
    parameter.wndClass.lpszClassName = L"bsewnd";

    win64::global::mainWindow = win64::create_window( parameter );
    assert( win64::global::mainWindow.handle );

  }
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Audio ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if !defined(BSE_BUILD_SKIP_AUDIO)    
  if ( !initParams.audio.skipInitAudio )
  {
  }
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Worker Threads ///////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init App //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    win64::global::bseCore.on_reload( &win64::global::platform );
  }

  return result;
}


void bse_win64_process_window_messages( HWND );
void bse_win64_loop()
{
  bse::Platform& platform = win64::global::platform;
  LARGE_INTEGER beginCounter = win64::get_timer();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Process Input & Window Messages ///////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bse_win64_process_window_messages( win64::global::mainWindow.handle );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Process Core Tick /////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    static LARGE_INTEGER previousFrameTime = beginCounter;
    platform.thisFrame.deltaTime = win64::get_seconds_elapsed( previousFrameTime, beginCounter );
    #if defined(BSE_BUILD_DEBUG)
    //so delta time doesn't get messed up with breakpoints
    static float averageDeltaTime = platform.settings.fpsCap ? 1.0f / platform.settings.fpsCap : 0.0f;
    if ( platform.thisFrame.deltaTime > 0.05f )
    {
      platform.thisFrame.deltaTime = averageDeltaTime;
    }
    constexpr float A = 0.9f;
    averageDeltaTime = A * averageDeltaTime + (1.0f - A) * platform.thisFrame.deltaTime;
    #endif
    previousFrameTime = beginCounter;
    win64::global::bseCore.tick( &platform );
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Adjust to FPS Target //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    float secondsElapsed = win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
    if ( platform.settings.fpsCap )
    {
      static float sleepMsSubtraction = 0.0f;
      float const spfTarget = 1.0f / platform.settings.fpsCap;

      //      PROFILE_SCOPE( debug_CyclesSleep );
      if ( secondsElapsed < spfTarget )
      {
        float const msSleep = ((spfTarget - secondsElapsed) * 1000.f) + sleepMsSubtraction;
        // thread::sleep( s32( max( msSleep, 0.0f ) ) );
        float secondsElapsedIncludingSleep =  win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
        float const delta = 1000.0f * (spfTarget - secondsElapsedIncludingSleep);
        sleepMsSubtraction += min( 0.f, delta ) - (delta > 2.0f) * 1.0f;

        //   log_info( "[WIN32_CLOCK] frame ", win64::global::bseCoreData.currentFrameIndex, " had ", delta, " ms left after sleeping for ", max( msSleep, 0.f ),
        //                                        " ms\n - - - next sleep reduced by ", -sleepMsSubtraction, " ms\n" );
        do
        {
          secondsElapsedIncludingSleep = win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
        } while ( secondsElapsedIncludingSleep < spfTarget );
      }
      else
      {
        //  log_info( "[WIN32_CLOCK] Missed fps target for frame: ", win64::global::bseCoreData.currentFrameIndex,
        //                                        "\n- - - - - - - Actual ms: ", 1000.f * secondsElapsed,
        //                                       "   fps: ", float( 1.f / secondsElapsed ), "\n" );
      }
    } // PROFILE_SCOPE( debug_CyclesSleep );
  }

  //LARGE_INTEGER endCounter = win64::get_timer();
}

void bse_win64_process_window_messages( HWND windowHandle )
{
  #if !defined(BSE_BUILD_SKIP_INPUT)
  bse::Input& input = win64::global::platform.thisFrame.input;
  memset( input.down, 0, bse::Input::STATE_COUNT );
  input.mousePos[0].start = input.mousePos[0].end;
  #endif
  MSG message;
  while ( PeekMessage( &message, windowHandle, 0, 0, PM_REMOVE ) )
  {
    switch ( message.message )
    {
      case WM_XBUTTONDOWN:
      case WM_XBUTTONUP:
        #if !defined(BSE_BUILD_SKIP_INPUT)
      {
        assert( message.wParam == 65568 || message.wParam == 131136 );
        u32 code = message.wParam == 65568 ? bse::Input::MOUSE_4 : bse::Input::MOUSE_5;
        u8 isDown = (u8) !(message.lParam & (1 << 31));
        u8 wasDown = input.held[code];
        input.down[code] = ((!wasDown) && isDown);
        input.held[code] = isDown;
        break;
      }
      case WM_MOUSEWHEEL:
      {
        input.mouseWheelDelta = GET_WHEEL_DELTA_WPARAM( message.wParam ) / WHEEL_DELTA;
        //TODO what is this constant here?
        //DEBUG::scale -= float( input.mouseWheelDelta ) * 0.03f;
        break;
      }
      case WM_MOUSEMOVE:
      {
        input.mousePos[0].min
          = input.mousePos[0].max
          = input.mousePos[0].end
          = { s32( s16( LOWORD( message.lParam ) ) ), s32( s16( HIWORD( message.lParam ) ) ) };
        break;
      }
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
      case WM_KEYDOWN:
      case WM_KEYUP:
      {
        assert( message.wParam < bse::Input::STATE_COUNT );
        u32 code = (u32) message.wParam;

        u8 isDown = (u8) !(message.lParam & (1 << 31));
        u8 wasDown = input.held[code];
        input.held[code] = isDown;

        if ( !wasDown && isDown )
        {
          switch ( code )
          {
            case bse::Input::KEY_F9:
              break;
            case bse::Input::KEY_F10:
              break;
            case bse::Input::KEY_F11:
              break;
            case bse::Input::KEY_F12:
              // win32::ServerHandshake( global::netData.udpSocket, global::netData.server, global_debugUsername );
              break;
            case bse::Input::KEY_ESCAPE:
              win64::global::running = false;
              break;
            default:
              input.down[code] = 1;
              break;
          }
        }

        //TODO REMOVE HARDCODED SHIT
        if ( input.held[bse::Input::KEY_ALT] && input.down[bse::Input::KEY_F4] ) win64::global::running = false;

        break;
      }
      #endif
      default:
      {
        TranslateMessage( &message );
        DispatchMessage( &message );
      }
      break;
    }

    #if !defined(BSE_BUILD_SKIP_INPUT)
    win64::process_controller_input( win64::global::platform.thisFrame.input );
    #endif
  }
}

LRESULT CALLBACK bse_main_window_callback( HWND window, UINT message, WPARAM wParam, LPARAM lParam )
{
  LRESULT result = 0;
  switch ( message )
  {
    case WM_SIZE:
    {
      //overwrite platform settings
      win64::global::platform.settings.windowSize = int2 { s32( LOWORD( lParam ) ), s32( HIWORD( lParam ) ) };
      win64::global::mainWindow.cachedSize = win64::global::platform.settings.windowSize;
      if ( wParam == SIZE_MAXIMIZED || wParam == SIZE_MINIMIZED )
      {
        win64::opengl::resize_viewport( win64::global::platform.settings.windowSize );
      }
      break;
    }
    case WM_MOVE:
    {
      break;
    }
    case WM_DISPLAYCHANGE:
    case WM_EXITSIZEMOVE:
    {
      //overwrite platform settings
      win64::global::mainWindow.cachedSize = win64::global::platform.settings.windowSize;
      win64::opengl::resize_viewport( win64::global::platform.settings.windowSize );
      break;
    }
    case WM_DESTROY:
    {
      win64::global::running = false;
      break;
    }
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
    {
      if ( !wParam )
      {
        #if !defined(BSE_BUILD_SKIP_INPUT)
        memset( win64::global::platform.thisFrame.input.held, 0, bse::Input::STATE_COUNT );
        #endif
      }
      break;
    }
    default:
    {
      result = DefWindowProc( window, message, wParam, lParam );
      break;
    }
  }

  return result;
}

#if defined(BSE_BUILD_DEBUG_DEVELOPMENT)

char const* BSE_TMP_CORE_FILENAME_0 = "bse_core0.tmp.dll";
char const* BSE_TMP_CORE_FILENAME_1 = "bse_core1.tmp.dll";
char const* BSE_APP_FILENAME = "bse_core.dll";

void bse_win64_init_core()
{
  win64::BseCore& coreDll = win64::global::bseCore;

  _WIN32_FIND_DATAA findData;
  HANDLE findHandle = FindFirstFileA( BSE_APP_FILENAME, &findData );
  if ( findHandle != INVALID_HANDLE_VALUE )
  {
    FindClose( findHandle );

    if ( CopyFileA( BSE_APP_FILENAME, BSE_TMP_CORE_FILENAME_0, false ) )
    {
      coreDll.dll = LoadLibraryA( BSE_TMP_CORE_FILENAME_0 );//~120M cycles
      if ( coreDll.dll )
      {
        coreDll.initialize = (bse::core_initialize_fn*) GetProcAddress( coreDll.dll, "core_initialize_internal" );
        coreDll.on_reload = (bse::core_on_reload_fn*) GetProcAddress( coreDll.dll, "core_on_reload_internal" );
        coreDll.tick = (bse::core_tick_fn*) GetProcAddress( coreDll.dll, "core_tick_internal" );

        if ( coreDll.initialize && coreDll.on_reload && coreDll.tick )
        {
        }
        else
        {
          BREAK;
        }
      }
      else
      {
        BREAK;
      }
    }
  }
}

namespace bse
{
  Platform* platform;
};

#else
#include "core/bse_core.cpp"

void bse_win64_init_core()
{
  win64::global::bseCore.initialize = &bse::core_initialize_internal;
  win64::global::bseCore.on_reload = &bse::core_on_reload_internal;
  win64::global::bseCore.tick = &bse::core_tick_internal;
}

#endif


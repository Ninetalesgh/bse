#include "bse_win64.h"
#include "bse_win64_opengl.h"
#include "bse_win64_platform_callbacks.h"

LRESULT CALLBACK bse_main_window_callback( HWND window, UINT message, WPARAM wParam, LPARAM lParam );
void bse_win64_init_core();
void bse_win64_loop();

int bse_main( int argc, char** argv )
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Callbacks so we are able to use debug logs and the like //////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  win64::register_platform_callbacks();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Timer  ///////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  s32 result = 1;
  result = QueryPerformanceFrequency( (LARGE_INTEGER*) &win64::global::performanceCounterFrequency );
  assert( result );

  result = (s32) timeBeginPeriod( 1 );
  assert( result == TIMERR_NOERROR );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Core and fetch app specifications for platform ///////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bse_win64_init_core();
  bse::PlatformInitParams initParams {};
  initParams.programHandle = GetModuleHandle( NULL );
  initParams.platform = &win64::global::platform;
  initParams.commandLine.argumentCount = argc;
  initParams.commandLine.arguments = argv;
  win64::global::core.initialize( &initParams );

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Console (Is this relevant?) //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  if ( !initParams.console.skipInitConsole ) { CoInitializeEx( 0, COINIT_MULTITHREADED ); }


  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Network //////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if defined(BSE_BUILD_NETWORK)
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

  #if defined(BSE_BUILD_INPUT)
  if ( !initParams.input.skipInitController )
  {
    win64::load_xInput();
  }
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Graphics /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if defined (BSE_BUILD_GRAPHICS)
  win64::global::openglDll = LoadLibraryA( "opengl32.dll" );
  assert( win64::global::openglDll );

  if ( !initParams.window.skipInitWindow )
  {
    win64::set_fps_cap( initParams.window.fpsCap );

    win64::global::mainWindowSize = initParams.window.size;
    win64::WindowInitParameter parameter {};
    parameter.windowName             = initParams.window.name;
    parameter.width                  = initParams.window.size.x;
    parameter.height                 = initParams.window.size.y;
    parameter.x                      = initParams.window.position.x;
    parameter.y                      = initParams.window.position.y;
    parameter.wndClass.cbSize        = sizeof( WNDCLASSEX );
    parameter.wndClass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    parameter.wndClass.lpfnWndProc   = bse_main_window_callback;
    parameter.wndClass.hInstance     = (HINSTANCE) initParams.programHandle;
    parameter.wndClass.hCursor       = LoadCursor( (HINSTANCE) NULL, IDC_ARROW );
    parameter.wndClass.lpszClassName = L"bsewnd";

    win64::global::openglDll = LoadLibraryA( "opengl32.dll" );
    assert( win64::global::openglDll );

    win64::global::mainWindow = win64::init_window( parameter );
    assert( win64::global::mainWindow );

    HDC deviceContext = GetDC( win64::global::mainWindow );
    assert( deviceContext );

    u32 openglInitResult = win64::opengl::init( deviceContext, initParams.window.size );
    assert( openglInitResult );
  }
  #endif

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init Audio ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  #if defined (BSE_BUILD_AUDIO)    
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
  ////////// Init App Callbacks ////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    win64::global::core.on_reload( &win64::global::platform );
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Init App //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Run Core Loop /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  win64::global::running = true;
  while ( win64::global::running )
  {
    //1) Pause thread if requested?

    //2) Loop
    bse_win64_loop();

    //3) Swap Buffers?

    //4) Increase Frame Index
    ++win64::global::platform.thisFrame.frameIndex;
  }

  return result;
}

void bse_win64_process_window_messages();

void bse_win64_loop()
{
  LARGE_INTEGER beginCounter = win64::get_timer();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Process Input & Window Messages ///////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  bse_win64_process_window_messages();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Process Core Tick /////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    win64::global::core.tick( &win64::global::platform );
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////// Adjust to FPS /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  {
    static float sleepMsSubtraction = 0.0f;

    float secondsElapsed = win64::get_seconds_elapsed( beginCounter, win64::get_timer() );

    if ( win64::global::spfCap )
    {
      //      PROFILE_SCOPE( debug_CyclesSleep );
      if ( secondsElapsed < win64::global::spfCap )
      {
        float const msSleep = ((win64::global::spfCap - secondsElapsed) * 1000.f) + sleepMsSubtraction;
        // thread::sleep( s32( max( msSleep, 0.0f ) ) );
        float secondsElapsedIncludingSleep =  win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
        float const delta = 1000.0f * (win64::global::spfCap - secondsElapsedIncludingSleep);
        sleepMsSubtraction += min( 0.f, delta ) - (delta > 2.0f) * 1.0f;


        //   log_info( "[WIN32_CLOCK] frame ", win64::global::coreData.currentFrameIndex, " had ", delta, " ms left after sleeping for ", max( msSleep, 0.f ),
        //                                        " ms\n - - - next sleep reduced by ", -sleepMsSubtraction, " ms\n" );
        do
        {
          secondsElapsedIncludingSleep = win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
        } while ( secondsElapsedIncludingSleep < win64::global::spfCap );
      }
      else
      {
        //  log_info( "[WIN32_CLOCK] Missed fps target for frame: ", win64::global::coreData.currentFrameIndex,
        //                                        "\n- - - - - - - Actual ms: ", 1000.f * secondsElapsed,
        //                                       "   fps: ", float( 1.f / secondsElapsed ), "\n" );
      }
    } // PROFILE_SCOPE( debug_CyclesSleep );
  }

  log_info( "hah" );

  //LARGE_INTEGER endCounter = win64::get_timer();
}

void bse_win64_process_window_messages()
{
  bse::Input& input = win64::global::platform.thisFrame.input;
  memset( input.down, 0, bse::Input::STATE_COUNT );
  MSG message;
  while ( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) )
  {
    input.mousePos[0].start = input.mousePos[0].end;

    switch ( message.message )
    {
      case WM_XBUTTONDOWN:
      case WM_XBUTTONUP:
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
      default:
      {
        TranslateMessage( &message );
        DispatchMessage( &message );
      }
      break;
    }

    win64::process_controller_input( win64::global::platform.thisFrame.input );
  }
}



LRESULT CALLBACK bse_main_window_callback( HWND window, UINT message, WPARAM wParam, LPARAM lParam )
{
  LRESULT result = 0;
  switch ( message )
  {
    case WM_SIZE:
    {
      win64::global::mainWindowSize = int2 { s32( LOWORD( lParam ) ), s32( HIWORD( lParam ) ) };
      if ( wParam == SIZE_MAXIMIZED || wParam == SIZE_MINIMIZED )
      {
        win64::opengl::resize_viewport( win64::global::mainWindowSize );
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
      win64::opengl::resize_viewport( win64::global::mainWindowSize );
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
        //        memset( win64::global::coreData.input.held, 0, bse::Input::STATE_COUNT );
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

#if defined (BSE_DEVELOP_BUILD) || defined(BSE_DEBUG_BUILD)

char const* BSE_TMP_CORE_FILENAME_0 = "bse_core0.tmp.dll";
char const* BSE_TMP_CORE_FILENAME_1 = "bse_core1.tmp.dll";
char const* BSE_APP_FILENAME = "bse_core.dll";

void bse_win64_init_core()
{
  win64::BseCore& coreDll = win64::global::core;

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
  Platform* platform = &win64::global::platform;
};

#else
#include "bse_core.cpp"

void bse_win64_init_core()
{
  win64::global::core.initialize = &bse::core_initialize_internal;
  win64::global::core.on_reload = &bse::core_on_reload_internal;
  win64::global::core.tick = &bse::core_tick_internal;
}

#endif


#include "bse_win64.h"
#include "bse_win64_opengl.h"

LRESULT CALLBACK bse_main_window_callback( HWND window, UINT message, WPARAM wParam, LPARAM lParam );
void bse_init_core();
void bse_win64_loop();

int bse_main( int argc, char** argv )
{
  s32 result = 1;
  result = QueryPerformanceFrequency( (LARGE_INTEGER*) &win64::global::performanceCounterFrequency );
  assert( result );

  ////////////////////////////////////////////////////////////////////////
  //// Init Core and fetch app specifications for platform ///////////////
  ////////////////////////////////////////////////////////////////////////

  bse_init_core();
  bse::PlatformInitParams initParams;
  initParams.programHandle = GetModuleHandle( NULL );
  initParams.commandLine.argumentCount = argc;
  initParams.commandLine.arguments = argv;
  win64::global::app.initialize( &initParams );

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Console (Is this relevant?) ////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  if ( !initParams.console.skipInitConsole ) { CoInitializeEx( 0, COINIT_MULTITHREADED ); }

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Network ////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  #if defined(BSE_BUILD_NETWORK)
  if ( !initParams.network.skipInitNetwork )
  {
    WSADATA wsaData;
    result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    assert( result == 0 );
  }
  #endif

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Input //////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  #if defined(BSE_BUILD_INPUT)
  if ( !initParams.input.skipInitController )
  {
    win64::load_xInput();
  }
  #endif

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Graphics ///////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  #if defined (BSE_BUILD_GRAPHICS)
  win64::global::openglDll = LoadLibraryA( "opengl32.dll" );
  assert( win64::global::openglDll );

  if ( !initParams.window.skipInitWindow )
  {
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

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Audio //////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  #if defined (BSE_BUILD_AUDIO)    
  if ( !initParams.audio.skipInitAudio )
  {
  }
  #endif

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Worker Threads /////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {

  }

  ////////////////////////////////////////////////////////////////////////
  ////////// Init Time  //////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {
    result = (s32) timeBeginPeriod( 1 );
    assert( result == TIMERR_NOERROR );
  }

  ////////////////////////////////////////////////////////////////////////
  ////////// Init App Callbacks //////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {

  }

  ////////////////////////////////////////////////////////////////////////
  ////////// Init App ////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {

  }

  ////////////////////////////////////////////////////////////////////////
  ////////// Run Core Loop ///////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  win64::global::running = true;
  while ( win64::global::running )
  {
    //1) Pause thread if requested?

    //2) Loop
    bse_win64_loop();

    //3) Swap Buffers?

    //4) Increase Frame Index
    ++win64::global::platform.thisFrame.frameIndex;
    printf( "%d", u32( win64::global::platform.thisFrame.frameIndex ) );
  }

  return result;
}

void bse_win64_loop()
{

  LARGE_INTEGER beginCounter = win64::get_timer();

  ////////////////////////////////////////////////////////////////////////
  ////////// Process Input & Window Messages /////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {
    //peek input message

    win64::process_controller_input( win64::global::platform.thisFrame.input );
  }

  ////////////////////////////////////////////////////////////////////////
  ////////// Process Core Tick ///////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {

  }

  ////////////////////////////////////////////////////////////////////////
  ////////// Adjust FPS //////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  {
    constexpr float APP_TARGET_FPS = 60.0f;
    constexpr float APP_TARGET_SPF = 1.0f / float( APP_TARGET_FPS );
    static float sleepMsSubtraction = 0.0f;

    float secondsElapsed = win64::get_seconds_elapsed( beginCounter, win64::get_timer() );

    if ( win64::global::spfCap )
    {

      //      PROFILE_SCOPE( debug_CyclesSleep );
      if ( secondsElapsed < APP_TARGET_SPF )
      {
        float const msSleep = ((APP_TARGET_SPF - secondsElapsed) * 1000.f) + sleepMsSubtraction;
        // thread::sleep( s32( max( msSleep, 0.0f ) ) );
        float secondsElapsedIncludingSleep =  win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
        float const delta = 1000.0f * (APP_TARGET_SPF - secondsElapsedIncludingSleep);
        sleepMsSubtraction += min( 0.f, delta ) - (delta > 2.0f) * 1.0f;

        //   log_info( "[WIN32_CLOCK] frame ", win64::global::appData.currentFrameIndex, " had ", delta, " ms left after sleeping for ", max( msSleep, 0.f ),
        //                                        " ms\n - - - next sleep reduced by ", -sleepMsSubtraction, " ms\n" );
        do
        {
          secondsElapsedIncludingSleep = win64::get_seconds_elapsed( beginCounter, win64::get_timer() );
        } while ( secondsElapsedIncludingSleep < APP_TARGET_SPF );
      }
      else
      {
        //  log_info( "[WIN32_CLOCK] Missed fps target for frame: ", win64::global::appData.currentFrameIndex,
        //                                        "\n- - - - - - - Actual ms: ", 1000.f * secondsElapsed,
        //                                       "   fps: ", float( 1.f / secondsElapsed ), "\n" );
      }
    } // PROFILE_SCOPE( debug_CyclesSleep );

    //LARGE_INTEGER endCounter = win64::get_timer();
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
        //        memset( win64::global::appData.input.held, 0, bs::input::STATE_COUNT );
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

#if defined (BSE_RELEASE_BUILD)
#include "bse_core.cpp"

void bse_init_core()
{
  win64::global::app.initialize = &bse::core_initialize_internal;
  win64::global::app.on_reload = &bse::core_on_reload_internal;
  win64::global::app.tick = &bse::core_tick_internal;

  // char exePath[1024] = {};
  // win64::get_exe_path( exePath, 1024 );
  win64::global::app.initialize( initParams );
}

#else

char const* BSE_TMP_CORE_FILENAME_0 = "bse_core0.tmp.dll";
char const* BSE_TMP_CORE_FILENAME_1 = "bse_core1.tmp.dll";
char const* BSE_APP_FILENAME = "bse_core.dll";

void bse_init_core()
{
  win64::App& newApp = win64::global::app;

  _WIN32_FIND_DATAA findData;
  HANDLE findHandle = FindFirstFileA( BSE_APP_FILENAME, &findData );
  if ( findHandle != INVALID_HANDLE_VALUE )
  {
    FindClose( findHandle );

    if ( CopyFileA( BSE_APP_FILENAME, BSE_TMP_CORE_FILENAME_0, false ) )
    {
      newApp.dll = LoadLibraryA( BSE_TMP_CORE_FILENAME_0 );//~120M cycles
      if ( newApp.dll )
      {
        newApp.initialize = (bse::core_initialize_fn*) GetProcAddress( newApp.dll, "core_initialize_internal" );
        newApp.on_reload = (bse::core_on_reload_fn*) GetProcAddress( newApp.dll, "core_on_reload_internal" );
        newApp.tick = (bse::core_tick_fn*) GetProcAddress( newApp.dll, "core_tick_internal" );

        if ( newApp.initialize && newApp.on_reload && newApp.tick )
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
#endif


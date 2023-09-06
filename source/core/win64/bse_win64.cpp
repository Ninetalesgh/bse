#include "bse_win64.h"
#include "bse_win64_opengl.h"


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
        glViewport( 0, 0, win64::global::mainWindowSize.x, win64::global::mainWindowSize.y );
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
      glViewport( 0, 0, win64::global::mainWindowSize.x, win64::global::mainWindowSize.y );
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



void bse_init_core();
int bse_main( int argc, char** argv )
{
  HINSTANCE hInstance = GetModuleHandle( NULL );

  s32 result = 1;
  result = QueryPerformanceFrequency( (LARGE_INTEGER*) &win64::global::performanceCounterFrequency );
  assert( result );

  bse_init_core();
  bse::PlatformInitParams initParams;
  initParams.commandLine.argumentCount = argc;
  initParams.commandLine.arguments = argv;

  win64::global::app.initialize( &initParams );

  if ( !initParams.console.skipInitConsole )
  {
    CoInitializeEx( 0, COINIT_MULTITHREADED );
  }

  if ( !initParams.network.skipInitNetwork )
  {
    #if defined(BSE_BUILD_NETWORK)
    WSADATA wsaData;
    result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    assert( result == 0 );
    #else
    BREAK;
    #endif
  }

  if ( !initParams.input.skipInitController )
  {
    #if defined(BSE_BUILD_INPUT)
    win64::load_xInput();
    #else
    BREAK;
    #endif
  }

  if ( !initParams.window.skipInitWindow )
  {
    #if defined (BSE_BUILD_GRAPHICS)
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
    parameter.wndClass.hInstance     = hInstance;
    parameter.wndClass.hCursor       = LoadCursor( (HINSTANCE) NULL, IDC_ARROW );
    parameter.wndClass.lpszClassName = L"bsewnd";

    win64::global::mainWindow = win64::init_window( parameter );

    win64::global::openglDll = LoadLibraryA( "opengl32.dll" );
    if ( !win64::global::openglDll )
    {
      BREAK;
    }
    assert( win64::global::mainWindow != 0 );
    HDC deviceContext = GetDC( win64::global::mainWindow );
    assert( deviceContext != 0 );
    win64::opengl::init( deviceContext, initParams.window.size );

    #else
    BREAK;
    #endif
  }

  if ( !initParams.audio.skipInitAudio )
  {
    #if defined (BSE_BUILD_AUDIO)    
    #else
    BREAK;
    #endif
  }

  return 0;
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


#include "bse_win64.h"


LRESULT CALLBACK bse_main_window_callback( HWND window, UINT message, WPARAM wParam, LPARAM lParam )
{
  LRESULT result = 0;
  // switch ( message )
  // {
  //   case WM_SIZE:
  //   {
  //     global::mainWindow.size.x = s32( LOWORD( lParam ) );
  //     global::mainWindow.size.y = s32( HIWORD( lParam ) );
  //     if ( wParam == SIZE_MAXIMIZED || wParam == SIZE_MINIMIZED )
  //     {
  //       opengl::resize_viewport( global::mainWindow.size );
  //     }
  //     break;
  //   }
  //   case WM_MOVE:
  //   {
  //     global::mainWindow.pos.x = s32( LOWORD( lParam ) );
  //     global::mainWindow.pos.y = s32( HIWORD( lParam ) );
  //     break;
  //   }
  //   case WM_DISPLAYCHANGE:
  //   case WM_EXITSIZEMOVE:
  //   {
  //     opengl::resize_viewport( global::mainWindow.size );
  //     break;
  //   }
  //   case WM_DESTROY:
  //   {
  //     global::running = false;
  //     break;
  //   }
  //   case WM_ACTIVATE:
  //   case WM_ACTIVATEAPP:
  //   {
  //     if ( !wParam )
  //     {
  //       memset( global::appData.input.held, 0, bs::input::STATE_COUNT );
  //     }
  //     break;
  //   }
  //   default:
  //   {
  //     result = DefWindowProc( window, message, wParam, lParam );
  //     break;
  //   }
  // }

  return result;
}


void bse_init_core( bse::PlatformInitParams* );
int bse_main( int argc, char** argv )
{
  CoInitializeEx( 0, COINIT_MULTITHREADED );
  s32 result = 1;
  HINSTANCE hInstance = GetModuleHandle( NULL );

  result = QueryPerformanceFrequency( (LARGE_INTEGER*) &win64::global::performanceCounterFrequency );
  assert( result );

  bse::PlatformInitParams initParams;
  bse_init_core( &initParams );

  #if defined(BSE_BUILD_NETWORK)
  WSADATA wsaData;
  result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
  assert( result == 0 );
  #endif

  #if defined(BSE_BUILD_INPUT)
  win64::load_xInput();
  #endif


  ///////////////////////////
  //TODO wait for dll loader here?
  ///////////////////////////


  #if defined (BSE_BUILD_GRAPHICS)
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
  parameter.wndClass.lpszClassName = L"bswnd";
  #endif

  #if defined (BSE_BUILD_AUDIO)

  #endif

  return 0;
}


#if defined (BSE_RELEASE_BUILD)
#include "bse_core.cpp"

void bse_init_core( bse::PlatformInitParams* initParams )
{
  win64::global::app.initialize = &bse::core_initialize_internal;
  win64::global::app.on_reload = &bse::core_on_reload_internal;
  win64::global::app.tick = &bse::core_tick_internal;

  // char exePath[1024] = {};
  // win64::get_exe_path( exePath, 1024 );
  win64::global::app.initialize( initParams );

  win64::global::app.on_reload( initParams );
}

#else

char const* BSE_TMP_CORE_FILENAME_0 = "bse_core0.tmp.dll";
char const* BSE_TMP_CORE_FILENAME_1 = "bse_core1.tmp.dll";
char const* BSE_APP_FILENAME = "bse_core.dll";

void bse_init_core( bse::PlatformInitParams* initParams )
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

  win64::global::app.initialize( initParams );
}

namespace bse
{
  Platform* platform;
};
#endif


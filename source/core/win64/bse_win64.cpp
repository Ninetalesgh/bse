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


void bse_init_core();
int bse_main( int argc, char** argv )
{
  CoInitializeEx( 0, COINIT_MULTITHREADED );
  s32 result = 1;
  HINSTANCE hInstance = GetModuleHandle( NULL );


  result = QueryPerformanceFrequency( (LARGE_INTEGER*) &win64::global::performanceCounterFrequency );
  assert( result );

  bse_init_core();

  //TODO forward args to app, app should have a full shortcut built in


  #if defined(BSE_BUILD_NETWORK)
  WSADATA wsaData;
  result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
  assert( result == 0 );
  #endif

  #if defined(BSE_BUILD_INPUT)
  win64::load_xInput();
  #endif

  #if defined (BSE_BUILD_GRAPHICS)
  win64::WindowInitParameter parameter {};
  parameter.windowName = L"tmp_window_name";
  //parameter.width = DEFAULT_WINDOW_SIZE.x;
  //parameter.height = DEFAULT_WINDOW_SIZE.y;
  parameter.x = 900;//-parameter.width - 200;
  parameter.y = 0;//200;
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


#if 1
//#ifdef BS_RELEASE_BUILD
#include "bse_core.cpp"

void bse_init_core()
{
  win64::global::app.initialize = &bse::core_initialize_internal;
  win64::global::app.on_reload = &bse::core_on_reload_internal;
  win64::global::app.tick = &bse::core_tick_internal;

  // char exePath[1024] = {};
  // win64::get_exe_path( exePath, 1024 );
  win64::global::app.on_reload( &win64::global::platform );
}

#else

void bse_init_core()
{
  win32::PrmThreadDllLoader dllLoaderPrm = {};
  thread::ThreadInfo standaloneDllLoadThread {};
  dllLoaderPrm.threadInfo = &standaloneDllLoadThread;
  dllLoaderPrm.appDll =  &global::appDll;
  dllLoaderPrm.renderContext = opengl::create_render_context_for_worker_thread();
  CloseHandle( CreateThread( 0, 0, win32::thread_DllLoader, &dllLoaderPrm, 0, (LPDWORD) &dllLoaderPrm.threadInfo->id ) );
  //wait until stack can pop
  while ( dllLoaderPrm.threadInfo != nullptr )
  {
    thread::sleep( 0 );
  }
}

namespace bsp
{
  PlatformCallbacks* platform = &global::platformCallbacks;
};
#endif


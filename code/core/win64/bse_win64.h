#pragma once

//Winsock
#pragma comment(lib,"ws2_32.lib")
//WinMain
#pragma comment(lib,"user32.lib")
//Time
#pragma comment(lib,"winmm.lib")
//CoInitialize
#pragma comment(lib,"Ole32.lib")
//Graphics
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"Gdi32.lib")


#include "core/bse_core.h"

#include "core/bse_precompiled_assets.generated.cpp"

#include <stdio.h>

#include <windows.h>
#ifdef ERROR
# undef ERROR
#endif

#ifdef min
# undef min
#endif
#ifdef max
# undef max
#endif
#include <Xinput.h>

//#include "bse_win64_opengl.h"

constexpr s32 BSE_PATH_MAX = 1024;
namespace win64
{
  void windows_error()
  {
    u32 errormsg = GetLastError();
    log_error( "[WINDOWS ERROR] ", errormsg );
    #if defined(BSE_BUILD_DEBUG)
    BREAK;
    #endif
  }

  s32 utf8_to_wchar( char const* utf8String, wchar_t* out_wcharString, s32 wcharLengthMax )
  {
    s32 wcharLength = MultiByteToWideChar( CP_UTF8, 0, utf8String, -1, 0, 0 );
    assert( wcharLength <= wcharLengthMax );
    return MultiByteToWideChar( CP_UTF8, 0, utf8String, -1, out_wcharString, min( wcharLength, wcharLengthMax ) );
  }

  s32 wchar_to_utf8( wchar_t const* wcharString, char* out_utf8String, s32 utf8StringLengthMax )
  {
    s32 utf8StringLength = WideCharToMultiByte( CP_UTF8, 0, wcharString, -1, 0, 0, 0, 0 );
    assert( utf8StringLength <= utf8StringLengthMax );
    return WideCharToMultiByte( CP_UTF8, 0, wcharString, -1, out_utf8String, min( utf8StringLength, utf8StringLengthMax ), 0, 0 );
  }

  s32 get_exe_path( char* out_exePath, s32 exePathLengthMax )
  {
    wchar_t wideChars[BSE_PATH_MAX];
    s32 filePathLength = GetModuleFileNameW( 0, wideChars, BSE_PATH_MAX );
    assert( filePathLength <= exePathLengthMax );
    return wchar_to_utf8( wideChars, out_exePath, exePathLengthMax );
  }

  using xInputGetState_fn = DWORD WINAPI( DWORD, XINPUT_STATE* );
  using xInputSetState_fn = DWORD WINAPI( DWORD, XINPUT_VIBRATION* );

  namespace stub
  {
    void core_initialize( bse::PlatformInitParams*, bse::Platform* ) {}
    void core_on_reload( bse::Platform* ) {}
    void core_tick( bse::Platform* ) {}

    DWORD WINAPI xInputGetState( DWORD, XINPUT_STATE* ) { return ERROR_DEVICE_NOT_CONNECTED; }
    DWORD WINAPI xInputSetState( DWORD, XINPUT_VIBRATION* ) { return ERROR_DEVICE_NOT_CONNECTED; }
  };

  struct BseCore
  {
    HMODULE dll;
    bse::core_initialize_fn* initialize = stub::core_initialize;
    bse::core_on_reload_fn* on_reload = stub::core_on_reload;
    bse::core_tick_fn* tick = stub::core_tick;
  };

  struct WindowContext
  {
    HWND handle;
    HDC deviceContext;
    HGLRC renderContext;
    int2 size;
  };

  namespace global
  {
    static bse::Platform      platform;
    static xInputGetState_fn* xInputGetState;
    static xInputSetState_fn* xInputSetState;
    static s64                performanceCounterFrequency;
    static BseCore            bseCore;
    static HMODULE            openglDll;
    static WindowContext      mainWindow;
    static bool               running;
  };

  INLINE LARGE_INTEGER get_timer()
  {
    LARGE_INTEGER result;
    QueryPerformanceCounter( &result );
    return result;
  }

  INLINE float get_seconds_elapsed( LARGE_INTEGER beginCounter, LARGE_INTEGER endCounter )
  {
    return float( endCounter.QuadPart - beginCounter.QuadPart ) / float( global::performanceCounterFrequency );
  }

  #if !defined(BSE_BUILD_SKIP_INPUT)
  void load_xInput()
  {
    HMODULE module = LoadLibraryA( "xinput1_4.dll" );

    if ( !module )
    {
      module = LoadLibraryA( "xinput1_3.dll" );
    }

    if ( module )
    {
      global::xInputGetState = (xInputGetState_fn*) GetProcAddress( module, "XInputGetState" );
      global::xInputSetState = (xInputSetState_fn*) GetProcAddress( module, "XInputSetState" );

      #if DEBUG_LOG_INPUT
      OutputDebugStringA( "[WIN32_INPUT] Controller input initialized.\n" );
      #endif

    }
    else
    {
      OutputDebugStringA( "[WIN32_INPUT] ERROR - neither xinput1_4.dll or xinput1_3.dll found.\n" );
      global::xInputGetState = stub::xInputGetState;
      global::xInputSetState = stub::xInputSetState;
    }
  }

  INLINE void process_xInput_digital_button( WORD wButtons, DWORD buttonBit, bse::Input::ControllerButton& state, bse::Input::ControllerButton const& oldState )
  {
    state.halfTransitionCount = (state.endedDown != oldState.endedDown) ? 1 : 0;
    state.endedDown = wButtons & buttonBit;
  }

  void process_controller_input( bse::Input& input )
  {
    constexpr s32 supportedControllers = array_count( input.controller );
    constexpr s32 maxControllerCount = min( supportedControllers, XUSER_MAX_COUNT );

    for ( DWORD iController = 0; iController < (DWORD) maxControllerCount; ++iController )
    {
      XINPUT_STATE controllerState;
      if ( global::xInputGetState( iController, &controllerState ) == ERROR_SUCCESS )
      {
        //controller plugged in
        XINPUT_GAMEPAD& pad = controllerState.Gamepad;
        bse::Input::Controller& newController = input.controller[iController];

        process_xInput_digital_button( pad.wButtons, XINPUT_GAMEPAD_DPAD_UP, newController.up, newController.up );
        process_xInput_digital_button( pad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN, newController.down, newController.down );
        process_xInput_digital_button( pad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT, newController.left, newController.left );
        process_xInput_digital_button( pad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT, newController.right, newController.right );
        process_xInput_digital_button( pad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, newController.leftShoulder, newController.leftShoulder );
        process_xInput_digital_button( pad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER, newController.rightShoulder, newController.rightShoulder );

        float stickX         = float( pad.sThumbLX ) / (pad.sThumbLX < 0 ? 32768.f : 32767.f);
        float stickY         = float( pad.sThumbLY ) / (pad.sThumbLY < 0 ? 32768.f : 32767.f);

        newController.start = newController.end;
        newController.min = newController.max = newController.end = { stickX, stickY };
        //bool start         = pad.wButtons & XINPUT_GAMEPAD_START;
        //bool back          = pad.wButtons & XINPUT_GAMEPAD_BACK;
        //bool buttonA       = pad.wButtons & XINPUT_GAMEPAD_A;
        //bool buttonB       = pad.wButtons & XINPUT_GAMEPAD_B;
        //bool buttonX       = pad.wButtons & XINPUT_GAMEPAD_X;
        //bool buttonY       = pad.wButtons & XINPUT_GAMEPAD_Y;
      }
      else
      {
        //controller not available
      }
    }

    // {
    //   XINPUT_VIBRATION vibrationState { 0, 0 };
    //   global::xInputSetState( 0, &vibrationState );
    // }
  }
  #endif
};

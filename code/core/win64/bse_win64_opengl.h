#pragma once
#include "core/include/bse_opengl_ext.h"
#include "core/include/bse_color.h"

namespace win64
{
  namespace opengl
  {
    s32 load_extensions();
    s32 init_extensions();
    // u32 init( HDC deviceContext, int2 viewportDimensions );

    HGLRC create_render_context( HDC deviceContext, HGLRC shareContext );

    s32 init_device_context( HDC deviceContext );
    s32 assign_render_context_to_current_thread( HDC deviceContext, HGLRC shareContext );

    //called on the main thread the context was established on
    HGLRC create_render_context_for_worker_thread();

    //called on the new thread
    void set_worker_thread_render_context( HGLRC renderContext );

    void swap_buffers( HDC deviceContext );

    void resize_viewport( int2 dimensions );
  };
};

namespace opengl_ext
{
  using wglCreateContextAttribsARB = HGLRC WINAPI( HDC hDC, HGLRC hShareContext, const int* attribList );
  using wglChoosePixelFormatARB = BOOL WINAPI( HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats );
  using wglGetPixelFormatAttribivARB = BOOL WINAPI( HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int* piValues );
  using wglGetPixelFormatAttribfvARB = BOOL WINAPI( HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT* pfValues );
  using wglSwapIntervalEXT = BOOL WINAPI( int );
};

static opengl_ext::wglChoosePixelFormatARB* wglChoosePixelFormatARB;
static opengl_ext::wglCreateContextAttribsARB* wglCreateContextAttribsARB;
static opengl_ext::wglGetPixelFormatAttribivARB* wglGetPixelFormatAttribivARB;
static opengl_ext::wglGetPixelFormatAttribfvARB* wglGetPixelFormatAttribfvARB;
static opengl_ext::wglSwapIntervalEXT* wglSwapIntervalEXT;

namespace win64
{
  namespace opengl
  {
    struct OpenGlInfo
    {
      bool extEnabled;
      char const* vendor;
      char const* renderer;
      char const* version;
      char const* shadingLanguageVersion;
      char const* extensions;
      u32 GL_EXT_texture_sRGB;
      u32 GL_EXT_framebuffer_sRGB;
    };

    struct Context
    {
      HGLRC renderContext;
      HDC   deviceContext;
    };

    namespace oglglobal
    {
      static OpenGlInfo info;
      static u32 defaultTextureFormat;
      static s32 rcAttributes[] =
      {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB /*| WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB*/,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0
      };
    };

    void init_opengl_info()
    {
      oglglobal::info.vendor = (char const*) glGetString( GL_VENDOR );
      oglglobal::info.renderer = (char const*) glGetString( GL_RENDERER );
      oglglobal::info.version = (char const*) glGetString( GL_VERSION );
      oglglobal::info.shadingLanguageVersion = (char const*) glGetString( GL_SHADING_LANGUAGE_VERSION );
      oglglobal::info.extensions = (char const*) glGetString( GL_EXTENSIONS );
      oglglobal::info.GL_EXT_texture_sRGB = bse::string_contains( oglglobal::info.extensions, "GL_EXT_texture_sRGB" ) != nullptr;
      oglglobal::info.GL_EXT_framebuffer_sRGB = bse::string_contains( oglglobal::info.extensions, "GL_EXT_framebuffer_sRGB" ) != nullptr;
    };

    s32 set_pixel_format_for_dc( HDC deviceContext )
    {
      s32 suggestedPixelFormatIndex = 0;

      if ( wglChoosePixelFormatARB )
      {
        s32 intAttribList[] =
        {
          WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
          WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
          WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
          WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
          WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
          WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
          WGL_COLOR_BITS_ARB, 24,
          WGL_RED_BITS_ARB, 8,
          WGL_RED_SHIFT_ARB, 16,
          WGL_GREEN_BITS_ARB, 8,
          WGL_GREEN_SHIFT_ARB, 8,
          WGL_BLUE_BITS_ARB, 8,
          WGL_BLUE_SHIFT_ARB, 0,
          WGL_ALPHA_BITS_ARB, 8,
          WGL_ALPHA_SHIFT_ARB, 24,
        0 };
        //TODO this doesn't give me any alpha bits and alpha shift, whereas the non wgl call does?
        float floatAttribList[] = { 0 };
        u32 extendedPick = 0;
        wglChoosePixelFormatARB( deviceContext, intAttribList, floatAttribList, 1, &suggestedPixelFormatIndex, &extendedPick );
      }
      else
      {
        PIXELFORMATDESCRIPTOR desiredPixelFormat {};
        desiredPixelFormat.nSize = sizeof( desiredPixelFormat );
        desiredPixelFormat.nVersion = 1;
        desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        desiredPixelFormat.cColorBits = 32;
        desiredPixelFormat.cAlphaBits = 8;
        desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;
        suggestedPixelFormatIndex = ChoosePixelFormat( deviceContext, &desiredPixelFormat );
      }

      PIXELFORMATDESCRIPTOR suggestedPixelFormat {};
      DescribePixelFormat( deviceContext, suggestedPixelFormatIndex, sizeof( suggestedPixelFormat ), &suggestedPixelFormat );

      s32 result = SetPixelFormat( deviceContext, suggestedPixelFormatIndex, &suggestedPixelFormat );
      return result;
    }

    s32 load_extensions()
    {
      s32 result = 0;
      HWND dummyWindow = 0;
      WNDCLASSW wndClass {};
      wndClass.lpfnWndProc   = DefWindowProcW;
      wndClass.hInstance     = GetModuleHandle( NULL );
      wndClass.lpszClassName = L"OpenGlExtensionLoaderDummy";

      if ( RegisterClassW( &wndClass ) )
      {
        dummyWindow = CreateWindowExW( 0,
                                       wndClass.lpszClassName, L"dummy",
                                       0,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       0,
                                       0,
                                       wndClass.hInstance,
                                       0 );

        HDC dummyDC = GetDC( dummyWindow );
        set_pixel_format_for_dc( dummyDC );

        HGLRC dummyRC = wglCreateContext( dummyDC );
        if ( !wglMakeCurrent( dummyDC, dummyRC ) )
        {
          BREAK;
          return 0;
        }

        ::wglChoosePixelFormatARB =    (opengl_ext::wglChoosePixelFormatARB*) wglGetProcAddress( "wglChoosePixelFormatARB" );
        ::wglCreateContextAttribsARB =  (opengl_ext::wglCreateContextAttribsARB*) wglGetProcAddress( "wglCreateContextAttribsARB" );
        ::wglGetPixelFormatAttribivARB = (opengl_ext::wglGetPixelFormatAttribivARB*) wglGetProcAddress( "wglGetPixelFormatAttribivARB" );
        ::wglGetPixelFormatAttribfvARB = (opengl_ext::wglGetPixelFormatAttribfvARB*) wglGetProcAddress( "wglGetPixelFormatAttribfvARB" );
        ::wglSwapIntervalEXT =         (opengl_ext::wglSwapIntervalEXT*) wglGetProcAddress( "wglSwapIntervalEXT" );

        if ( ::wglChoosePixelFormatARB == nullptr ) { BREAK; return 0; }
        if ( ::wglCreateContextAttribsARB == nullptr ) { BREAK; return 0; }
        if ( ::wglGetPixelFormatAttribivARB == nullptr ) { BREAK; return 0; }
        if ( ::wglGetPixelFormatAttribfvARB == nullptr ) { BREAK; return 0; }
        if ( ::wglSwapIntervalEXT == nullptr ) { BREAK; return 0; }

        oglglobal::info.extEnabled = wglCreateContextAttribsARB == nullptr ? false : true;

        wglMakeCurrent( 0, 0 );
        wglDeleteContext( dummyRC );
        ReleaseDC( dummyWindow, dummyDC );
        DestroyWindow( dummyWindow );

        result = 1;
      }

      return result;
    }

    s32 init_extensions( opengl_ext::get_proc_address_fn* get_proc_address )
    {
      opengl_ext::init( get_proc_address );
      glEnable( GL_DEBUG_OUTPUT );
      //TODO debug message callback
      // glDebugMessageCallback( gl_debug_message_callback, nullptr );
      check_gl_error();
      init_opengl_info();
      return 1;
    }

    s32 init_framebuffer()
    {
      oglglobal::defaultTextureFormat = oglglobal::info.GL_EXT_texture_sRGB ? GL_RGBA8 : GL_SRGB8_ALPHA8;
      if ( oglglobal::info.GL_EXT_framebuffer_sRGB )
      {
        glEnable( GL_FRAMEBUFFER_SRGB );
      }

      if ( wglSwapIntervalEXT ) //vsync probably TODO move this somewhere else
      {
        wglSwapIntervalEXT( 1 );
      }

      glEnable( GL_DEPTH_TEST );
      glDepthFunc( GL_LESS );

      float4 bg = color::float4_from_rgba( color::rgba( 30, 30, 30, 255 ) );
      glClearColor( bg.x * bg.x, bg.y * bg.y, bg.z * bg.z, bg.w );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      check_gl_error();

      return 1;
    }

    HGLRC create_render_context( HDC deviceContext, HGLRC shareContext )
    {
      HGLRC renderContext {};

      if ( wglCreateContextAttribsARB )
      {
        renderContext = wglCreateContextAttribsARB( deviceContext, shareContext, oglglobal::rcAttributes );
      }

      if ( !renderContext )
      {
        renderContext = wglCreateContext( deviceContext );
        BREAK;
      }
      return renderContext;
    }

    s32 assign_render_context_to_current_thread( HDC deviceContext, HGLRC renderContext )
    {
      if ( !wglMakeCurrent( deviceContext, renderContext ) )
      {
        BREAK;
        return 0;
      }

      return 1;
    }

    s32 init_lazy_default( HDC deviceContext, int2 viewportDimensions )
    {
      resize_viewport( viewportDimensions );

      // glEnable( GL_CULL_FACE );
      // glCullFace( GL_BACK );



      return 1;
    }

    HGLRC create_render_context_for_worker_thread( HDC deviceContext, HGLRC renderContext )
    {
      check_gl_error();
      HGLRC rc = wglCreateContextAttribsARB( deviceContext, renderContext, oglglobal::rcAttributes );
      check_gl_error();
      return rc;
    }

    void set_worker_thread_render_context( HDC deviceContext, HGLRC renderContext )
    {
      if ( !wglMakeCurrent( deviceContext, renderContext ) )
      {
        BREAK;
      }
    }

    void swap_buffers( HDC deviceContext )
    {
      SwapBuffers( deviceContext );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void resize_viewport( int2 dimensions )
    {
      glViewport( 0, 0, dimensions.x, dimensions.y );
    }
  };
};


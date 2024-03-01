
#include "bse_core.h"

void initialize_platform( bse::PlatformInitParams* );
void on_reload();
void initialize_app_data();
void tick();

struct AppData;

#if defined(BSE_BUILD_APP_PATH)
#include _STRINGIZE(BSE_BUILD_APP_PATH)
#else
struct AppData {}
//include devenv here?
void initialize_platform( bse::PlatformInitParams* ) { log_info( "Initialized nothing." ); }
void initialize_app_data() {}
void on_reload() { log_info( "Reloaded nothing." ); }
void tick() { log_info( "ticked nothing. dt: ", bse::platform->thisFrame.deltaTime, "\n- frame index: ", bse::platform->thisFrame.frameIndex ); }
#endif

AppData* appData;
namespace bse
{
  Platform* platform;

  void core_initialize_internal( PlatformInitParams* platformParameters, Platform* pf )
  {
    platformParameters->window.name = "bse";
    platformParameters->window.size = int2 { 1024,720 };
    platformParameters->window.position = int2 { -platformParameters->window.size.x - 200, 200 };

    platform = pf;
    initialize_platform( platformParameters );
  }

  void core_on_reload_internal( Platform* pf )
  {
    platform = pf;
    #if  !defined(BSE_BUILD_SKIP_GRAPHICS)
    opengl_ext::init( platform->opengl_get_process_address );
    #endif

    if ( !platform->userData )
    {
      void*& ptr = bse::platform->userData;
      ptr = bse::memory::allocate_thread_safe( sizeof( AppData ) );
      memset( ptr, 0, sizeof( AppData ) );
      appData = new (ptr) AppData();
      initialize_app_data();
    }
    on_reload();
  }

  void core_tick_internal( Platform* pf )
  {
    tick();
  }

};


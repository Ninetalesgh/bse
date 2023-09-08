
#include "bse_core.h"

void initialize( bse::PlatformInitParams* );
void on_reload();
void tick();


namespace bse
{
  void core_initialize_internal( PlatformInitParams* platformParameters, Platform* pf )
  {
    platformParameters->window.name = "bse";
    platformParameters->window.size = int2 { 1024,720 };
    platformParameters->window.position = int2 { -platformParameters->window.size.x - 200, 200 };

    platform = pf;
    initialize( platformParameters );
  }

  void core_on_reload_internal( Platform* pf )
  {
    platform = pf;
    #if  !defined(BSE_BUILD_SKIP_GRAPHICS)
    opengl_ext::init( platform->opengl_get_process_address );
    #endif
    on_reload();
  }

  void core_tick_internal( Platform* pf )
  {
    tick();
  }

  Platform* platform;
};

#if defined(BSE_BUILD_APP_PATH)
#include BSE_PREPROCESSOR_STRING_INDIRECTION(BSE_BUILD_APP_PATH)
#else
//include devenv here?
void initialize( bse::PlatformInitParams* ) { log_info( "Initialized nothing." ); }
void on_reload() { log_info( "Reloaded nothing." ); }
void tick()
{
  log_info( "ticked nothing. dt: ", bse::platform->thisFrame.deltaTime, "\n- - frame index: ", bse::platform->thisFrame.frameIndex );
}
#endif

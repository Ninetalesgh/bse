
#include "bse_core.h"

namespace bse
{

  void core_initialize_internal( PlatformInitParams* platformParameters )
  {
    platformParameters->window.name = "bse";
    platformParameters->window.size = int2 { 1024,720 };
    platformParameters->window.position = int2 { -platformParameters->window.size.x - 200, 200 };
    platformParameters->window.fpsCap = 60.0f;

    platform = platformParameters->platform;

    //TODO here ccomes the app init
  }

  void core_on_reload_internal( Platform* pf )
  {
    platform = pf;

    //TODO here ccomes the app reload
  }

  void core_tick_internal( Platform* pf )
  {
    //TODO here ccomes the app tick
  }

  Platform* platform;
};

#include "bse_core.h"

namespace bse
{
  void core_initialize_internal( PlatformInitParams* platformParameters )
  {
    platformParameters->window.size = int2 { 1024,720 };
    platformParameters->window.position = int2 { -platformParameters->window.size.x - 200, 200 };

  }

  void core_on_reload_internal( Platform* pf )
  {
    BREAK;
  }

  void core_tick_internal( Platform* pf )
  {
    BREAK;
  }

  Platform* platform;
};
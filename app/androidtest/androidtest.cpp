#include "androidtest.h"








void initialize_platform( bse::PlatformInitParams* initParameters )
{
  //called once before the platform is fully initialized to set init parameters
}

void initialize_app_data()
{
  //called once after the platform is fully initialized for you to initialize your app data
}

void on_reload()
{
  //called once every time you hot reload
}

void testfun( int, bse::String ) {}

void tick()
{
  auto fun2 = bse::make_function( &on_reload );
  auto fun = bse::make_function( &testfun );
  //called once every frame
}

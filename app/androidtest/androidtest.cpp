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

struct A {
  void test( int ) {}
};

void tick()
{
  //auto fun2 = bse::make_function( &on_reload );
  //auto fun = bse::make_function( &testfun );
  //auto fun3 = bse::make_function( &A::test );
  //called once every frame
}






/*
Tests that need to be done:
TODO

leading_zeros(u16,u32,u64);
sleep
is_current_thread
get_current_thread_id
interlocked_compare_exchange
interlocked_increment
interlocked_decrement



*/

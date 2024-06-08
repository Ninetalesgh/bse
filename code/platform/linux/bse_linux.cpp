#include "bse_linux.h"
#include "bse_linux_platform_callbacks.h"

#include <iostream>

#include "core/bse_core.cpp"


int bse_linux_init();



int bse_main( int argc, char** argv )
{
  int a = 0;
  bse_linux_init();

  return 0;
}

int bse_linux_init()
{
  std::cout << "Hello, Linux World!" << std::endl;



  //initialize_app_data();

  return 0;
}

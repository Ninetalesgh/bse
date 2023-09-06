#define UNICODE

#include "bse_platform.h"

int bse_main( int argc, char** argv );

int main( int argc, char** argv )
{
  return bse_main( argc, argv );
}

#ifdef _WIN32
#include "win64/bse_win64.cpp"
#endif
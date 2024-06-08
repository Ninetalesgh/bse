#define UNICODE

int bse_main( int argc, char** argv );

int main( int argc, char** argv )
{
  return bse_main( argc, argv );
}

#if defined (BSE_PLATFORM_WINDOWS)
#include "platform/win64/bse_win64.cpp"
#elif defined (BSE_PLATFORM_LINUX)
#include "platform/linux/bse_linux.cpp"
#elif defined (BSE_PLATFORM_ANDROID)
#error "Android isn't here, what are you doing?"
#endif
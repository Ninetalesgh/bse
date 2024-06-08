#include "bse_android.h"
#include "bse_android_platform_callbacks.h"


#include <string>

#include "core/bse_core.cpp"

int bse_android_init()
{
  bse::platform->info.virtualMemoryPageSize = sysconf( _SC_PAGE_SIZE );
  log_info( "Virtual memory page size: ", bse::platform->info.virtualMemoryPageSize );

  bse::memory::init_virtual_memory_layout( bse::platform->allocators.virtualMemory );
  log_info( "Virtual memory layout initialized." );

  bse::platform->allocators.mainThread = bse::memory::new_multipool( &bse::platform->allocators.virtualMemory
        , bse::platform->info.virtualMemoryPageSize - sizeof( bse::memory::Multipool ), 16 );

  bse::platform->allocators.threadSafe = bse::memory::new_multipool( &bse::platform->allocators.virtualMemory
    , bse::platform->info.virtualMemoryPageSize - sizeof( bse::memory::Multipool ), 16
    , bse::memory::AllocatorPolicyFlags::AllowGrowth | bse::memory::AllocatorPolicyFlags::GeometricGrowth | bse::memory::AllocatorPolicyFlags::ThreadSafe );

  log_info( "Multipool allocators initialized." );
  //bse::platform->allocate_virtual_memory

//  s64 address = 

  s64 size = GigaBytes( 20 );

  void* result = mmap( 0, size, PROT_READ | PROT_WRITE | PROT_EXEC,
          MAP_ANONYMOUS, 0, 0 );


  if ( result )
  {



    if ( munmap( result, size ) != 0 )
    {
      log_error( "free virtual memory failed with errno: ", errno );
    }
  }







  initialize_app_data();

  return 1;
}








extern "C" JNIEXPORT void JNICALL Java_com_bse_core_BseMainActivity_initializeNative( JNIEnv * env, jobject /* this */, jobject surface ) {
  bse::platform = &global::platform;
  android::register_platform_callbacks();

  global::window = ANativeWindow_fromSurface( env, surface );

  if ( global::window != nullptr )
  {
    if ( !bse_android_init() )
    {
      log_error( "ERROR - Initialization failed." );
      ANativeWindow_release( global::window );
    }
  }
}

extern "C" JNIEXPORT void JNICALL Java_com_bse_core_BseMainActivity_shutdownNative( JNIEnv * env, jobject /* this */ )
{
  if ( bse::platform != nullptr )
  {
    log_info( "Shutting down bse native" );

    ANativeWindow_release( global::window );
    global::window = nullptr;
  }
}



extern "C" JNIEXPORT jstring JNICALL Java_com_bse_core_BseMainActivity_stringFromJNI( JNIEnv * env, jobject /* this */ )
{
  bse::platform = &global::platform;
  android::register_platform_callbacks();

  log_info( "we're testers BSE BSE Hello" );

  //initialize_app_data();

  std::string hello = "Hello from C++";
  return env->NewStringUTF( hello.c_str() );
}






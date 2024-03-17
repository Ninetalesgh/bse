#include "bse_android.h"
#include "bse_android_platform_callbacks.h"


//#include <GLES3/gl32.h>
//#include <GLES2/gl3ext.h>

#include <string>




void test()
{

}


extern "C" JNIEXPORT jstring JNICALL Java_com_bse_core_BseMainActivity_stringFromJNI( JNIEnv * env, jobject /* this */ )
{
  bse::platform = &global::platform;
  android::register_platform_callbacks();

  log_info( "we're testers BSE BSE Hello" );

  std::string hello = "Hello from C++";
  return env->NewStringUTF( hello.c_str() );
}


#include "core/bse_core.cpp"


//#include "androidtest.h"

#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_bse_core_BseMainActivity_stringFromJNI(
        JNIEnv * env,
        jobject /* this */ ) {
  char hello[] = "Hello from C++";
  return env->NewStringUTF( hello );
}



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

void tick()
{
  //called once every frame
}


#include <jni.h>
#include <string>





// #if defined(BSE_BUILD_APP_PATH)
// #include _STRINGIZE(BSE_BUILD_APP_PATH)
// #else



extern "C" JNIEXPORT jstring JNICALL Java_com_bse_core_BseMainActivity_stringFromJNI( JNIEnv * env, jobject /* this */ )
{
  std::string hello = "Hello from C++";
  return env->NewStringUTF( hello.c_str() );
}



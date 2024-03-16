#include "bse_android.h"



#include <string>


namespace bse
{
  Platform* platform;
}


// #if defined(BSE_BUILD_APP_PATH)
// #include _STRINGIZE(BSE_BUILD_APP_PATH)
// #else

void test()
{

}

struct A
{
  void test( int ) {}
};

extern "C" JNIEXPORT jstring JNICALL Java_com_bse_core_BseMainActivity_stringFromJNI( JNIEnv * env, jobject /* this */ )
{

  auto fun = bse::make_function( &test );
  auto fun2 = bse::make_function( &A::test );


  std::string hello = "Hello from C++";
  return env->NewStringUTF( hello.c_str() );
}



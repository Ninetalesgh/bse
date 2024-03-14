@echo off

rem -------------------------------------------------------------------------------------
rem Make sure these environment variables result in something sensible 
rem if you want to compile for android
rem -------------------------------------------------------------------------------------

rem this directory should contain:
rem javac.exe
rem keytool.exe
set BSE_JAVA_BIN_PATH="C:\Program Files\Android\jdk\jdk-8.0.302.8-hotspot\jdk8u302-b08\bin"

rem -------------------------------------------------------------------------------------
rem These expect ANDROID_SDK_ROOT to exist
rem -------------------------------------------------------------------------------------

rem this directory should contain:
rem bin\clang++.exe
rem sysroot
set BSE_CLANG_ROOT="%ANDROID_SDK_ROOT:"=%\ndk\25.1.8937393\toolchains\llvm\prebuilt\windows-x86_64"

rem this directory should contain:
rem zipalign.exe
rem apksigner.bat
rem aapt.exe
rem d8.bat
set BSE_ANDROID_BUILD_TOOLS_PATH="%ANDROID_SDK_ROOT:"=%\build-tools\34.0.0"

rem directory of android.jar
set BSE_ANDROID_JAR_PATH="%ANDROID_SDK_ROOT:"=%\platforms\android-34\android.jar"
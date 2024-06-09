@echo off

rem -------------------------------------------------------------------------------------
rem Make sure these environment variables result in something sensible 
rem if you want to compile for android
rem -------------------------------------------------------------------------------------

rem -------------------------------------------------------------------------------------
rem This expects JAVA_HOME and ANDROID_SDK_ROOT to exist as environment variables
rem -------------------------------------------------------------------------------------

IF NOT DEFINED ANDROID_SDK_ROOT (
  echo ERROR - ANDROID_SDK_ROOT environment variable not set, aborting.
  goto bse_android_shell_error_section
)

IF NOT DEFINED JAVA_HOME (
  echo ERROR - JAVA_HOME environment variable not set, aborting.
  goto bse_android_shell_error_section
)

rem this directory should contain:
rem javac.exe
rem keytool.exe
set BSE_JAVA_BIN_PATH="%JAVA_HOME:"=%\bin"

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

set BSE_ANDROID_BUILD_SHELL_READY=1
goto bse_android_shell_end

:bse_android_shell_error_section
set BSE_ANDROID_BUILD_SHELL_READY=

:bse_android_shell_end
@echo off 
IF NOT DEFINED BSE_JAVA_BIN_PATH call shell_android.bat
setlocal EnableDelayedExpansion

set package_path=com\bse\core

set bat_path=%~dp0
set build_config=Build-configuration:
set app_path=""
set out_name=
set out_path=

IF "%~1"=="build" SHIFT
IF "%~1"=="?" goto help_section

:loop_parse_parameters
SHIFT
IF NOT "%~0"=="" (
  IF "%~0"=="debug" (
    set build_config=%build_config%-debug
    goto loop_parse_parameters
  )
  IF "%~0"=="development" (
    set build_config=%build_config%-development
    goto loop_parse_parameters
  )
  IF "%~0"=="release" (
    set build_config=%build_config%-release
    goto loop_parse_parameters
  )
  IF "%~0"=="-app" (
    IF NOT "%~1"=="" (
      set app_path="%~1"
      IF "%out_name%"=="" set out_name=%~n1
      IF "%out_path%"=="" set out_path=%~dp1..\..\build\%~n1


      SHIFT
    )
    goto loop_parse_parameters
  )
  IF "%~0"=="-o" (
    IF NOT "%~1"=="" (
      set out_name=%~n1
      SHIFT
    )
  )
goto loop_parse_parameters
)


IF %build_config%==Build-configuration: set build_config=%build_config%-debug-development-release 
IF "%out_name%"=="" set out_name=bse_core
IF "%out_path%"=="" ( set out_path=%bat_path%..\build\bse_core )

echo --------------------------------------------------------------
echo %build_config:-= %

IF %app_path%=="" (
  echo App path - - - - - : no -app parameter given, building core only
) ELSE (
  echo App path - - - - : %app_path:"=%
)
echo Output build name: %out_name%
echo --------------------------------------------------------------

set out_path=%out_path%\android
IF NOT EXIST "%out_path%" mkdir "%out_path%"
pushd "%out_path%"

set code_path=bse\code
set /a x=0
:search_code_path
IF NOT EXIST %code_path%\bse_main.cpp (
set code_path=..\%code_path%
 IF %x% LSS 5 ( 
  set /a "x+=1"
  goto search_code_path 
  ) ELSE (
    echo couldn't find directory \bse\code
    popd
    goto end
  )
)

rem go back one more for the debug/development/release subdirectories
set code_path=..\%code_path%

set projectpath="C:\personal\bse\code\platform\android"

set zipalign="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\zipalign.exe"
set apksigner="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\apksigner.bat"
set aapt="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\aapt.exe"
set d8="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\d8.bat"
set clang="%BSE_CLANG_ROOT:"=%\bin\clang++.exe"
set sysroot="%BSE_CLANG_ROOT:"=%\sysroot"

set javac="%BSE_JAVA_BIN_PATH:"=%\javac.exe"
set keytool="%BSE_JAVA_BIN_PATH:"=%\keytool.exe"

set res_path="%projectpath:"=%\res"
set src_path="%projectpath:"=%\src"
set manifest_path="%projectpath:"=%\AndroidManifest.xml"
set keypass=bse_generic_password


set src_file=%src_path%\BseMainActivity.java


set compiler_options=--sysroot=%sysroot% -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wformat -Werror=format-security -fno-limit-debug-info -fPIC
IF NOT %app_path%=="" set compiler_options=%compiler_options% -DBSE_BUILD_APP_PATH=%app_path%
set linker_options=-static-libstdc++ -shared -Wl,--build-id=sha1 -Wl,--no-rosegment -Wl,--fatal-warnings -Wl,--gc-sections -Wl,--no-undefined -Qunused-arguments -landroid -llog -latomic -lm

@REM ------------------------------------------------------------------------
@REM -------- Debug Build ---------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:debug=%==x%build_config% goto skip_build_debug
  echo ------ Debug -------------------------------------------------
  echo --------------------------------------------------------------

  IF EXIST debug rmdir /s /q debug
  IF NOT EXIST debug mkdir debug
  pushd debug

  IF NOT EXIST obj mkdir obj
  IF NOT EXIST bin mkdir bin
  IF NOT EXIST key mkdir key
  IF NOT EXIST lib mkdir lib

  for %%A in ("%out_path%\") do set debug_out_path=%%~dpA\debug
  set debug_out_path=%debug_out_path:\\=\%
  set result_unsigned=%debug_out_path%\bin\%out_name%_unsigned.apk

  set keystore_path=%debug_out_path%\key\bse_key.jks
  set result_zipaligned=%debug_out_path%\bin\%out_name%_zipaligned.apk
  set result=%debug_out_path%\%out_name%_debug.apk

@REM ------------------------------------------------------------------------
@REM -------- Compile Java and Package --------------------------------------
@REM ------------------------------------------------------------------------

  %aapt% package -v -f -m -S %res_path% -J %debug_out_path% -M %manifest_path% -I %BSE_ANDROID_JAR_PATH%
  %javac% -d "obj" -source 1.7 -target 1.7 -classpath "%BSE_ANDROID_JAR_PATH%;%debug_out_path%\%out_name%" -sourcepath "%debug_out_path%" %src_file%
  call %d8% %debug_out_path%\obj\%package_path%\* --output=%debug_out_path%\bin --no-desugaring
  %aapt% package -v -f -m -S %res_path% -J %src_path% -M %manifest_path% -I %BSE_ANDROID_JAR_PATH% -F %result_unsigned% %debug_out_path%\bin
  
@REM ------------------------------------------------------------------------
@REM -------- Compile Native Libraries --------------------------------------
@REM ------------------------------------------------------------------------

  pushd lib
  IF NOT EXIST arm64-v8a mkdir arm64-v8a
  IF NOT EXIST armeabi-v7a mkdir armeabi-v7a
  IF NOT EXIST x86 mkdir x86
  IF NOT EXIST x86_64 mkdir x86_64

  set compiler_options_debug=%compiler_options% -DBSE_BUILD_DEBUG

  echo Building native libraries...
  pushd arm64-v8a
  echo Building for arm64-v8a
  %clang% %src_path%\bse_android.cpp --target=aarch64-none-linux-android29 %compiler_options_debug% -o lib%out_name%.so %linker_options%
  popd

  pushd armeabi-v7a
  echo Building for armeabi-v7a
  %clang% %src_path%\bse_android.cpp --target=armv7-none-linux-androideabi29 %compiler_options_debug% -o lib%out_name%.so %linker_options%
  popd

  pushd x86
  echo Building for x86
  %clang% %src_path%\bse_android.cpp --target=i686-none-linux-android29 %compiler_options_debug% -o lib%out_name%.so %linker_options%
  popd

  pushd x86_64
  echo Building for x86_64
  %clang% %src_path%\bse_android.cpp --target=x86_64-none-linux-android29 %compiler_options_debug% -o lib%out_name%.so %linker_options%
  popd
  popd rem lib

@REM ------------------------------------------------------------------------
@REM -------- Add Native Libraries to Package -------------------------------
@REM ------------------------------------------------------------------------

  zip -r -u %result_unsigned% lib

@REM ------------------------------------------------------------------------
@REM -------- Sign Package --------------------------------------------------
@REM ------------------------------------------------------------------------

  %keytool% -genkeypair -validity 10000 -dname "CN=%out_name%, C=AT" -keystore %keystore_path% -storepass %keypass% -keypass %keypass% -alias %out_name% -keyalg RSA
  %zipalign% -f 4 %result_unsigned% %result_zipaligned%
  call %apksigner% sign -v --out %result% --ks %keystore_path% --ks-key-alias %out_name% --ks-pass pass:"%keypass%" --key-pass pass:"%keypass%" %result_zipaligned% 
  rem I suppose verified everytime isn't necessary 
  rem call %apksigner% verify -v --print-certs %result%

  echo --------------------------------------------------------------

  IF EXIST %result% echo Output file: %result%
  

  popd rem debug
:skip_build_debug

@REM ------------------------------------------------------------------------
@REM -------- Release Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:release=%==x%build_config% goto skip_build_release
  echo ------ Release -----------------------------------------------
  echo --------------------------------------------------------------


@REM -------- TODO BUILD RELEASE -------------------------------------------------



:skip_build_release



popd
goto end
:help_section
  echo --------------------------------------------------------------
  echo ---- TODO HELP -----------------------------------------------
  echo --------------------------------------------------------------

:end
endlocal
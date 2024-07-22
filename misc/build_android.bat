@echo off 
IF NOT DEFINED BSE_ANDROID_BUILD_SHELL_READY call shell_android.bat

IF NOT DEFINED BSE_CODE_PATH (
  echo ERROR - Don't run build_android.bat directly, run "build.bat android" instead.
  goto error_section
)

setlocal EnableDelayedExpansion
pushd "%BSE_OUT_PATH%"

set package_path=com\bse\core

set zipalign="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\zipalign.exe"
set apksigner="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\apksigner.bat"
set aapt="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\aapt.exe"
set d8="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\d8.bat"
set clang="%BSE_CLANG_ROOT:"=%\bin\clang++.exe"
set sysroot="%BSE_CLANG_ROOT:"=%\sysroot"

set javac="%BSE_JAVA_BIN_PATH:"=%\javac.exe"
set keytool="%BSE_JAVA_BIN_PATH:"=%\keytool.exe"

set projectpath="%BSE_CODE_PATH%\platform\android"
set res_path="%projectpath:"=%\res"
set src_path="%projectpath:"=%\src"
set manifest_path="%projectpath:"=%\AndroidManifest.xml"
set keypass=bse_generic_password

set java_src_path=%src_path%\java
set cpp_src_path=%src_path%\cpp

set compiler_options=-I "%BSE_CODE_PATH%" --sysroot=%sysroot% -g -DANDROID -DBSE_PLATFORM_ANDROID -std=c++20 -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wformat -Wall -Werror=format-security -fno-limit-debug-info -fPIC

IF NOT %BSE_APP_PATH%=="" set compiler_options=%compiler_options% -DBSE_BUILD_APP_PATH=%BSE_APP_PATH%
set linker_options=-lGLESv3 -lvulkan -static-libstdc++ -shared -Wl,--build-id=sha1 -Wl,--no-rosegment -Wl,--fatal-warnings -Wl,--gc-sections -Wl,--no-undefined -Qunused-arguments -landroid -llog -latomic -lm 

rem QUICK HACK TO REDIRECT DEBUG TO DEVELOPMENT UNTIL A DEBUGGER RUNS ON AN ANDROID DEVICE
IF NOT x%BSE_BUILD_CONFIG:debug=%==x%BSE_BUILD_CONFIG% set BSE_BUILD_CONFIG=%BSE_BUILD_CONFIG%-development

@REM ------------------------------------------------------------------------
@REM -------- Development Build ---------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%BSE_BUILD_CONFIG:development=%==x%BSE_BUILD_CONFIG% goto skip_build_development
  echo ------ Development -------------------------------------------
  echo --------------------------------------------------------------

  IF EXIST development rmdir /s /q development
  IF NOT EXIST development mkdir development
  pushd development

  IF NOT EXIST obj mkdir obj
  IF NOT EXIST bin mkdir bin
  IF NOT EXIST key mkdir key
  IF NOT EXIST lib mkdir lib

  set development_out_path=%BSE_OUT_PATH%\development
  set result_unsigned=%development_out_path%\bin\%BSE_OUT_NAME%_unsigned.apk
  set keystore_path=%development_out_path%\key\bse_key.jks
  set result_zipaligned=%development_out_path%\bin\%BSE_OUT_NAME%_zipaligned.apk
  set result=%development_out_path%\%BSE_OUT_NAME%_development.apk

@REM ------------------------------------------------------------------------
@REM -------- Compile Native Libraries --------------------------------------
@REM ------------------------------------------------------------------------

  pushd lib
  set compiler_options_development=%compiler_options% -DBSE_BUILD_DEVELOPMENT
  echo Building native libraries...

  IF NOT EXIST arm64-v8a mkdir arm64-v8a
  pushd arm64-v8a
  echo Building for arm64-v8a
  %clang% %cpp_src_path%\bse_android.cpp --target=aarch64-none-linux-android29 %compiler_options_development% -o lib%BSE_OUT_NAME%.so %linker_options%
  popd
  if %errorlevel% neq 0 (
    popd rem lib
    goto error_section_compile
  )

  @REM IF NOT EXIST armeabi-v7a mkdir armeabi-v7a
  @REM pushd armeabi-v7a
  @REM echo Building for armeabi-v7a
  @REM %clang% %cpp_src_path%\bse_android.cpp --target=armv7-none-linux-androideabi29 %compiler_options_development% -o lib%BSE_OUT_NAME%.so %linker_options%
  @REM popd
  @REM if %errorlevel% neq 0 (
  @REM   popd rem lib
  @REM   goto error_section_compile
  @REM )

  @REM IF NOT EXIST x86_64 mkdir x86_64
  @REM pushd x86_64
  @REM echo Building for x86_64
  @REM %clang% %cpp_src_path%\bse_android.cpp --target=x86_64-none-linux-android29 %compiler_options_development% -mlzcnt -o lib%BSE_OUT_NAME%.so %linker_options%
  @REM popd
  @REM if %errorlevel% neq 0 (
  @REM   popd rem lib
  @REM   goto error_section_compile
  @REM )
  
  @REM IF NOT EXIST x86 mkdir x86
  @REM pushd x86
  @REM echo Building for x86
  @REM %clang% %cpp_src_path%\bse_android.cpp --target=i686-none-linux-android29 %compiler_options_development% -mlzcnt -o lib%BSE_OUT_NAME%.so %linker_options%
  @REM popd
  @REM if %errorlevel% neq 0 (
  @REM   popd rem lib
  @REM   goto error_section_compile
  @REM )

  popd rem lib

@REM ------------------------------------------------------------------------
@REM -------- Compile Java and Package --------------------------------------
@REM ------------------------------------------------------------------------

  %aapt% package -v -f -m -S %res_path% -J %development_out_path% -M %manifest_path% -I %BSE_ANDROID_JAR_PATH%
  if %errorlevel% neq 0 goto error_section_compile
  echo --------------------------------------------------------------
  echo ------ Compiling Java ----------------------------------------
  echo --------------------------------------------------------------

  %javac% -d "obj" -source 1.7 -target 1.7 -classpath "%BSE_ANDROID_JAR_PATH%;%development_out_path%\%BSE_OUT_NAME%" -sourcepath "%development_out_path%" %java_src_path%\*
  if %errorlevel% neq 0 goto error_section_compile

  call %d8% %development_out_path%\obj\%package_path%\* --output=%development_out_path%\bin --no-desugaring
  if %errorlevel% neq 0 goto error_section_compile

  %aapt% package -v -f -m -S %res_path% -J %development_out_path% -M %manifest_path% -I %BSE_ANDROID_JAR_PATH% -F %result_unsigned% %development_out_path%\bin
  if %errorlevel% neq 0 goto error_section_compile

@REM ------------------------------------------------------------------------
@REM -------- Add Native Libraries to Package -------------------------------
@REM ------------------------------------------------------------------------

  zip -r -u %result_unsigned% lib
  if %errorlevel% neq 0 goto error_section_compile

@REM ------------------------------------------------------------------------
@REM -------- Sign Package --------------------------------------------------
@REM ------------------------------------------------------------------------

  %keytool% -genkeypair -validity 10000 -dname "CN=%BSE_OUT_NAME%, C=AT" -keystore %keystore_path% -storepass %keypass% -keypass %keypass% -alias %BSE_OUT_NAME% -keyalg RSA
  if %errorlevel% neq 0 goto error_section_compile

  %zipalign% -f 4 %result_unsigned% %result_zipaligned%
  if %errorlevel% neq 0 goto error_section_compile

  call %apksigner% sign -v --out %result% --ks %keystore_path% --ks-key-alias %BSE_OUT_NAME% --ks-pass pass:"%keypass%" --key-pass pass:"%keypass%" %result_zipaligned% 
  if %errorlevel% neq 0 goto error_section_compile

  rem I suppose verified everytime isn't necessary 
  rem call %apksigner% verify -v --print-certs %result%

  echo --------------------------------------------------------------

  IF EXIST %result% echo Output file: %result%
  
  popd rem development
:skip_build_development

@REM ------------------------------------------------------------------------
@REM -------- Release Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%BSE_BUILD_CONFIG:release=%==x%BSE_BUILD_CONFIG% goto skip_build_release
  echo ------ Release -----------------------------------------------
  echo --------------------------------------------------------------


@REM -------- TODO BUILD RELEASE -------------------------------------------------

:skip_build_release

popd rem %BSE_OUT_PATH%


@REM ------------------------------------------------------------------------
@REM -------- Install apk if a device is connected --------------------------
@REM ------------------------------------------------------------------------

echo skipping apk install

goto end

  IF EXIST %result% (
    echo --------------------------------------------------------------
    echo ------ Installing apk ----------------------------------------
    echo --------------------------------------------------------------
    call android_install_package.bat %result% %package_path:\=.%
    echo --------------------------------------------------------------
  )

goto end

:error_section_compile
  rem debug/development/release
  popd 
  goto error_section

:error_section
  rem %BSE_OUT_PATH%
  popd 
  echo ==============================================================
  echo --------------------------------------------------------------
  echo -- ERRORS BUILDING ANDROID, PLEASE READ THE LOGS ABOVE ------- 
  echo --------------------------------------------------------------

:end
endlocal
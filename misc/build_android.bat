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

for %%A in ("%code_path%\") do set temp_path=%%~dpA
set code_path=%temp_path%

set zipalign="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\zipalign.exe"
set apksigner="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\apksigner.bat"
set aapt="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\aapt.exe"
set d8="%BSE_ANDROID_BUILD_TOOLS_PATH:"=%\d8.bat"
set clang="%BSE_CLANG_ROOT:"=%\bin\clang++.exe"
set sysroot="%BSE_CLANG_ROOT:"=%\sysroot"

set javac="%BSE_JAVA_BIN_PATH:"=%\javac.exe"
set keytool="%BSE_JAVA_BIN_PATH:"=%\keytool.exe"

set projectpath="%code_path%\platform\android"
set res_path="%projectpath:"=%\res"
set src_path="%projectpath:"=%\src"
set manifest_path="%projectpath:"=%\AndroidManifest.xml"
set keypass=bse_generic_password

set java_src_path=%src_path%\java
set cpp_src_path=%src_path%\cpp


set compiler_options=-I "%code_path%\" --sysroot=%sysroot% -g -DBSE_PLATFORM_ANDROID -std=c++20 -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wformat -Werror=format-security -fno-limit-debug-info -fPIC
IF NOT %app_path%=="" set compiler_options=%compiler_options% -DBSE_BUILD_APP_PATH=%app_path%
set linker_options=-static-libstdc++ -shared -Wl,--build-id=sha1 -Wl,--no-rosegment -Wl,--fatal-warnings -Wl,--gc-sections -Wl,--no-undefined -Qunused-arguments -landroid -llog -latomic -lm

@REM ------------------------------------------------------------------------
@REM -------- Development Build ---------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:development=%==x%build_config% goto skip_build_development
  echo ------ Development -------------------------------------------
  echo --------------------------------------------------------------

  IF EXIST development rmdir /s /q development
  IF NOT EXIST development mkdir development
  pushd development

  IF NOT EXIST obj mkdir obj
  IF NOT EXIST bin mkdir bin
  IF NOT EXIST key mkdir key
  IF NOT EXIST lib mkdir lib

  for %%A in ("%out_path%\") do set development_out_path=%%~dpA\development
  set development_out_path=%development_out_path:\\=\%
  set result_unsigned=%development_out_path%\bin\%out_name%_unsigned.apk

  set keystore_path=%development_out_path%\key\bse_key.jks
  set result_zipaligned=%development_out_path%\bin\%out_name%_zipaligned.apk
  set result=%development_out_path%\%out_name%_development.apk

@REM ------------------------------------------------------------------------
@REM -------- Compile Native Libraries --------------------------------------
@REM ------------------------------------------------------------------------

  pushd lib
  set compiler_options_development=%compiler_options% -DBSE_BUILD_DEVELOPMENT
  echo Building native libraries...

  IF NOT EXIST arm64-v8a mkdir arm64-v8a
  pushd arm64-v8a
  echo Building for arm64-v8a
  %clang% %cpp_src_path%\bse_android.cpp --target=aarch64-none-linux-android29 %compiler_options_development% -o lib%out_name%.so %linker_options%
  popd
  if %errorlevel% neq 0 (
    popd rem lib
    goto error_section_development
  )

  IF NOT EXIST armeabi-v7a mkdir armeabi-v7a
  pushd armeabi-v7a
  echo Building for armeabi-v7a
  %clang% %cpp_src_path%\bse_android.cpp --target=armv7-none-linux-androideabi29 %compiler_options_development% -o lib%out_name%.so %linker_options%
  popd
  if %errorlevel% neq 0 (
    popd rem lib
    goto error_section_development
  )

  IF NOT EXIST x86_64 mkdir x86_64
  pushd x86_64
  echo Building for x86_64
  %clang% %cpp_src_path%\bse_android.cpp --target=x86_64-none-linux-android29 %compiler_options_development% -o lib%out_name%.so %linker_options%
  popd
  if %errorlevel% neq 0 (
    popd rem lib
    goto error_section_development
  )

  rem IF NOT EXIST x86 mkdir x86
  rem pushd x86
  rem echo Building for x86
  rem %clang% %cpp_src_path%\bse_android.cpp --target=i686-none-linux-android29 %compiler_options_development% -o lib%out_name%.so %linker_options%
  rem popd
  rem if %errorlevel% neq 0 (
  rem   popd rem lib
  rem   goto error_section_development
  rem )

  popd rem lib

@REM ------------------------------------------------------------------------
@REM -------- Compile Java and Package --------------------------------------
@REM ------------------------------------------------------------------------

  %aapt% package -v -f -m -S %res_path% -J %development_out_path% -M %manifest_path% -I %BSE_ANDROID_JAR_PATH%
  if %errorlevel% neq 0 goto error_section_development

  %javac% -d "obj" -source 1.7 -target 1.7 -classpath "%BSE_ANDROID_JAR_PATH%;%development_out_path%\%out_name%" -sourcepath "%development_out_path%" %java_src_path%\*
  if %errorlevel% neq 0 goto error_section_development

  call %d8% %development_out_path%\obj\%package_path%\* --output=%development_out_path%\bin --no-desugaring
  if %errorlevel% neq 0 goto error_section_development

  %aapt% package -v -f -m -S %res_path% -J %development_out_path% -M %manifest_path% -I %BSE_ANDROID_JAR_PATH% -F %result_unsigned% %development_out_path%\bin
  if %errorlevel% neq 0 goto error_section_development

@REM ------------------------------------------------------------------------
@REM -------- Add Native Libraries to Package -------------------------------
@REM ------------------------------------------------------------------------

  zip -r -u %result_unsigned% lib
  if %errorlevel% neq 0 goto error_section_development

@REM ------------------------------------------------------------------------
@REM -------- Sign Package --------------------------------------------------
@REM ------------------------------------------------------------------------

  %keytool% -genkeypair -validity 10000 -dname "CN=%out_name%, C=AT" -keystore %keystore_path% -storepass %keypass% -keypass %keypass% -alias %out_name% -keyalg RSA
  if %errorlevel% neq 0 goto error_section_development

  %zipalign% -f 4 %result_unsigned% %result_zipaligned%
  if %errorlevel% neq 0 goto error_section_development

  call %apksigner% sign -v --out %result% --ks %keystore_path% --ks-key-alias %out_name% --ks-pass pass:"%keypass%" --key-pass pass:"%keypass%" %result_zipaligned% 
  if %errorlevel% neq 0 goto error_section_development

  rem I suppose verified everytime isn't necessary 
  rem call %apksigner% verify -v --print-certs %result%

  echo --------------------------------------------------------------

  IF EXIST %result% echo Output file: %result%
  
  popd rem development
:skip_build_development

@REM ------------------------------------------------------------------------
@REM -------- Release Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:release=%==x%build_config% goto skip_build_release
  echo ------ Release -----------------------------------------------
  echo --------------------------------------------------------------


@REM -------- TODO BUILD RELEASE -------------------------------------------------

:skip_build_release

popd rem %out_path%

@REM ------------------------------------------------------------------------
@REM -------- Install apk if a device is connected --------------------------
@REM ------------------------------------------------------------------------

  IF EXIST %result% (
    echo --------------------------------------------------------------
    echo ------ Installing apk ----------------------------------------
    echo --------------------------------------------------------------
    call android_install.bat %result% %package_path:\=.%
    echo --------------------------------------------------------------
  )

goto end


:help_section
  echo --------------------------------------------------------------
  echo ---- TODO HELP -----------------------------------------------
  echo --------------------------------------------------------------
goto end

:error_section_development
  popd rem development
  goto error_section

:error_section
  popd rem %out_path%
  echo ==============================================================
  echo --------------------------------------------------------------
  echo ---------- ERROR, PLEASE READ THE LOGS ABOVE ----------------- 
  echo --------------------------------------------------------------


:end
endlocal
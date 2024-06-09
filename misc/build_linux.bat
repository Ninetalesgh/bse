@echo off 
IF NOT DEFINED BSE_CLANG_ROOT call shell_linux.bat

IF NOT DEFINED BSE_CODE_PATH (
  echo ERROR - Don't run build_linux.bat directly, run "build.bat linux" instead.
  goto error_section
)

IF NOT DEFINED BSE_CLANG_ROOT (
  echo ERROR - Don't run build_linux.bat directly, run "build.bat linux" instead.
  goto error_section
)

setlocal EnableDelayedExpansion
pushd "%BSE_OUT_PATH%"



set clang="%BSE_CLANG_ROOT:"=%\bin\clang++.exe"
set sysroot="%BSE_CLANG_SYSROOT:"=%"



set compiler_options=-I "%BSE_CODE_PATH%\" --sysroot=%sysroot% -g -DBSE_PLATFORM_LINUX -std=c++20 -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wformat -Wall -Werror=format-security -fno-limit-debug-info -fPIC
IF NOT %BSE_APP_PATH%=="" set compiler_options=%compiler_options% -DBSE_BUILD_APP_PATH=%BSE_APP_PATH%
set linker_options=-lGLESv3 -lvulkan -static-libstdc++ -shared -Wl,--build-id=sha1 -Wl,--no-rosegment -Wl,--fatal-warnings -Wl,--gc-sections -Wl,--no-undefined -Qunused-arguments -llog -latomic -lm 


rem QUICK HACK TO REDIRECT DEBUG TO DEVELOPMENT UNTIL A DEBUGGER RUNS ON A LINUX DEVICE
IF NOT x%BSE_BUILD_CONFIG:debug=%==x%BSE_BUILD_CONFIG% set BSE_BUILD_CONFIG=%BSE_BUILD_CONFIG%-development

goto skip_build_debug

@REM ------------------------------------------------------------------------
@REM -------- Debug Build ---------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%BSE_BUILD_CONFIG:debug=%==x%BSE_BUILD_CONFIG% goto skip_build_debug
  echo ------ Debug -------------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST debug mkdir debug
  pushd debug

  rem TODO DEBUG BUILD

  echo --------------------------------------------------------------
  popd
:skip_build_debug
@REM ------------------------------------------------------------------------
@REM -------- Develop Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%BSE_BUILD_CONFIG:development=%==x%BSE_BUILD_CONFIG% goto skip_build_develop
  echo ------ Development -------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST development mkdir development
  pushd development

  set compiler_options_development=%compiler_options% -DBSE_BUILD_DEVELOPMENT

  %clang% %BSE_CODE_PATH%\bse_main.cpp --target=x86_64-linux-gnu %compiler_options_development% %linker_options%

  rem %clang% %BSE_CODE_PATH%\bse_main.cpp --target=x86_64-linux-gnu %compiler_options_development% -o %BSE_OUT_NAME% %linker_options%
  if %errorlevel% neq 0 goto error_section_compile

  echo --------------------------------------------------------------
  popd
:skip_build_develop
@REM ------------------------------------------------------------------------
@REM -------- Release Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%BSE_BUILD_CONFIG:release=%==x%BSE_BUILD_CONFIG% goto skip_build_release
  echo ------ Release -----------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST release mkdir release
  pushd release

  rem TODO RELEASE BUILD


  echo --------------------------------------------------------------
  popd
:skip_build_release



popd
goto end

:error_section_compile
  popd rem debug/development/release
  goto error_section

:error_section
  popd rem %BSE_OUT_PATH%
  echo ==============================================================
  echo --------------------------------------------------------------
  echo -- ERRORS BUILDING LINUX, PLEASE READ THE LOGS ABOVE --------- 
  echo --------------------------------------------------------------

:end
endlocal
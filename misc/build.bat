@echo off


set preserved_arguments=%*

set BSE_PLATFORM_CONFIG=Platform-configuration:
set BSE_BUILD_CONFIG=Build-configuration:
set BSE_APP_PATH=""
set BSE_OUT_NAME=
set BSE_OUT_PATH=
set BSE_CODE_PATH=
set BSE_INCLUDE_PATHS=""
set BSE_LIBRARY_PATHS=""
set BSE_BAT_PATH=%~dp0

IF "%~1"=="build" SHIFT
IF "%~1"=="?" goto help_section

:loop_parse_parameters
SHIFT
IF NOT "%~0"=="" (
  IF "%~0"=="win64" (
    set BSE_PLATFORM_CONFIG=%BSE_PLATFORM_CONFIG%-win64
    goto loop_parse_parameters
  )
  IF "%~0"=="android" (
    set BSE_PLATFORM_CONFIG=%BSE_PLATFORM_CONFIG%-android
    goto loop_parse_parameters
  )
  IF "%~0"=="linux" (
    set BSE_PLATFORM_CONFIG=%BSE_PLATFORM_CONFIG%-linux
    goto loop_parse_parameters
  )
  IF "%~0"=="debug" (
    set BSE_BUILD_CONFIG=%BSE_BUILD_CONFIG%-debug
    goto loop_parse_parameters
  )
  IF "%~0"=="development" (
    set BSE_BUILD_CONFIG=%BSE_BUILD_CONFIG%-development
    goto loop_parse_parameters
  )
  IF "%~0"=="release" (
    set BSE_BUILD_CONFIG=%BSE_BUILD_CONFIG%-release
    goto loop_parse_parameters
  )
  IF "%~0"=="-app" (
    IF NOT "%~1"=="" (
      set BSE_APP_PATH="%~1"
      IF "%BSE_OUT_NAME%"=="" set BSE_OUT_NAME=%~n1
      IF "%BSE_OUT_PATH%"=="" set BSE_OUT_PATH=%~dp1..\..\build\%~n1
      SHIFT
    ) ELSE (
      echo '-app' needs the path to the app as parameter
      goto build_end
    )
    goto loop_parse_parameters
  )
  IF "%~0"=="-include" (
    IF NOT "%~1"=="" (
      set BSE_INCLUDE_PATHS="%~1"
      SHIFT
    ) ELSE (
      echo '-include' needs include folder paths as parameter
      goto build_end
    )
    goto loop_parse_parameters
  )
  IF "%~0"=="-lib" (
    IF NOT "%~1"=="" (
      set BSE_LIBRARY_PATHS="%~1"
      SHIFT
    ) ELSE (
      echo '-lib' needs include folder paths as parameter
      goto build_end
    )
    goto loop_parse_parameters
  )

  goto loop_parse_parameters
)

IF "%BSE_OUT_NAME%"=="" set BSE_OUT_NAME=bse_core
IF "%BSE_OUT_PATH%"=="" ( set BSE_OUT_PATH=%BSE_BAT_PATH%..\build\bse_core )

for %%A in ("%BSE_OUT_PATH%\") do set temp_path=%%~dpA
set BSE_OUT_PATH=%temp_path%

rem default debug
IF %BSE_BUILD_CONFIG%==Build-configuration: set BSE_BUILD_CONFIG=%BSE_BUILD_CONFIG%-debug 

rem default win64
IF %BSE_PLATFORM_CONFIG%==Platform-configuration: set BSE_PLATFORM_CONFIG=%BSE_PLATFORM_CONFIG%-win64

set BSE_CODE_PATH=bse\code
set /a x=0
:search_code_path
IF NOT EXIST %BSE_CODE_PATH%\bse_main.cpp (
set BSE_CODE_PATH=..\%BSE_CODE_PATH%
 IF %x% LSS 5 ( 
  set /a "x+=1"
  goto search_code_path 
  ) ELSE (
    echo couldn't find directory \bse\code
    popd
    goto build_end
  )
)

for %%A in ("%BSE_CODE_PATH%\") do set temp_path=%%~dpA
set BSE_CODE_PATH=%temp_path%
set BSE_OUT_PATH_ROOT=%BSE_OUT_PATH%


echo.
echo ==============================================================
echo BSE build info
echo --------------------------------------------------------------
echo %BSE_PLATFORM_CONFIG:-= %
echo %BSE_BUILD_CONFIG:-= %
IF %BSE_APP_PATH%=="" (
  echo App path:    no -app parameter given, building core only
) ELSE (
  echo App path:    %BSE_APP_PATH:"=%
)
echo Output path: %BSE_OUT_PATH_ROOT%
echo ==============================================================
echo.


@REM ------------------------------------------------------------------------
@REM -------- Win64 ---------------------------------------------------------
@REM ------------------------------------------------------------------------
IF x%BSE_PLATFORM_CONFIG:win64=%==x%BSE_PLATFORM_CONFIG% goto skip_platform_win64
  set BSE_OUT_PATH=%BSE_OUT_PATH_ROOT%win64
  IF NOT EXIST "%BSE_OUT_PATH%" mkdir "%BSE_OUT_PATH%"
  echo ==============================================================
  echo Building win64
  echo --------------------------------------------------------------
  echo Output path: %BSE_OUT_PATH% 
  echo --------------------------------------------------------------
  call build_win64.bat %*
  echo ==============================================================
  echo.
:skip_platform_win64


@REM ------------------------------------------------------------------------
@REM -------- Linux ---------------------------------------------------------
@REM ------------------------------------------------------------------------
IF x%BSE_PLATFORM_CONFIG:linux=%==x%BSE_PLATFORM_CONFIG% goto skip_platform_linux
  set BSE_OUT_PATH=%BSE_OUT_PATH_ROOT%linux
  IF NOT EXIST "%BSE_OUT_PATH%" mkdir "%BSE_OUT_PATH%"
  echo ==============================================================
  echo Building linux
  echo --------------------------------------------------------------
  echo Output path: %BSE_OUT_PATH%
  echo --------------------------------------------------------------
  call build_linux.bat %*
  echo ==============================================================
  echo.
:skip_platform_linux


@REM ------------------------------------------------------------------------
@REM -------- Android -------------------------------------------------------
@REM ------------------------------------------------------------------------
IF x%BSE_PLATFORM_CONFIG:android=%==x%BSE_PLATFORM_CONFIG% goto skip_platform_android
  set BSE_OUT_PATH=%BSE_OUT_PATH_ROOT%android
  IF NOT EXIST "%BSE_OUT_PATH%" mkdir "%BSE_OUT_PATH%"
  echo ==============================================================
  echo Building android
  echo --------------------------------------------------------------
  echo Output path: %BSE_OUT_PATH%
  echo --------------------------------------------------------------
  call build_android.bat %*
  echo ==============================================================
  echo.
:skip_platform_android




:build_end

set BSE_BUILD_CONFIG=
set BSE_PLATFORM_CONFIG=
set BSE_APP_PATH=
set BSE_OUT_NAME=
set BSE_OUT_PATH=
set BSE_OUT_PATH_ROOT=
set BSE_CODE_PATH=
set BSE_INCLUDE_PATHS=
set BSE_LIBRARY_PATHS=
set BSE_BAT_PATH=
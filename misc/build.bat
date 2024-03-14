@echo off


set preserved_arguments=%*

set platform_config=Platform-configuration:

:loop_parse_parameters
SHIFT
IF NOT "%~0"=="" (
  IF "%~0"=="win64" (
    set platform_config=%platform_config%-win64
    goto loop_parse_parameters
  )
  IF "%~0"=="android" (
    set platform_config=%platform_config%-android
    goto loop_parse_parameters
  )
goto loop_parse_parameters
)

rem default win64 for now
IF %platform_config%==Platform-configuration: set platform_config=%platform_config%-win64

echo ==============================================================
echo %platform_config:-= %
echo ==============================================================

@REM ------------------------------------------------------------------------
@REM -------- Win64 ---------------------------------------------------------
@REM ------------------------------------------------------------------------
IF x%platform_config:win64=%==x%platform_config% goto skip_platform_win64
  echo --------------------------------------------------------------
  echo building win64 
  call build_win64.bat %*
  echo ==============================================================
:skip_platform_win64


@REM ------------------------------------------------------------------------
@REM -------- Android -------------------------------------------------------
@REM ------------------------------------------------------------------------
IF x%platform_config:android=%==x%platform_config% goto skip_platform_android
  echo --------------------------------------------------------------
  echo building android
  call build_android.bat %*
  echo ==============================================================
:skip_platform_android

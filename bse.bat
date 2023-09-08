@echo off
setlocal EnableDelayedExpansion

IF "%~1"=="?" goto help_section

:loop_parse_parameters
SHIFT
IF NOT "%~0"=="" (
  IF "%~0"=="build" (
    call misc/build.bat %*
    goto end
  )
  IF "%~0"=="createapp" (
    call misc/createapp.bat %*
    goto end
  )

goto loop_parse_parameters
)

:help_section
  echo --------------------------------------------------------------
  echo ---- Run Build -----------------------------------------------
  echo --------------------------------------------------------------
  echo - build followed by configuration and app to build.
  echo - for more info run bse build ?
  echo --------------------------------------------------------------
  echo --------------------------------------------------------------
  echo --------------------------------------------------------------
  echo ---- Create User App -----------------------------------------
  echo --------------------------------------------------------------
  echo - createapp followed by the name of your new app.
  echo - for example: bse createapp exampleapp
  echo --------------------------------------------------------------
:end
endlocal
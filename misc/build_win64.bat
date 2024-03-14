@echo off
IF NOT DEFINED DevEnvDir call shell_win64.bat

setlocal EnableDelayedExpansion

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

set out_path=%out_path%\win64
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

set compiler_options=/I %code_path% /GR- /EHa- /FC /MT /nologo /volatile:iso /W4 /wd4068 /wd4100 /wd4201 /wd4701 /wd4189 /wd4530
IF NOT %app_path%=="" set compiler_options=%compiler_options% /DBSE_BUILD_APP_PATH=%app_path%
set linker_options=/link /opt:ref /incremental:no
set app_exports=/EXPORT:core_initialize_internal /EXPORT:core_on_reload_internal /EXPORT:core_tick_internal

@REM ------------------------------------------------------------------------
@REM -------- Debug Build ---------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:debug=%==x%build_config% goto skip_build_debug
  echo ------ Debug -------------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST debug mkdir debug
  pushd debug
  del *.pdb > NUL 2> NUL

  set compiler_options_debug=%compiler_options% /Z7 /Od /DBSE_BUILD_DEBUG
  cl /LD %code_path%\core\bse_core.cpp %compiler_options_debug% /Fe:bse_core.dll /Fmbse_core.map %linker_options% %app_exports% /PDB:"%out_name%_%random%.pdb"
  cl     %code_path%\bse_main.cpp /Fe:"%out_name%_debug.exe" %compiler_options_debug% %linker_options% 
  echo --------------------------------------------------------------
  popd
:skip_build_debug
@REM ------------------------------------------------------------------------
@REM -------- Develop Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:development=%==x%build_config% goto skip_build_develop
  echo ------ Development -------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST development mkdir development
  pushd development
  del *.pdb > NUL 2> NUL

  set compiler_options_development=%compiler_options% /Od /DBSE_BUILD_DEVELOPMENT
  cl /LD %code_path%\core\bse_core.cpp %compiler_options_development% /Fe:bse_core.dll %linker_options% %app_exports%
  cl     %code_path%\bse_main.cpp /Fe:"%out_name%_develop.exe" %compiler_options_development% %linker_options% 
  echo --------------------------------------------------------------
  popd
:skip_build_develop
@REM ------------------------------------------------------------------------
@REM -------- Release Build -------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%build_config:release=%==x%build_config% goto skip_build_release
  echo ------ Release -----------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST release mkdir release
  pushd release

  set compiler_options_release=%compiler_options% /Ox
  cl %code_path%\bse_main.cpp /Fe:"%out_name%.exe" %compiler_options_release% %linker_options% 
  
  del *.obj > NUL 2> NUL
  echo --------------------------------------------------------------
  popd
:skip_build_release


popd
goto end
:help_section
  echo --------------------------------------------------------------
  echo ---- Build Configuration -------------------------------------
  echo --------------------------------------------------------------
  echo - set any parameters among debug, develop, or release.
  echo - for example: build develop release
  echo --------------------------------------------------------------
  echo --------------------------------------------------------------
  echo --------------------------------------------------------------
  echo ---- User App Build Path -------------------------------------
  echo --------------------------------------------------------------
  echo - -app followed by the path to your app.
  echo - for example: build develop -app ../apps/testapp.cpp
  echo - -o followed by the name you want the output to have (without extension), default is the app name.
  echo --------------------------------------------------------------

:end
endlocal

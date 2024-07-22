@echo off
IF NOT DEFINED DevEnvDir call shell_win64.bat

IF NOT DEFINED VULKAN_SDK (
  echo ERROR - VULKAN_SDK environment variable not set, aborting
  goto error_section
)

IF NOT DEFINED BSE_CODE_PATH (
  echo ERROR - Don't run build_win64.bat directly, run "build.bat win64" instead.
  goto error_section
)

setlocal EnableDelayedExpansion
pushd "%BSE_OUT_PATH%"

set include_paths="%VULKAN_SDK%\Include" "%BSE_INCLUDE_PATHS:;=" "%"
set include_paths=%include_paths:""="%
set include_paths=%include_paths:/=\%

echo Include paths:
echo - %include_paths:" "=" & echo - "%
set include_paths=/I %include_paths:" "=" /I "%
set library_paths="%BSE_LIBRARY_PATHS:;=" "%"
set library_paths=%library_paths:""="%
set library_paths=%library_paths:/=\%

IF NOT %BSE_LIBRARY_PATHS%=="" (
  echo Dependency paths:
  echo - %library_paths:" "=" & echo - "% 
  echo --------------------------------------------------------------
)

set core_libraries="%VULKAN_SDK%\Lib\vulkan-1.lib"

set compiler_options=%include_paths% /DBSE_PLATFORM_WINDOWS /GR- /EHa- /FC /MT /nologo /volatile:iso /W4 /wd4068 /wd4100 /wd4201 /wd4701 /wd4189 /wd4530
IF NOT %BSE_APP_PATH%=="" set compiler_options=%compiler_options% /DBSE_BUILD_APP_PATH=%BSE_APP_PATH%
set linker_options=/link /opt:ref /incremental:no %core_libraries% %library_paths%
set app_exports=/EXPORT:core_initialize_internal /EXPORT:core_on_reload_internal /EXPORT:core_tick_internal

@REM ------------------------------------------------------------------------
@REM -------- Debug Build ---------------------------------------------------
@REM ------------------------------------------------------------------------
  IF x%BSE_BUILD_CONFIG:debug=%==x%BSE_BUILD_CONFIG% goto skip_build_debug
  echo ------ Debug -------------------------------------------------
  echo --------------------------------------------------------------
  IF NOT EXIST debug mkdir debug
  pushd debug
  del *.pdb > NUL 2> NUL

  set compiler_options_debug=%compiler_options% /Z7 /Od /DBSE_BUILD_DEBUG
  cl /LD %BSE_CODE_PATH%\core\bse_core.cpp %compiler_options_debug% /Fe:bse_core.dll /Fmbse_core.map %linker_options% %app_exports% /PDB:"%BSE_OUT_NAME%_%random%.pdb"
  if %errorlevel% neq 0 goto error_section_compile
  
  cl     %BSE_CODE_PATH%\bse_main.cpp /Fe:"%BSE_OUT_NAME%_debug.exe" %compiler_options_debug% %linker_options% 
  if %errorlevel% neq 0 goto error_section_compile

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
  del *.pdb > NUL 2> NUL

  set compiler_options_development=%compiler_options% /Od /DBSE_BUILD_DEVELOPMENT
  cl /LD %BSE_CODE_PATH%\core\bse_core.cpp %compiler_options_development% /Fe:bse_core.dll %linker_options% %app_exports%
  if %errorlevel% neq 0 goto error_section_compile

  cl     %BSE_CODE_PATH%\bse_main.cpp /Fe:"%BSE_OUT_NAME%_development.exe" %compiler_options_development% %linker_options% 
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

  set compiler_options_release=%compiler_options% /Ox
  cl %BSE_CODE_PATH%\bse_main.cpp /Fe:"%BSE_OUT_NAME%.exe" %compiler_options_release% %linker_options% 
  if %errorlevel% neq 0 goto error_section_compile
  
  del *.obj > NUL 2> NUL
  echo --------------------------------------------------------------
  popd
:skip_build_release


popd

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
  echo -- ERRORS BUILDING WIN64, PLEASE READ THE LOGS ABOVE --------- 
  echo --------------------------------------------------------------

:end
endlocal

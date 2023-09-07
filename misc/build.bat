@echo off
set build_mode="%1"

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build


set compiler_options=/I %codepath% /GR- /EHa- /FC /MT /nologo /volatile:iso /W4 /wd4068 /wd4100 /wd4201 /wd4701 /wd4189
set linker_options=/link /opt:ref /incremental:no

set app_exports=/EXPORT:core_initialize_internal /EXPORT:core_on_reload_internal /EXPORT:core_tick_internal

  echo ------------------------------------

IF %build_mode%=="debug" goto build_debug
IF %build_mode%=="develop" goto build_develop
IF %build_mode%=="release" goto build_release

:build_debug
  echo ------------- debug ----------------
  echo ------------------------------------
  IF NOT EXIST debug mkdir debug
  pushd debug
  del *.pdb > NUL 2> NUL

  set codepath=..\..\code\core
  set compiler_options_dev=%compiler_options% /Z7 /Od /DBSE_BUILD_DEBUG

  cl /LD %codepath%\bse_core.cpp %compiler_options_dev% /Fe:bse_core.dll /Fmbse_core.map %linker_options% %app_exports% /PDB:bse_core_temp_%random%.pdb 
  cl     %codepath%\bse_main.cpp /Fe:bse_debug.exe %compiler_options_dev% %linker_options% 

  echo ------------------------------------
  popd
 IF %build_mode%=="debug" goto end

:build_develop
  echo ------------- develop --------------
  echo ------------------------------------
  IF NOT EXIST development mkdir development
  pushd development
  del *.pdb > NUL 2> NUL

  set codepath=..\..\code\core
  set compiler_options_dev=%compiler_options% /Z7 /Od /DBSE_BUILD_DEVELOP

  cl /LD %codepath%\bse_core.cpp %compiler_options_dev% /Fe:bse_core.dll /Fmbse_core.map %linker_options% %app_exports% /PDB:bse_core_temp_%random%.pdb 
  cl     %codepath%\bse_main.cpp /Fe:bse_develop.exe %compiler_options_dev% %linker_options% 

  echo ------------------------------------
  popd
 IF %build_mode%=="develop" goto end

:build_release
  echo ------------- release --------------
  echo ------------------------------------
  IF NOT EXIST release mkdir release
  pushd release

  set codepath=..\..\code\core
  set compiler_options_release=%compiler_options% /Ox
  
  cl %codepath%\bse_main.cpp /Fe:bse.exe %compiler_options_release% %linker_options% 
  echo ------------------------------------

  del *.obj > NUL 2> NUL
  popd
  IF %build_mode%=="release" goto end

:end
popd


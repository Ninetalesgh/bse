@echo off

setlocal EnableDelayedExpansion

IF "%~1"=="createapp" SHIFT
IF "%~1"=="?" goto help_section
IF "%~1"=="" goto help_section

SHIFT
set app_name=%~0

:loop_parse_parameters
SHIFT
IF NOT "%~0"=="" (
  set app_name=%app_name% %~0

  goto loop_parse_parameters
)

set app_name="%app_name%"

IF EXIST code\bse_main.cpp (
  set out_path=app
) ELSE (
  IF EXIST ..\code\bse_main.cpp (
  set out_path=..\app
  ) ELSE (
    echo couldn't find bse_main.cpp in either code or ../code 
    goto end
  )
)

set cpp_name="%app_name:"=%.cpp"
set h_name="%app_name:"=%.h"
set bat_name="build_%app_name:"=%.bat"

set response=y
IF EXIST %out_path%\%app_name%\%cpp_name% (
  echo %cpp_name% already exists, do you want to overwrite it? [Y/N]
  set /p response=
)

IF /I NOT %response%==y (
  goto end
)

IF NOT EXIST %out_path% mkdir %out_path%
pushd %out_path%
IF NOT EXIST %app_name% mkdir %app_name%
pushd %app_name%

> %h_name% echo #pragma once
>> %h_name% echo #include "bse_core.h"
>> %h_name% echo:
>> %h_name% echo struct AppData
>> %h_name% echo {
>> %h_name% echo   //your hot-reload persistent data goes here!
>> %h_name% echo };
>> %h_name% echo:
>> %h_name% echo //this will be set for you every time 
>> %h_name% echo extern AppData* appData;
>> %h_name% echo:

> %cpp_name% echo #include %h_name%
>> %cpp_name% echo:
>> %cpp_name% echo void initialize_platform( bse::PlatformInitParams* initParameters )
>> %cpp_name% echo {
>> %cpp_name% echo   //called once before the platform is fully initialized to set init parameters
>> %cpp_name% echo }
>> %cpp_name% echo:
>> %cpp_name% echo void initialize_app_data()
>> %cpp_name% echo {
>> %cpp_name% echo   //called once after the platform is fully initialized for you to initialize your app data
>> %cpp_name% echo }
>> %cpp_name% echo:
>> %cpp_name% echo void on_reload()
>> %cpp_name% echo {
>> %cpp_name% echo   //called once every time you hot reload
>> %cpp_name% echo }
>> %cpp_name% echo:
>> %cpp_name% echo void tick()
>> %cpp_name% echo {
>> %cpp_name% echo   //called once every frame
>> %cpp_name% echo }
>> %cpp_name% echo:

> %bat_name% echo @echo off
>> %bat_name% echo set misc_path=bse\misc
>> %bat_name% echo set /a x=0
>> %bat_name% echo :search_build
>> %bat_name% echo IF NOT EXIST %%misc_path%%\build.bat (
>> %bat_name% echo set misc_path=..\%%misc_path%%
>> %bat_name% echo  IF %%x%% LSS 5 ( 
>> %bat_name% echo   set /a "x+=1"
>> %bat_name% echo   goto search_build 
>> %bat_name% echo   ) ELSE (
>> %bat_name% echo     echo couldn't find directory \bse\misc
>> %bat_name% echo     goto end
>> %bat_name% echo   )
>> %bat_name% echo )
>> %bat_name% echo set app_name="%%~n0"
>> %bat_name% echo set app_name=%%app_name:build_=%%
>> %bat_name% echo set app_name=%%app_name:"=%%
>> %bat_name% echo set cpp_path="%%~dp0%%app_name%%.cpp"
>> %bat_name% echo pushd %%misc_path%%
>> %bat_name% echo call build.bat -app %%cpp_path%% %%*
>> %bat_name% echo popd
>> %bat_name% echo :end


popd
popd

goto end
:help_section
  echo --------------------------------------------------------------
  echo ---- No Info -------------------------------------------------
  echo --------------------------------------------------------------
:end
endlocal
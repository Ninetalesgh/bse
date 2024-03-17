@echo off

set package_to_install=%1
set package=%2

if "%package%"=="" set package=com.bse.core

set adb="%ANDROID_SDK_ROOT%\platform-tools\adb"


%adb% uninstall %package%
%adb% install %package_to_install%
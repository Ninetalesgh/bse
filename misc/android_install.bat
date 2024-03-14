@echo off

set adb="C:\Users\CamilloLukesch\AppData\Local\Android\Sdk\platform-tools\adb"

set package=com.bse.core


%adb% uninstall %package%

%adb% install "C:\personal\bse\build\android\bse.apk"
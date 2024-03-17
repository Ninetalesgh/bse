@echo off

set avd=Medium_Tablet_API_VanillaIceCream

set emulator="%ANDROID_SDK_ROOT%\emulator\emulator.exe"
set adb="%ANDROID_SDK_ROOT%\platform-tools\adb.exe"

start "[BSE] Emulator" %emulator% @%avd%
start "[BSE] Logcat" %adb% logcat -s "[BSE]"

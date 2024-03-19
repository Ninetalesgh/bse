@echo off

set avd=Medium_Tablet_API_VanillaIceCream

set emulator="%ANDROID_SDK_ROOT%\emulator\emulator.exe"
set adb="%ANDROID_SDK_ROOT%\platform-tools\adb.exe"

start "[BSE] Android emulator" %emulator% @%avd%
start "[BSE] Android logcat BSE only" %adb% logcat -s "[BSE]"
rem start "[BSE] Android logcat all" %adb% logcat

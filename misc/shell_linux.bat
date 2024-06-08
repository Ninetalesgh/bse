@echo off

rem -------------------------------------------------------------------------------------
rem Make sure these environment variables result in something sensible 
rem if you want to compile for linux
rem -------------------------------------------------------------------------------------

rem this directory should contain:
rem bin\clang++.exe
rem sysroot

set BSE_CLANG_ROOT="C:\Users\CamilloLukesch\AppData\Local\Android\Sdk\ndk\25.1.8937393\toolchains\llvm\prebuilt\windows-x86_64"
set BSE_CLANG_SYSROOT="\\wsl.localhost\Ubuntu"
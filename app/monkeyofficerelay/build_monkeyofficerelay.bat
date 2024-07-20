@echo off
set misc_path=bse\misc
set /a x=0
:search_build
IF NOT EXIST %misc_path%\build.bat (
set misc_path=..\%misc_path%
 IF %x% LSS 5 ( 
  set /a "x+=1"
  goto search_build 
  ) ELSE (
    echo couldn't find directory \bse\misc
    goto end
  )
)
set app_name="%~n0"
set app_name=%app_name:build_=%
set app_name=%app_name:"=%
set cpp_path="%~dp0%app_name%.cpp"
pushd %misc_path%
call build.bat -app %cpp_path% -include "%~dp0openssl\include" -lib "%~dp0openssl\libcrypto.lib;%~dp0openssl\libssl.lib" %*
popd
:end

@echo off

set app_build_config=%~1

set cpp_name=%~n0
set cpp_name="%~dp0%cpp_name:build_=%.cpp"

set relative_path=..\..

pushd %relative_path%
  call build -app %cpp_name% %app_build_config%
popd

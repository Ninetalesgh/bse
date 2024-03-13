@echo off

rem Please set these to the paths you want

rem Compiled horde server data
set sourcedir=C:\Work\FN\Engine\Source\Programs\Horde\Horde.Server\bin\Release\net8.0

rem where you want all the files to go
set outdir=C:\Work\I\

rem where you installed wix
set wixinstalldir=C:\Program Files (x86)\WiX Toolset v3.11\bin\

rem path to the horde wxs files
set hordewxsdir=C:\Work\FN\Engine\Source\Programs\Horde\Installer


rem --------------------------------------------------------------------------------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------------------------------------------------------------------------------
rem --------------------------------------------------------------------------------------------------------------------------------------------------------

set outserverbulk=%outdir%\Server
set outtoolsbulk=%outdir%\Tools

IF EXIST "%outserverbulk%" rmdir /S /Q "%outserverbulk%"
IF EXIST "%outtoolsbulk%" rmdir /S /Q "%outtoolsbulk%"
Xcopy /E /I "%sourcedir%" "%outserverbulk%"
Xcopy /E /I "%sourcedir%" "%outtoolsbulk%"
del %outdir%\Server-Bulk\HordeServer.exe



echo ---------------------------------------------------------------
echo building HordeServerFiles.wxs...
call "%wixinstalldir%heat.exe" dir "%outserverbulk%" -nologo -cg HordeServerFiles -scom -suid -sreg -gg -dr InstallDir -srd -var var.SourceDir -out "%outdir%HordeServerFiles.wxs" -arch x64
echo ---------------------------------------------------------------
echo building HordeServerFiles.wixobj...
call "%wixinstalldir%candle.exe" "%outdir%HordeServerFiles.wxs" -out "%outdir%HordeServerFiles.wixobj" -dSourceDir="%sourcedir%" -arch x64
echo ---------------------------------------------------------------
echo building HordeToolsFiles.wxs...
call "%wixinstalldir%heat.exe" dir "%outtoolsbulk%" -nologo -cg HordeToolsFiles -scom -suid -sreg -gg -dr InstallDir -srd -var var.SourceDir -out "%outdir%HordeToolsFiles.wxs" -arch x64
call wix_name_mangle.exe -wxs "%outdir%HordeToolsFiles.wxs"
rem echo ---------------------------------------------------------------
echo building HordeToolsFiles.wixobj...
call "%wixinstalldir%candle.exe" "%outdir%HordeToolsFiles.wxs" -out "%outdir%HordeToolsFiles.wixobj" -dSourceDir="%sourcedir%" -arch x64
echo ---------------------------------------------------------------
echo building HordeServerFiles.wixobj...
call "%wixinstalldir%candle.exe" -nologo -out "%outdir%HordeServer.wixobj" -ext WixUtilExtension -ext WixUIExtension -arch x64 -dStagingDir="%sourcedir%" "%hordewxsdir%\ServerInstaller.wxs"

echo ---------------------------------------------------------------

pushd "%hordewxsdir%"
echo ---------------------------------------------------------------
echo building .msi file...
call "%wixinstalldir%light.exe" -nologo -out "%outdir%HordeServer.msi" -ext WixUtilExtension -ext WixUIExtension -spdb "%outdir%HordeServerFiles.wixobj" -spdb "%outdir%HordeToolsFiles.wixobj" "%outdir%HordeServer.wixobj"
popd

echo ---------------------------------------------------------------
echo files written to: %outdir%
echo ---------------------------------------------------------------
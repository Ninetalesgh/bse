@echo off 

set androidbuildtoolspath="C:\Users\CamilloLukesch\AppData\Local\Android\Sdk\build-tools\34.0.0"
set androidjarpath="C:\Program Files (x86)\Android\android-sdk\platforms\android-33\android.jar"
set projectpath="C:\personal\test\src"
set javapath="C:\Program Files\Android\jdk\jdk-8.0.302.8-hotspot\jdk8u302-b08\bin"

rem ------------------------------------------------------------------------------------------------------------
rem ------------------------------------------------------------------------------------------------------------
rem ------------------------------------------------------------------------------------------------------------

IF EXIST obj rmdir /s /q obj
IF EXIST bin rmdir /s /q bin
IF EXIST key rmdir /s /q key

mkdir obj
mkdir bin
mkdir key

echo ============================================================================================================
echo ---- Running aapt ------------------------------------------------------------------------------------------
echo ============================================================================================================

set thispath=%~dp0
set aapt="%androidbuildtoolspath:"=%\aapt.exe"
set resourcepath="%projectpath:"=%\res"
set srcpath="%projectpath:"=%\src"
set manifestpath="%projectpath:"=%\AndroidManifest.xml"

%aapt% package -v -f -m -S %resourcepath% -J %srcpath% -M %manifestpath% -I %androidjarpath%

echo ============================================================================================================
echo ---- Running javac -----------------------------------------------------------------------------------------
echo ============================================================================================================

set javac="%javapath:"=%\javac.exe"
set helloworldpath="%srcpath:"=%/com/bse/HelloWorld/*"
set helloworldpath="%helloworldpath:\=/%"

%javac% -d "obj" -source 1.7 -target 1.7 -classpath %androidjarpath% -sourcepath %srcpath% %helloworldpath%

echo ============================================================================================================
echo ---- Running d8 --------------------------------------------------------------------------------------------
echo ============================================================================================================

set d8="%androidbuildtoolspath:"=%\d8.bat"

call %d8% %thispath%obj\com\bse\HelloWorld\* --output=%thispath%bin --no-desugaring

echo ============================================================================================================
echo ---- Running aapt ------------------------------------------------------------------------------------------
echo ============================================================================================================

set resultunsigned=%thispath%bin\HelloWorld.unsigned.apk

%aapt% package -v -f -m -S %resourcepath% -J %srcpath% -M %manifestpath% -I %androidjarpath% -F %resultunsigned% %thispath%bin

echo ============================================================================================================
echo ---- Running keytool ---------------------------------------------------------------------------------------
echo ============================================================================================================

set keytool="%javapath:"=%\keytool.exe"
set appname=BSEtestapp
set keystorepath=%thispath%key\mykey.jks

%keytool% -genkeypair -validity 10000 -dname "CN=%appname%, C=AT" -keystore %keystorepath% -storepass password -keypass password -alias %appname% -keyalg RSA

echo ============================================================================================================
echo ---- Running zipalign --------------------------------------------------------------------------------------
echo ============================================================================================================

set zipalign="%androidbuildtoolspath:"=%\zipalign.exe"
set resultzipaligned=%thispath%bin\HelloWorld_zipaligned.apk

%zipalign% -f 4 %resultunsigned% %resultzipaligned%

echo ============================================================================================================
echo ---- Running apksigner -------------------------------------------------------------------------------------
echo ============================================================================================================

set apksigner="%androidbuildtoolspath:"=%\apksigner.bat"
set result=%thispath%bin\HelloWorld.apk

call %apksigner% sign -v --out %result% --ks %keystorepath% --ks-key-alias %appname% --ks-pass pass:"password" --key-pass pass:"password" %resultzipaligned% 
call %apksigner% verify -v --print-certs %result%

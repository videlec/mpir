@echo off
:: check system
set YASMEXE="%ProgramFiles%\Microsoft Visual Studio 9.0\VC\bin\yasm.exe"
if not exist %YASMEXE% (
        echo Error could not find %YASMEXE%
        goto :EOF
)
:: or does these need root access????
copy yasm.rules "%ProgramFiles%\Microsoft Visual Studio 9.0\VC\VCProjectDefaults\yasm.rules"
:: default is  32bit pentium3 static library
set LIBTYPE=lib
set BIT=Win32
set CPU=p3
set OS=x86
:lp
shift
if "%0" == "" goto :exitlp
if "%0" == "--enable-shared" ( set LIBTYPE=dll)
if "%0" == "--enable-static" ( set LIBTYPE=lib)
if "%0" == "--disable-shared" ( set LIBTYPE=lib)
if "%0" == "--disable-static" ( set LIBTYPE=dll)
if "%0" == "32" ( set BIT=Win32)
if "%0" == "64" ( set BIT=x64)
if "%0" == "pentium" ( set CPU=p0)
if "%0" == "pentium3" ( set CPU=p3)
if "%0" == "pentium4" ( set CPU=p4)
if "%0" == "amd64" ( set CPU=amd64)
if "%0" == "core2" ( set CPU=core2)
goto :lp
:exitlp
if %BIT% == Win32 ( set OS=x86)
if %BIT% == x64 (set OS=x64)
echo set LIBTYPE=%LIBTYPE%> config_params.bat
echo set BIT=%BIT%>> config_params.bat
echo set CPU=%CPU%>> config_params.bat
echo set OS=%OS%>> config_params.bat
echo setting params to
type config_params.bat
:EOF

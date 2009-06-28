@echo off

:: find a compiler
cl config.guess.c > crap 2>&1
if not errorlevel 1 goto :STEP2
if exist %VS90COMNTOOLS%\..\..\VC\bin\vcvars32.bat (
  call "%VS90COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
  cl config.guess.c > crap 2>&1
  if not errorlevel 1 goto :STEP2
)
echo ERROR Cant find a working compiler
goto :EOF
:: found compiler and got config.guess.exe
:STEP2


:: check for yasm
set VCPATH="c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC"
set VCENV="c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"
set YASMEXE64="c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\yasm.exe"
set YASMEXE32="c:\Program Files\Microsoft Visual Studio 9.0\VC\bin\yasm.exe"
if not exist %YASMEXE64% (
	set VCPATH="c:\Program Files\Microsoft Visual Studio 9.0\VC"
	:: what happend to this file below???
	::set VCENV="c:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvarsall.bat"
	set VCENV="c:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"
	if not exist %YASMEXE32% (
		echo Error could not find %YASMEXE64%
	        echo Error could not find %YASMEXE32%
	        goto :EOF
	)
)

:: or does these need root access???? we can still check for existence
:: copy yasm.rules "%VCPATH\VCProjectDefaults\yasm.rules"

:: default is  32bit pentium3 static library
config.guess.exe
config.guess.exe var > config.guess.bat
call config.guess.bat
del config.guess.bat
set LIBTYPE=lib
set BIT=Win32
if "%BITS%" == "32" ( set BIT=Win32)
if "%BITS%" == "64" ( set BIT=x64)
set CPU=p3
set JOS=x86
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
if %BIT% == Win32 ( set JOS=x86)
if %BIT% == x64 (set JOS=x64)
echo set LIBTYPE=%LIBTYPE%> config_params.bat
echo set BIT=%BIT%>> config_params.bat
echo set CPU=%CPU%>> config_params.bat
echo set JOS=%JOS%>> config_params.bat
echo set VCPATH=%VCPATH%>> config_params.bat
echo set VCENV=%VCENV%>> config_params.bat
echo setting params to
type config_params.bat
:EOF

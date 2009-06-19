@echo off
call "config_params.bat"
if "%DONECONFIG%" == "no" goto :EOF
call "%ProgramFiles%\Microsoft Visual Studio 9.0\VC\bin\vcvarsall.bat" %OS%
if "%1" == ""        goto :make
if "%1" == "clean"   goto :clean
if "%1" == "install" goto :install
if "%1" == "check"   goto :check
echo Unkwown option
goto :EOF

:make
vcbuild gen-mpir\gen-mpir.vcproj "Release|%BIT%"
vcbuild gen-bases\gen-bases.vcproj "Release|%BIT%"
vcbuild gen-fac_ui\gen-fac_ui.vcproj "Release|%BIT%"
vcbuild gen-fib\gen-fib.vcproj "Release|%BIT%"
vcbuild gen-psqr\gen-psqr.vcproj "Release|%BIT%"
set LIBBUILD=%LIBTYPE%_mpir_%CPU%
vcbuild %LIBBUILD%\%LIBBUILD%.vcproj "Release|%BIT%"
:: c++ to build  if static
vcbuild lib_mpir_cpp\lib_mpir_cpp.vcproj "Release|%BIT%"
goto :EOF

:check
:: this gives an error if we dont build the c++ stuff
vcbuild mpir-tests.sln "Release|%BIT%"
::copy gmp-mparam.h %LIBTYPE%\%BIT%\Release\gmp-mparam.h
::vcbuild /override:mpir-tests\test-config.vsprops mpir-tests\add-test-lib.vcproj "Release|%BIT%"
::vcbuild mpir-tests\add-test-lib.vcproj "Release|%BIT%"
cd mpir-tests
c:\python26\python.exe run-tests.py
cd ..
goto :EOF

:install
goto :EOF

:clean
echo echo must run configure first > config_params.bat
echo set DONECONFIG=no >> config_params.bat
goto :EOF

:EOF

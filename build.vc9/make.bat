@echo off
if not exist config_params.bat (
	echo run configure first
	goto :EOF
)
call config_params.bat
:: only really want to run this once as it keep appending to the path
call %VCENV% %OS%
if "%1" == ""        goto :make
if "%1" == "clean"   goto :clean
if "%1" == "install" goto :install
if "%1" == "check"   goto :check
echo Unkwown option
goto :EOF

:make
vcbuild gen-mpir\gen-mpir.vcproj "Release|Win32"
vcbuild gen-bases\gen-bases.vcproj "Release|Win32"
vcbuild gen-fac_ui\gen-fac_ui.vcproj "Release|Win32%"
vcbuild gen-fib\gen-fib.vcproj "Release|Win32"
vcbuild gen-psqr\gen-psqr.vcproj "Release|Win32"
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
del config_params.bat
goto :EOF

:EOF

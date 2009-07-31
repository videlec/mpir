@echo off
if "%1" == "" (
	echo Usage: %0 mpir_directory
	exit /b 1
)
pushd %1\build.vc9
call configure && call make && call make check && call make clean
if %errorlevel% 1 (
	echo "ERROR"
	exit /b 1
)
call configure --enable-shared && call make && call make clean
if %errorlevel% 1 (
	echo "ERROR --enable-shared"
	exit /b 1
)
call configure --cpu-none && call make && call make check && call make clean
if %errorlevel% 1 (
	echo "ERROR --cpu-none"
	exit /b 1
)
call configure --cpu-none --enable-shared && call make && call make clean
if %errorlevel% 1 (
	echo "ERROR --cpu-none --enable-shared"
	exit /b 1
)
popd
exit /b 0

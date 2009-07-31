@echo off
pushd %1
pushd build.vc9
dir

configure.bat
make
make check
make clean

configure.bat --enable-shared
make
::make check    this is broken for now
make clean

configure.bat --cpu-none
make
make check
make clean

configure.bat --cpu-none --enable-shared
make
::make check    this is broken for now
make clean

popd
popd

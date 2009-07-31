@echo off
pushd %1\build.vc9

call configure
call make
call make check
call make clean

call configure --enable-shared
call make
::call make check    this is broken for now
call make clean

call configure --cpu-none
call make
call make check
call make clean

call configure --cpu-none --enable-shared
call make
::call make check    this is broken for now
call make clean

popd

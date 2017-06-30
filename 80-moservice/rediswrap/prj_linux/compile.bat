#!/bin/sh

echo makefile_linux_x86_d clean
make -f makefile_linux_x86_d clean
make -f makefile_linux_x86_d 1> ../../../10-common/version/compileinfo/rediswrap_linux_x86_debug.txt 2>&1
cp ./librediswrap.so ../../../10-common/lib/debug/linux/
echo makefile_linux_x86_d completed

echo makefile_linux_x86_r clean
make -f makefile_linux_x86_r clean
make -f makefile_linux_x86_r 1> ../../../10-common/version/compileinfo/rediswrap_linux_x86_release.txt 2>&1
cp ./librediswrap.so ../../../10-common/lib/release/linux/
echo makefile_linux_x86_r completed

cd ..

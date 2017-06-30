#!/bin/sh

# **********************makefile_linux_ppc_5020_2637_r
make -f makefile_linux_ppc_5020_d clean
make -f makefile_linux_ppc_5020_d 1> ../../../10-common/version/compileinfo/mqwrapper_linux_ppc_5020_d.txt 2>&1
cp ./librabbitmqwrap.a ../../../10-common/lib/debug/linux_ppc_qoriq_2637/
echo makefile_linux_ppc_5020_d completed

make -f makefile_linux_ppc_5020_r clean
make -f makefile_linux_ppc_5020_r 1> ../../../10-common/version/compileinfo/mqwrapper_linux_ppc_5020_r.txt 2>&1
cp ./librabbitmqwrap.a ../../../10-common/lib/release/linux_ppc_qoriq_2637/
echo makefile_linux_ppc_5020_r completed


echo makefile_linux_x86_d clean
make -f makefile_linux_x86_d clean
make -f makefile_linux_x86_d 1> ../../../10-common/version/compileinfo/mqwrapper_linux_x86_debug.txt 2>&1
cp ./librabbitmqwrap.so ../../../10-common/lib/debug/linux/
echo makefile_linux_x86_d completed

echo makefile_linux_x86_r clean
make -f makefile_linux_x86_r clean
make -f makefile_linux_x86_r 1> ../../../10-common/version/compileinfo/mqwrapper_linux_x86_release.txt 2>&1
cp ./librabbitmqwrap.so ../../../10-common/lib/release/linux/
echo makefile_linux_x86_r completed

cd ..
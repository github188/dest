#!/bin/sh

# **********************makefile_linux_ppc_5020_2637_r
#make -f makefile_linux_ppc_5020_r clean

#make -f makefile_linux_ppc_5020_r clean all 1> ../../../10-common/version/compileinfo/css_linux_ppc_5020_r.txt
#cp ./css ../../../10-common/version/release/linux_ppc_qoriq_2637/

#echo makefile_linux_ppc_5020_r completed

echo makefile_linux_x86_r

make -f makefile_linux_x86_r clean
make -f makefile_linux_x86_r 1> ../../../10-common/version/compileinfo/css_linux_x86_r.txt 

cp ./css ../../../10-common/version/release/linux

echo makefile_linux_x86_r completed

cd ..
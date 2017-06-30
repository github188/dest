#!/bin/sh

make -f makefile_linux_ppc_5020_r clean

make -f makefile_linux_ppc_5020_r 1> ../../../10-common/version/compileinfo/commonlib_linux_ppc_5020_r.txt
cp ./libcommonlib.a ../../../10-common/lib/release/linux_ppc_qoriq_2637/

echo makefile_linux_ppc_5020_r completed!


make -f makefile_linux_x86_r clean

make -f makefile_linux_x86_r 1> ../../../10-common/version/compileinfo/commonlib_linux_x86_r.txt
cp ./libcommonlib.a ../../../10-common/lib/release/linux/

echo makefile_linux_x86_r completed!


make -f makefile_linux_ppc_1010_r clean

make -f makefile_linux_ppc_1010_r 1> ../../../10-common/version/compileinfo/commonlib_linux_ppc_1010_r.txt
cp ./libcommonlib.a ../../../10-common/lib/release/linux_ppc_qoriq_2637_smu/

echo makefile_linux_ppc_1010_r completed!

cd ..

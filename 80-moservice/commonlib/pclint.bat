echo off
cd prj_win32
call pclint.bat

cd ..
cd prj_linux
call pclint.bat
echo on
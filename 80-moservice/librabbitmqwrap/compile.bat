

cd prj_win32
call "%VS100COMNTOOLS%\vsvars32.bat"
echo build clean debug VS2010
@devenv prjLibRabbitmqWrapper.vcxproj /ReBuild "Debug" /Out ..\..\..\10-common\version\CompileInfo\rabbitmqwrap_d_2010.txt
copy /Y .\Debug\librabbitmqwrap.lib ..\..\..\10-common\lib\debug\win32_2010\
copy /Y .\Debug\librabbitmqwrap.dll ..\..\..\10-common\lib\debug\win32_2010\

echo build clean release VS2010
@devenv prjLibRabbitmqWrapper.vcxproj /ReBuild "Release" /Out ..\..\..\10-common\version\CompileInfo\rabbitmqwrap_r_2010.txt
copy /Y .\Release\librabbitmqwrap.lib ..\..\..\10-common\lib\release\win32_2010\
copy /Y .\Release\librabbitmqwrap.dll ..\..\..\10-common\lib\release\win32_2010\
cd ..


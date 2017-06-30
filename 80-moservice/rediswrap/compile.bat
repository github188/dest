

cd prj_win32
call "%VS100COMNTOOLS%\vsvars32.bat"
echo build clean debug VS2010
@devenv rediswrap.vcxproj /ReBuild "Debug" /Out ..\..\..\10-common\version\CompileInfo\rediswrap_d_2010.txt
copy /Y .\Debug\librediswrap.lib ..\..\..\10-common\lib\debug\win32_2010\
copy /Y .\Debug\librediswrap.dll ..\..\..\10-common\lib\debug\win32_2010\

echo build clean release VS2010
@devenv rediswrap.vcxproj /ReBuild "Release" /Out ..\..\..\10-common\version\CompileInfo\rediswrap_r_2010.txt
copy /Y .\Release\librediswrap.lib ..\..\..\10-common\lib\release\win32_2010\
copy /Y .\Release\librediswrap.dll ..\..\..\10-common\lib\release\win32_2010\
cd ..


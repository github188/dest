
@echo.
@echo      ==============================================
@echo      =      make win32 debug aps.exe              =
@echo      ==============================================

del /F /Q Debug\*.*
del /F /Q ..\..\..\..\10-common\version\compileinfo\aps_windows_debug.txt
devenv aps.vcxproj /Rebuild "Debug|Win32"  /Out ..\..\..\..\10-common\version\compileinfo\aps_windows_debug.txt

@echo.
@echo      ==============================================
@echo      =      make win32 release aps.exe            =
@echo      ==============================================

del /F /Q Release\*.*
del /F /Q ..\..\..\..\10-common\version\compileinfo\aps_windows_release.txt
devenv aps.vcxproj /Rebuild "Release|Win32"  /Out ..\..\..\..\10-common\version\compileinfo\aps_windows_release.txt


echo aps.exe build over
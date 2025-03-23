@echo off
echo Searching for built module files...

:: Look in the build directory
echo Searching in build directory:
dir /s /b build\mouse_control* 

:: If the file is found in build/Release
if exist build\Release\mouse_control* (
    echo Found in build/Release:
    dir build\Release\mouse_control*
    echo Copying files from build/Release...
    copy /Y build\Release\mouse_control* .
)

:: If the file is found in build/lib/Release
if exist build\lib\Release\mouse_control* (
    echo Found in build/lib/Release:
    dir build\lib\Release\mouse_control*
    echo Copying files from build/lib/Release...
    copy /Y build\lib\Release\mouse_control* .
)

:: Check for any mouse_control files directly in the build directory
if exist build\mouse_control* (
    echo Found in build:
    dir build\mouse_control*
    echo Copying files from build...
    copy /Y build\mouse_control* .
)

:: Look specifically for .dll files
if exist build\lib\Release\*.dll (
    echo Found DLL files in lib/Release:
    dir build\lib\Release\*.dll
    echo Copying DLL files...
    copy /Y build\lib\Release\*.dll .
)

if exist build\Release\*.dll (
    echo Found DLL files in Release:
    dir build\Release\*.dll
    echo Copying DLL files...
    copy /Y build\Release\*.dll .
)

echo.
echo Files in current directory:
dir mouse_control*

echo.
echo If you see .dll files but not .pyd files, you can rename the .dll to .pyd:
echo copy mouse_control*.dll mouse_control.pyd

pause 
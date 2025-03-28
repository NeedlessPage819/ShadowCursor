@echo off
setlocal enabledelayedexpansion

:: Check if running as administrator
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo This script requires administrator privileges.
    echo Please right-click and select "Run as administrator".
    pause
    exit /b 1
)

:: Set the driver file name
set "DRIVER_FILE=xnxc.sys"

:: Check if the driver file exists
if not exist "%DRIVER_FILE%" (
    echo Error: Driver file not found at: %DRIVER_FILE%
    echo Please make sure the driver file is in the same directory as this script.
    pause
    exit /b 1
)

echo Driver file found: %DRIVER_FILE%

:: Check for an existing driver service
echo Checking for existing driver service...
sc query cla300 >nul 2>&1
if %errorLevel% equ 0 (
    echo Stopping existing driver service...
    sc stop cla300 >nul 2>&1
    timeout /t 2 >nul
    sc delete cla300 >nul 2>&1
    timeout /t 2 >nul
    echo Existing driver service removed.
) else (
    echo No existing driver service found.
)

:: Create the driver service
echo Creating driver service...
sc create cla300 binPath= "%CD%\%DRIVER_FILE%" type= kernel >nul 2>&1
if %errorLevel% neq 0 (
    echo Error: Failed to create driver service.
    pause
    exit /b 1
)

:: Start the driver service
echo Starting driver service...
sc start cla300 >nul 2>&1
if %errorLevel% neq 0 (
    echo Error: Failed to start driver service.
    echo Removing failed service...
    sc delete cla300 >nul 2>&1
    pause
    exit /b 1
)

:: Success message
echo Driver loaded successfully!
echo You can now run mouse_example.py to use the enhanced mouse control features.
echo.
echo Note: To unload the driver, run: sc stop cla300 && sc delete cla300
echo.

pause
@echo off
echo Building Undetectable Mouse Control Python Module...

:: Check if Python is installed
python --version > nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Python not found! Please install Python 3.6 or newer.
    exit /b 1
)

:: Check if CMake is installed 
cmake --version > nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo CMake not found! Please install CMake.
    exit /b 1
)

:: Verify vcpkg is installed and VCPKG_ROOT is set
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo ERROR: vcpkg not found! Please install vcpkg and set VCPKG_ROOT environment variable.
    echo You can install vcpkg with these commands:
    echo.
    echo git clone https://github.com/Microsoft/vcpkg.git
    echo cd vcpkg
    echo .\bootstrap-vcpkg.bat
    echo.
    echo Then set VCPKG_ROOT environment variable to the vcpkg directory.
    exit /b 1
)

:: Use vcpkg to install pybind11
echo Installing pybind11 using vcpkg...
"%VCPKG_ROOT%\vcpkg" install pybind11:x64-windows
if %ERRORLEVEL% NEQ 0 (
    echo Failed to install pybind11 with vcpkg!
    exit /b 1
)

:: Set up paths for pybind11
set VCPKG_INSTALLED=%VCPKG_ROOT%\installed\x64-windows
set PYBIND11_DIR=%VCPKG_INSTALLED%\share\pybind11

echo pybind11 directory: %PYBIND11_DIR%

:: Install keyboard module for Python
echo Installing keyboard module...
python -m pip install keyboard > nul 2>&1

:: Create build directory if it doesn't exist
if not exist build mkdir build

cd build

:: Run CMake configuration
echo Running CMake configuration...
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -Dpybind11_DIR="%PYBIND11_DIR%" ..
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

:: Build the project with CMake
echo Building with CMake...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    cd ..
    exit /b 1
)

:: Copy the compiled module to the parent directory
echo Copying module...
copy /Y lib\Release\*.pyd ..

cd ..

:: Check if the module was built successfully
if exist mouse_control*.pyd (
    echo.
    echo Build completed successfully!
    echo.
    echo You can now use the module in Python with:
    echo    import mouse_control
    echo.
    echo Try running the example:
    echo    python mouse_example.py
    echo.
) else (
    echo.
    echo Build may have failed. Could not find the compiled module.
    exit /b 1
)

pause 
@echo off
echo ==========================================
echo MCX Windows Build Script
echo ==========================================
echo.

:: Check for Visual Studio
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: Visual Studio compiler not found.
    echo Please run this script from "Developer PowerShell for VS 2022"
    echo or "Developer Command Prompt for VS 2022"
    exit /b 1
)

echo [1/4] Checking prerequisites...
cmake --version >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: CMake not found. Please install CMake.
    exit /b 1
)
echo - CMake: OK

echo.
echo [2/4] Creating build directory...
if not exist build mkdir build
cd build

echo.
echo [3/4] Configuring with CMake...
cmake -S .. -B . -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed
    exit /b 1
)

echo.
echo [4/4] Building MCX...
cmake --build . --config Release --parallel
if %errorlevel% neq 0 (
    echo ERROR: Build failed
    exit /b 1
)

echo.
echo ==========================================
echo Build Complete!
echo ==========================================
echo.
echo Output files:
echo   build\Release\mcx.exe
echo   build\Release\mcx_tests.exe
echo   build\modules\sample_module.dll
echo.
echo To run MCX:
echo   cd build\Release
echo   mcx.exe --setup
echo   mcx.exe --server
echo.

@echo off
setlocal EnableDelayedExpansion

echo.
echo ====================================
echo   Fish - Minecraft Auto-Fisher
echo   Build Script
echo ====================================
echo.

:: Check for CMake
where cmake >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

:: Check for JAVA_HOME
if "%JAVA_HOME%"=="" (
    echo [WARNING] JAVA_HOME not set
    echo Attempting to find JDK automatically...
    
    :: Try common JDK locations
    for %%D in (
        "%ProgramFiles%\Java\jdk*"
        "%ProgramFiles%\Eclipse Adoptium\jdk*"
        "%ProgramFiles%\Microsoft\jdk*"
        "%ProgramFiles%\Amazon Corretto\jdk*"
    ) do (
        if exist "%%~D" (
            set "JAVA_HOME=%%~D"
            echo Found JDK: !JAVA_HOME!
            goto :found_jdk
        )
    )
    
    echo [ERROR] Could not find JDK. Please install JDK and set JAVA_HOME
    pause
    exit /b 1
)

:found_jdk
echo Using JAVA_HOME: %JAVA_HOME%

:: Create build directory
if not exist "build" mkdir build
cd build

:: Configure with CMake
echo.
echo [*] Configuring project...
cmake -G "Visual Studio 17 2022" -A x64 ..
if errorlevel 1 (
    echo.
    echo [*] Trying Visual Studio 2019...
    cmake -G "Visual Studio 16 2019" -A x64 ..
)
if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

:: Build
echo.
echo [*] Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo.
echo ====================================
echo   Build Complete!
echo ====================================
echo.
echo Output files:
echo   build\bin\Release\Fish.dll
echo   build\bin\Release\FishInjector.exe
echo.
echo To use:
echo   1. Start Minecraft
echo   2. Run FishInjector.exe as Administrator
echo   3. Hold a fishing rod and cast into water
echo   4. Press INSERT to toggle auto-fishing
echo   5. Press END to eject the DLL
echo.

pause

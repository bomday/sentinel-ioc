@echo off
REM Sentinel IOC Management System - Build & Run Script
REM Usage: build.bat [cli|gui|clean]

if "%1"=="help" goto :help
if "%1"=="-h" goto :help
if "%1"=="--help" goto :help
if "%1"=="cli" goto :cli
if "%1"=="gui" goto :gui
if "%1"=="clean" goto :clean
if "%1"=="" goto :gui

:gui
echo Building Sentinel IOC GUI...
if not exist "build" mkdir build
cd build
cmake .. -G "MinGW Makefiles"
if errorlevel 1 goto :error
mingw32-make
if errorlevel 1 goto :error
cd ..
echo.
echo ========================================
echo SUCCESS! Starting Sentinel IOC GUI...
echo ========================================
SentinelIOC-GUI.exe
exit /b 0

:cli
echo Building Sentinel IOC CLI...
if not exist "build" mkdir build
cd build
cmake .. -G "MinGW Makefiles"
if errorlevel 1 goto :error
mingw32-make SentinelIOC-CLI
if errorlevel 1 goto :error
cd ..
echo.
echo ========================================
echo SUCCESS! Starting Sentinel IOC CLI...
echo ========================================
SentinelIOC-CLI.exe
exit /b 0

:clean
echo Cleaning build directory...
if exist "build" rmdir /s /q build
echo Build directory cleaned.
exit /b 0

:error
echo Build failed! Make sure you have:
echo - MinGW-w64 installed
echo - Qt6 installed (for GUI version)
echo - CMake installed
cd ..
exit /b 1

:help
echo Sentinel IOC Management System
echo.
echo Usage: build.bat [command]
echo.
echo Commands:
echo   gui     Build and run GUI version (default)
echo   cli     Build and run CLI version
echo   clean   Clean build directory
echo   help    Show this help
echo.
exit /b 0
    )
) else (
    echo Qt6 found - Building GUI application...
    cmake --build . --target SentinelIOC-GUI
    if errorlevel 1 (
        echo GUI Build failed!
        cd ..
        exit /b 1
    ) else (
        echo GUI Build successful! Executable created: build\SentinelIOC-GUI.exe
        cd ..
        exit /b 0
    )
)

:run
REM Check if GUI executable exists first
if exist "build\SentinelIOC-GUI.exe" (
    echo Running Sentinel IOC GUI Management System...
    echo ---------------------------------------------
    cd build
    SentinelIOC-GUI.exe
    cd ..
    exit /b 0
) else if exist "build\SentinelIOC-CLI.exe" (
    echo GUI not available, running CLI version...
    echo Running Sentinel IOC CLI Management System...
    echo ---------------------------------------------
    cd build
    SentinelIOC-CLI.exe
    cd ..
    exit /b 0
) else (
    echo No executable found!
    echo Run 'build.bat build' or 'build.bat all' to compile first.
    exit /b 1
)

:cli
echo Building and running CLI version...
if not exist "build" mkdir build
cd build

echo Configuring project with CMake...
cmake .. -G "MinGW Makefiles"
if errorlevel 1 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

echo Building CLI application...
cmake --build . --target SentinelIOC-CLI
if errorlevel 1 (
    echo CLI Build failed!
    cd ..
    exit /b 1
)

echo CLI Build successful! Running...
SentinelIOC-CLI.exe
cd ..
exit /b 0

:gui
goto :all

:clean
echo Cleaning up build files...
if exist "build" rmdir /s /q build
echo Cleanup complete!
exit /b 0

:help
echo Sentinel IOC Management System - Build Script (Windows)
echo Usage: %0 [build^|run^|all^|clean^|cli^|gui^|help]
echo.
echo Commands:
echo   build - Compile the GUI project only
echo   run   - Run the GUI program (requires existing executable)
echo   all   - Build and run the GUI program (default)
echo   clean - Remove executable and build files
echo   cli   - Build and run CLI version
echo   gui   - Build and run GUI version (same as 'all')
echo   help  - Show this help message
echo.
exit /b 0

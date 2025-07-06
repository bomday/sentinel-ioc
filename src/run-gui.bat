@echo off
REM Quick launcher for Sentinel IOC GUI
REM This script ensures Qt6 DLLs are found

echo Starting Sentinel IOC GUI...

REM Add Qt6 to PATH if it exists
if exist "C:\Qt\6.9.1\mingw_64\bin" (
    set PATH=C:\Qt\6.9.1\mingw_64\bin;%PATH%
) else if exist "C:\Qt\6.8.0\mingw_64\bin" (
    set PATH=C:\Qt\6.8.0\mingw_64\bin;%PATH%
) else if exist "C:\Qt\6.7.0\mingw_64\bin" (
    set PATH=C:\Qt\6.7.0\mingw_64\bin;%PATH%
) else (
    echo Warning: Qt6 not found in common locations.
    echo Please add Qt6\bin to your PATH manually.
)

REM Check if executable exists
if not exist "SentinelIOC-GUI.exe" (
    echo GUI executable not found! Building first...
    call build.bat gui
    if %errorlevel% neq 0 (
        echo Build failed!
        pause
        exit /b %errorlevel%
    )
)

echo.
echo ========================================
echo SUCCESS! Starting Sentinel IOC GUI...
echo ========================================

REM Run the GUI from src directory
SentinelIOC-GUI.exe

#!/bin/bash
# Quick run script for Sentinel IOC GUI
# This script ensures Qt6 libraries are found

echo "Starting Sentinel IOC GUI..."

# Add Qt6 to PATH if it exists (Windows paths in Git Bash)
if [ -d "/c/Qt/6.9.1/mingw_64/bin" ]; then
    export PATH="/c/Qt/6.9.1/mingw_64/bin:$PATH"
    echo "Found Qt6 at: /c/Qt/6.9.1/mingw_64/bin"
elif [ -d "/c/Qt/6.8.0/mingw_64/bin" ]; then
    export PATH="/c/Qt/6.8.0/mingw_64/bin:$PATH"
    echo "Found Qt6 at: /c/Qt/6.8.0/mingw_64/bin"
elif [ -d "/c/Qt/6.7.0/mingw_64/bin" ]; then
    export PATH="/c/Qt/6.7.0/mingw_64/bin:$PATH"
    echo "Found Qt6 at: /c/Qt/6.7.0/mingw_64/bin"
else
    echo "Warning: Qt6 not found in common locations."
    echo "Please add Qt6/bin to your PATH manually."
fi

# Check if executable exists
if [ ! -f "build/SentinelIOC-GUI.exe" ]; then
    echo "GUI executable not found! Building first..."
    ./build.sh gui
    exit $?
fi

# Run the GUI
cd build
./SentinelIOC-GUI.exe
cd ..

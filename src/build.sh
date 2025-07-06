#!/bin/bash

# Sentinel IOC Management System - Build & Run Script
# Usage: ./build.sh [cli|gui|clean]

show_help() {
    echo "Sentinel IOC Management System"
    echo ""
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  gui     Build and run GUI version (default)"
    echo "  cli     Build and run CLI version"
    echo "  clean   Clean build directory"
    echo "  help    Show this help"
    echo ""
    echo "Requirements:"
    echo "  - CMake, MinGW-w64"
    echo "  - Qt6 (for GUI version)"
    echo ""
}

build_gui() {
    echo "Building Sentinel IOC GUI..."
    
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
    
    mkdir -p build
    cd build
    cmake .. -G "MinGW Makefiles"
    if [ $? -ne 0 ]; then
        echo "Build failed! Check requirements."
        cd ..
        exit 1
    fi
    mingw32-make
    if [ $? -ne 0 ]; then
        echo "Build failed!"
        cd ..
        exit 1
    fi
    echo ""
    echo "========================================"
    echo "SUCCESS! Starting Sentinel IOC GUI..."
    echo "========================================"
    cd ..
    ./SentinelIOC-GUI.exe
}

build_cli() {
    echo "Building Sentinel IOC CLI..."
    mkdir -p build
    cd build
    cmake .. -G "MinGW Makefiles"
    if [ $? -ne 0 ]; then
        echo "Build failed! Check requirements."
        cd ..
        exit 1
    fi
    mingw32-make SentinelIOC-CLI
    if [ $? -ne 0 ]; then
        echo "Build failed!"
        cd ..
        exit 1
    fi
    echo ""
    echo "========================================"
    echo "SUCCESS! Starting Sentinel IOC CLI..."
    echo "========================================"
    cd ..
    ./SentinelIOC-CLI.exe
}

clean_build() {
    echo "Cleaning build directory..."
    rm -rf build
    echo "Build directory cleaned."
}

# Main script logic
case "${1:-gui}" in
    "gui"|"")
        build_gui
        ;;
    "cli")
        build_cli
        ;;
    "clean")
        clean_build
        ;;
    "help"|"-h"|"--help")
        show_help
        ;;
    *)
        echo "Unknown command: $1"
        show_help
        exit 1
        ;;
esac

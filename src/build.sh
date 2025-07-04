#!/bin/bash

# Build and Run script for Sentinel IOC Management System
# Usage: ./build.sh [build|run|all|clean]
#   build - Only compile the project
#   run   - Only run the program (requires existing executable)
#   all   - Build and then run the program (default)
#   clean - Remove executable and object files

show_help() {
    echo "Sentinel IOC Management System - Build Script"
    echo "Usage: $0 [build|run|all|clean|help]"
    echo ""
    echo "Commands:"
    echo "  build - Compile the project only"
    echo "  run   - Run the program (requires existing executable)"
    echo "  all   - Build and run the program (default)"
    echo "  clean - Remove executable and object files"
    echo "  help  - Show this help message"
    echo ""
}

build_project() {
    echo "Building Sentinel IOC Management System..."
    
    # Compile all source files with g++
    g++ -std=c++17 -Wall -Wextra -g -I. -static -o sentinel-ioc \
        main.cpp \
        fileManager/fileManager.cpp \
        indicator/indicator.cpp \
        indicatorManager/indicatorManager.cpp \
        maliciousHash/maliciousHash.cpp \
        maliciousIP/maliciousIP.cpp \
        maliciousURL/maliciousURL.cpp \
        utils/utils.cpp \
        cli/cli.cpp

    if [ $? -eq 0 ]; then
        echo "Build successful! Executable created: sentinel-ioc.exe"
        echo "File size: $(ls -lh sentinel-ioc.exe | awk '{print $5}')"
        return 0
    else
        echo "Build failed!"
        return 1
    fi
}

run_program() {
    if [ ! -f "sentinel-ioc.exe" ]; then
        echo "Executable 'sentinel-ioc.exe' not found!"
        echo "Run '$0 build' or '$0 all' to compile first."
        return 1
    fi
    
    echo "Running Sentinel IOC Management System..."
    echo "----------------------------------------"
    ./sentinel-ioc.exe
}

clean_project() {
    echo "Cleaning up build files..."
    rm -f sentinel-ioc.exe *.o
    echo "Cleanup complete!"
}

# Main script logic
case "${1:-all}" in
    "build")
        build_project
        ;;
    "run")
        run_program
        ;;
    "all")
        if build_project; then
            echo ""
            run_program
        fi
        ;;
    "clean")
        clean_project
        ;;
    "help"|"-h"|"--help")
        show_help
        ;;
    *)
        echo "❌ Unknown command: $1"
        echo ""
        show_help
        exit 1
        ;;
esac

#!/bin/bash

# Diagnostic script for Sentinel IOC build issues
# Run this in Git Bash to diagnose build problems

echo "=================================="
echo "Sentinel IOC Build Diagnostics"
echo "=================================="
echo ""

# Check terminal type
echo "1. Terminal Check:"
if [ -n "$BASH_VERSION" ]; then
    echo "   ✅ Running in Bash: $BASH_VERSION"
else
    echo "   ❌ Not running in Bash - use Git Bash instead"
    exit 1
fi
echo ""

# Check compiler
echo "2. Compiler Check:"
if command -v g++ > /dev/null 2>&1; then
    echo "    GCC found: $(g++ --version | head -1)"
    echo "    Location: $(which g++)"
else
    echo "    GCC not found - install Git Bash or MSYS2"
    echo "    Download: https://git-scm.com/download/win"
    exit 1
fi
echo ""

# Check C++17 support
echo "3. C++17 Support:"
echo 'int main(){return 0;}' > test_cpp17.cpp
if g++ -std=c++17 test_cpp17.cpp -o test_cpp17 2>/dev/null; then
    echo "   C++17 supported"
    rm -f test_cpp17.cpp test_cpp17.exe test_cpp17
else
    echo "   C++17 not supported - update your compiler"
    rm -f test_cpp17.cpp
    exit 1
fi
echo ""

# Check file structure
echo "4. File Structure Check:"
required_files=(
    "build.sh"
    "main.cpp"
    "CLI/cli.cpp"
    "fileManager/fileManager.cpp"
    "indicator/indicator.cpp"
    "indicatorManager/indicatorManager.cpp"
    "maliciousHash/maliciousHash.cpp"
    "maliciousIP/maliciousIP.cpp"
    "maliciousURL/maliciousURL.cpp"
    "utils/utils.cpp"
)

missing_files=()
for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "    $file"
    else
        echo "    $file (missing)"
        missing_files+=("$file")
    fi
done

if [ ${#missing_files[@]} -gt 0 ]; then
    echo ""
    echo "    Missing files detected. Make sure you're in the src/ directory"
    echo "    Run: cd /path/to/sentinel-ioc/src"
    exit 1
fi
echo ""

# Check build.sh permissions
echo "5. Build Script Check:"
if [ -x "build.sh" ]; then
    echo "    build.sh is executable"
else
    echo "     build.sh not executable - fixing..."
    chmod +x build.sh
    if [ -x "build.sh" ]; then
        echo "    Fixed: build.sh is now executable"
    else
        echo "    Failed to make build.sh executable"
        exit 1
    fi
fi
echo ""

# Test build
echo "6. Test Build:"
echo "   🔨 Attempting to build..."
if ./build.sh build 2>&1 | grep -q "Build successful"; then
    echo "    Build test successful!"
    echo "    Your environment is ready!"
else
    echo "    Build test failed"
    echo "    Try running: ./build.sh build"
    echo "    Check the error message above"
    exit 1
fi

echo ""
echo "=================================="
echo " All checks passed!"
echo "You can now run: ./build.sh"
echo "=================================="

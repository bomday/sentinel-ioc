# Qt6 Installation Guide for Sentinel IOC GUI

This guide will help you install Qt6 to enable the GUI version of Sentinel IOC.

## Option 1: Qt Online Installer (Recommended)

### Step 1: Download Qt Installer
1. Go to https://www.qt.io/download-qt-installer
2. Download the Qt Online Installer for Windows
3. Run the installer as Administrator

### Step 2: Install Qt6
1. Create a Qt account (free)
2. In the installer, select "Qt 6.x.x" (latest LTS version recommended)
3. Under Qt 6.x.x, select:
   - ✅ **MinGW 11.2.0 64-bit** (matches your current setup)
   - ✅ **Qt 5 Compatibility Module**
   - ✅ **Additional Libraries** → **Qt Debug Information Files**

### Step 3: Set Environment Variables
After installation, add Qt to your PATH:

**Option A: Automatic Setup**
```bash
# Add to your ~/.bashrc or run each time
export PATH="/c/Qt/6.x.x/mingw_64/bin:$PATH"
export CMAKE_PREFIX_PATH="/c/Qt/6.x.x/mingw_64"
```

**Option B: Windows Environment Variables**
1. Open "Environment Variables" in Windows Settings
2. Add to System PATH: `C:\Qt\6.x.x\mingw_64\bin`
3. Add new variable: `CMAKE_PREFIX_PATH` = `C:\Qt\6.x.x\mingw_64`

## Option 2: Manual Installation

### Step 1: Download Qt6 Binaries
1. Go to https://download.qt.io/official_releases/qt/
2. Navigate to the latest 6.x version
3. Download the Windows MinGW package

### Step 2: Extract and Setup
1. Extract to `C:\Qt\`
2. Follow Step 3 from Option 1 above

## Option 3: Package Manager (Advanced)

### Using MSYS2
```bash
# Install MSYS2 first, then:
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-tools
```

### Using vcpkg
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install qt6-base:x64-windows
```

## Verification

After installation, verify Qt6 is available:

```bash
# Check if qmake is available
qmake --version

# Check if Qt6 can be found by CMake
cd build
cmake .. -G "MinGW Makefiles"
```

You should see: `Qt6 found - GUI version will be built`

## Building GUI After Qt6 Installation

Once Qt6 is installed:

```bash
# Clean previous build
./build.sh clean

# Build GUI version
./build.sh all
```

## Troubleshooting

### Issue: "Qt6Config.cmake not found"
**Solution:** Ensure CMAKE_PREFIX_PATH points to your Qt installation directory.

### Issue: "MinGW not compatible"
**Solution:** Use the Qt-provided MinGW or ensure versions match.

### Issue: "Permission denied"
**Solution:** Run installer/terminal as Administrator.

### Issue: Build works but GUI doesn't start
**Solution:** Ensure Qt DLLs are in PATH or copy them to build directory.

## File Size Expectations

- **CLI Version**: ~2-5 MB
- **GUI Version**: ~15-25 MB (includes Qt dependencies)

## Alternative: Portable Qt Installation

For a minimal setup, you can:
1. Install Qt6 on another machine
2. Copy the Qt6 installation folder
3. Set CMAKE_PREFIX_PATH to the copied location

This is useful if you can't use the online installer.

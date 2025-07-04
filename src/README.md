# Sentinel IOC Management System

A powerful C++ application for managing Indicators of Compromise (IOCs) with both CLI and GUI interfaces featuring a sleek terminal/matrix theme.

## 🚀 Quick Start

### Prerequisites
- **MinGW-w64** compiler with g++
- **CMake** (version 3.16+)
- **Qt6** (for GUI version only)

### Running the Application

**GUI Version (Recommended):**
```bash
# Windows
build.bat

# Linux/macOS
./build.sh
```

**CLI Version:**
```bash
# Windows
build.bat cli

# Linux/macOS
./build.sh cli
```

**Clean Build:**
```bash
# Windows
build.bat clean

# Linux/macOS
./build.sh clean
```

## 📋 Features

### GUI Features
- **Matrix-themed interface** with terminal aesthetics
- **IOC Management**: Add, edit, delete, and search IOCs
- **Statistics Dashboard** with real-time analytics
- **Advanced Search** with filters and detailed results
- **Data Export/Import** capabilities
- **System Log** tracking all operations

### CLI Features
- Interactive command-line interface
- Full CRUD operations for IOCs
- Search and filter capabilities
- Data persistence and file operations

### Supported IOC Types
- **Hashes**: MD5, SHA1, SHA256, SHA512
- **IP Addresses**: IPv4 with geolocation data
- **URLs**: Malicious websites with protocol analysis

## 🎯 Usage Examples

**Adding IOCs:**
- GUI: Use the "◈ ADD IOC" button in the control panel
- CLI: Select "Add IOC" from the main menu

**Searching:**
- GUI: Click "◈ SEARCH IOC" for advanced filtering
- CLI: Use the search functionality in the menu

**Statistics:**
- GUI: Access "◈ STATISTICS" for visual analytics
- CLI: View statistics from the main menu

## 📁 Data Storage
IOC data is stored in `data/ioc.csv` and automatically managed by the application.

## 🛠️ Manual Build (Advanced)
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

For GUI support, ensure Qt6 is in your PATH or specify:
```bash
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
```

### GUI Version
```bash
cd build
./SentinelGUI
```

## 🔧 Troubleshooting

**GUI won't start - Qt6 DLL error:**
```bash
# Windows: Use the run script that handles Qt6 PATH automatically
run-gui.bat

# Linux/macOS: Use the shell script
./run-gui.sh

# Manual Qt6 PATH setup (if needed):
# Windows:
set PATH=C:\Qt\6.9.1\mingw_64\bin;%PATH%

# Linux/macOS:
export PATH="/c/Qt/6.9.1/mingw_64/bin:$PATH"
```

**Build fails:**
- Ensure MinGW-w64 and CMake are installed
- For GUI: Install Qt6 (the scripts auto-detect common Qt6 locations)
- Check that all tools are in your system PATH

**Qt6 Auto-Detection:**
The build scripts automatically detect Qt6 installations at:
- `/c/Qt/6.9.1/mingw_64/bin`
- `/c/Qt/6.8.0/mingw_64/bin` 
- `/c/Qt/6.7.0/mingw_64/bin`

## 📄 License
MIT License - See LICENSE file for details.

## 🤝 Contributing
Contributions welcome! Please read the contributing guidelines before submitting PRs.

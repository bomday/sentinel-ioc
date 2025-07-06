cmake_minimum_required(VERSION 3.16)

project(SentinelIOC VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find Qt6 (optional for GUI)
find_package(Qt6 QUIET COMPONENTS Core Widgets Gui)

# Check if Qt6 was found
if(Qt6_FOUND)
    message(STATUS "Qt6 found - GUI version will be built")
    set(BUILD_GUI TRUE CACHE BOOL "Build GUI version" FORCE)
else()
    message(WARNING "Qt6 not found - Only CLI version will be built")
    message(STATUS "To build GUI version, install Qt6 and ensure it's in CMAKE_PREFIX_PATH")
    set(BUILD_GUI FALSE CACHE BOOL "Build GUI version" FORCE)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/indicator
    ${CMAKE_CURRENT_SOURCE_DIR}/maliciousHash
    ${CMAKE_CURRENT_SOURCE_DIR}/maliciousIP
    ${CMAKE_CURRENT_SOURCE_DIR}/maliciousURL
    ${CMAKE_CURRENT_SOURCE_DIR}/indicatorManager
    ${CMAKE_CURRENT_SOURCE_DIR}/fileManager
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/CLI
)

# Core source files (used by both CLI and GUI)
set(CORE_SOURCES
    indicator/indicator.cpp
    maliciousHash/maliciousHash.cpp
    maliciousIP/maliciousIP.cpp
    maliciousURL/maliciousURL.cpp
    indicatorManager/indicatorManager.cpp
    fileManager/fileManager.cpp
    utils/utils.cpp
)

set(CORE_HEADERS
    indicator/indicator.hpp
    maliciousHash/maliciousHash.hpp
    maliciousIP/maliciousIP.hpp
    maliciousURL/maliciousURL.hpp
    indicatorManager/indicatorManager.hpp
    fileManager/fileManager.hpp
    utils/utils.hpp
)

# Set output directory to src root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Always build CLI version
add_executable(SentinelIOC-CLI
    ${CORE_SOURCES}
    CLI/cli.cpp
    main.cpp
)

# Conditionally build GUI version if Qt6 is available
if(BUILD_GUI)
    # Qt specific settings
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    # Add GUI include directory
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gui)
    
    # GUI specific sources
    set(GUI_SOURCES
        ${CORE_SOURCES}
        gui/mainwindow.cpp
        gui/matrixwidget.cpp
        gui/matrixtitle.cpp
        gui/ioctablewidget.cpp
        gui/addiocdialog.cpp
        gui/searchdialog.cpp
        gui/statisticsdialog.cpp
        gui/matrixsplash.cpp
        main_gui.cpp
    )
    
    set(GUI_HEADERS
        ${CORE_HEADERS}
        gui/mainwindow.h
        gui/matrixwidget.h
        gui/matrixtitle.h
        gui/ioctablewidget.h
        gui/addiocdialog.h
        gui/searchdialog.h
        gui/statisticsdialog.h
        gui/matrixsplash.h
    )
    
    # Create GUI executable
    add_executable(SentinelIOC-GUI ${GUI_SOURCES} ${GUI_HEADERS})
    
    # Link Qt libraries to GUI
    target_link_libraries(SentinelIOC-GUI Qt6::Core Qt6::Widgets Qt6::Gui)
    
    message(STATUS "GUI executable 'SentinelIOC-GUI' will be built")
else()
    message(STATUS "GUI executable will NOT be built (Qt6 not found)")
    message(STATUS "Only CLI executable 'SentinelIOC-CLI' will be built")
endif()

# Copy data directory to build folder for compatibility
add_custom_target(copy_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
    COMMENT "Copying data directory to build folder"
)

cmake_minimum_required(VERSION 3.16)

project(SentinelIOC VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Qt specific settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
)

# Source files
set(SOURCES
    # Core classes
    indicator/indicator.cpp
    maliciousHash/maliciousHash.cpp
    maliciousIP/maliciousIP.cpp
    maliciousURL/maliciousURL.cpp
    indicatorManager/indicatorManager.cpp
    fileManager/fileManager.cpp
    utils/utils.cpp
    
    # GUI classes
    gui/mainwindow.cpp
    gui/matrixwidget.cpp
    gui/ioctablewidget.cpp
    gui/hackerterminal.cpp
    gui/addiocdialog.cpp
    gui/searchdialog.cpp
    gui/statisticsdialog.cpp
    
    # Main
    main_gui.cpp
)

set(HEADERS
    indicator/indicator.hpp
    maliciousHash/maliciousHash.hpp
    maliciousIP/maliciousIP.hpp
    maliciousURL/maliciousURL.hpp
    indicatorManager/indicatorManager.hpp
    fileManager/fileManager.hpp
    utils/utils.hpp
    
    gui/mainwindow.h
    gui/matrixwidget.h
    gui/ioctablewidget.h
    gui/hackerterminal.h
    gui/addiocdialog.h
    gui/searchdialog.h
    gui/statisticsdialog.h
)

# Create executable
add_executable(SentinelIOC-GUI ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(SentinelIOC-GUI Qt6::Core Qt6::Widgets Qt6::Gui)

# CLI version
add_executable(SentinelIOC-CLI
    indicator/indicator.cpp
    maliciousHash/maliciousHash.cpp
    maliciousIP/maliciousIP.cpp
    maliciousURL/maliciousURL.cpp
    indicatorManager/indicatorManager.cpp
    fileManager/fileManager.cpp
    utils/utils.cpp
    CLI/cli.cpp
    main.cpp
)

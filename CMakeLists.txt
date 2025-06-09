cmake_minimum_required(VERSION 3.29)
project(sentinel_ioc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(src/analyzerIOC)
include_directories(src/fileManager)
include_directories(src/indicator)
include_directories(src/maliciousHash)
include_directories(src/maliciousIP)
include_directories(src/maliciousURL)
include_directories(src/utils)

add_executable(sentinel_ioc
    src/analyzerIOC/analyzerIOC.cpp
    src/analyzerIOC/analyzerIOC.hpp
    src/fileManager/FileManager.cpp
    src/fileManager/FileManager.hpp
    src/indicator/indicator.cpp
    src/indicator/indicator.hpp
    src/maliciousHash/maliciousHash.cpp
    src/maliciousHash/maliciousHash.hpp
    src/maliciousIP/maliciousIP.cpp
    src/maliciousIP/maliciousIP.hpp
    src/maliciousURL/maliciousURL.cpp
    src/maliciousURL/maliciousURL.hpp
    src/utils/utils.cpp
    src/utils/utils.hpp
    src/main.cpp)

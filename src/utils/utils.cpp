#include "utils.hpp"
#include <sys/stat.h>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#endif

void ensureFileAndDirectoryExist(const std::string& filepath) {
    std::string folder = "data";

    struct stat info;
    if (stat(folder.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        mkdir(folder.c_str());
        std::cout << "Created folder: " << folder << "\n";
    }

    std::ifstream infile(filepath);
    if (!infile.good()) {
        std::ofstream f(filepath);
        if (f.is_open()) {
            f.close();
            std::cout << "Created file: " << filepath << "\n";
        } else {
            std::cerr << "❌ Failed to create file: " << filepath << "\n";
        }
    }
}

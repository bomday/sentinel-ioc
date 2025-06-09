#include <vector>
#include "FileManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

bool FileManager::saveIOCs(const std::vector<std::unique_ptr<Indicator>>& list, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
        return false;
    }

    for (const auto& ioc : list) {
        file << ioc->getType() << ";"
             << ioc->getValue() << ";"
             << ioc->getSeverity() << ";"
             << ioc->getOrigin() << ";"
             << ioc->getTimestamp() << ";"
             << ioc->getDescription() << "\n";
    }

    file.close();
    return true;
}

std::vector<std::unique_ptr<Indicator>> FileManager::loadIOCs(const std::string& filename) {
    std::vector<std::unique_ptr<Indicator>> list;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << "\n";
        return list;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::unique_ptr<Indicator> ioc = Indicator::fromLine(line);
        if (ioc) {
            list.push_back(std::move(ioc));
        }
    }

    file.close();
    return list;
}

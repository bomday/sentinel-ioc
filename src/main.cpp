#include "utils.hpp"
#include "FileManager.hpp"
#include <iostream>
#include <memory>
#include "maliciousIP.hpp"

int main() {
    const std::string path = "data/iocs.txt";

    ensureFileAndDirectoryExist(path);

    std::vector<std::unique_ptr<Indicator>> iocs = FileManager::loadIOCs(path);

    std::cout << "\nLoaded " << iocs.size() << " IOCs:\n";
    for (const auto& ioc : iocs) {
        ioc->printInfo();
        std::cout << "------------------------\n";
    }

    std::cout << "\nAdding test IOC...\n";
    iocs.push_back(std::make_unique<MaliciousIP>(
        "8.8.4.4", "Low", "USA", "2025-06-08 23:30", "Test DNS IP"
    ));

    if (FileManager::saveIOCs(iocs, path)) {
        std::cout << "Saved IOCs back to file successfully.\n";
    } else {
        std::cout << "Failed to save IOCs.\n";
    }

    return 0;
}

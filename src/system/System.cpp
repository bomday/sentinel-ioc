#include "System.hpp"
#include "../fileManager/FileManager.hpp"
#include <iostream>

/**
 * Loads IOC entries from the specified file into the internal vector.
 */
void System::loadIOCsFromFile(const std::string& filename) {
    FileManager fm;
    iocList = fm.loadIOCs(filename);
}

/**
 * Prints all IOCs to the terminal with formatted output.
 * If the list is empty, notifies the user.
 */
void System::listIOCs() const {
    if (iocList.empty()) {
        std::cout << "No IOCs registered.\n";
        return;
    }

    std::cout << "\nRegistered IOCs:\n";
    std::cout << "------------------------------\n";

    int index = 1;
    for (const auto& ioc : iocList) {
        std::cout << "IOC #" << index++ << "\n";
        std::cout << "Type:       " << ioc->getType() << "\n";
        std::cout << "Value:      " << ioc->getValue() << "\n";
        std::cout << "Severity:   " << ioc->getSeverity() << "\n";
        std::cout << "Source:     " << ioc->getOrigin() << "\n";
        std::cout << "Date:       " << ioc->getTimestamp() << "\n";
        std::cout << "Description:" << ioc->getDescription() << "\n";
        std::cout << "------------------------------\n";
    }
}

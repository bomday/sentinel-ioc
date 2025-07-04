#include "cli.hpp"
#include "../utils/utils.hpp"
#include "../indicator/indicator.hpp"
#include "../maliciousHash/maliciousHash.hpp"
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"

#include <iostream>
#include <limits>
#include <climits>

CLI::CLI() : wasModified(false) {
    // Load indicators from file on startup
    manager.loadIndicatorsFromFile("data/ioc.csv");
    std::cout << "[DEBUG] Load Complete.\n";
}

void CLI::runMenu() {
    std::cout << "--- Welcome to the Sentinel IOC Management System ---\n";
    
    int optionNumber;
    do {
        showMenu();
        optionNumber = getValidIntInput("Enter your option: ", 0, 6);
        
        switch (optionNumber) {
            case 1:
                handleAdd();
                wasModified = true;
                break;
            case 2:
                handleList();
                break;
            case 3:
                handleSearch();
                break;
            case 4:
                handleEdit();
                wasModified = true;
                break;
            case 5:
                handleDelete();
                wasModified = true;
                break;
            case 6:
                handleStatistics();
                break;
            case 0:
                std::cout << "\nExiting the Sentinel IOC Management System. Goodbye!\n";
                break;
        }
        
        std::cout << "\n";
    } while (optionNumber != 0);
    
    // Save data if modified
    if (wasModified) {
        manager.saveIndicatorsToFile("data/ioc.csv");
        std::cout << "[DEBUG] Save complete.\n";
    } else {
        std::cout << "[DEBUG] No changes made. Skipping save.\n";
    }
}

void CLI::showMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Create New IOC\n";
    std::cout << "2. List All IOCs\n";
    std::cout << "3. Search IOC\n";
    std::cout << "4. Edit IOC\n";
    std::cout << "5. Remove IOC\n";
    std::cout << "6. Access statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "-----------------\n\n";
}

void CLI::handleAdd() {
    std::cout << "\n=== Create New IOC ===\n";
    
    // Get IOC type
    std::string type;
    while (true) {
        type = getStringInput("Enter the type of IOC (IP, URL or Hash): ");
        if (type == "IP" || type == "URL" || type == "Hash") {
            break;
        }
        std::cout << "Invalid type. Please enter IP, URL, or Hash.\n";
    }
    
    // Get severity
    int severity = getValidIntInput("Enter the severity (1-5): ", 1, 5);
    
    // Get description and origin
    std::string description = getStringInput("Enter the description: ");
    std::string origin = getStringInput("Enter the origin: ");
    
    // Generate timestamp and ID
    std::string timestamp = getTimestamp();
    
    // Create type-specific IOC
    if (type == "IP") {
        std::string ip = getStringInput("Enter the IP address: ");
        std::string country = getStringInput("Enter the country: ");
        std::string isp = getStringInput("Enter the ISP: ");
        
        manager.addMaliciousIP(severity, type, description, origin, timestamp, ip, country, isp);
    } else if (type == "URL") {
        std::string url = getStringInput("Enter the URL: ");
        std::string protocol = getStringInput("Enter the protocol (HTTP/HTTPS): ");
        
        manager.addMaliciousURL(severity, type, description, origin, timestamp, url, protocol);
    } else if (type == "Hash") {
        std::string hash = getStringInput("Enter the hash value: ");
        std::string algorithm = getStringInput("Enter the algorithm (MD5/SHA1/SHA256): ");
        
        manager.addMaliciousHash(severity, type, description, origin, timestamp, hash, algorithm);
    }

    std::cout << "IOC created successfully!\n";
}

void CLI::handleList() {
    std::cout << "\n=== List All IOCs ===\n";
    manager.listIndicators();
}

void CLI::handleSearch() {
    std::cout << "\n=== Search IOC ===\n";
    displaySearchMenu();
    
    int option = getValidIntInput("Choose a criterion: ", 1, 6);
    
    switch (option) {
        case 1: {
            std::string value = getStringInput("Enter the IOC value: ");
            manager.searchByValue(value);
            break;
        }
        case 2: {
            std::string type = getStringInput("Enter the IOC type: ");
            manager.searchByType(type);
            break;
        }
        case 3: {
            int severity = getValidIntInput("Enter the severity (1-5): ", 1, 5);
            manager.searchBySeverity(severity);
            break;
        }
        case 4: {
            std::string origin = getStringInput("Enter the IOC origin: ");
            manager.searchByOrigin(origin);
            break;
        }
        case 5: {
            std::string date = getStringInput("Enter the IOC date (DD-MM-YYYY): ");
            manager.searchByDate(date);
            break;
        }
        case 6: {
            std::string description = getStringInput("Enter the IOC description: ");
            manager.searchByDescription(description);
            break;
        }
    }
}

void CLI::handleEdit() {
    std::cout << "\n=== Edit IOC ===\n";
    
    if (manager.isEmpty()) {
        std::cout << "No IOCs available to edit.\n";
        return;
    }
    
    int id = getValidIntInput("Enter the ID of the IOC to edit: ");
    
    // Get the indicator to edit
    Indicator* iocToEdit = manager.findIndicatorById(id);
    if (!iocToEdit) {
        std::cout << "No indicator found with ID " << id << ".\n";
        return;
    }
    
    int option;
    bool wasEdited = false;
    
    do {
        std::cout << "\n--- Editing IOC ID: " << iocToEdit->getIndicatorId() 
                  << " (" << iocToEdit->getType() << ") ---\n";
        std::cout << "Current Description: " << iocToEdit->getDescription() << std::endl;
        std::cout << "-------------------------------------------\n";
        
        displayEditMenu(iocToEdit->getType());
        
        option = getValidIntInput("Choose an option: ", 0, 6);
        
        switch (option) {
            case 1: {
                int newSeverity = getValidIntInput("Enter new severity (1-5): ", 1, 5);
                manager.updateIndicatorSeverity(id, newSeverity);
                std::cout << "Severity updated!\n";
                wasEdited = true;
                break;
            }
            case 2: {
                std::string newDescription = getStringInput("Enter new description: ");
                manager.updateIndicatorDescription(id, newDescription);
                std::cout << "Description updated!\n";
                wasEdited = true;
                break;
            }
            case 3: {
                std::string newOrigin = getStringInput("Enter new origin: ");
                manager.updateIndicatorOrigin(id, newOrigin);
                std::cout << "Origin updated!\n";
                wasEdited = true;
                break;
            }
            case 4: {
                if (iocToEdit->getType() == "IP") {
                    std::string newIP = getStringInput("Enter new IP address: ");
                    manager.updateMaliciousIPAddress(id, newIP);
                } else if (iocToEdit->getType() == "URL") {
                    std::string newURL = getStringInput("Enter new URL: ");
                    manager.updateMaliciousURL(id, newURL);
                } else if (iocToEdit->getType() == "Hash") {
                    std::string newHash = getStringInput("Enter new hash: ");
                    manager.updateMaliciousHash(id, newHash);
                }
                std::cout << "Field updated!\n";
                wasEdited = true;
                break;
            }
            case 5: {
                if (iocToEdit->getType() == "IP") {
                    std::string newCountry = getStringInput("Enter new country: ");
                    manager.updateMaliciousIPCountry(id, newCountry);
                } else if (iocToEdit->getType() == "URL") {
                    std::string newProtocol = getStringInput("Enter new protocol: ");
                    manager.updateMaliciousURLProtocol(id, newProtocol);
                } else if (iocToEdit->getType() == "Hash") {
                    std::string newAlgorithm = getStringInput("Enter new algorithm: ");
                    manager.updateMaliciousHashAlgorithm(id, newAlgorithm);
                }
                std::cout << "Field updated!\n";
                wasEdited = true;
                break;
            }
            case 6: {
                if (iocToEdit->getType() == "IP") {
                    std::string newISP = getStringInput("Enter new ISP: ");
                    manager.updateMaliciousIPISP(id, newISP);
                    std::cout << "ISP updated!\n";
                    wasEdited = true;
                }
                break;
            }
            case 0:
                std::cout << "Finished editing IOC ID: " << id << std::endl;
                break;
        }
    } while (option != 0);
    
    if (wasEdited) {
        manager.updateIndicatorTimestamp(id, getTimestamp());
    } else {
        std::cout << "No changes were made to the IOC.\n";
    }
}

void CLI::handleDelete() {
    std::cout << "\n=== Remove IOC ===\n";
    
    if (manager.isEmpty()) {
        std::cout << "No IOCs available to remove.\n";
        return;
    }
    
    int id = getValidIntInput("Enter the ID of the IOC to remove: ");
    
    if (!manager.findIndicatorById(id)) {
        std::cout << "No indicator found with ID " << id << ".\n";
        return;
    }
    
    if (getConfirmation("Are you sure you want to remove the indicator with ID " + std::to_string(id) + "? (y/n): ")) {
        if (manager.removeIndicatorById(id)) {
            std::cout << "Indicator with ID " << id << " was successfully removed.\n";
        } else {
            std::cout << "Failed to remove indicator with ID " << id << ".\n";
        }
    } else {
        std::cout << "Removal cancelled.\n";
    }
}

void CLI::handleStatistics() {
    std::cout << "\n=== IOC Statistics ===\n";
    manager.generateStatistics();
}

// Helper methods for input validation and collection
int CLI::getValidIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail() || value < min || value > max) {
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string CLI::getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

bool CLI::getConfirmation(const std::string& prompt) {
    char confirmation;
    std::cout << prompt;
    std::cin >> confirmation;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return (confirmation == 'y' || confirmation == 'Y');
}

void CLI::displayEditMenu(const std::string& type) {
    std::cout << "1. Edit Severity\n";
    std::cout << "2. Edit Description\n";
    std::cout << "3. Edit Origin\n";
    
    if (type == "IP") {
        std::cout << "4. Edit IP Address\n";
        std::cout << "5. Edit Country\n";
        std::cout << "6. Edit ISP\n";
    } else if (type == "URL") {
        std::cout << "4. Edit URL\n";
        std::cout << "5. Edit Protocol\n";
    } else if (type == "Hash") {
        std::cout << "4. Edit Hash\n";
        std::cout << "5. Edit Algorithm\n";
    }
    
    std::cout << "0. Finish Editing\n";
    std::cout << "-------------------------------------------\n";
}

void CLI::displaySearchMenu() {
    std::cout << "1 - By value\n";
    std::cout << "2 - By type\n";
    std::cout << "3 - By severity\n";
    std::cout << "4 - By origin\n";
    std::cout << "5 - By date\n";
    std::cout << "6 - By description\n";
}

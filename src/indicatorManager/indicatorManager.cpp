#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <ctime>   
 
#include "indicatorManager.hpp"
#include "indicator.hpp"
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "maliciousHash.hpp"
#include "utils.hpp"

// Indicator creation
void IndicatorManager::createIndicator() {
    int severity;
    std::string type, description, origin, timestamp;

    while (true) {
        std::cout << "\nEnter the type of IOC (IP, URL or Hash): ";
        std::getline(std::cin, type);

        if (type == "IP" || type == "URL" || type == "Hash") {
            break;
        } else {
            std::cout << "\nInvalid type. Please enter IP, URL or Hash.";
        }
    }

    while (true) {
        std::cout << "\nNow enter the severity (1 - 5): ";
        std::cin >> severity;

        if (std::cin.fail() || severity < 1 || severity > 5) {
            std::cout << "\nInvalid severity. Please enter a value between 1 and 5.\n";
            std::cin.clear(); 

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break; 
        }
    }

    std::cout << "\nEnter the origin: ";
    std::getline(std::cin, origin);

    timestamp = getTimestamp();
    
    std::cout << "\nEnter the description: ";
    std::getline(std::cin, description);

    int indicatorId = generateUniqueId();

    Indicator* newIndicator = nullptr;

    // Create a MaliciousIP object if the type is IP
    if (type == "IP") {
        newIndicator = MaliciousIP::createMaliciousIP(indicatorId, severity, type, description, origin, timestamp);

    // Create a MaliciousURL object if the type is URL
    } else if (type == "URL") {
        newIndicator = MaliciousURL::createMaliciousURL(indicatorId, severity, type, description, origin, timestamp);

    // Create a MaliciousHash object if the type is Hash
    } else if (type == "Hash") {
        newIndicator = MaliciousHash::createMaliciousHash(indicatorId, severity, type, description, origin, timestamp);
    }

    indicators.push_back(std::unique_ptr<Indicator>(newIndicator));
    
    std::cout << "\nIndicator created successfully with ID: " << indicatorId << std::endl;
    std::cout << "-------------------------------------------\n";
}

// Helper function to print type-specific details for an indicator
static void printSpecificIndicatorDetails(const Indicator* ioc) {
    std::string hash_val = "";
    std::string algorithm_val = "";
    std::string ip_val = "";
    std::string country_val = "";
    std::string isp_val = "";
    std::string url_val = "";
    std::string protocol_val = "";

    if (const MaliciousHash* mh = dynamic_cast<const MaliciousHash*>(ioc)) {
        hash_val = mh->getHash();
        algorithm_val = mh->getAlgorithm();
    } else if (const MaliciousIP* mip = dynamic_cast<const MaliciousIP*>(ioc)) {
        ip_val = mip->getIP();
        country_val = mip->getCountry();
        isp_val = mip->getISP();
    } else if (const MaliciousURL* murl = dynamic_cast<const MaliciousURL*>(ioc)) {
        url_val = murl->getURL();
        protocol_val = murl->getProtocol();
    }

    std::cout << std::setw(65) << hash_val
              << std::setw(10) << algorithm_val
              << std::setw(16) << ip_val
              << std::setw(16) << country_val
              << std::setw(21) << isp_val
              << std::setw(51) << url_val
              << std::setw(10) << protocol_val;
}

// Function to list all registered Indicators
// Displays detailed information about each IOC in a formatted, readable way.
void IndicatorManager::listIndicators() const {
    std::cout << "\n --- LISTING REGISTERED IOCs --- \n";
    std::cout << "-------------------------------------------\n";

    // Check if the list of IOCs is empty.
    if (indicators.empty()) {
        std::cout << " No IOCs registered yet.\n";
        std::cout << "-------------------------------------------\n";
        return; // Exit the function if no IOCs are present
    }

    // If there are IOCs, iterate and display them

    // Table Header
    std::cout << std::left
              << std::setw(8)  << "indicatorId"
              << std::setw(12) << "Severity"
              << std::setw(10) << "Type"
              << std::setw(40) << "Description"
              << std::setw(15) << "Origin"
              << std::setw(20) << "Timestamp"
              << std::setw(65) << "Hash"
              << std::setw(10) << "Algorithm"
              << std::setw(16) << "IP"
              << std::setw(16) << "Country"
              << std::setw(21) << "ISP"
              << std::setw(51) << "URL"
              << std::setw(10) << "Protocol"
              << std::endl;
    std::cout << std::string(150, '-') << std::endl; // Separator line for the header

    // Iterate over pointers in the vector
    for (const auto& ioc : indicators) { // Display properties in a formatted, tabular way
        std::cout << std::left
                  << std::setw(8)  << ioc->getIndicatorId()
                  << std::setw(12) << ioc->getSeverity()
                  << std::setw(10) << ioc->getType()
                  << std::setw(40) << ioc->getDescription()
                  << std::setw(15) << ioc->getOrigin()
                  << std::setw(20) << ioc->getTimestamp();

        // Call the helper function to print type-specific details
        printSpecificIndicatorDetails(ioc.get()); 
                  
        std::cout << std::endl;

        // Visually separate each IOC with a line.
        std::cout << std::string(150, '-') << std::endl;
    }

    std::cout << "\nIOC listing complete.\n";
    std::cout << "-------------------------------------------\n";
}

// Method to edit an indicator by its ID
void IndicatorManager::editIndicatorById(int id) {
    if (indicators.empty()) {
        std::cout << "\nNo IOCs to edit. The list is empty.\n";
        return; 
    }

    // Find the indicator with the given ID
    auto indicatorToEdit = std::find_if(indicators.begin(), indicators.end(),
                                [id](const std::unique_ptr<Indicator>& indicator) {
                                    return indicator->getIndicatorId() == id;
                                });

    if (indicatorToEdit == indicators.end()) {
        std::cout << "\nNo indicator found with ID " << id << ".\n";
        return; 
    }

    Indicator* iocToEdit = indicatorToEdit->get();
    int option;
    bool wasEdited = false;

    do {
        std::cout << "\n--- Editing IOC ID: " << iocToEdit->getIndicatorId() << " (" << iocToEdit->getType() << ") ---\n";
        std::cout << "Current Description: " << iocToEdit->getDescription() << std::endl;
        std::cout << "-------------------------------------------\n";
        std::cout << "1. Edit Severity\n";
        std::cout << "2. Edit Description\n";
        std::cout << "3. Edit Origin\n";

        // Type-specific options using dynamic_cast
        if (MaliciousIP* ip = dynamic_cast<MaliciousIP*>(iocToEdit)) {
            std::cout << "4. Edit IP Address\n";
            std::cout << "5. Edit Country\n";
            std::cout << "6. Edit ISP\n";
        } else if (MaliciousURL* url = dynamic_cast<MaliciousURL*>(iocToEdit)) {
            std::cout << "4. Edit URL\n";
            std::cout << "5. Edit Protocol\n";
        } else if (MaliciousHash* hash = dynamic_cast<MaliciousHash*>(iocToEdit)) {
            std::cout << "4. Edit Hash\n";
            std::cout << "5. Edit Algorithm\n";
        }

        std::cout << "Enter 0 to Finish Editing\n";
        std::cout << "-------------------------------------------\n";
        std::cout << "Choose an option: ";

        std::cin >> option;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string newValue;

        switch (option) {
            case 1: {
                int newSeverity;
                while (true) {
                    std::cout << "Current severity: " << iocToEdit->getSeverity() << std::endl;
                    std::cout << "Enter new severity (1-5): ";
                    std::cin >> newSeverity;

                    if (!std::cin.fail() && newSeverity >= 1 && newSeverity <= 5) {
                        iocToEdit->setSeverity(newSeverity);
                        std::cout << "\nField updated!\n";
                        wasEdited = true;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    } else {
                        std::cout << "Invalid severity.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                break;
            }
            case 2:
                std::cout << "Current description: " << iocToEdit->getDescription() << std::endl;
                std::cout << "Enter new description: ";
                std::getline(std::cin, newValue);
                iocToEdit->setDescription(newValue);
                std::cout << "\nField updated!\n";
                wasEdited = true; 
                break;
            case 3:
                std::cout << "Current origin: " << iocToEdit->getOrigin() << std::endl;
                std::cout << "Enter new origin: ";
                std::getline(std::cin, newValue);
                iocToEdit->setOrigin(newValue);
                std::cout << "\nField updated!\n";
                wasEdited = true;
                break;
            case 4: // Type-specific
                if (MaliciousIP* ip = dynamic_cast<MaliciousIP*>(iocToEdit)) {
                    std::cout << "Current IP Address: " << ip->getIP() << std::endl;
                    std::cout << "Enter new IP Address: "; 
                    std::getline(std::cin, newValue); 
                    ip->setIP(newValue);
                    std::cout << "\nField updated!\n"; // Confirmation message just if the dynamic_cast is successful
                    wasEdited = true;
                } else if (MaliciousURL* url = dynamic_cast<MaliciousURL*>(iocToEdit)) {
                    std::cout << "Current URL: " << url->getURL() << std::endl;
                    std::cout << "Enter new URL: "; 
                    std::getline(std::cin, newValue); 
                    url->setURL(newValue);
                    std::cout << "\nField updated!\n"; // Confirmation message just if the dynamic_cast is successful
                    wasEdited = true;
                } else if (MaliciousHash* hash = dynamic_cast<MaliciousHash*>(iocToEdit)) {
                    std::cout << "Current Hash: " << hash->getHash() << std::endl;
                    std::cout << "Enter new Hash: "; 
                    std::getline(std::cin, newValue); 
                    hash->setHash(newValue);
                    std::cout << "\nField updated!\n"; // Confirmation message just if the dynamic_cast is successful
                    wasEdited = true;
                }
                break;
            case 5: // Type-specific
                if (MaliciousIP* ip = dynamic_cast<MaliciousIP*>(iocToEdit)) {
                    std::cout << "Current Country: " << ip->getCountry() << std::endl;
                    std::cout << "Enter new Country: "; 
                    std::getline(std::cin, newValue); 
                    ip->setCountry(newValue);
                    std::cout << "\nField updated!\n"; // Confirmation message just if the dynamic_cast is successful
                    wasEdited = true;
                } else if (MaliciousURL* url = dynamic_cast<MaliciousURL*>(iocToEdit)) {
                    std::cout << "Current Protocol: " << url->getProtocol() << std::endl;
                    std::cout << "Enter new Protocol: "; 
                    std::getline(std::cin, newValue); 
                    url->setProtocol(newValue);
                    std::cout << "\nField updated!\n"; // Confirmation message just if the dynamic_cast is successful
                    wasEdited = true;
                } else if (MaliciousHash* hash = dynamic_cast<MaliciousHash*>(iocToEdit)) {
                    std::cout << "Current Algorithm: " << hash->getAlgorithm() << std::endl;
                    std::cout << "Enter new Algorithm: "; 
                    std::getline(std::cin, newValue); 
                    hash->setAlgorithm(newValue);
                    std::cout << "\nField updated!\n"; // Confirmation message just if the dynamic_cast is successful
                    wasEdited = true;   
                }
                break;
            case 6: // IP-specific
                if (MaliciousIP* ip = dynamic_cast<MaliciousIP*>(iocToEdit)) {
                    std::cout << "Current ISP: " << ip->getISP() << std::endl;
                    std::cout << "Enter new ISP: "; 
                    std::getline(std::cin, newValue); 
                    ip->setISP(newValue);
                    std::cout << "\nField updated!\n";
                    wasEdited = true;
                }
                break;
            case 0:
                std::cout << "Finished editing IOC ID: " << id << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (option != 0);

    if (!wasEdited) {
        std::cout << "No changes were made to the IOC.\n";
        return; 
    }
    // If any field was edited, update the timestamp
    std::string timeEdited = getTimestamp();
    iocToEdit->setTimestamp(timeEdited);
}

// Method to remove an indicator by its ID
void IndicatorManager::removeIndicatorById(int id) {
    if (indicators.empty()) {
        std::cout << "\nNo IOCs to remove. The list is empty.\n";
        return;
    }

    //verify if want to remove an indicator
    std::cout << "\nAre you sure you want to remove the indicator with ID " << id << "? (y/n): ";
    char confirmation;
    std::cin >> confirmation;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirmation != 'y' && confirmation != 'Y') {
        std::cout << "\nRemoval cancelled.\n";
        return; 
    }

    auto indicatorToRemove = std::remove_if(indicators.begin(), indicators.end(),
                                [id](const std::unique_ptr<Indicator>& indicator) {
                                    return indicator->getIndicatorId() == id;
                                });
    if (indicatorToRemove != indicators.end()) {
        indicators.erase(indicatorToRemove, indicators.end());
        std::cout << "\nIndicator with ID " << id << " has been removed successfully.\n";
    } else {
        std::cout << "\nNo indicator found with ID " << id << ".\n";
    }
}

void IndicatorManager::searchIndicator() const {
    int option;

    std::cout << "=== Search IOC ===\n";
    std::cout << "1 - By value\n";
    std::cout << "2 - By type\n";
    std::cout << "3 - By severity\n";
    std::cout << "4 - By origin\n";
    std::cout << "5 - By date\n";
    std::cout << "6 - By description\n";
    std::cout << "Choose a criterion: ";
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool found = false;
    int resultCount = 0;

    switch (option) {
        case 1: {
            std::string value;
            std::cout << "Enter the IOC value: ";
            std::getline(std::cin, value);

            for (const auto& ioc : indicators) {
                if (icontains(ioc->getDescription(), value)) {
                    resultCount++;
                    printIOC(ioc.get(), resultCount);
                    found = true;
                }
            }
            break;
        }
        case 2: {
            std::string type;
            std::cout << "Enter the IOC type: ";
            std::getline(std::cin, type);

            for (const auto& ioc : indicators) {
                if (toLower(ioc->getType()) == toLower(type)) {
                    resultCount++;
                    printIOC(ioc.get(), resultCount);
                    found = true;
                }
            }
            break;
        }
        case 3: {
            int severity;
            while (true) {
                std::cout << "Enter the IOC severity (1-5): ";
                std::cin >> severity;

                if (std::cin.fail() || severity < 1 || severity > 5) {
                    std::cout << "\nInvalid severity! Please enter a number between 1 and 5.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
            }

            for (const auto& ioc : indicators) {
                if (ioc->getSeverity() == severity) {
                    resultCount++;
                    printIOC(ioc.get(), resultCount);
                    found = true;
                }
            }
            break;
        }
        case 4: {
            std::string origin;
            std::cout << "Enter the IOC origin: ";
            std::getline(std::cin, origin);

            for (const auto& ioc : indicators) {
                if (icontains(ioc->getOrigin(), origin)) {
                    resultCount++;
                    printIOC(ioc.get(), resultCount);
                    found = true;
                }
            }
            break;
        }
        case 5: {
            std::string date;
            std::cout << "Enter the IOC date (DD-MM-YYYY): ";
            std::getline(std::cin, date);

            for (const auto& ioc : indicators) {
                if (icontains(ioc->getTimestamp(), date)) {
                    resultCount++;
                    printIOC(ioc.get(), resultCount);
                    found = true;
                }
            }
            break;
        }
        case 6: {
            std::string description;
            std::cout << "Enter the IOC description: ";
            std::getline(std::cin, description);

            for (const auto& ioc : indicators) {
                if (icontains(ioc->getDescription(), description)) {
                    resultCount++;
                    printIOC(ioc.get(), resultCount);
                    found = true;
                }
            }
            break;
        }
        default:
            std::cout << "Invalid option!\n";
            return;
    }

    if (!found) {
        std::cout << "\nNo IOC found with the given criteria.\n";
    } else {
        std::cout << "\nSearch completed! " << resultCount << " IOC(s) found.\n";
    }
}

void IndicatorManager::printIOC(const Indicator* ioc, int index) const {
    std::cout << "\nIOC #" << index << "\n";
    std::cout << "ID         : " << ioc->getIndicatorId() << "\n";
    std::cout << "Type       : " << ioc->getType() << "\n";
    std::cout << "Severity   : " << ioc->getSeverity() << "\n";
    std::cout << "Origin     : " << ioc->getOrigin() << "\n";
    std::cout << "Date       : " << ioc->getTimestamp() << "\n";
    std::cout << "Description: " << ioc->getDescription() << "\n";
}

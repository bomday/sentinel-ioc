// Analysis class
#include "analyzerIOC.hpp"
#include <iostream>
#include "utils/utils.hpp"
#include <limits>
#include <memory>
#include "indicator.hpp"

// Accessing the global vector of unique_ptr to Indicators
extern std::vector<std::unique_ptr<Indicator>> indicators; // Call the global vector of unique pointers

// Method to search IOCs by criteria
void AnalyzerIOC::searchIOC() {
    int option;

    std::cout << "=== Search IOC ===\n";
    std::cout << "1 - By value teste\n";
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
                if (icontains(ioc->getDescription(), value)) { // Case-insensitive substring match
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
                if (toLower(ioc->getType()) == toLower(type)) { // Normalize strings for comparison
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

// Helper method to print a single IOC's details
void AnalyzerIOC::printIOC(const Indicator* ioc, int index) const {
    std::cout << "\nIOC #" << index << "\n";
    std::cout << "ID         : " << ioc->getIndicatorId() << "\n";
    std::cout << "Type       : " << ioc->getType() << "\n";
    std::cout << "Severity   : " << ioc->getSeverity() << "\n";
    std::cout << "Origin     : " << ioc->getOrigin() << "\n";
    std::cout << "Date       : " << ioc->getTimestamp() << "\n";
    std::cout << "Description: " << ioc->getDescription() << "\n";
}

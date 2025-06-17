#include <iostream>  
#include <vector>    
#include <limits>           

#include "IOCAnalyzer/IOCAnalyzer.hpp"
#include "IOCFactory/IOCFactory.hpp"
#include "utils/utils.hpp"

// Menu display function prototype
void showMenu(); // Assuming this is defined in utils.cpp and declared in utils.hpp

int main() {
    IOCAnalyzer iocAnalyzer;  // Object responsible for managing IOCs

    int optionNumber; // Stores user menu option

    std::cout << "--- Welcome to the Sentinel IOC Management System ---\n";

    do {
        showMenu();
        std::cout << "Enter your option: ";
        std::cin >> optionNumber;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (optionNumber) {
            case 1: {
                // Create new IOC using the IOCFactory
                std::unique_ptr<Indicator> newIoc = IOCFactory::createIOC();
                if (newIoc) {
                    iocAnalyzer.addIndicator(std::move(newIoc));
                }
                break;
            }

            case 2:
                iocAnalyzer.listIndicators();
                break;

            case 3:
                std::cout << "\nEdit IOC functionality not yet implemented.\n";
                break;

            case 4:
                std::cout << "\nRemove IOC functionality not yet implemented.\n";
                break;

            case 0:
                std::cout << "\nExiting the Sentinel IOC Management System. Goodbye!\n";
                break;

            default:
                std::cout << "\nInvalid option. Please try again.\n";
        }

        std::cout << "\n";

    } while (optionNumber != 0);

    return 0;
}

// --- Menu function (should be moved later to utils.cpp) ---
void showMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Create New IOC\n";
    std::cout << "2. List All IOCs\n";
    std::cout << "3. Edit IOC\n";
    std::cout << "4. Remove IOC\n";
    std::cout << "0. Exit\n";
    std::cout << "-----------------\n";
}

#include <iostream>  
#include <vector>   
#include <limits>  

#include "fileManager/fileManager.hpp"
#include "indicatorManager/indicatorManager.hpp"
#include "indicator/indicator.hpp"
#include "maliciousHash/maliciousHash.hpp"
#include "maliciousIP/maliciousIP.hpp"
#include "maliciousURL/maliciousURL.hpp"
#include "utils/utils.hpp"

int main() {
    int optionNumber; // Variable to store the user's menu option
    bool wasModified = false; // Flag to track if the data was changed
    IndicatorManager manager; // Create an instance of IndicatorManager to manage IOCs

    manager.loadIndicatorsFromFile("data/ioc.csv");
    std::cout << "[DEBUG] Load Complete.\n"; // Debug message to confirm loading of IOCs

    std::cout << "--- Welcome to the Sentinel IOC Management System ---\n";

    // Loop to display the menu and process user input
    do {
        showMenu(); // Call showMenu function to display the menu
        std::cout << "Enter your option: ";
        std::cin >> optionNumber;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (optionNumber) {
            case 1: {
                manager.createIndicator(); // Create a new IOC
                wasModified = true;
                break;
            }
            case 2: {
                manager.listIndicators(); // List all IOCs
                break;
            }
            case 3: {
                // Implement Listing IOCs here
                manager.searchIndicator(); 
                break;
            }
            case 4: {
                // Edit IOC
                int idToEdit;
                std::cout << "\nEnter the ID of the IOC to edit: ";
                std::cin >> idToEdit;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                manager.editIndicatorById(idToEdit);
                wasModified = true;
                break;
            }
            case 5: {
                // Remove IOC
                int idToRemove;
                std::cout << "\nEnter the ID of the IOC to remove: ";
                std::cin >> idToRemove;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                manager.removeIndicatorById(idToRemove);
                wasModified = true;
                break;
            }
            case 0: {
                std::cout << "\nExiting the Sentinel IOC Management System. Goodbye!\n";
                break;
            }
            default: {
                std::cin.clear(); // Clear any error flags
                std::cout << "\nInvalid option. Please try again.\n";
            }
        }

        std::cout << "\n"; // Add spacing after each operation
    } while (optionNumber != 0);

    if (wasModified) {
        manager.saveIndicatorsToFile("data/ioc.csv");
        std::cout << "[DEBUG] Save complete.\n";
    } else {
        std::cout << "[DEBUG] No changes made. Skipping save.\n";
    }

    return 0;
}

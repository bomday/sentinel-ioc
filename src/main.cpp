#include <iostream>  
#include <vector>   
#include <limits>  

#include "indicatorManager.hpp"
#include "indicator.hpp"      
#include "maliciousHash.hpp"   
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "utils.hpp"

int main() {
    int optionNumber; // Variable to store the user's menu option
    IndicatorManager manager; // Create an instance of IndicatorManager to manage IOCs

    std::cout << "--- Welcome to the Sentinel IOC Management System ---\n";

    // Loop to display the menu and process user input
    do {
        showMenu(); // Call showMenu function to display the menu
        std::cout << "Enter your option: ";
        std::cin >> optionNumber;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (optionNumber) {
            case 1: {
                // Create a new IOC
                manager.createIndicator(); // Call the function to create an indicator
                break;
            }
            case 2: {
                // Implement Listing IOCs here
                manager.listIndicators(); 
                break;
            }
            case 3: {
                // Edit IOC (placeholder)
                int idToEdit;
                std::cout << "\nEnter the ID of the IOC to edit: ";
                std::cin >> idToEdit;
                manager.editIndicatorById(idToEdit);
                break;
            }
            case 4: {
                // Remove IOC (placeholder)
                int idToRemove;
                std::cout << "\nEnter the ID of the IOC to remove: ";
                std::cin >> idToRemove;

                manager.removeIndicatorById(idToRemove);

                break;
            }
            case 0: {
                // Exit the program
                std::cout << "\nExiting the Sentinel IOC Management System. Goodbye!\n";
                break;
            }
            default: {
                // Handle invalid option
                std::cin.clear(); // Clear any error flags
                std::cout << "\nInvalid option. Please try again.\n";
            }
        }
        std::cout << "\n"; // Add a newline for better spacing after each operation
    } while (optionNumber != 0);

    return 0;
}
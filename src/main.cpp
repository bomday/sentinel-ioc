#include <iostream>  
#include <vector>   
#include "indicator.hpp"      
#include "maliciousHash.hpp"   
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "utils.hpp"
#include <limits>  

Indicator* createIndicator(); 

int main() {
    int optionNumber; // Variable to store the user's menu option

    std::cout << "--- Welcome to the Sentinel IOC Management System ---\n";

    // Loop to display the menu and process user input
    do {
        showMenu(); // Call showMenu function to display the menu
        std::cout << "Enter your option: ";
        std::cin >> optionNumber;
        // Important: Clear the input buffer after reading an integer,
        // especially if you're mixing std::cin >> var with std::getline later.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (optionNumber) {
            case 1:
                // When creating an indicator, ensure createIndicator() returns a dynamically
                // allocated Indicator* that AnalyzerIOC can manage.
                { // Use a block to declare a local variable `newIoc`
                    Indicator* newIoc = createIndicator(); // Call the function to create an indicator
                }
                break;
            case 2:
                // Implement Listing IOCs here
                listIndicators(); // Call the listIndicators method from your analyzer object
                break;
            case 3:
                // Edit IOC (placeholder)
                std::cout << "\nEdit IOC functionality not yet implemented.\n";
                break;
            case 4:
                // Remove IOC (placeholder)
                std::cout << "\nRemove IOC functionality not yet implemented.\n";
                break;
            case 0:
                std::cout << "\nExiting the Sentinel IOC Management System. Goodbye!\n";
                break;
            default:
                std::cout << "\nInvalid option. Please try again.\n";
        }
        std::cout << "\n"; // Add a newline for better spacing after each operation
    } while (optionNumber != 0);

    // The iocAnalyzer object will automatically call its destructor when main() exits,
    // which handles the deallocation of all Indicator objects.

    return 0;
}
#include <iostream>  // For std::cout, std::cin, std::endl
#include <vector>    // Not strictly needed in main.cpp if AnalyzerIOC handles the vector
#include "analyzerIOC.hpp"     // Include the AnalyzerIOC class
#include "indicator.hpp"       // Include the base Indicator class
#include "maliciousHash.hpp"   // Include derived classes for creating specific IOCs
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "utils.hpp"
#include <limits>           

// Function prototypes (declarations)
// If createIndicator() is defined in indicator.cpp (as per our previous discussion),
// its prototype should be here.
// If showMenu() is in utils.hpp/cpp, its prototype should be in utils.hpp.
void showMenu(); // Assuming this is defined in utils.cpp or similar
Indicator* createIndicator(); // Assuming this is defined in indicator.cpp

int main() {
    // Create an instance of AnalyzerIOC. This object will manage all your IOCs.
    analyzerIOC iocAnalyzer;

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
                    iocAnalyzer.addIndicator(newIoc); // Add the newly created IOC to the analyzer
                }
                break;
            case 2:
                // Implement Listing IOCs here
                iocAnalyzer.listIndicators(); // Call the listIndicators method from your analyzer object
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

// --- Placeholder for showMenu() if it's not in utils.cpp/hpp yet ---
// You should define showMenu() in utils.cpp and declare it in utils.hpp
void showMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Create New IOC\n";
    std::cout << "2. List All IOCs\n";
    std::cout << "3. Edit IOC\n";
    std::cout << "4. Remove IOC\n";
    std::cout << "0. Exit\n";
    std::cout << "-----------------\n";
}
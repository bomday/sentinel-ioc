#include <iostream>
#include <vector>
#include "analyzerIOC.hpp"
#include "indicator.hpp"
#include "maliciousHash.hpp"
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "utils.hpp"

Indicator* createIndicator();

int main() {
    int optionNumber; // Variable to store the user's menu option

    // Loop to display the menu and process user input
    do {
        showMenu();          // Call showMenu function to display the menu
        std::cin >> optionNumber;
        std::cin.ignore();

        switch (optionNumber) {
            case 1:
                createIndicator(); // Call the function to create an indicator
                break;
            case 2:
                // Search IOC
                break;
            case 3:
                // Edit IOC
                break;
            case 4:
                // Remove IOC
                break;
            case 0:
                std::cout << "\nSaindo...\n";
                break;
            default:
                std::cout << "\nOpção inválida.\n";
        }

    } while (optionNumber != 0);

    return 0;
}
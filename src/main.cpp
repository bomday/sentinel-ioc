#include "../System/System.hpp"
#include <iostream>
#include <limits>

int main() {
    System system;
    system.loadIOCsFromFile("src/data/iocs.txt");

    int option;
    do {
        std::cout << "\n===  MAIN MENU ===\n";
        std::cout << "1.  List IOCs\n";
        // std::cout << "2. Edit IOC\n";
        // std::cout << "3.  Remove IOC\n";
        std::cout << "0.  Exit\n";
        std::cout << "Choice: ";
        std::cin >> option;

        switch (option) {
            case 1:
                system.listIOCs();
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;

            // case 2: system.editIOC(); break;
            // case 3: system.removeIOC(); break;

            case 0:
                std::cout << "Exiting...\n";
            break;

            default:
                std::cout << " Invalid option. Try again.\n";
        }

    } while (option != 0);

    return 0;
}

// Utility functions
#include "utils.hpp"
#include "../indicator/indicator.hpp"
#include "../maliciousHash/maliciousHash.hpp"
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Function to display the main menu
void showMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Create New IOC\n";
    std::cout << "2. List All IOCs\n";
    std::cout << "3. Edit IOC\n";
    std::cout << "4. Remove IOC\n";
    std::cout << "0. Exit\n";
    std::cout << "-----------------\n\n";
}

// Subfunction to obtain the current timestamp in a formatted string
std::string getTimestamp() {
    auto nowTime = std::chrono::system_clock::now(); // Get the current time point with system clock from chrono
    std::time_t todayTime = std::chrono::system_clock::to_time_t(nowTime); // Convert to time_t for formatting
    std::ostringstream oss; // Create an output string stream for formatting
    oss << std::put_time(std::localtime(&todayTime), "%d-%m-%Y"); // Format: DD-MM-YYYY in string type
    return oss.str();
}

// Generate a unique ID for each indicator
int generateUniqueId() {
    static int currentId = 1;
    return currentId++;
}
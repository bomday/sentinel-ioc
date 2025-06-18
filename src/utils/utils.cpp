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
#include <vector>
#include <memory>

// Definition of the global indicators vector
std::vector<std::unique_ptr<Indicator>> indicators; 

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
void listIndicators()  {
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
    std::cout << std::string(294, '-') << std::endl; // Separator line for the header

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
        std::cout << std::string(294, '-') << std::endl;
    }

    std::cout << "\n✅ IOC listing complete.\n";
    std::cout << "-------------------------------------------\n";
}

// Method to remove an indicator by its ID
void removeIndicatorById(int id) {
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

    auto it = std::remove_if(indicators.begin(), indicators.end(),
                                [id](const std::unique_ptr<Indicator>& indicator) {
                                    return indicator->getIndicatorId() == id;
                                });
    if (it != indicators.end()) {
        indicators.erase(it, indicators.end());
        std::cout << "\nIndicator with ID " << id << " has been removed successfully.\n";
    } else {
        std::cout << "\nNo indicator found with ID " << id << ".\n";
    }
}
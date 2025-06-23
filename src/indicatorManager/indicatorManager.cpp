#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <ctime>   
 
#include "indicatorManager.hpp"
#include "indicator.hpp"
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "maliciousHash.hpp"
#include "utils.hpp"

// Indicator creation
void IndicatorManager::createIndicator() {
    int severity;
    std::string type, description, origin, timestamp;

    while (true) {
        std::cout << "\nEnter the type of IOC (IP, URL or Hash): ";
        std::getline(std::cin, type);

        if (type == "IP" || type == "URL" || type == "Hash") {
            break;
        } else {
            std::cout << "\nInvalid type. Please enter IP, URL or Hash.";
        }
    }

    while (true) {
        std::cout << "\nNow enter the severity (1 - 5): ";
        std::cin >> severity;

        if (std::cin.fail() || severity < 1 || severity > 5) {
            std::cout << "\nInvalid severity. Please enter a value between 1 and 5.\n";
            std::cin.clear(); 

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break; 
        }
    }

    std::cout << "\nEnter the origin: ";
    std::getline(std::cin, origin);

    timestamp = getTimestamp();
    
    std::cout << "\nEnter the description: ";
    std::getline(std::cin, description);

    int indicatorId = generateUniqueId();

    Indicator* newIndicator = nullptr;

    // Create a MaliciousIP object if the type is IP
    if (type == "IP") {
        newIndicator = MaliciousIP::createMaliciousIP(indicatorId, severity, type, description, origin, timestamp);

    // Create a MaliciousURL object if the type is URL
    } else if (type == "URL") {
        newIndicator = MaliciousURL::createMaliciousURL(indicatorId, severity, type, description, origin, timestamp);

    // Create a MaliciousHash object if the type is Hash
    } else if (type == "Hash") {
        newIndicator = MaliciousHash::createMaliciousHash(indicatorId, severity, type, description, origin, timestamp);
    }

    indicators.push_back(std::unique_ptr<Indicator>(newIndicator));
    
    std::cout << "\nIndicator created successfully with ID: " << indicatorId << "\n";
    std::cout << "-------------------------------------------\n";
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
void IndicatorManager::listIndicators() const {
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
    std::cout << std::string(150, '-') << std::endl; // Separator line for the header

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
        std::cout << std::string(150, '-') << std::endl;
    }

    std::cout << "\nIOC listing complete.\n";
    std::cout << "-------------------------------------------\n";
}

// Method to remove an indicator by its ID
void IndicatorManager::removeIndicatorById(int id) {
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

    auto indicatorToRemove = std::remove_if(indicators.begin(), indicators.end(),
                                [id](const std::unique_ptr<Indicator>& indicator) {
                                    return indicator->getIndicatorId() == id;
                                });
    if (indicatorToRemove != indicators.end()) {
        indicators.erase(indicatorToRemove, indicators.end());
        std::cout << "\nIndicator with ID " << id << " has been removed successfully.\n";
    } else {
        std::cout << "\nNo indicator found with ID " << id << ".\n";
    }
}
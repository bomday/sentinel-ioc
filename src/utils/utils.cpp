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
#include <algorithm>

// Definition of the global indicators vector
std::vector<std::unique_ptr<Indicator>> indicators; 

// Function to display the main menu
void showMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Create New IOC\n";
    std::cout << "2. List All IOCs\n";
    std::cout << "3. Search IOC\n";
    std::cout << "4. Edit IOC\n";
    std::cout << "5. Remove IOC\n";
    std::cout << "6. Acess statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "-----------------\n\n";
}

// Subfunction to obtain the current timestamp in a formatted string
std::string getTimestamp() {
    auto nowTime = std::chrono::system_clock::now(); // Get the current time point with system clock from chrono
    std::time_t todayTime = std::chrono::system_clock::to_time_t(nowTime); // Convert to time_t for formatting
    std::ostringstream oss; // Create an output string stream for formatting
    oss << std::put_time(std::localtime(&todayTime), "%Y-%m-%dT%H:%M:%SZ"); // Format: YYYY-MM-DDTH:M:SZ in string type
    return oss.str();
}

// Function to convert timestamp (string) to time_t
std::time_t stringToTimeT(const std::string& timestamp) {   // Gets a timestamp in string format
    std::tm tm = {};
    std::istringstream ss(timestamp); // Read the string as a file with istringstream
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");  // Save the parsed time into the tm structure
    return mktime(&tm); // Convert the tm structure to time_t, which represents seconds since epoch (1970-01-01 00:00:00 UTC)
}

// Function to check if the IOC was registered in the current month
bool isCurrentMonth(const std::string& timestamp) {
    std::time_t iocTime = stringToTimeT(timestamp);
    std::time_t now = std::time(nullptr);

    std::tm* nowTm = std::localtime(&now); // Get the current time as a tm structure
    std::tm* iocTm = std::localtime(&iocTime); // Convert the IOC time to a tm structure

    return (iocTm->tm_year == nowTm->tm_year && iocTm->tm_mon == nowTm->tm_mon);
}

// Function to get the time_t value for one month ago
std::time_t lastMonth() {
    std::time_t now = std::time(nullptr);
    std::tm tm = *std::localtime(&now);

    // Subtract one month from the current time
    tm.tm_mon -= 1;

    // Handle year change if necessary (if month -1 so it's December of the previous year)
    if (tm.tm_mon < 0) {
        tm.tm_mon += 12;
        tm.tm_year -= 1;
    }

    // Adjust the day to the last day of the previous month if necessary
    return std::mktime(&tm);
}

// Function to check if an IOC was registered in the last month
bool registerLastMonth(const std::string& timestamp) {
    std::time_t dataIOC = stringToTimeT(timestamp);
    std::time_t limite = lastMonth();
    return dataIOC >= limite; // Returns true if the IOC was registered within the last month
}

// Function for case-insensitive string comparison
bool icontains(const std::string& str, const std::string& substr) {
    auto it = std::search(
        str.begin(), str.end(),
        substr.begin(), substr.end(),
        [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    );
    return (it != str.end());
}

// Function to convert string to lowercase
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
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
#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <ctime>   
#include <map>
#include <stdexcept>
#include <fstream>
 
#include "indicatorManager.hpp"
#include "indicator/indicator.hpp"
#include "maliciousIP/maliciousIP.hpp"
#include "maliciousURL/maliciousURL.hpp"
#include "maliciousHash/maliciousHash.hpp"
#include "utils/utils.hpp"
#include "fileManager/fileManager.hpp"

// Getter for the next available ID
int IndicatorManager::getNextId() {
    return this->nextIndicatorId++;
}

// Add operations
int IndicatorManager::addMaliciousIP(int severity, const std::string& type, const std::string& description, 
                                     const std::string& origin, const std::string& timestamp, const std::string& ip, 
                                     const std::string& country, const std::string& isp) {
    int newId = getNextId(); // Get the next available ID for the new indicator
    // Create a new MaliciousIP object
    auto newIndicator = std::make_unique<MaliciousIP>(newId, severity, type, description, origin, timestamp, ip, country, isp);
    indicators.push_back(std::move(newIndicator));
    return newId;
}

int IndicatorManager::addMaliciousURL(int severity, const std::string& type, const std::string& description, 
                                      const std::string& origin, const std::string& timestamp, const std::string& url, 
                                      const std::string& protocol) {
    int newId = getNextId(); // Get the next available ID for the new indicator
    // Create a new MaliciousURL object
    auto newIndicator = std::make_unique<MaliciousURL>(newId, severity, type, description, origin, timestamp, url, protocol);
    indicators.push_back(std::move(newIndicator));
    return newId;
}

int IndicatorManager::addMaliciousHash(int severity, const std::string& type, const std::string& description, 
                                       const std::string& origin, const std::string& timestamp, const std::string& hash, 
                                       const std::string& algorithm) {
    int newId = getNextId(); // Get the next available ID for the new indicator
    // Create a new MaliciousHash object
    auto newIndicator = std::make_unique<MaliciousHash>(newId, severity, type, description, origin, timestamp, hash, algorithm);
    indicators.push_back(std::move(newIndicator));
    return newId;
}

// Helper function to print type-specific details for an indicator
static void printSpecificIndicatorDetails(const Indicator* ioc) {
    // Initialize empty strings for specific details
    std::string hash_val = "";
    std::string algorithm_val = "";
    std::string ip_val = "";
    std::string country_val = "";
    std::string isp_val = "";
    std::string url_val = "";
    std::string protocol_val = "";

    // Check the type of the IOC and extract specific details
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

    // Print the specific details in a formatted manner
    std::cout << std::setw(65) << hash_val
              << std::setw(10) << algorithm_val
              << std::setw(16) << ip_val
              << std::setw(16) << country_val
              << std::setw(21) << isp_val
              << std::setw(51) << url_val
              << std::setw(10) << protocol_val;
}

// Read operations
void IndicatorManager::listIndicators() const {
    std::cout << "\n --- LISTING REGISTERED IOCs --- \n";
    std::cout << "-------------------------------------------\n";

    if (indicators.empty()) {
        std::cout << " No IOCs registered yet.\n";
        std::cout << "-------------------------------------------\n";
        return;
    }

    // Table Header
    std::cout << std::left
              << std::setw(8)  << "ID"
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
    std::cout << std::string(294, '-') << std::endl;

    // Iterate through each indicator and print its details
    for (const auto& ioc : indicators) {
        std::cout << std::left
                  << std::setw(8)  << ioc->getIndicatorId()
                  << std::setw(12) << ioc->getSeverity()
                  << std::setw(10) << ioc->getType()
                  << std::setw(40) << ioc->getDescription()
                  << std::setw(15) << ioc->getOrigin()
                  << std::setw(20) << ioc->getTimestamp();

        printSpecificIndicatorDetails(ioc.get());
        std::cout << std::endl;
        std::cout << std::string(294, '-') << std::endl;
    }

    std::cout << "\n IOC listing complete.\n";
    std::cout << "-------------------------------------------\n";
}

// Find an indicator by its ID
Indicator* IndicatorManager::findIndicatorById(int id) const {
    auto it = std::find_if(indicators.begin(), indicators.end(),
                          [id](const std::unique_ptr<Indicator>& indicator) {
                              return indicator->getIndicatorId() == id;
                          });
    return (it != indicators.end()) ? it->get() : nullptr;
}

// Check if the indicator list is empty
bool IndicatorManager::isEmpty() const {
    return indicators.empty();
}

// Update operations
bool IndicatorManager::updateIndicatorSeverity(int id, int severity) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setSeverity(severity);
        return true;
    }
    return false;
}

// Update the description of an indicator
bool IndicatorManager::updateIndicatorDescription(int id, const std::string& description) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setDescription(description);
        return true;
    }
    return false;
}

// Update the origin of an indicator
bool IndicatorManager::updateIndicatorOrigin(int id, const std::string& origin) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setOrigin(origin);
        return true;
    }
    return false;
}

// Update the timestamp of an indicator
bool IndicatorManager::updateIndicatorTimestamp(int id, const std::string& timestamp) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setTimestamp(timestamp);
        return true;
    }
    return false;
}

// Update the IP address of a MaliciousIP indicator
bool IndicatorManager::updateMaliciousIPAddress(int id, const std::string& ip) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousIP* maliciousIP = dynamic_cast<MaliciousIP*>(indicator)) {
        maliciousIP->setIP(ip);
        return true;
    }
    return false;
}

// Update the country of a MaliciousIP indicator
bool IndicatorManager::updateMaliciousIPCountry(int id, const std::string& country) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousIP* maliciousIP = dynamic_cast<MaliciousIP*>(indicator)) {
        maliciousIP->setCountry(country);
        return true;
    }
    return false;
}

// Update the ISP of a MaliciousIP indicator
bool IndicatorManager::updateMaliciousIPISP(int id, const std::string& isp) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousIP* maliciousIP = dynamic_cast<MaliciousIP*>(indicator)) {
        maliciousIP->setISP(isp);
        return true;
    }
    return false;
}

// Update the URL of a MaliciousURL indicator
bool IndicatorManager::updateMaliciousURL(int id, const std::string& url) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousURL* maliciousURL = dynamic_cast<MaliciousURL*>(indicator)) {
        maliciousURL->setURL(url);
        return true;
    }
    return false;
}

// Update the protocol of a MaliciousURL indicator
bool IndicatorManager::updateMaliciousURLProtocol(int id, const std::string& protocol) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousURL* maliciousURL = dynamic_cast<MaliciousURL*>(indicator)) {
        maliciousURL->setProtocol(protocol);
        return true;
    }
    return false;
}

// Update the hash of a MaliciousHash indicator
bool IndicatorManager::updateMaliciousHash(int id, const std::string& hash) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousHash* maliciousHash = dynamic_cast<MaliciousHash*>(indicator)) {
        maliciousHash->setHash(hash);
        return true;
    }
    return false;
}

// Update the algorithm of a MaliciousHash indicator
bool IndicatorManager::updateMaliciousHashAlgorithm(int id, const std::string& algorithm) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousHash* maliciousHash = dynamic_cast<MaliciousHash*>(indicator)) {
        maliciousHash->setAlgorithm(algorithm);
        return true;
    }
    return false;
}

// Delete operations
bool IndicatorManager::removeIndicatorById(int id) {
    auto it = std::remove_if(indicators.begin(), indicators.end(),
                            [id](const std::unique_ptr<Indicator>& indicator) {
                                return indicator->getIndicatorId() == id;
                            });
    
    if (it != indicators.end()) {
        indicators.erase(it, indicators.end());
        return true;
    }
    return false;
}

// Search operations
void IndicatorManager::searchByValue(const std::string& value) const {
    bool found = false;
    int resultCount = 0;

    for (const auto& ioc : indicators) {
        bool matches = false;
        
        if (const MaliciousIP* ip = dynamic_cast<const MaliciousIP*>(ioc.get())) {
            matches = icontains(ip->getIP(), value);
        } else if (const MaliciousURL* url = dynamic_cast<const MaliciousURL*>(ioc.get())) {
            matches = icontains(url->getURL(), value);
        } else if (const MaliciousHash* hash = dynamic_cast<const MaliciousHash*>(ioc.get())) {
            matches = icontains(hash->getHash(), value);
        }
        
        if (matches) {
            printIOC(ioc.get(), ++resultCount);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No IOCs found with value: " << value << std::endl;
    } else {
        std::cout << "\nFound " << resultCount << " IOC(s) matching the value.\n";
    }
}

// Search operations by type
void IndicatorManager::searchByType(const std::string& type) const {
    bool found = false;
    int resultCount = 0;

    for (const auto& ioc : indicators) {
        if (icontains(ioc->getType(), type)) {
            printIOC(ioc.get(), ++resultCount);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No IOCs found with type: " << type << std::endl;
    } else {
        std::cout << "\nFound " << resultCount << " IOC(s) of type " << type << ".\n";
    }
}

// Search operations by severity
void IndicatorManager::searchBySeverity(int severity) const {
    bool found = false;
    int resultCount = 0;

    for (const auto& ioc : indicators) {
        if (ioc->getSeverity() == severity) {
            printIOC(ioc.get(), ++resultCount);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No IOCs found with severity: " << severity << std::endl;
    } else {
        std::cout << "\nFound " << resultCount << " IOC(s) with severity " << severity << ".\n";
    }
}

// Search operations by origin
void IndicatorManager::searchByOrigin(const std::string& origin) const {
    bool found = false;
    int resultCount = 0;

    for (const auto& ioc : indicators) {
        if (icontains(ioc->getOrigin(), origin)) {
            printIOC(ioc.get(), ++resultCount);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No IOCs found with origin: " << origin << std::endl;
    } else {
        std::cout << "\nFound " << resultCount << " IOC(s) from origin " << origin << ".\n";
    }
}

// Search operations by date
void IndicatorManager::searchByDate(const std::string& date) const {
    bool found = false;
    int resultCount = 0;

    for (const auto& ioc : indicators) {
        std::string timestamp = ioc->getTimestamp();
        if (timestamp.find(date) != std::string::npos || 
            timestamp.substr(0, 10) == date || 
            (date.length() == 10 && timestamp.substr(8, 2) + "-" + timestamp.substr(5, 2) + "-" + timestamp.substr(0, 4) == date)) {
            printIOC(ioc.get(), ++resultCount);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No IOCs found for date: " << date << std::endl;
    } else {
        std::cout << "\nFound " << resultCount << " IOC(s) for date " << date << ".\n";
    }
}

// Search operations by description
void IndicatorManager::searchByDescription(const std::string& description) const {
    bool found = false;
    int resultCount = 0;

    for (const auto& ioc : indicators) {
        if (icontains(ioc->getDescription(), description)) {
            printIOC(ioc.get(), ++resultCount);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No IOCs found with description containing: " << description << std::endl;
    } else {
        std::cout << "\nFound " << resultCount << " IOC(s) with matching description.\n";
    }
}

// Print details of an IOC
void IndicatorManager::printIOC(const Indicator* ioc, int index) const {
    std::cout << "\nIOC #" << index << "\n";
    std::cout << "ID         : " << ioc->getIndicatorId() << "\n";
    std::cout << "Type       : " << ioc->getType() << "\n";
    std::cout << "Severity   : " << ioc->getSeverity() << "\n";
    std::cout << "Origin     : " << ioc->getOrigin() << "\n";
    std::cout << "Date       : " << ioc->getTimestamp() << "\n";
    std::cout << "Description: " << ioc->getDescription() << "\n";
    
    // Print type-specific information
    if (const MaliciousIP* ip = dynamic_cast<const MaliciousIP*>(ioc)) {
        std::cout << "IP         : " << ip->getIP() << "\n";
        std::cout << "Country    : " << ip->getCountry() << "\n";
        std::cout << "ISP        : " << ip->getISP() << "\n";
    } else if (const MaliciousURL* url = dynamic_cast<const MaliciousURL*>(ioc)) {
        std::cout << "URL        : " << url->getURL() << "\n";
        std::cout << "Protocol   : " << url->getProtocol() << "\n";
    } else if (const MaliciousHash* hash = dynamic_cast<const MaliciousHash*>(ioc)) {
        std::cout << "Hash       : " << hash->getHash() << "\n";
        std::cout << "Algorithm  : " << hash->getAlgorithm() << "\n";
    }
}

// Save and load indicators to/from a file
void IndicatorManager::saveIndicatorsToFile(const std::string& filename) {
    if (!FileManager::saveData(filename, indicators)) {
        throw std::runtime_error("Failed to save data to file: " + filename);
    }
}

// Load indicators from a file
void IndicatorManager::loadIndicatorsFromFile(const std::string& filename) {
    auto loadedIndicators = FileManager::loadData(filename);
    if (loadedIndicators.empty()) {
        // Check if file exists first
        std::ifstream file(filename);
        if (!file.good()) {
            throw std::runtime_error("File not found or cannot be opened: " + filename);
        }
        // If file exists but no indicators loaded, it might be empty (which is okay)
    }
    indicators = std::move(loadedIndicators);
    updateNextId(); // Update the next available ID based on loaded indicators
}

// Function to update the next available ID based on existing indicators
void IndicatorManager::updateNextId() {
    if (!indicators.empty()) {
        // Assumes the last element has the highest ID
        this->nextIndicatorId = indicators.back()->getIndicatorId() + 1;
    } else {
        this->nextIndicatorId = 1; // Reset to 1 if no indicators exist
    }
}

// Generate statistics about the indicators
void IndicatorManager::generateStatistics() const {
    // Ensure there are indicators to analyze
    std::map<std::string, int> forType;
    std::map<int, int> forSeverity;
    int last30Days = 0;
    int thisMonth = 0;

    // Iterate through the indicators to gather statistics
    for (const auto& ioc : indicators) {
        forType[ioc->getType()]++;
        forSeverity[ioc->getSeverity()]++;
        
        if (registerLastMonth(ioc->getTimestamp())) {
            last30Days++;
        }
        
        if (isCurrentMonth(ioc->getTimestamp())) {
            thisMonth++;
        }
    }
    
    // Print the statistics
    std::cout << "\n=========== IOCs Statistics ===========\n\n";
    std::cout << "Total: " << indicators.size() << " IOCs\n\n";

    std::cout << "Distribution by Type:\n";
    for (const auto& [type, quantity] : forType) {
        std::cout << "  " << type << ": " << quantity << "\n";
    }

    std::cout << "\nDistribution by Severity:\n";
    for (const auto& [sev, quantity] : forSeverity) {
        std::cout << "  Severity " << sev << ": " << quantity << "\n";
    }

    std::cout << "\nIOCs registered in last month: " << last30Days << "\n";
    std::cout << "IOCs registered in current month: " << thisMonth << "\n";

    std::cout << "\n===========================================\n";
}

int IndicatorManager::getIOCsRegisteredLastMonth() const {
    int lastMonth = 0;
    for (const auto& ioc : indicators) {
        if (registerLastMonth(ioc->getTimestamp())) {
            lastMonth++;
        }
    }
    return lastMonth;
}

// Statistics helper methods for GUI
std::map<std::string, int> IndicatorManager::getTypeDistribution() const {
    std::map<std::string, int> typeDistribution;
    for (const auto& ioc : indicators) {
        typeDistribution[ioc->getType()]++;
    }
    return typeDistribution;
}

std::map<int, int> IndicatorManager::getSeverityDistribution() const {
    std::map<int, int> severityDistribution;
    for (const auto& ioc : indicators) {
        severityDistribution[ioc->getSeverity()]++;
    }
    return severityDistribution;
}

int IndicatorManager::getIOCsRegisteredCurrentMonth() const {
    int currentMonth = 0;
    for (const auto& ioc : indicators) {
        if (isCurrentMonth(ioc->getTimestamp())) {
            currentMonth++;
        }
    }
    return currentMonth;
}

int IndicatorManager::getCriticalThreatCount() const {
    int criticalCount = 0;
    for (const auto& ioc : indicators) {
        if (ioc->getSeverity() >= 4) { // Severity 4 and 5 are critical
            criticalCount++;
        }
    }
    return criticalCount;
}

// Search methods that return results (for GUI)
std::vector<const Indicator*> IndicatorManager::findByValue(const std::string& value) const {
    std::vector<const Indicator*> results;
    for (const auto& ioc : indicators) {
        // Check if value matches any type-specific field
        if (const MaliciousIP* ip = dynamic_cast<const MaliciousIP*>(ioc.get())) {
            if (ip->getIP().find(value) != std::string::npos ||
                ip->getCountry().find(value) != std::string::npos ||
                ip->getISP().find(value) != std::string::npos) {
                results.push_back(ioc.get());
            }
        } else if (const MaliciousURL* url = dynamic_cast<const MaliciousURL*>(ioc.get())) {
            if (url->getURL().find(value) != std::string::npos ||
                url->getProtocol().find(value) != std::string::npos) {
                results.push_back(ioc.get());
            }
        } else if (const MaliciousHash* hash = dynamic_cast<const MaliciousHash*>(ioc.get())) {
            if (hash->getHash().find(value) != std::string::npos ||
                hash->getAlgorithm().find(value) != std::string::npos) {
                results.push_back(ioc.get());
            }
        }
        
        // Also check common fields
        if (ioc->getDescription().find(value) != std::string::npos ||
            ioc->getOrigin().find(value) != std::string::npos) {
            // Avoid duplicates
            if (std::find(results.begin(), results.end(), ioc.get()) == results.end()) {
                results.push_back(ioc.get());
            }
        }
    }
    return results;
}

std::vector<const Indicator*> IndicatorManager::findByType(const std::string& type) const {
    std::vector<const Indicator*> results;
    for (const auto& ioc : indicators) {
        if (ioc->getType() == type) {
            results.push_back(ioc.get());
        }
    }
    return results;
}

std::vector<const Indicator*> IndicatorManager::findBySeverity(int severity) const {
    std::vector<const Indicator*> results;
    for (const auto& ioc : indicators) {
        if (ioc->getSeverity() == severity) {
            results.push_back(ioc.get());
        }
    }
    return results;
}

std::vector<const Indicator*> IndicatorManager::findByOrigin(const std::string& origin) const {
    std::vector<const Indicator*> results;
    for (const auto& ioc : indicators) {
        if (ioc->getOrigin().find(origin) != std::string::npos) {
            results.push_back(ioc.get());
        }
    }
    return results;
}

std::vector<const Indicator*> IndicatorManager::findByDate(const std::string& date) const {
    std::vector<const Indicator*> results;
    for (const auto& ioc : indicators) {
        if (ioc->getTimestamp().find(date) != std::string::npos) {
            results.push_back(ioc.get());
        }
    }
    return results;
}

std::vector<const Indicator*> IndicatorManager::findByDescription(const std::string& description) const {
    std::vector<const Indicator*> results;
    for (const auto& ioc : indicators) {
        if (ioc->getDescription().find(description) != std::string::npos) {
            results.push_back(ioc.get());
        }
    }
    return results;
}

std::vector<const Indicator*> IndicatorManager::searchWithFilters(const std::string& value, const std::string& type, 
                                                                  int severity, const std::string& dateFrom, 
                                                                  const std::string& dateTo) const {
    std::vector<const Indicator*> results;
    
    for (const auto& ioc : indicators) {
        bool matches = true;
        
        // Check value filter (if not empty)
        if (!value.empty()) {
            bool valueMatch = false;
            
            // Check type-specific fields
            if (const MaliciousIP* ip = dynamic_cast<const MaliciousIP*>(ioc.get())) {
                if (ip->getIP().find(value) != std::string::npos ||
                    ip->getCountry().find(value) != std::string::npos ||
                    ip->getISP().find(value) != std::string::npos) {
                    valueMatch = true;
                }
            } else if (const MaliciousURL* url = dynamic_cast<const MaliciousURL*>(ioc.get())) {
                if (url->getURL().find(value) != std::string::npos ||
                    url->getProtocol().find(value) != std::string::npos) {
                    valueMatch = true;
                }
            } else if (const MaliciousHash* hash = dynamic_cast<const MaliciousHash*>(ioc.get())) {
                if (hash->getHash().find(value) != std::string::npos ||
                    hash->getAlgorithm().find(value) != std::string::npos) {
                    valueMatch = true;
                }
            }
            
            // Check common fields
            if (!valueMatch && (ioc->getDescription().find(value) != std::string::npos ||
                               ioc->getOrigin().find(value) != std::string::npos)) {
                valueMatch = true;
            }
            
            if (!valueMatch) {
                matches = false;
            }
        }
        
        // Check type filter (if not "All Types")
        if (matches && !type.empty() && type != "All Types") {
            if (ioc->getType() != type) {
                matches = false;
            }
        }
        
        // Check severity filter (if not 0 which means "Any")
        if (matches && severity > 0) {
            if (ioc->getSeverity() != severity) {
                matches = false;
            }
        }
        
        // Date filtering would require more complex date parsing
        // For now, we'll skip date filtering in the combined search
        
        if (matches) {
            results.push_back(ioc.get());
        }
    }
    
    return results;
}

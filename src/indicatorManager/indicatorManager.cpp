#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <ctime>   
#include <map>
 
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
void IndicatorManager::addMaliciousIP(int severity, const std::string& type, const std::string& description, 
                                     const std::string& origin, const std::string& timestamp, const std::string& ip, 
                                     const std::string& country, const std::string& isp) {
    int newId = getNextId(); // Get the next available ID for the new indicator
    // Create a new MaliciousIP object
    auto newIndicator = std::make_unique<MaliciousIP>(newId, severity, type, description, origin, timestamp, ip, country, isp);
    indicators.push_back(std::move(newIndicator));
}

void IndicatorManager::addMaliciousURL(int severity, const std::string& type, const std::string& description, 
                                      const std::string& origin, const std::string& timestamp, const std::string& url, 
                                      const std::string& protocol) {
    int newId = getNextId(); // Get the next available ID for the new indicator
    // Create a new MaliciousURL object
    auto newIndicator = std::make_unique<MaliciousURL>(newId, severity, type, description, origin, timestamp, url, protocol);
    indicators.push_back(std::move(newIndicator));
}

void IndicatorManager::addMaliciousHash(int severity, const std::string& type, const std::string& description, 
                                       const std::string& origin, const std::string& timestamp, const std::string& hash, 
                                       const std::string& algorithm) {
    int newId = getNextId(); // Get the next available ID for the new indicator
    // Create a new MaliciousHash object
    auto newIndicator = std::make_unique<MaliciousHash>(newId, severity, type, description, origin, timestamp, hash, algorithm);
    indicators.push_back(std::move(newIndicator));
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

Indicator* IndicatorManager::findIndicatorById(int id) const {
    auto it = std::find_if(indicators.begin(), indicators.end(),
                          [id](const std::unique_ptr<Indicator>& indicator) {
                              return indicator->getIndicatorId() == id;
                          });
    return (it != indicators.end()) ? it->get() : nullptr;
}

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

bool IndicatorManager::updateIndicatorDescription(int id, const std::string& description) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setDescription(description);
        return true;
    }
    return false;
}

bool IndicatorManager::updateIndicatorOrigin(int id, const std::string& origin) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setOrigin(origin);
        return true;
    }
    return false;
}

bool IndicatorManager::updateIndicatorTimestamp(int id, const std::string& timestamp) {
    Indicator* indicator = findIndicatorById(id);
    if (indicator) {
        indicator->setTimestamp(timestamp);
        return true;
    }
    return false;
}

bool IndicatorManager::updateMaliciousIPAddress(int id, const std::string& ip) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousIP* maliciousIP = dynamic_cast<MaliciousIP*>(indicator)) {
        maliciousIP->setIP(ip);
        return true;
    }
    return false;
}

bool IndicatorManager::updateMaliciousIPCountry(int id, const std::string& country) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousIP* maliciousIP = dynamic_cast<MaliciousIP*>(indicator)) {
        maliciousIP->setCountry(country);
        return true;
    }
    return false;
}

bool IndicatorManager::updateMaliciousIPISP(int id, const std::string& isp) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousIP* maliciousIP = dynamic_cast<MaliciousIP*>(indicator)) {
        maliciousIP->setISP(isp);
        return true;
    }
    return false;
}

bool IndicatorManager::updateMaliciousURL(int id, const std::string& url) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousURL* maliciousURL = dynamic_cast<MaliciousURL*>(indicator)) {
        maliciousURL->setURL(url);
        return true;
    }
    return false;
}

bool IndicatorManager::updateMaliciousURLProtocol(int id, const std::string& protocol) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousURL* maliciousURL = dynamic_cast<MaliciousURL*>(indicator)) {
        maliciousURL->setProtocol(protocol);
        return true;
    }
    return false;
}

bool IndicatorManager::updateMaliciousHash(int id, const std::string& hash) {
    Indicator* indicator = findIndicatorById(id);
    if (MaliciousHash* maliciousHash = dynamic_cast<MaliciousHash*>(indicator)) {
        maliciousHash->setHash(hash);
        return true;
    }
    return false;
}

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

void IndicatorManager::saveIndicatorsToFile(const std::string& filename) {
    if (FileManager::saveData(filename, indicators)) {
        std::cout << "\nData saved successfully to " << filename << std::endl;
    } else {
        std::cout << "\nError saving data to " << filename << std::endl;
    }
}

void IndicatorManager::loadIndicatorsFromFile(const std::string& filename) {
    indicators = FileManager::loadData(filename);
    std::cout << "\nLoaded " << indicators.size() << " IOCs from \"" << filename << "\".\n";
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

void IndicatorManager::generateStatistics() const {
    std::map<std::string, int> forType;
    std::map<int, int> forSeverity;
    int last30Days = 0;
    int thisMonth = 0;

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

// Derivative class
#include "maliciousURL.hpp"
#include <iostream>

// Constructor
MaliciousURL::MaliciousURL(int indicatorId, 
                           int severity, 
                           std::string type, 
                           std::string description, 
                           std::string origin, 
                           std::string timestamp, 
                           std::string url, 
                           std::string protocol)
    : Indicator(indicatorId, severity, type, description, origin, timestamp),
      url(url),
      protocol(protocol) {}

// Getters
std::string MaliciousURL::getURL() const {
  return url;
}

std::string MaliciousURL::getProtocol() const {
  return protocol;
}

// Setters
void MaliciousURL::setURL(const std::string& newURL) {
  url = newURL;
}

void MaliciousURL::setProtocol(const std::string& newProtocol) {
  protocol = newProtocol;
}

// Static method to create a MaliciousURL object
Indicator* MaliciousURL::createMaliciousURL(int indicatorId, 
                                       int severity, 
                                       std::string type, 
                                       std::string description, 
                                       std::string origin, 
                                       std::string timestamp) {
    std::string url, protocol;

    std::cout << "\nEnter the url address: ";
    std::getline(std::cin, url);

    std::cout << "\nEnter the protocol type: ";
    std::getline(std::cin, protocol);
    
    std::cout << "\nURL indicator successfully created!\n";

    return new MaliciousURL(indicatorId, severity, type, description, origin, timestamp, url, protocol);
}

std::string MaliciousURL::toCSV() const {
    return std::to_string(getIndicatorId()) + "," +
           std::to_string(getSeverity()) + "," +
           getType() + "," +
           getDescription() + "," +
           getOrigin() + "," +
           getTimestamp() + "," +
           "" + "," + "" + "," + // hash, algorithm
           "" + "," + "" + "," + "" + "," + // ip, country, isp
           url + "," + protocol; // url and protocol filled
}




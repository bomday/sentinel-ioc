// Derivative class
#include "maliciousIP.hpp"
#include <iostream>

// Constructor
MaliciousIP::MaliciousIP(int indicatorId, 
                         int severity, 
                         std::string type, 
                         std::string description, 
                         std::string origin, 
                         std::string timestamp, 
                         std::string ip, 
                         std::string country, 
                         std::string isp)
    : Indicator(indicatorId, severity, type, description, origin, timestamp),
      ip(ip),
      country(country),
      isp(isp) {}

// Getters
std::string MaliciousIP::getIP() const {
  return ip;
}

std::string MaliciousIP::getCountry() const {
  return country;
}

std::string MaliciousIP::getISP() const {
  return isp;
}

// Setters
void MaliciousIP::setIP(const std::string& ipAddress) {
  ip = ipAddress;
}

void MaliciousIP::setCountry(const std::string& countryName) {
  country = countryName;
}

void MaliciousIP::setISP(const std::string& ispName) {
  isp = ispName;
} 

// Static method to create a MaliciousIP object
Indicator* MaliciousIP::createMaliciousIP(int indicatorId, int severity, const std::string& type, const std::string& description, const std::string& origin, const std::string& timestamp) {     
  std::string ip, country, isp;

  std::cout << "\nEnter the IP address: ";
  std::getline(std::cin, ip);

  std::cout << "\nEnter the country: ";
  std::getline(std::cin, country);

  std::cout << "\nEnter the ISP: ";
  std::getline(std::cin, isp);
  std::cout << "\nIP indicator successfully created!\n";

  return new MaliciousIP(indicatorId, severity, type, description, origin, timestamp, ip, country, isp);
}

std::string MaliciousIP::toCSV() const {
    return std::to_string(getIndicatorId()) + "," +
           std::to_string(getSeverity()) + "," +
           getType() + "," +
           getDescription() + "," +
           getOrigin() + "," +
           getTimestamp() + "," +
           "" + "," + "" + "," + // hash, algorithm
           ip + "," + country + "," + isp + "," + "" + "," + ""; // + empty url/protocol
}



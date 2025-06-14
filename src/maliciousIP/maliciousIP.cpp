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

// Static method to create a MaliciousIP object
Indicator* MaliciousIP::createMaliciousIP(int indicatorId, int severity, const std::string& type, const std::string& description, const std::string& origin, const std::string& timestamp) {     
  std::string ip, country, isp;

  std::cout << "\nInforme o endereço IP: ";
  std::getline(std::cin, ip);

  std::cout << "\nInforme o país: ";
  std::getline(std::cin, country);

  std::cout << "\nInforme o ISP: ";
  std::getline(std::cin, isp);
  std::cout << "\nIndicador IP criado com sucesso!\n";

  return new MaliciousIP(indicatorId, severity, type, description, origin, timestamp, ip, country, isp);
}


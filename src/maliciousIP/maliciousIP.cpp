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

// Implementation of displayInfo (from base class Indicator)
void MaliciousIP::displayInfo() const {
    std::cout << "ID: " << indicatorId << "\n";
    std::cout << "Tipo: " << type << "\n";
    std::cout << "Severidade: " << severity << "\n";
    std::cout << "Origem: " << origin << "\n";
    std::cout << "Data: " << timestamp << "\n";
    std::cout << "Descrição: " << description << "\n";
    std::cout << "Endereço IP: " << ip << "\n";
    std::cout << "País: " << country << "\n";
    std::cout << "ISP: " << isp << "\n";
}

std::string MaliciousIP::getValue() const { // Implementation of getValue (from base class Indicator)
    return ip;
}

// Static factory method
Indicator* MaliciousIP::createMaliciousIP(int indicatorId, int severity, const std::string& type,
                                          const std::string& description, const std::string& origin,
                                          const std::string& timestamp) {     
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

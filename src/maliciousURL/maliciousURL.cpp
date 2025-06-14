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

// Static method to create a MaliciousURL object
Indicator* MaliciousURL::createMaliciousURL(int indicatorId, 
                                       int severity, 
                                       std::string type, 
                                       std::string description, 
                                       std::string origin, 
                                       std::string timestamp) {
    std::string url, protocol;

    std::cout << "\nInforme o endereço url: ";
    std::getline(std::cin, url);

    std::cout << "\nInforme o protocolo: ";
    std::getline(std::cin, protocol);
    
    std::cout << "\nIndicador URL criado com sucesso!\n";

    return new MaliciousURL(indicatorId, severity, type, description, origin, timestamp, url, protocol);
}


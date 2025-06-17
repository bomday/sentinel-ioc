#include "maliciousURL.hpp"
#include <iostream>

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

void MaliciousURL::displayInfo() const {
    std::cout << "ID: " << indicatorId << "\n";
    std::cout << "Tipo: " << type << "\n";
    std::cout << "Severidade: " << severity << "\n";
    std::cout << "Origem: " << origin << "\n";
    std::cout << "Data: " << timestamp << "\n";
    std::cout << "Descrição: " << description << "\n";
    std::cout << "URL: " << url << "\n";
    std::cout << "Protocolo: " << protocol << "\n";
}

std::string MaliciousURL::getValue() const { return url; }

  
Indicator* MaliciousURL::createMaliciousURL(int indicatorId, 
                                            int severity, 
                                            std::string type, 
                                            std::string description, 
                                            std::string origin, 
                                            std::string timestamp) {
    std::string url, protocol;

    std::cout << "\nInforme o endereço URL: ";
    std::getline(std::cin, url);

    std::cout << "\nInforme o protocolo: ";
    std::getline(std::cin, protocol);
    
    std::cout << "\nIndicador URL criado com sucesso!\n";

    return new MaliciousURL(indicatorId, severity, type, description, origin, timestamp, url, protocol);
}

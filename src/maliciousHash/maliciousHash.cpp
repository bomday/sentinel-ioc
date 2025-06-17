#include "maliciousHash.hpp"
#include <iostream>

MaliciousHash::MaliciousHash(int indicatorId, 
                             int severity, 
                             std::string type, 
                             std::string description, 
                             std::string origin, 
                             std::string timestamp, 
                             std::string hash, 
                             std::string algorithm)
    : Indicator(indicatorId, severity, type, description, origin, timestamp),
      hash(hash),
      algorithm(algorithm) {}

void MaliciousHash::displayInfo() const {
    std::cout << "ID: " << indicatorId << "\n";
    std::cout << "Tipo: " << type << "\n";
    std::cout << "Severidade: " << severity << "\n";
    std::cout << "Origem: " << origin << "\n";
    std::cout << "Data: " << timestamp << "\n";
    std::cout << "Descrição: " << description << "\n";
    std::cout << "Hash: " << hash << "\n";
    std::cout << "Algoritmo: " << algorithm << "\n";
}

std::string MaliciousHash::getValue() const { return hash; }
   


Indicator* MaliciousHash::createMaliciousHash(int indicatorId, 
                                              int severity, 
                                              std::string type, 
                                              std::string description, 
                                              std::string origin, 
                                              std::string timestamp) {
    std::string hash, algorithm;

    std::cout << "\nInforme o hash: ";
    std::getline(std::cin, hash);

    std::cout << "\nInforme o algoritmo: ";
    std::getline(std::cin, algorithm);

    std::cout << "\nIndicador Hash criado com sucesso!\n";

    return new MaliciousHash(indicatorId, severity, type, description, origin, timestamp, hash, algorithm);
}

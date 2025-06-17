// Derivative class
#include "maliciousHash.hpp"
#include <iostream>

// Constructor
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

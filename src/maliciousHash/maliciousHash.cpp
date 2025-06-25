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

// Getters
std::string MaliciousHash::getHash() const { 
  return hash; 
}
std::string MaliciousHash::getAlgorithm() const { 
  return algorithm; 
}

// Setters
void MaliciousHash::setHash(const std::string& hashValue) { 
  hash = hashValue; 
}
void MaliciousHash::setAlgorithm(const std::string& algorithmValue) { 
  algorithm = algorithmValue; 
}

Indicator* MaliciousHash::createMaliciousHash(int indicatorId, 
                                        int severity, 
                                        std::string type, 
                                        std::string description, 
                                        std::string origin, 
                                        std::string timestamp) {
    std::string hash, algorithm;

    std::cout << "\nEnter the hash: ";
    std::getline(std::cin, hash);

    std::cout << "\nEnter the algorithm: ";
    std::getline(std::cin, algorithm);

    std::cout << "\nHash indicator created successfully!\n";

    return new MaliciousHash(indicatorId, severity, type, description, origin, timestamp, hash, algorithm);
}

std::string MaliciousHash::toCSV() const {
    return std::to_string(getIndicatorId()) + "," +
           std::to_string(getSeverity()) + "," +
           getType() + "," +
           getDescription() + "," +
           getOrigin() + "," +
           getTimestamp() + "," +
           hash + "," + algorithm + "," +
           "" + "," + "" + "," + "" + "," + "" + "," + "";
}



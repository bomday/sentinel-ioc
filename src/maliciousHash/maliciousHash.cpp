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




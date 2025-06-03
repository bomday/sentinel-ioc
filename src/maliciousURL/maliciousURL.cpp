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


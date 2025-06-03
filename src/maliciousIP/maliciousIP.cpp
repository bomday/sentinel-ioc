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


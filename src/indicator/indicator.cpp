// Base abstract class
#include "indicator.hpp"
#include <iostream>

// Constructor
Indicator::Indicator(int indicatorId, 
                    int severity, 
                    std::string type, 
                    std::string description, 
                    std::string origin, 
                    std::string timestamp)
    : indicatorId(indicatorId),
      severity(severity),
      type(type),
      description(description),
      origin(origin),
      timestamp(timestamp) {
}
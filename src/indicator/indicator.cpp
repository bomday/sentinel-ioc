// Base abstract class
#include "Indicator.hpp"
#include <iostream>

Indicator::Indicator(
    int indicatorId,            
    int severity,               
    std::string type,             
    std::string description,    
    std::string origin,         
    std::string timestamp       
)
    : indicatorId(indicatorId),     
      severity(severity),
      type(type),
      description(description),
      origin(origin),
      timestamp(timestamp) { }

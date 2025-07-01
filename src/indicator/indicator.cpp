#include "indicator.hpp"
#include "utils/utils.hpp"
#include "maliciousIP/maliciousIP.hpp" 
#include "maliciousURL/maliciousURL.hpp"
#include "maliciousHash/maliciousHash.hpp" 
#include <iostream>
#include <limits> 
#include <memory>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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

// Getters
int Indicator::getIndicatorId() const { 
  return indicatorId; 
}

int Indicator::getSeverity() const { 
  return severity; 
}
std::string Indicator::getType() const { 
  return type; 
}

std::string Indicator::getDescription() const { 
  return description; 
}

std::string Indicator::getOrigin() const { 
  return origin; 
}

std::string Indicator::getTimestamp() const { 
  return timestamp; 
}

// Setters
void Indicator::setSeverity(int sev) { 
  severity = sev; 
}

void Indicator::setType(const std::string& t) { 
  type = t; 
}

void Indicator::setDescription(const std::string& desc) { 
  description = desc; 
}

void Indicator::setOrigin(const std::string& orig) { 
  origin = orig; 
}

void Indicator::setTimestamp(const std::string& ts) { 
  timestamp = ts; 
}
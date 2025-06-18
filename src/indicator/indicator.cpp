#include "indicator.hpp"
#include "utils.hpp"
#include "maliciousIP.hpp" 
#include "maliciousURL.hpp"
#include "maliciousHash.hpp" 
#include <iostream>
#include <limits> 
#include <memory>
#include <vector>

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

extern std::vector<std::unique_ptr<Indicator>> indicators; // Call the global vector of unique pointers

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

// Indicator creation    
Indicator* createIndicator() {
  int severity; 
  std::string type, description, origin, timestamp; 

  while (true) {
      std::cout << "\nEnter the type of IOC (IP, URL or Hash): ";
      std::getline(std::cin, type);

      if (type == "IP" || type == "URL" || type == "Hash") {
          break;
      } else {
          std::cout << "\nInvalid type. Please enter IP, URL or Hash.";
      }
  }

  while (true) {
      std::cout << "\nNow enter the severity (1 - 5): ";
      std::cin >> severity;

      if (std::cin.fail() || severity < 1 || severity > 5) {
          std::cout << "\nInvalid severity. Please enter a value between 1 and 5.\n";
          std::cin.clear(); 

          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      } else {
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          break; 
      }
  }

  std::cout << "\nEnter the origin: ";
  std::getline(std::cin, origin);

  timestamp = getTimestamp();
  
  std::cout << "\nEnter the description: ";
  std::getline(std::cin, description);

  int indicatorId = generateUniqueId();

  // Create a MaliciousIP object if the type is IP
  if (type == "IP") { 
      Indicator* newMaliciousIP = MaliciousIP::createMaliciousIP(indicatorId, severity, type, description, origin, timestamp);
      indicators.push_back(std::unique_ptr<Indicator>(newMaliciousIP));
      return newMaliciousIP;
  }
  
  // Create a MaliciousURL object if the type is URL
  else if (type == "URL") {
      Indicator* newMaliciousURL = MaliciousURL::createMaliciousURL(indicatorId, severity, type, description, origin, timestamp);
      indicators.push_back(std::unique_ptr<Indicator>(newMaliciousURL));
      return newMaliciousURL;
  }

  // Create a MaliciousHash object if the type is Hash
  else if (type == "Hash") {
      // Assume que MaliciousHash::createMaliciousHash é um método estático que retorna um Indicator*
      Indicator* newMaliciousHash = MaliciousHash::createMaliciousHash(indicatorId, severity, type, description, origin, timestamp);
      indicators.push_back(std::unique_ptr<Indicator>(newMaliciousHash));
      return newMaliciousHash;
  }

  return nullptr; // Return nullptr if the type is not recognized

}
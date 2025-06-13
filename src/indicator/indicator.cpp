// Base abstract class
#include "indicator.hpp"
#include "utils.hpp"
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "maliciousHash.hpp"
#include <iostream>
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


std::vector<Indicator*> indicators; // Create a vector to store the indicators

// Indicator creation    
Indicator* createIndicator() {
  int severity; // ID and severity declarations
  std::string type, description, origin, timestamp; // Type, description, origin, and timestamp declarations

  // Prompt the user for the type of IOC
  while (true) {
    std::cout << "\nDigite o tipo do IOC (IP, URL ou Hash): ";
    std::getline(std::cin, type);

    if (type == "IP" || type == "URL" || type == "Hash") {
      break; // Exit the loop if the type is valid
    } else {
      std::cout << "\nTipo inválido. Por favor, digite IP, URL ou Hash.";
    }
  } 
  
  while(true){
    std::cout << "\nAgora digite a severidade (1 - 5): ";
    std::cin >> severity;
    if (severity < 1 || severity > 5) {
      std::cout << "\nSeveridade inválida. Por favor, digite um valor entre 1 e 5.\n";
      std::cin.clear(); // Clear the error flag on cin
      std::cin.ignore(); // Ignore the invalid input in the buffer
    }
    
    else{
      std::cin.ignore(); // Clear the input buffer to avoid issues with getline
      break; // Exit the loop if the severity is valid
    }
  }

  std::cout << "\nDigite a origem: ";
  std::getline(std::cin, origin);

  timestamp = getTimestamp(); // Get the date from the subfunction

  std::cout << "\nDigite a descrição: ";
  std::getline(std::cin, description);

  int indicatorId = generateUniqueId(); // Get a unique ID for the indicator

  // Create a MaliciousIP object if the type is IP
  if (type == "IP") { 
    Indicator* newIP = MaliciousIP::createMaliciousIP(indicatorId, severity, type, description, origin, timestamp);
    indicators.push_back(newIP);
    return newIP;
  }

  // Create a MaliciousURL object if the type is URL
  else if (type == "URL") { 

    Indicator* newIP = MaliciousURL::createMaliciousURL(indicatorId, severity, type, description, origin, timestamp);
    indicators.push_back(newIP);
    return newIP;
  }

  // Create a MaliciousHash object if the type is Hash
  else if (type == "Hash") { 

    Indicator* newIP = MaliciousHash::createMaliciousHash(indicatorId, severity, type, description, origin, timestamp);
    indicators.push_back(newIP);
    return newIP;
  }
  
  return nullptr; // Return nullptr if the type is not recognized

}

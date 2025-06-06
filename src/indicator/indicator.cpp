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

  std::cout << "Digite o tipo do IOC (IP, URL ou Hash): ";
  std::getline(std::cin, type); // Using getline to allow spaces in type input (get the entire input line)

  std::cout << "Agora digite a severidade (1 - 3): ";
  std::cin >> severity;
  std::cin.ignore(); // Using ignore to empty the enter buffer for getline (delete the '/n ' left by std::cin)

  std::cout << "Digite a origem: ";
  std::getline(std::cin, origin);

  timestamp = getTimestamp(); // Get the date from the subfunction

  std::cout << "Digite a descrição: ";
  std::getline(std::cin, description);

  int indicatorId = generateUniqueId(); // Get a unique ID for the indicator

  // Create a MaliciousIP object if the type is IP
  if (type == "IP") { 
    Indicator* newIP = MaliciousIP::createIP(indicatorId, severity, type, description, origin, timestamp);
    indicators.push_back(newIP);
    return newIP;
  }

  // Create a MaliciousURL object if the type is URL
  else if (type == "URL") { 

    Indicator* newIP = MaliciousURL::createURL(indicatorId, severity, type, description, origin, timestamp);
    indicators.push_back(newIP);
    return newIP;
  }

  // Create a MaliciousHash object if the type is Hash
  else if (type == "Hash") { 

    Indicator* newIP = MaliciousHash::createHash(indicatorId, severity, type, description, origin, timestamp);
    indicators.push_back(newIP);
    return newIP;
  }

  // If the type is not recognized, print an error message
  std::cout << "\nTipo de IOC inválido!\n";
  return nullptr;
}

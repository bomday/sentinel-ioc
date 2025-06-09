#include "indicator.hpp"
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"
#include "../maliciousHash/maliciousHash.hpp"
#include <sstream>
#include <iostream>

Indicator::Indicator(std::string type,
                     std::string value,
                     std::string severity,
                     std::string origin,
                     std::string timestamp,
                     std::string description)
    : type(type),
      value(value),
      severity(severity),
      origin(origin),
      timestamp(timestamp),
      description(description) {}

std::unique_ptr<Indicator> Indicator::fromLine(const std::string& line) {
    std::stringstream ss(line);
    std::string type, value, severity, origin, timestamp, description;

    if (!std::getline(ss, type, ';') ||
        !std::getline(ss, value, ';') ||
        !std::getline(ss, severity, ';') ||
        !std::getline(ss, origin, ';') ||
        !std::getline(ss, timestamp, ';') ||
        !std::getline(ss, description)) {
        std::cerr << "[Parse Error] Malformed line: " << line << "\n";
        return nullptr;
        }

    try {
        if (type == "IP") {
            return std::make_unique<MaliciousIP>(value, severity, origin, timestamp, description);
        } else if (type == "URL") {
            return std::make_unique<MaliciousURL>(value, severity, origin, timestamp, description);
        } else if (type == "Hash") {
            return std::make_unique<MaliciousHash>(value, severity, origin, timestamp, description);
        } else {
            std::cerr << "[Type Error] Unknown indicator type: " << type << "\n";
            return nullptr;
        }
    } catch (const std::exception& e) {
        std::cerr << "[Factory Error] Exception creating indicator: " << e.what() << "\n";
        return nullptr;
    }
}

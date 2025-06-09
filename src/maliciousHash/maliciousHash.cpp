#include "maliciousHash.hpp"
#include <iostream>

MaliciousHash::MaliciousHash(std::string value,
                             std::string severity,
                             std::string origin,
                             std::string timestamp,
                             std::string description)
    : Indicator("Hash", value, severity, origin, timestamp, description),
      hash(value),
      algorithm("Unknown") {}

void MaliciousHash::printInfo() const {
    std::cout << "[Malicious Hash]\n"
              << "  Hash: " << hash << "\n"
              << "  Severity: " << getSeverity() << "\n"
              << "  Origin: " << getOrigin() << "\n"
              << "  Timestamp: " << getTimestamp() << "\n"
              << "  Description: " << getDescription() << "\n"
              << "  Algorithm: " << algorithm << "\n";
}

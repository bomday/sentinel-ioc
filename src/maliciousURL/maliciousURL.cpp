#include "maliciousURL.hpp"
#include <iostream>

MaliciousURL::MaliciousURL(std::string value,
                           std::string severity,
                           std::string origin,
                           std::string timestamp,
                           std::string description)
    : Indicator("URL", value, severity, origin, timestamp, description),
      url(value),
      protocol("Unknown") {}

void MaliciousURL::printInfo() const {
    std::cout << "[Malicious URL]\n"
              << "  URL: " << url << "\n"
              << "  Severity: " << getSeverity() << "\n"
              << "  Origin: " << getOrigin() << "\n"
              << "  Timestamp: " << getTimestamp() << "\n"
              << "  Description: " << getDescription() << "\n"
              << "  Protocol: " << protocol << "\n";
}

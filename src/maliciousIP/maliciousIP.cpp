#include "maliciousIP.hpp"
#include <iostream>

// Constructor: uses base Indicator constructor with hardcoded type = "IP"
MaliciousIP::MaliciousIP(std::string value,
                         std::string severity,
                         std::string origin,
                         std::string timestamp,
                         std::string description)
    : Indicator("IP", value, severity, origin, timestamp, description),
      ip(value),
      country(""),   // Can be set later
      isp("")        // Can be set later
{}

// Optional print function
void MaliciousIP::printInfo() const {
    std::cout << "[Malicious IP]\n"
              << "  IP: " << ip << "\n"
              << "  Severity: " << getSeverity() << "\n"
              << "  Origin: " << getOrigin() << "\n"
              << "  Timestamp: " << getTimestamp() << "\n"
              << "  Description: " << getDescription() << "\n"
              << "  Country: " << country << "\n"
              << "  ISP: " << isp << "\n";
}

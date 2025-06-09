#ifndef MALICIOUSIP_HPP
#define MALICIOUSIP_HPP

#include "indicator.hpp"
#include <string>

class MaliciousIP : public Indicator {
public:
    // Constructor compatible with Indicator::fromLine()
    MaliciousIP(std::string value,
                std::string severity,
                std::string origin,
                std::string timestamp,
                std::string description);

    // Getters
    std::string getIP() const { return ip; }
    std::string getCountry() const { return country; }
    std::string getISP() const { return isp; }

    // Setters (optional for later enrichment)
    void setCountry(const std::string& newCountry) { country = newCountry; }
    void setISP(const std::string& newISP) { isp = newISP; }

    // Display
    void printInfo() const;

private:
    std::string ip;
    std::string country;
    std::string isp;
};

#endif // MALICIOUSIP_HPP

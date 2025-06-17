#ifndef MALICIOUS_IP_HPP
#define MALICIOUS_IP_HPP

#include <string>
#include "indicator/indicator.hpp"

class MaliciousIP : public Indicator {
private:
    std::string ip;
    std::string country;
    std::string isp;

public:
    MaliciousIP(int indicatorId, int severity, std::string type, std::string description,
                std::string origin, std::string timestamp, std::string ip, std::string country, std::string isp);

    static Indicator* createMaliciousIP(int indicatorId, int severity, const std::string& type,
                                        const std::string& description, const std::string& origin,
                                        const std::string& timestamp);

    virtual ~MaliciousIP() = default;

    std::string getIP() const { return ip; }
    std::string getCountry() const { return country; }
    std::string getISP() const { return isp; }
    std::string getValue() const override;  // Apenas a assinatura!

    void setIP(const std::string& ipAddress) { ip = ipAddress; }
    void setCountry(const std::string& countryName) { country = countryName; }
    void setISP(const std::string& ispName) { isp = ispName; }

    void displayInfo() const override;
};

#endif

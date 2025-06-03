// Derivative class

#ifndef MALICIOUS_IP_HPP
#define MALICIOUS_IP_HPP

#include <string>
#include "../indicator/indicator.hpp"

class MaliciousIP : public Indicator {
    private:
        std::string ip;
        std::string country;
        std::string isp;
        
    public:
        MaliciousIP(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp, std::string ip, std::string country, std::string isp);
};

#endif
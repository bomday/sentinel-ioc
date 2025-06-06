// Derivative class

#ifndef MALICIOUS_IP_HPP
#define MALICIOUS_IP_HPP

#include <string>
#include "../indicator/indicator.hpp"

class MaliciousIP : public Indicator {
    private:
        // Atributes
        std::string ip;
        std::string country;
        std::string isp;
        
    public:
        // Constructor
        MaliciousIP(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp, std::string ip, std::string country, std::string isp);

        // Static method to create a MaliciousIP object
        static Indicator* createIP(int indicatorId, int severity, const std::string& type, const std::string& description, const std::string& origin, const std::string& timestamp);
};

#endif
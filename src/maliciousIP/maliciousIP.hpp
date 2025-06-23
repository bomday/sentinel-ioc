// Derivative class

#ifndef MALICIOUS_IP_HPP
#define MALICIOUS_IP_HPP

#include <string>
#include "../indicator/indicator.hpp"

// Derivative class for Malicious IP Indicators
// This class inherits from the Indicator class and represents a specific type of indicator that contains an IP address, country, and ISP info
class MaliciousIP : public Indicator {
    private:
        // Atributes
        std::string ip;
        std::string country;
        std::string isp;
        
    public:
        // Constructor
        MaliciousIP(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp, std::string ip, std::string country, std::string isp);

        // Function to create an indicator of type MaliciousIP
        static Indicator* createMaliciousIP(int indicatorId, int severity, const std::string& type, const std::string& description, const std::string& origin, const std::string& timestamp);

        // Virtual destructor
        virtual ~MaliciousIP() = default;
        
        // Getters
        std::string getIP() const;
        std::string getCountry() const;
        std::string getISP() const;

        // Setters
        void setIP(const std::string& ipAddress);
        void setCountry(const std::string& countryName);
        void setISP(const std::string& ispName);

};

#endif
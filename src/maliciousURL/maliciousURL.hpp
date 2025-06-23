// Derivative class

#ifndef MALICIOUS_URL_HPP
#define MALICIOUS_URL_HPP

#include <string>
#include "../indicator/indicator.hpp"

// Derivative class for Malicious URL Indicators
// This class inherits from the Indicator class and represents a specific type of indicator that contains a URL and its protocol
class MaliciousURL : public Indicator {
    private:
        // Atributes
        std::string url;
        std::string protocol;

    public:
        // Constructor
        MaliciousURL(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp, std::string url, std::string protocol);

        // Function to create an indicator of type MaliciousURL
        static Indicator* createMaliciousURL(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);

        // Virtual destructor
        virtual ~MaliciousURL() = default;

        // Getters
        std::string getURL() const;
        std::string getProtocol() const;

        // Setters
        void setURL(const std::string& urlAddress);
        void setProtocol(const std::string& protocolName);
        
};  

#endif
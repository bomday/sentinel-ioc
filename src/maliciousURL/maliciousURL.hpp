// Derivative class

#ifndef MALICIOUS_URL_HPP
#define MALICIOUS_URL_HPP

#include <string>
#include "../indicator/indicator.hpp"

class MaliciousURL : public Indicator {
    private:
        // Atributes
        std::string url;
        std::string protocol;

    public:
        // Constructor
        MaliciousURL(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp, std::string url, std::string protocol);

    static Indicator* createURL(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);
};  

#endif
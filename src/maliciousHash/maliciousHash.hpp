// Derivative class

#ifndef MALICIOUS_HASH_HPP
#define MALICIOUS_HASH_HPP

#include <string>
#include "../indicator/indicator.hpp"

class MaliciousHash : public Indicator {
    private:
        // Atributes
        std::string hash;
        std::string algorithm;

    public:
        // Constructor
        MaliciousHash(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp, std::string hash, std::string algorithm);

    static Indicator* createHash(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);
};

#endif
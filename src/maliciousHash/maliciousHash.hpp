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

        // Function to create an indicator of type MaliciousHash
        static Indicator* createMaliciousHash(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);

        // Virtual destructor
        virtual ~MaliciousHash() = default;

        // Getters
        std::string getHash() const { return hash; }
        std::string getAlgorithm() const { return algorithm; }
        
        // Setters
        void setHash(const std::string& hashValue) { hash = hashValue; }
        void setAlgorithm(const std::string& algorithmName) { algorithm = algorithmName; }
};

#endif
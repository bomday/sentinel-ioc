// Derivative class

#ifndef MALICIOUS_HASH_HPP
#define MALICIOUS_HASH_HPP

#include <string>
#include "../indicator/indicator.hpp"

// Derivative class for Malicious Hash Indicators
// This class inherits from the Indicator class and represents a specific type of indicator that contains a hash and its algorithm
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
        std::string getHash() const;
        std::string getAlgorithm() const;
        
        // Setters
        void setHash(const std::string& hashValue);
        void setAlgorithm(const std::string& algorithmName);

        std::string toCSV() const override;
        // Function to convert the indicator to a string representation
};

#endif
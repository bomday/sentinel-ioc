#ifndef MALICIOUSHASH_HPP
#define MALICIOUSHASH_HPP

#include "indicator.hpp"
#include <string>

class MaliciousHash : public Indicator {
public:
    // Compatible constructor with Indicator::fromLine()
    MaliciousHash(std::string value,
                  std::string severity,
                  std::string origin,
                  std::string timestamp,
                  std::string description);

    std::string getHash() const { return hash; }
    std::string getAlgorithm() const { return algorithm; }

    void setAlgorithm(const std::string& algo) { algorithm = algo; }

    void printInfo() const;

private:
    std::string hash;
    std::string algorithm;
};

#endif // MALICIOUSHASH_HPP

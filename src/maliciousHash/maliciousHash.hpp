// Derivative class

#ifndef MALICIOUS_HASH_HPP
#define MALICIOUS_HASH_HPP

#include <string>
#include "../indicator/indicator.hpp"

using namespace std;

class MaliciousHash : public Indicator {
    private:
        string hash;
        string algorithm;

    public:
        MaliciousHash(int indicatorId, int severity, string type, string description, string origin, string timestamp, string hash, string algorithm);
};

#endif
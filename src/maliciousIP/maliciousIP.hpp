// Derivative class

#ifndef MALICIOUS_IP_HPP
#define MALICIOUS_IP_HPP

#include <string>
#include "../indicator/indicator.hpp"

using namespace std;
class MaliciousIP : public Indicator {
    private:
        string ip;
        string country;
        string isp;
        
    public:
        MaliciousIP(int indicatorId, int severity, string type, string description, string origin, string timestamp, string ip, string country, string isp);
};

#endif
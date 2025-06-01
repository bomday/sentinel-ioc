// Derivative class

#ifndef MALICIOUS_URL_HPP
#define MALICIOUS_URL_HPP

#include <string>
#include "../indicator/indicator.hpp"

using namespace std;

class MaliciousURL : public Indicator {
    private:
        string url;
        string protocol;

    public:
        MaliciousURL(int indicatorId, int severity, string type, string description, string origin, string timestamp, string url, string protocol);
};  

#endif
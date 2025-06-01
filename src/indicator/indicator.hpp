// Base abstract class

#ifndef INDICATOR_HPP
#define INDICATOR_HPP
#include <string>

using namespace std;
class Indicator {
    private:
        int indicatorId;
        int severity;
        string type;
        string description;
        string origin;
        string timestamp;
    
    public:
        Indicator(int indicatorId, int severity, string type, string description, string origin, string timestamp);
};

#endif
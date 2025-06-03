// Base abstract class

#ifndef INDICATOR_HPP
#define INDICATOR_HPP
#include <string>

class Indicator {
    private:
        // Atributes
        int indicatorId;
        int severity;
        std::string type;
        std::string description;
        std::string origin;
        std::string timestamp;
    
    public:
        // Constructor
        Indicator(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);
};

#endif
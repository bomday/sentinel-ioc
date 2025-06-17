// Base abstract class
#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <string>
#include <vector>

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
        
        // Virtual destructor
        virtual ~Indicator() = default;

        // Getters
        int getIndicatorId() const { return indicatorId; }
        int getSeverity() const { return severity; }
        std::string getType() const { return type; }
        std::string getDescription() const { return description; }
        std::string getOrigin() const { return origin; }
        std::string getTimestamp() const { return timestamp; }

        // Setters
        void setSeverity(int sev) { severity = sev; }    
        void setType(const std::string& t) { type = t; }
        void setDescription(const std::string& desc) { description = desc; }
        void setOrigin(const std::string& orig) { origin = orig; }
        void setTimestamp(const std::string& ts) { timestamp = ts; }
};

#endif // INDICATOR_HPP
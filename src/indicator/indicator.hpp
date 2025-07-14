// Base abstract class
#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <string>
#include <vector>

// represents an indicator
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
        int getIndicatorId() const;
        int getSeverity() const;
        std::string getType() const;
        std::string getDescription() const;
        std::string getOrigin() const;
        std::string getTimestamp() const;

        // Setters
        void setSeverity(int sev);   
        void setType(const std::string& t);
        void setDescription(const std::string& desc);
        void setOrigin(const std::string& orig);
        void setTimestamp(const std::string& ts);

        virtual std::string toCSV() const = 0; // Pure virtual function to convert the indicator to a CSV format
        virtual std::string getValue() const = 0; // Pure virtual function to get the main value (hash, IP, URL)

};

#endif // INDICATOR_HPP
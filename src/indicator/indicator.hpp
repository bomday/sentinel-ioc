#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <string>

class Indicator {
protected:
    int indicatorId;
    int severity;
    std::string type;
    std::string description;
    std::string origin;
    std::string timestamp;

public:
    Indicator(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);
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

    // Método polimórfico
    virtual void displayInfo() const = 0;  // Método puro virtual!
    
    virtual std::string getValue() const = 0; // Pure virtual method for getting specific value (IP, URL, Hash, etc.)

};

#endif

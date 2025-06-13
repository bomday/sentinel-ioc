// Base abstract class

#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <string>

class Indicator {
private:
    // Attributes
    int indicatorId;
    int severity;
    std::string type;
    std::string description;
    std::string origin;
    std::string timestamp; // Changed 'data' to 'timestamp' for consistency with your header

public:
    // Constructor
    Indicator(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~Indicator() = default;
    
    // Getter methods (pure virtual, making Indicator an abstract class)
    // These methods must be implemented by concrete derived classes
    virtual int getIndicatorId() const = 0;
    virtual int getSeverity() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getOrigin() const = 0;
    virtual std::string getTimestamp() const = 0; // Corresponds to your 'data' requirement
};

#endif // INDICATOR_HPP
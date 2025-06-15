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
    std::string timestamp;

public:
    // Constructor
    Indicator(int indicatorId, int severity, std::string type, std::string description, std::string origin, std::string timestamp);

    // Getters (Virtual Pure Methods to make the class Abstract)
    // These methods must be implemented by concrete derived classes.
    virtual int getIndicatorId() const = 0;
    virtual int getSeverity() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getOrigin() const = 0;
    virtual std::string getTimestamp() const = 0;

    // Virtual Destructor: Important for proper memory deallocation
    // when deleting derived class objects through a base class pointer.
    virtual ~Indicator() = default;
};

#endif // INDICATOR_HPP
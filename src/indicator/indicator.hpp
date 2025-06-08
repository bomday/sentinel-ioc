#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <string>
#include <memory>

class Indicator {
protected:
    std::string type;
    std::string value;
    std::string severity;
    std::string origin;
    std::string timestamp;
    std::string description;

public:
    Indicator(std::string type,
             std::string value,
             std::string severity,
             std::string origin,
             std::string timestamp,
             std::string description);
    
    virtual ~Indicator() = default;

    // Getters
    std::string getType() const { return type; }
    std::string getValue() const { return value; }
    std::string getSeverity() const { return severity; }
    std::string getOrigin() const { return origin; }
    std::string getTimestamp() const { return timestamp; }
    std::string getDescription() const { return description; }

    // Changed return type to match implementation
    static std::unique_ptr<Indicator> fromLine(const std::string& line);
};

#endif

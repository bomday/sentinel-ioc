#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <iostream>
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

    std::string getType() const { return type; }
    std::string getValue() const { return value; }
    std::string getSeverity() const { return severity; }
    std::string getOrigin() const { return origin; }
    std::string getTimestamp() const { return timestamp; }
    std::string getDescription() const { return description; }

    virtual void printInfo() const {
        std::cout << "Tipo: " << getType() << "\n"
                  << "Valor: " << getValue() << "\n"
                  << "Severidade: " << getSeverity() << "\n"
                  << "Origem: " << getOrigin() << "\n"
                  << "Data: " << getTimestamp() << "\n"
                  << "Descrição: " << getDescription() << "\n";
    }

    // Changed return type to match implementation
    static std::unique_ptr<Indicator> fromLine(const std::string& line);
};

#endif

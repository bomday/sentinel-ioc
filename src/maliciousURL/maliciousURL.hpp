#ifndef MALICIOUS_URL_HPP
#define MALICIOUS_URL_HPP

#include <string>
#include "indicator/indicator.hpp"

class MaliciousURL : public Indicator {
private:
    std::string url;
    std::string protocol;

public:
    MaliciousURL(int indicatorId, int severity, std::string type, std::string description,
                 std::string origin, std::string timestamp, std::string url, std::string protocol);

    void displayInfo() const override;  // 
    std::string getValue() const override;

    static Indicator* createMaliciousURL(int indicatorId, int severity, std::string type,
                                         std::string description, std::string origin, std::string timestamp);

    virtual ~MaliciousURL() = default;

    // Getters and Setters
    std::string getURL() const { return url; }
    std::string getProtocol() const { return protocol; }

    void setURL(const std::string& urlAddress) { url = urlAddress; }
    void setProtocol(const std::string& protocolName) { protocol = protocolName; }
};

#endif

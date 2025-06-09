#ifndef MALICIOUSURL_HPP
#define MALICIOUSURL_HPP

#include "indicator.hpp"
#include <string>

class MaliciousURL : public Indicator {
public:
    MaliciousURL(std::string value,
                 std::string severity,
                 std::string origin,
                 std::string timestamp,
                 std::string description);

    std::string getURL() const { return url; }
    std::string getProtocol() const { return protocol; }

    void setProtocol(const std::string& proto) { protocol = proto; }

    void printInfo() const;

private:
    std::string url;
    std::string protocol;
};

#endif // MALICIOUSURL_HPP

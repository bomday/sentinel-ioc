#ifndef IOC_ANALYZER_HPP
#define IOC_ANALYZER_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>

// Update the path below if indicator.hpp is in a subfolder, e.g., "indicator/indicator.hpp"
#include "indicator/indicator.hpp"

// Forward declarations
class MaliciousIP;
class MaliciousURL;
class MaliciousHash;

class IOCAnalyzer {
private:
    std::vector<std::unique_ptr<Indicator>> indicatorsList;  // Store smart pointers to indicators

public:
    IOCAnalyzer();

    // Add an indicator
    void addIndicator(std::unique_ptr<Indicator> indicator);

    // List all indicators
    void listIndicators() const;

    // Access to indicators list (if needed by FileManager, for example)
    std::vector<std::unique_ptr<Indicator>>& getIndicators();

    ~IOCAnalyzer();
};

#endif // IOC_ANALYZER_HPP

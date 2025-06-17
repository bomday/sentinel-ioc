#include "IOCAnalyzer/IOCAnalyzer.hpp"
#include "indicator/indicator.hpp"
#include "maliciousIP/maliciousIP.hpp"
#include "maliciousURL/maliciousURL.hpp"
#include "maliciousHash/maliciousHash.hpp"

#include <iostream>
#include <iomanip>



#include <iostream>
#include <iomanip>
#include <string>

IOCAnalyzer::IOCAnalyzer() {
    // Optional future initialization (e.g., load from file)
}

IOCAnalyzer::~IOCAnalyzer() {
    // No manual deletion needed when using unique_ptr
}

void IOCAnalyzer::addIndicator(std::unique_ptr<Indicator> indicator) {
    if (indicator != nullptr) {
        indicatorsList.push_back(std::move(indicator));
        std::cout << "✅ Indicator added successfully!\n";
    } else {
        std::cout << "⚠️ Attempted to add a null indicator. Operation cancelled.\n";
    }
}

void IOCAnalyzer::listIndicators() const {
    std::cout << "\n🛡️ --- LISTING REGISTERED IOCs --- 🛡️\n";
    std::cout << "-------------------------------------------\n";

    if (indicatorsList.empty()) {
        std::cout << "⚠️ No IOCs registered yet.\n";
        std::cout << "-------------------------------------------\n";
        return;
    }

    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(10) << "Type"
              << std::setw(12) << "Severity"
              << std::setw(25) << "Value"
              << std::setw(15) << "Origin"
              << std::setw(20) << "Timestamp"
              << std::setw(40) << "Description"
              << std::endl;

    std::cout << std::string(150, '-') << std::endl;

    for (const auto& ioc : indicatorsList) {
        std::cout << std::left
                  << std::setw(8)  << ioc->getIndicatorId()
                  << std::setw(10) << ioc->getType()
                  << std::setw(12) << ioc->getSeverity()
                  << std::setw(25) << ioc->getValue()  // Derived classes must implement this!
                  << std::setw(15) << ioc->getOrigin()
                  << std::setw(20) << ioc->getTimestamp()
                  << std::setw(40) << ioc->getDescription()
                  << std::endl;

        std::cout << std::string(150, '-') << std::endl;
    }

    std::cout << "\n✅ IOC listing complete.\n";
    std::cout << "-------------------------------------------\n";
}

std::vector<std::unique_ptr<Indicator>>& IOCAnalyzer::getIndicators() {
    return indicatorsList;
}

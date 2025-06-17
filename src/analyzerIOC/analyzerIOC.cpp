// Class containing CRUD and analysis
#include "analyzerIOC.hpp" // Include the header file for AnalyzerIOC
#include "indicator.hpp"   // Include Indicator base class, as we manage its pointers
#include "maliciousIP.hpp" // Include derived classes for example usage/testing
#include "maliciousURL.hpp"
#include "maliciousHash.hpp"

#include <iostream>  // For std::cout, std::endl
#include <iomanip>   // For std::setw, std::left (output formatting)
#include <string>    // For std::string manipulations

// Constructor implementation
// Initializes the AnalyzerIOC object. Nothing specific needed here for now,
// as the std::vector indicatorsList is default-constructed.
analyzerIOC::analyzerIOC() {
    // You can add initialization logic here if needed in the future,
    // e.g., loading indicators from a file.
}

// Destructor implementation
// This is CRUCIAL for memory management. It iterates through the
// indicatorsList and deletes each dynamically allocated Indicator object
// to prevent memory leaks.
analyzerIOC::~analyzerIOC() {
    for (Indicator* ioc : indicatorsList) {
        delete ioc; // Free the memory allocated for each Indicator object
    }
    indicatorsList.clear(); // Clear the vector after deleting objects
}

// Function to add an Indicator to the list
// Takes a pointer to an Indicator object and adds it to the internal vector.
// This function relies on the caller (e.g., your createIndicator function)
// to have dynamically allocated the Indicator object using 'new'.
void analyzerIOC::addIndicator(Indicator* indicator) {
    if (indicator != nullptr) {
        indicatorsList.push_back(indicator);
        std::cout << "✅ Indicator added successfully!\n";
    } else {
        std::cout << "⚠️ Attempted to add a null indicator. Operation cancelled.\n";
    }
}

// Function to list all registered Indicators
// Displays detailed information about each IOC in a formatted, readable way.
void analyzerIOC::listIndicators() const {
    std::cout << "\n🛡️ --- LISTING REGISTERED IOCs --- 🛡️\n";
    std::cout << "-------------------------------------------\n";

    // Check if the list of IOCs is empty.
    if (indicatorsList.empty()) {
        std::cout << "⚠️ No IOCs registered yet.\n";
        std::cout << "-------------------------------------------\n";
        return; // Exit the function if no IOCs are present
    }

    // If there are IOCs, iterate and display them.
    // Using std::setw for column formatting and std::left for left alignment.

    // Table Header
    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(10) << "Type"
              << std::setw(12) << "Severity"
              << std::setw(25) << "Value" // Placeholder for value (IP/URL/Hash specific)
              << std::setw(15) << "Origin"
              << std::setw(20) << "Timestamp"
              << std::setw(40) << "Description"
              << std::endl;
    std::cout << std::string(150, '-') << std::endl; // Separator line for the header

    for (const auto& ioc : indicatorsList) { // Iterate over pointers in the vector
        // Display properties in a formatted, tabular way.
        // We'll use getValor() here, assuming it's implemented in derived classes
        // and provides the appropriate IP, URL, or Hash string.
        std::cout << std::left
                  << std::setw(8)  << ioc->getIndicatorId()
                  << std::setw(10) << ioc->getType()
                  << std::setw(12) << ioc->getSeverity()
                  << std::setw(25) << ioc->getValue() // Assuming a getValue() method exists
                  << std::setw(15) << ioc->getOrigin()
                  << std::setw(20) << ioc->getTimestamp()
                  << std::setw(40) << ioc->getDescription()
                  << std::endl;

        // Visually separate each IOC with a line.
        std::cout << std::string(150, '-') << std::endl;
    }

    std::cout << "\n✅ IOC listing complete.\n";
    std::cout << "-------------------------------------------\n";
}
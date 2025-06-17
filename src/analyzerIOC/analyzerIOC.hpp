// Class containing CRUD and analysis
#ifndef ANALYZER_IOC_HPP
#define ANALYZER_IOC_HPP

#include <vector>     // For std::vector
#include <string>     // For std::string
#include <iostream>   // For std::cout (though primarily used in .cpp, it's good practice for output functions)
#include <iomanip>    // For output formatting (std::setw, std::left)

#include "indicator.hpp" // Include the base abstract class Indicator

// Forward declarations of derived classes.
// This is a good practice to avoid circular dependencies if these classes
// also include AnalyzerIOC.hpp. We only need their declaration here.
class MaliciousIP;
class MaliciousURL;
class MaliciousHash;

class analyzerIOC {
private:
    std::vector<Indicator*> indicatorsList; // The vector to store pointers to Indicator objects

public:
    // Constructor
    analyzerIOC();

    // Function to add an Indicator to the list
    void addIndicator(Indicator* indicator);

    // Function to list all Indicators
    void listIndicators() const; // Marked const as it only reads data

    // Destructor to free dynamically allocated memory
    ~analyzerIOC();
};

#endif // ANALYZER_IOC_HPP
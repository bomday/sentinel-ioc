#ifndef ENHANCED_INTERFACE_HPP
#define ENHANCED_INTERFACE_HPP

#include <string>
#include <vector>
#include "../indicatorManager/indicatorManager.hpp"

class EnhancedInterface {
private:
    IndicatorManager* manager;
    
    // Color constants for terminal output
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string BOLD;
    
    void clearScreen();
    void showBanner();
    void showMainMenu();
    void showSearchMenu();
    void showStatistics();
    void showHelp();
    void waitForEnter();
    
public:
    EnhancedInterface(IndicatorManager* mgr);
    ~EnhancedInterface() = default;
    
    void run();
    void displayColorfulTable();
    void displayThreatMatrix();
};

#endif

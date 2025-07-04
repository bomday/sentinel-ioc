#ifndef CLI_HPP
#define CLI_HPP

#include "../indicatorManager/indicatorManager.hpp"
#include <string>

class CLI {
private:
    IndicatorManager manager;
    bool wasModified;

    // Private helper methods for handling user interactions
    void showMenu();
    void handleAdd();
    void handleList();
    void handleSearch();
    void handleEdit();
    void handleDelete();
    void handleStatistics();
    
    // Helper methods for input validation and collection
    int getValidIntInput(const std::string& prompt, int min = INT_MIN, int max = INT_MAX);
    std::string getStringInput(const std::string& prompt);
    bool getConfirmation(const std::string& prompt);
    
    // Menu display helpers
    void displayEditMenu(const std::string& type);
    void displaySearchMenu();

public:
    CLI();
    ~CLI() = default;
    
    // Main entry point for the CLI
    void runMenu();
};

#endif

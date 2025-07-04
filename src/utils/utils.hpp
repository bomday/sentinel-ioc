#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// Utility functions
std::string getTimestamp();
void showMenu();
void listIndicators();
bool icontains(const std::string& str, const std::string& substr);
std::string toLower(const std::string& str);
bool registerLastMonth(const std::string& timestamp);
bool isCurrentMonth(const std::string& timestamp);

#endif
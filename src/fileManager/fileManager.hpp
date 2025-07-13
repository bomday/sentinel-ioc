#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP
#include <vector>
#include <memory>
#include <string>
#include "../indicator/indicator.hpp"

class FileManager {
public:
    // Save and load methods for indicators
    static bool saveData(const std::string& path, const std::vector<std::unique_ptr<Indicator>>& indicators);
    // Load method to read indicators from a file
    static std::vector<std::unique_ptr<Indicator>> loadData(const std::string& path);
};

#endif 

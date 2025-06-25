#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP
#include <vector>
#include <memory>
#include <string>
#include "../indicator/indicator.hpp"

class FileManager {
public:
    static bool saveData(const std::string& path, const std::vector<std::unique_ptr<Indicator>>& indicators);
    static std::vector<std::unique_ptr<Indicator>> loadData(const std::string& path);
};

#endif 

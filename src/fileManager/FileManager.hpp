#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <vector>
#include <string>
#include <memory>
#include "../indicator/Indicator.hpp"

class FileManager {
public:
    static bool saveIOCs(const std::vector<std::unique_ptr<Indicator>>&, const std::string&);
    static std::vector<std::unique_ptr<Indicator>> loadIOCs(const std::string&);

};

#endif

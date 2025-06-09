#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../indicator/indicator.hpp"
#include <vector>
#include <memory>
#include <string>

class Indicator;

class System {
private:
    std::vector<std::unique_ptr<Indicator>> iocList;

public:
    void loadIOCsFromFile(const std::string& filename);

    void listIOCs() const;


};

#endif // SISTEMA_IOC_HPP

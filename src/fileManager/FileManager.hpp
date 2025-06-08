#ifndef GERENCIADOR_DE_ARQUIVO_HPP
#define GERENCIADOR_DE_ARQUIVO_HPP

#include <vector>
#include <string>

#include "../indicator/Indicator.hpp"

class FileManager {
public:
    static bool saveIOCs(const std::vector<Indicator*>& iocs, const std::string& filePath);
    static std::vector<Indicator*> loadIOCs(const std::string& filePath);
};

#endif

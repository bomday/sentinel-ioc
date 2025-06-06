#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "analyzerIOC.hpp"
#include "indicator.hpp"
#include "maliciousHash.hpp"
#include "maliciousIP.hpp"
#include "maliciousURL.hpp"
#include "utils.hpp"

std::vector<Indicator*> indicators;

// Indicator creation    
Indicator createIndicator() {
    int id, severity; // ID and severity declarations
    std::string type, description, origin, timestamp; // Type, description, origin, and timestamp declarations

    std::cout << "Digite o tipo do IOC: ";
    std::getline(std::cin, type); // Using getline to allow spaces in type input (get the entire input line)

    std::cout << "Agora digite a severidade (1 - 5): ";
    std::cin >> severity;
    std::cin.ignore(); // Using ignore to empty the enter buffer for getline (delete the '/n ' left by std::cin)

    /*std::cout << "Digite o ID do indicador: ";
    std::cin >> id;*/

    std::cout << "Digite a origem: ";
    std::getline(std::cin, origin);

    timestamp = getTimestamp(); // Get the date from the subfunction

    std::cout << "Digite a descrição: ";
    std::getline(std::cin, description);

    // Mudar para logica de criação de URL, IP ou Hash malicioso
    Indicator* ioc = new Indicator(id, severity, type, description, origin, timestamp); // Create the indicator object
    
    indicators.push_back(ioc); // Add the indicator to the vector
    std::cout << "IOC criado com sucesso!\n";
}

// Subfunction to obtain the current timestamp in a formatted string
std::string getTimestamp() {
    auto nowTime = std::chrono::system_clock::now(); // Get the current time point with system clock from chrono
    std::time_t todayTime = std::chrono::system_clock::to_time_t(nowTime); // Convert to time_t for formatting
    std::ostringstream oss; // Create an output string stream for formatting
    oss << std::put_time(std::localtime(&todayTime), "%d-%m-%Y"); // Format: DD-MM-YYYY in string type
    return oss.str();
}

int main() {
    Indicator newIndicator = createIndicator(); // Call the function newIndicator
    return 0;
}
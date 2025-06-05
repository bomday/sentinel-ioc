#include <iostream>
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

// Function to obtain the current timestamp in a formatted string
std::string obterTimestamp() {
    auto agora = std::chrono::system_clock::now(); // Get the current time point with system clock
    std::time_t tempoAtual = std::chrono::system_clock::to_time_t(agora); // Convert to time_t for formatting
    std::ostringstream oss; // Create an output string stream for formatting
    oss << std::put_time(std::localtime(&tempoAtual), "%d-%m-%Y"); // Format: DD-MM-YYYY
    return oss.str();
}
    
int main() {
    // Indicator creation
    int id, severity; // ID and severity declarations
    std::string type, description, origin, timestamp; // Type, description, origin, and timestamp declarations

    std::cout << "Digite o tipo do IOC: "; // Prompt for type
    std::getline(std::cin, type); // Using getline to allow spaces in type input

    std::cout << "Agora digite a severidade (Alta, Média ou Baixa): "; // Prompt for severity
    std::cin >> severity;
    std::cin.ignore(); // Ignore the newline to empty the enter buffer for getline

    /*std::cout << "Digite o ID do indicador: "; // Prompt for ID
    std::cin >> id;*/

    std::cout << "Digite a origem: "; // Prompt for origin
    std::getline(std::cin, origin);

    std::string timestamp = obterTimestamp(); // Get the date

    std::cout << "Digite a descrição: "; // Prompt for description
    std::getline(std::cin, description);

    Indicator meuIndicador(id, severity, type, description, origin, timestamp);

    return 0;
}

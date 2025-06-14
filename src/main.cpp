#include "analyzerIOC.hpp"     // Your main class for managing IOCs
#include "indicator.hpp"       // Base abstract class
#include "maliciousHash.hpp"   // Derivative class for malicious hashes
#include "maliciousIP.hpp"     // Derivative class for malicious IPs
#include "maliciousURL.hpp"    // Derivative class for malicious URLs
#include "utils.hpp"           // Utility functions (e.g., for loading from CSV)
#include <iostream>            // For input/output operations (std::cout, std::cin)
#include <limits>              // For std::numeric_limits, used with std::cin.ignore

// Function to display the main menu options to the user

void displayMenu() {
    std::cout << "\n--- Sentinel IOC - Menu Principal ---\n";
    std::cout << "1. Adicionar IOC (Exemplo)\n"; // Placeholder for future full implementation
    std::cout << "2. Listar IOCs Cadastrados\n";
    std::cout << "3. Sair\n";
    std::cout << "Escolha uma opção: ";
}

int main() {
    // Create an instance of your AnalyzerIOC system
    // This object will manage all your Indicators of Compromise
    AnalyzerIOC analyzer;

    // --- Example: Adding some IOCs for testing the list functionality ---
    // In a real application, these would come from user input or a database/file.
    // We're using 'new' here, so remember AnalyzerIOC's destructor will free this memory.

    // Add a MaliciousIP
    analyzer.addIOC(new MaliciousIP(
        1,         // indicatorId
        8,         // severity (e.g., on a scale of 1-10)
        "IP",      // type
        "Tentativa de ataque de força bruta detectada.", // description
        "Firewall Logs", // origin
        "2023-10-26 14:30:00", // timestamp
        "192.168.1.100", // specific IP attribute
        "Brazil",        // specific country attribute
        "TelecomCorp"    // specific ISP attribute
    ));

    // Add a MaliciousURL
    analyzer.addIOC(new MaliciousURL(
        2,
        9,
        "URL",
        "Link de phishing direcionado para roubo de credenciais.",
        "Email Phishing Campaign",
        "2023-11-01 09:15:00",
        "https://evil.example.com/login-verify", // specific URL attribute
        "Phishing" // specific type of threat
    ));

    // Add a MaliciousHash
    analyzer.addIOC(new MaliciousHash(
        3,
        7,
        "Hash",
        "Hash de arquivo EXE associado a ransomware recente.",
        "Threat Intelligence Feed",
        "2023-11-05 16:00:00",
        "a1b2c3d4e5f67890a1b2c3d4e5f67890a1b2c3d4e5f67890a1b2c3d4e5f67890", // specific hash attribute
        "SHA256" // specific algorithm attribute
    ));
    // --- End of Example IOCs ---

    int choice; // Variable to store user's menu choice
    do {
        displayMenu(); // Show the menu to the user
        std::cin >> choice; // Get user's input

        // Clear the input buffer to prevent issues with subsequent inputs (like std::getline)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                std::cout << "\n🛠️ Funcionalidade 'Adicionar IOC' ainda não implementada para entrada interativa. IOCs foram adicionados automaticamente para demonstração.\n";
                // In a full implementation, you'd prompt the user for IOC details here
                // and then call analyzer.addIOC(new SpecificIOCType(...));
                break;
            case 2:
                analyzer.listIOCs(); // Call the function to list all registered IOCs
                break;
            case 3:
                std::cout << "\n👋 Saindo do programa. Até mais!\n";
                break;
            default:
                std::cout << "\n❌ Opção inválida. Por favor, escolha uma opção válida do menu.\n";
                break;
        }
    } while (choice != 3); // Loop until the user chooses to exit

    // The 'analyzer' object will go out of scope here, and its destructor will be called,
    // freeing the memory allocated for the IOCs.

    return 0; // Indicate successful program execution
}

#include "IOCFactory.hpp"
#include "maliciousIP/maliciousIP.hpp"
#include "maliciousURL/maliciousURL.hpp"
#include "maliciousHash/maliciousHash.hpp"
#include "utils/utils.hpp"

#include <iostream>
#include <limits>
#include <memory>

namespace IOCFactory {

    std::unique_ptr<Indicator> createIOC() {
        int severity;
        std::string type, description, origin, timestamp;

        // Ask for IOC type
        while (true) {
            std::cout << "\nDigite o tipo do IOC (IP, URL ou Hash): ";
            std::getline(std::cin, type);

            if (type == "IP" || type == "URL" || type == "Hash") {
                break;
            } else {
                std::cout << "\nTipo inválido. Por favor, digite IP, URL ou Hash.";
            }
        }

        // Ask for severity
        while (true) {
            std::cout << "\nAgora digite a severidade (1 - 5): ";
            std::cin >> severity;

            if (std::cin.fail() || severity < 1 || severity > 5) {
                std::cout << "\nSeveridade inválida. Por favor, digite um valor entre 1 e 5.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }

        // Common fields
        std::cout << "\nDigite a origem: ";
        std::getline(std::cin, origin);

        timestamp = getTimestamp();

        std::cout << "\nDigite a descrição: ";
        std::getline(std::cin, description);

        int indicatorId = generateUniqueId();

        if (type == "IP") {
            std::string ipAddress, country, isp;

            std::cout << "Digite o endereço IP: ";
            std::getline(std::cin, ipAddress);

            std::cout << "Digite o país de origem: ";
            std::getline(std::cin, country);

            std::cout << "Digite o ISP: ";
            std::getline(std::cin, isp);

            return std::make_unique<MaliciousIP>(indicatorId, severity, type, description, origin, timestamp, ipAddress, country, isp);
        } 
        else if (type == "URL") {
            std::string url, protocol;

            std::cout << "Digite a URL: ";
            std::getline(std::cin, url);

            std::cout << "Digite o protocolo: ";
            std::getline(std::cin, protocol);

            return std::make_unique<MaliciousURL>(indicatorId, severity, type, description, origin, timestamp, url, protocol);
        } 
        else if (type == "Hash") {
            std::string hashValue, algorithm;

            std::cout << "Digite o valor do Hash: ";
            std::getline(std::cin, hashValue);

            std::cout << "Digite o algoritmo: ";
            std::getline(std::cin, algorithm);

            return std::make_unique<MaliciousHash>(indicatorId, severity, type, description, origin, timestamp, hashValue, algorithm);
        }

        return nullptr;  // Fallback
    }

}

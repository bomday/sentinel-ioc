// Base abstract class
#include "indicator.hpp"
#include "utils.hpp" // Assumindo que generateUniqueId() e getTimestamp() estão aqui
#include "maliciousIP.hpp" // Inclua MaliciousIP
#include "maliciousURL.hpp" // Inclua MaliciousURL
#include "maliciousHash.hpp" // Inclua MaliciousHash
#include <iostream>
#include <vector>
#include <limits> // Necessário para std::numeric_limits

// Constructor
// Implementação do construtor da classe base abstrata Indicator
Indicator::Indicator(int indicatorId,
                     int severity,
                     std::string type,
                     std::string description,
                     std::string origin,
                     std::string timestamp)
    : indicatorId(indicatorId),
      severity(severity),
      type(type),
      description(description),
      origin(origin),
      timestamp(timestamp) {
    // O construtor apenas inicializa os membros de dados da classe base.
    // Não há implementação para os getters aqui, pois eles são puros virtuais.
}

// Declaração de um vetor global para armazenar os indicadores.
// Em um sistema real, este vetor provavelmente estaria dentro de uma classe gerenciadora (ex: AnalisadorIOC).
std::vector<Indicator*> indicators;

// Função para criar um novo indicador, solicitando entrada do usuário.
// Esta função é uma fábrica para diferentes tipos de indicadores.
// Em um design mais puro de OO, esta lógica poderia estar em uma classe "IndicatorFactory".
Indicator* createIndicator() {
    int severity; // Declaração da severidade
    std::string type, description, origin, timestamp; // Declarações para tipo, descrição, origem e timestamp

    // Solicita o tipo de IOC ao usuário
    while (true) {
        std::cout << "\nDigite o tipo do IOC (IP, URL ou Hash): ";
        std::getline(std::cin, type);

        if (type == "IP" || type == "URL" || type == "Hash") {
            break; // Sai do loop se o tipo for válido
        } else {
            std::cout << "\nTipo inválido. Por favor, digite IP, URL ou Hash.";
        }
    }

    // Solicita a severidade ao usuário
    while (true) {
        std::cout << "\nAgora digite a severidade (1 - 5): ";
        std::cin >> severity;

        // Verifica se a entrada foi um número válido e se está no intervalo correto
        if (std::cin.fail() || severity < 1 || severity > 5) {
            std::cout << "\nSeveridade inválida. Por favor, digite um valor entre 1 e 5.\n";
            std::cin.clear(); // Limpa o erro do std::cin
            // Ignora o restante da linha de entrada para evitar loop infinito com entrada inválida
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            // Limpa o buffer de entrada para evitar problemas com std::getline subsequente
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break; // Sai do loop se a severidade for válida
        }
    }

    std::cout << "\nDigite a origem: ";
    std::getline(std::cin, origin);

    // Obtém o timestamp da função utilitária
    timestamp = getTimestamp();

    std::cout << "\nDigite a descrição: ";
    std::getline(std::cin, description);

    // Obtém um ID único para o indicador
    int indicatorId = generateUniqueId();

    // Cria um objeto MaliciousIP se o tipo for IP
    if (type == "IP") {
        // Assume que MaliciousIP::createMaliciousIP é um método estático que retorna um Indicator*
        Indicator* newIP = MaliciousIP::createMaliciousIP(indicatorId, severity, type, description, origin, timestamp);
        indicators.push_back(newIP);
        return newIP;
    }
    // Cria um objeto MaliciousURL se o tipo for URL
    else if (type == "URL") {
        // Assume que MaliciousURL::createMaliciousURL é um método estático que retorna um Indicator*
        Indicator* newURL = MaliciousURL::createMaliciousURL(indicatorId, severity, type, description, origin, timestamp);
        indicators.push_back(newURL);
        return newURL;
    }
    // Cria um objeto MaliciousHash se o tipo for Hash
    else if (type == "Hash") {
        // Assume que MaliciousHash::createMaliciousHash é um método estático que retorna um Indicator*
        Indicator* newHash = MaliciousHash::createMaliciousHash(indicatorId, severity, type, description, origin, timestamp);
        indicators.push_back(newHash);
        return newHash;
    }

    return nullptr; // Retorna nullptr se o tipo não for reconhecido
}
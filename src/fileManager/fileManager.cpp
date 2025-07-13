#include "fileManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"
#include "../maliciousHash/maliciousHash.hpp"

// Save the indicators to a CSV file
bool FileManager::saveData(const std::string& path, const std::vector<std::unique_ptr<Indicator>>& indicators) {
    // Check if the path is valid
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file for writing: " << path << std::endl;
        return false;
    }

    // Write the header to the CSV file
    // The header includes all the fields that will be saved in the CSV
    file << "indicatorId,severity,type,description,origin,timestamp,"
         << "hash,algorithm,ip,country,isp,url,protocol\n";
    // Iterate through the indicators and write each one to the file
    for (const auto& ioc : indicators) {
        file << ioc->toCSV() << '\n';
    }

    file.close();
    return true;
}

// Load the indicators from a CSV file
std::vector<std::unique_ptr<Indicator>> FileManager::loadData(const std::string& path) {
    // Check if the path is valid and open the file
    std::vector<std::unique_ptr<Indicator>> indicators;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file for reading: " << path << std::endl;
        return indicators;
    }
    // Read the file line by line, skipping the header
    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        // Improved CSV parsing to handle trailing empty fields and empty lines
        if (line.empty()) {
            continue;  // Skip empty lines
        }

        std::vector<std::string> tokens;
        std::string token;
        std::istringstream ss(line);
        
        // Parse CSV with proper handling of trailing commas
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        // If line ends with comma, add empty token for the trailing field
        if (!line.empty() && line.back() == ',') {
            tokens.push_back("");
        }

        try {
            // Ensure we have enough tokens to parse the IOC
            if (tokens.size() < 6) {
                std::cerr << "[WARN] Incomplete line. Skipping: " << line << "\n";
                continue;
            }
            // Parse the tokens into the IOC fields
            int id = std::stoi(tokens[0]);
            int severity = std::stoi(tokens[1]);
            std::string type = tokens[2];
            std::string desc = tokens[3];
            std::string origin = tokens[4];
            std::string timestamp = tokens[5];

            // Normalize type
            std::string normalizedType;
            if (type == "MaliciousHash") normalizedType = "Hash";
            else if (type == "MaliciousIP") normalizedType = "IP";
            else if (type == "MaliciousURL") normalizedType = "URL";
            else if (type == "Hash" || type == "IP" || type == "URL") normalizedType = type;
            else {
                std::cerr << "[WARN] Unknown IOC type: " << type << ". Skipping line: " << line << "\n";
                continue;
            }
            // Create the appropriate IOC based on the normalized type
            if (normalizedType == "Hash" && tokens.size() >= 8) {
                std::string hash = tokens[6];
                std::string algorithm = tokens[7];
                indicators.push_back(std::make_unique<MaliciousHash>(
                    id, severity, normalizedType, desc, origin, timestamp, hash, algorithm));
            } else if (normalizedType == "IP" && tokens.size() >= 11) {
                std::string ip = tokens[8];
                std::string country = tokens[9];
                std::string isp = tokens[10];
                indicators.push_back(std::make_unique<MaliciousIP>(
                    id, severity, normalizedType, desc, origin, timestamp, ip, country, isp));
            } else if (normalizedType == "URL" && tokens.size() >= 13) {
                std::string url = tokens[11];
                std::string protocol = tokens[12];
                indicators.push_back(std::make_unique<MaliciousURL>(
                    id, severity, normalizedType, desc, origin, timestamp, url, protocol));
            } else {
                std::cerr << "[WARN] Missing fields for type: " << normalizedType << ". Skipping line: " << line << "\n";
            }

        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse line: " << line << "\n";
            std::cerr << "        Reason: " << e.what() << "\n";
            continue;
        }
    }

    std::cout << "[DEBUG] Loaded " << indicators.size() << " IOCs from file.\n";
    return indicators;
}

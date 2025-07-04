#ifndef INDICATOR_MANAGER_HPP
#define INDICATOR_MANAGER_HPP

#include "../indicator/indicator.hpp"
#include <vector>
#include <memory>
#include <map>

// manages all the indicators
class IndicatorManager {
    private:
        std::vector<std::unique_ptr<Indicator>> indicators; 
        int nextIndicatorId = 1; 

        void updateNextId(); // Updates the next available ID based on existing indicators
        int getNextId(); // Returns the next available ID for a new indicator
    public:
        // Virtual destructor
        virtual ~IndicatorManager() = default;
        
        // Core CRUD operations (no UI logic)
        void addMaliciousIP(int severity, const std::string& type, const std::string& description, 
                           const std::string& origin, const std::string& timestamp, const std::string& ip, 
                           const std::string& country, const std::string& isp);
        void addMaliciousURL(int severity, const std::string& type, const std::string& description, 
                            const std::string& origin, const std::string& timestamp, const std::string& url, 
                            const std::string& protocol);
        void addMaliciousHash(int severity, const std::string& type, const std::string& description, 
                             const std::string& origin, const std::string& timestamp, const std::string& hash, 
                             const std::string& algorithm);
        
        // Read operations
        void listIndicators() const;
        Indicator* findIndicatorById(int id) const;
        bool isEmpty() const;
        
        // Update operations
        bool updateIndicatorSeverity(int id, int severity);
        bool updateIndicatorDescription(int id, const std::string& description);
        bool updateIndicatorOrigin(int id, const std::string& origin);
        bool updateIndicatorTimestamp(int id, const std::string& timestamp);
        bool updateMaliciousIPAddress(int id, const std::string& ip);
        bool updateMaliciousIPCountry(int id, const std::string& country);
        bool updateMaliciousIPISP(int id, const std::string& isp);
        bool updateMaliciousURL(int id, const std::string& url);
        bool updateMaliciousURLProtocol(int id, const std::string& protocol);
        bool updateMaliciousHash(int id, const std::string& hash);
        bool updateMaliciousHashAlgorithm(int id, const std::string& algorithm);
        
        // Delete operations
        bool removeIndicatorById(int id);
        
        // Search operations
        void searchByValue(const std::string& value) const;
        void searchByType(const std::string& type) const;
        void searchBySeverity(int severity) const;
        void searchByOrigin(const std::string& origin) const;
        void searchByDate(const std::string& date) const;
        void searchByDescription(const std::string& description) const;
        
        // Search operations that return results (for GUI)
        std::vector<const Indicator*> findByValue(const std::string& value) const;
        std::vector<const Indicator*> findByType(const std::string& type) const;
        std::vector<const Indicator*> findBySeverity(int severity) const;
        std::vector<const Indicator*> findByOrigin(const std::string& origin) const;
        std::vector<const Indicator*> findByDate(const std::string& date) const;
        std::vector<const Indicator*> findByDescription(const std::string& description) const;
        std::vector<const Indicator*> searchWithFilters(const std::string& value, const std::string& type, 
                                                        int severity, const std::string& dateFrom, 
                                                        const std::string& dateTo) const;
        
        // Utility methods
        void printIOC(const Indicator* ioc, int index) const;
        void saveIndicatorsToFile(const std::string& filename);
        void loadIndicatorsFromFile(const std::string& filename);
        void generateStatistics() const;
        
        // GUI helper methods
        size_t getIndicatorCount() const { return indicators.size(); }
        const Indicator* getIndicator(size_t index) const {
            return (index < indicators.size()) ? indicators[index].get() : nullptr;
        }
        int getIOCsRegisteredLastMonth() const;
        
        // Statistics helper methods for GUI
        std::map<std::string, int> getTypeDistribution() const;
        std::map<int, int> getSeverityDistribution() const;
        int getIOCsRegisteredCurrentMonth() const;
        int getCriticalThreatCount() const;
};

#endif
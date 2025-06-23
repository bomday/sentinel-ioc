#ifndef INDICATOR_MANAGER_HPP
#define INDICATOR_MANAGER_HPP

#include "Indicator.hpp"
#include <vector>
#include <memory>

// manages all the indicators
class IndicatorManager {
    private:
        std::vector<std::unique_ptr<Indicator>> indicators; 
    
    public:
        // Virtual destructor
        virtual ~IndicatorManager() = default;
        
        // Methods to manage indicators
        void createIndicator();
        void listIndicators() const;
        void editIndicatorById(int id);
        void removeIndicatorById(int id);
};

#endif
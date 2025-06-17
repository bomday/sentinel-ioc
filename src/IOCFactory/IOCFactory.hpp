#ifndef IOC_FACTORY_HPP
#define IOC_FACTORY_HPP

#include <memory>
#include "indicator/indicator.hpp"

namespace IOCFactory {
    std::unique_ptr<Indicator> createIOC();
}

#endif

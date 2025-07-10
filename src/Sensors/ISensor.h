#pragma once
#include <etl/observer.h>
#include <config.h>
#include "StaticVirtual.h"


/**
 * @brief an interface for Sensorm, utilizes static polimorfizem (CRTP)
 */
template <typename Derived, typename T>
class ISensor : public etl::observable<etl::observer<T>, MAX_OBSERVERS_PER_SENSOR>{
public:
    typedef etl::observer<T> observer;
    // Method to read the sensor value
    SV_METHOD0(Derived, T, read);
};


#pragma once

#include <Arduino.h>
#include "../ISensor.h"

class PresenceSensor : public ISensor<PresenceSensor, bool> {
private:
    uint8_t _pin         = -1;      // Pin number for the presence sensor
    bool    _activeState = HIGH; 
public:
    // Constructor
    inline void begin(uint8_t pin, bool activeState = HIGH);

    // Method to read the sensor value
    inline bool read();
};

inline void PresenceSensor::begin(uint8_t pin, bool activeState) {
    _pin = pin;
    _activeState = activeState;
    pinMode(_pin, INPUT);
    ESP_LOGD("PresenceSensor", "PresenceSensor %p was created with pin %d", this, _pin);
}

inline bool PresenceSensor::read(){
    return digitalRead(_pin) == _activeState;
}
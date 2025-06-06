// LimitSwitch.cpp
#include "LimitSwitch.h"

LimitSwitch::LimitSwitch(uint8_t pin, int mode)
    : _pin(pin), _state(false), _lastInterruptTick(0) {
    pinMode(_pin, INPUT_PULLDOWN); // or INPUT_PULLUP depending on wiring
    attachInterruptArg(digitalPinToInterrupt(_pin), isrHandler, this, mode);
}

LimitSwitch::~LimitSwitch() {
    detachInterrupt(_pin);
}

bool LimitSwitch::isPressed() const {
    return _state;
}

void IRAM_ATTR LimitSwitch::isrHandler(void* arg) {
    static_cast<LimitSwitch*>(arg)->onInterrupt();
}

void IRAM_ATTR LimitSwitch::onInterrupt() {
    TickType_t now = xTaskGetTickCountFromISR();

    if ((int32_t)(now - _lastInterruptTick) < debounceTicks) {
        return;  // still inside debounce window
    }
    _lastInterruptTick = now;

    SwitchState newState = toSwitchState(digitalRead(_pin));  // assumes ISR-safe on ESP32
    if (newState != _state) {
        _state = newState;
        LimitSwitchEvent evt = { newState, id };
        notifyFromISR(evt);
    }
}



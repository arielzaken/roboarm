#include "LimitSwitch.h"

LimitSwitch::LimitSwitch(uint8_t pin, int interruptMode)
    : _pin(pin), _state(false), _cb(nullptr)
{
    // Configure the pin: idle = LOW, pressed = HIGH
    pinMode(_pin, INPUT_PULLDOWN);

    // Use attachInterruptArg to pass 'this' into the static ISR.
    // On ESP32/Arduino: attachInterruptArg(pin, callback, arg, mode)
    attachInterruptArg(_pin, LimitSwitch::isrStatic, this, interruptMode);
}

LimitSwitch::~LimitSwitch() {
    detachInterrupt(_pin);
}

bool LimitSwitch::isPressed() const {
    return _state;
}

void LimitSwitch::setCallback(std::function<void(bool)> cb) {
    _cb = std::move(cb);
}

void LimitSwitch::handleInterrupt() {
    // Read new state: pressed when HIGH
    bool newState = (digitalRead(_pin) == HIGH);
    if (newState != _state) {
        _state = newState;
        if (_cb) {
            _cb(_state);
        }
    }
}

void IRAM_ATTR LimitSwitch::isrStatic(void* arg) {
    // Cast the argument back to our instance and invoke the member handler
    LimitSwitch* self = reinterpret_cast<LimitSwitch*>(arg);
    self->handleInterrupt();
}

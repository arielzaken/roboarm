#pragma once
#include <Arduino.h>
#include "ILimitSwitch.h"
#include <functional>

/// Concrete ILimitSwitch using a digital pin + attachInterruptArg.
/// Assumes: idle=LOW, pressed=HIGH.
class LimitSwitch : public ILimitSwitch {
public:
    /// @param pin           Arduino GPIO pin number
    /// @param interruptMode CHANGE, RISING, FALLING, etc.
    LimitSwitch(uint8_t pin, int interruptMode);
    virtual ~LimitSwitch();

    bool isPressed() const override;
    void setCallback(std::function<void(bool)> cb) override;

    /// Called from the static ISR wrapper (non‐member) to do the actual work
    void handleInterrupt();

private:
    uint8_t _pin;
    volatile bool _state;
    std::function<void(bool)> _cb;

    /// This static ISR wrapper simply casts the `void*` argument back to LimitSwitch*
    /// and calls its member function.
    static void IRAM_ATTR isrStatic(void* arg);
};

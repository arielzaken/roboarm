#pragma once

#include <functional>

/// Interface for a limit switch sensor.
/// We can register a callback that fires on a state change.
class ILimitSwitch {
public:
    virtual ~ILimitSwitch() = default;

    /// Returns true if the switch is currently "pressed."
    virtual bool isPressed() const = 0;

    /// Register a callback (ISR safe) to notify on any state change.
    /// Signature: void callback(bool newState)
    virtual void setCallback(std::function<void(bool)> cb) = 0;
};
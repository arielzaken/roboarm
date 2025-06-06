#pragma once
#include "Observer/ObservableISR.h"

struct LimitSwitchEvent {
    bool state;
};
/// Interface for a limit switch sensor.
/// We can register a callback that fires on a state change.
class LimitSwitchBase: public ObservableISR<LimitSwitchEvent, 4> {
public:
    virtual ~LimitSwitchBase() = default;

    /// Returns true if the switch is currently "pressed."
    virtual bool isPressed() const = 0;
};
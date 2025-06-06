#pragma once
#include <Arduino.h>
#include "ESP_LOG.h"

#include "Observer/ObservableISR.h"
#include "RobotTypes.h"

/// Interface for a limit switch sensor.
/// We can register a callback that fires on a state change.
class LimitSwitchBase: public ObservableISR<LimitSwitchEvent, 4> {
    static uint8_t instanseCounter;
protected:
    uint8_t id = 0xff;
public:
    LimitSwitchBase() {
        id = instanseCounter++;
        ESP_LOGD("LimitSwitchBase", "Created LimitSwitchBase instance with ID %d", id);
    }
    virtual ~LimitSwitchBase() = default;

    /// Returns true if the switch is currently "pressed."
    virtual bool isPressed() const = 0;
};
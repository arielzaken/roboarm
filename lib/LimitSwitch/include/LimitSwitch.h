// LimitSwitch.h
#pragma once

#include <Arduino.h>

#include <ILimitSwitch.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class LimitSwitch : public LimitSwitchBase {
public:
    LimitSwitch(uint8_t pin, int mode = CHANGE);

    ~LimitSwitch();

    bool isPressed() const override;

private:
    uint8_t _pin;
    volatile bool _state;
    static void IRAM_ATTR isrHandler(void* arg);
    void IRAM_ATTR onInterrupt();

    static constexpr TickType_t debounceTicks = pdMS_TO_TICKS(5);
    volatile TickType_t _lastInterruptTick = 0;
};

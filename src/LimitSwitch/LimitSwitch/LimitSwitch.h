// LimitSwitch.h
#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../LimitSwitchBase.h"

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

    static constexpr TickType_t debounceTicks = pdMS_TO_TICKS(1); // 0.1ms debounce time
    volatile TickType_t _lastInterruptTick = 0;
};

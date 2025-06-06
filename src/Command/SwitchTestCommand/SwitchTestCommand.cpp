#include "SwitchTestCommand.h"
#include "ESP_LOG.h"
#include <Arduino.h>
#include <config.h>
#include "pins.h"

SwitchTestCommand::SwitchTestCommand(const std::array<LimitSwitchBase*, NUM_JOINTS>& switches)
    : _switches(switches)
{
    // Initialize switch states to false (not pressed)
    for (int i = 0; i < NUM_JOINTS; i++) {
        SwitchStates[i] = switches[i]->isPressed();
    }
    
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (_switches[i]) {
            _switches[i]->addObserver(this);  // Register this command as an observer
            ESP_LOGD("SWITCH_TEST", "Switch %d observer registered.", i);
        } else {
            ESP_LOGW("SWITCH_TEST", "Switch %d is null, cannot register observer.", i);
        }
    }

}

SwitchTestCommand::~SwitchTestCommand() {
    // Unregister all observers
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (_switches[i]) {
            _switches[i]->removeObserver(this);
        }
    }
    ESP_LOGI("SWITCH_TEST", "SwitchTestCommand destroyed, observers unregistered.");
}

void SwitchTestCommand::execute() {
    ESP_LOGI("SWITCH_TEST", "Entering switch test mode. Type EXIT to stop.");
    // Register callbacks for each switch to update line on change
    // Wait until user types EXIT
    String input;
    while (true) {
        if (Serial.available()) {
            char c = Serial.read();
            if (isPrintable(c)) {
                input += c;
            }
            if (c == '\n' || c == '\r') {
                input.trim();
                if (input.equalsIgnoreCase("EXIT")) {
                    ESP_LOGI("SWITCH_TEST", "\nExiting switch test mode.");
                    return;
                }
                input.clear();
            }
        }
        delay(1);
    }
}

void SwitchTestCommand::onNotify(const LimitSwitchEvent& event)
{
    // ESP_LOGD("SWITCH_TEST", "Switch %d changed to %s", event.id, event.state == SWITCH_PRESSED ? "PRESSED" : "RELEASED");
    SwitchStates[event.id] = event.state;

    Serial.print("\r");
    for (int i = 0; i < NUM_JOINTS; i++) {
        Serial.print(SwitchStates[i] ? "1 " : "0 ");
    }
    Serial.flush();
}


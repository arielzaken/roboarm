#include "SwitchTestCommand.h"
#include "ESP_LOG.h"
#include <Arduino.h>
#include <config.h>

SwitchTestCommand::SwitchTestCommand(const std::array<LimitSwitchBase*, NUM_JOINTS>& switches)
    : _switches(switches)
{
    // Initialize switch states to false (not pressed)
    for (int i = 0; i < NUM_JOINTS; i++) {
        SwitchStates[i] = switches[i]->isPressed();
    }
    // Register observer for each switch
    for (int i = 0; i < NUM_JOINTS; i++) {
        _observers[i] = new SwitchTestObserver(this, i);
        _switches[i]->addObserver(_observers[i]);
    }
}

SwitchTestCommand::~SwitchTestCommand() {
    for (int i = 0; i < NUM_JOINTS; i++) {
        _switches[i]->removeObserver(_observers[i]);
        delete _observers[i];
    }
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

void SwitchTestObserver::onNotify(const LimitSwitchEvent& event)
{
    _This->SwitchStates[_SwitchIndex] = event.state;

    Serial.print("\r");
    for (int i = 0; i < NUM_JOINTS; i++) {
        Serial.print(_This->SwitchStates[i] ? "1 " : "0 ");
    }
    Serial.flush();
}


#include "SwitchTestCommand.h"
#include "ESP_LOG.h"
#include <Arduino.h>
#include <config.h>

SwitchTestCommand::SwitchTestCommand(const std::array<ILimitSwitch*, NUM_JOINTS>& switches)
    : _switches(switches)
{}

void SwitchTestCommand::execute() {
    ESP_LOGI("SWITCH_TEST", "Entering switch test mode. Type EXIT to stop.");

    // Read and display initial states
    bool prevState[NUM_JOINTS];
    for (int i = 0; i < NUM_JOINTS; i++) {
        prevState[i] = _switches[i]->isPressed();
    }
    char buffer[64];
    snprintf(buffer, sizeof(buffer),
             "SW0=%d SW1=%d SW2=%d SW3=%d",
             prevState[0], prevState[1], prevState[2], prevState[3]);
    Serial.println(buffer);

    // Register callbacks for each switch to update line on change
    for (int i = 0; i < NUM_JOINTS; i++) {
        _switches[i]->setCallback([i, &prevState](bool newState) {
            prevState[i] = newState;
            char buf[64];
            snprintf(buf, sizeof(buf),
                     "SW0=%d SW1=%d SW2=%d SW3=%d",
                     prevState[0], prevState[1], prevState[2], prevState[3]);
            Serial.print("\r");      // carriage return
            Serial.print(buf);
            Serial.print("    ");   // padding
        });
    }

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
                    ESP_LOGI("SWITCH_TEST", "Exiting switch test mode.");
                    // Clear callbacks
                    for (int i = 0; i < NUM_JOINTS; i++) {
                        _switches[i]->setCallback(nullptr);
                    }
                    Serial.println();
                    return;
                }
                input.clear();
            }
        }
        delay(1);
    }
}
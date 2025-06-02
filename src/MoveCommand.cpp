#include "MoveCommand.h"
#include "ESP_LOG.h"
#include <Arduino.h>
#include <config.h>

MoveCommand::MoveCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                         const std::array<long, NUM_JOINTS>& targets,
                         const std::array<bool, NUM_JOINTS>& valid,
                         unsigned long feedrate,
                         bool blocking)
    : _joints(joints), _targets(targets), _valid(valid),
      _feedrate(feedrate), _blocking(blocking)
{}

void MoveCommand::execute() {
    ESP_LOGI("GCODE", "Executing MoveCommand");

    // If feedrate > 0, override joint speeds
    if (_feedrate > 0) {
        for (int i = 0; i < NUM_JOINTS; i++) {
            if (_joints[i]) {
                _joints[i]->setSpeed(_feedrate);
            }
        }
    }

    // Issue moves
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (_valid[i] && _joints[i]) {
            ESP_LOGI("GCODE", "  Joint %c -> %ld", 'A' + i, _targets[i]);
            _joints[i]->moveTo(_targets[i]);
        }
    }

    // If blocking (G1), wait for all valid joints to finish
    if (_blocking) {
        bool anyRunning = true;
        while (anyRunning) {
            anyRunning = false;
            for (int i = 0; i < NUM_JOINTS; i++) {
                if (_valid[i] && _joints[i] && _joints[i]->isRunning()) {
                    anyRunning = true;
                }
            }
            delay(1);
        }
    }
}
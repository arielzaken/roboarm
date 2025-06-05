#include "MoveCommand.h"
#include "ESP_LOG.h"
#include <Arduino.h>
#include <config.h>

MoveCommand::MoveCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                         const std::array<long, NUM_JOINTS>& targets,
                         const std::array<bool, NUM_JOINTS>& valid,
                         unsigned long feedrate)
    : _joints(joints), _targets(targets), _valid(valid), _feedrate(feedrate){}

void MoveCommand::execute() {
    ESP_LOGI("GCODE", "Executing MoveCommand");

    // If feedrate > 0, override joint speeds
    if (_feedrate > 0) {
        for (int i = 0; i < NUM_JOINTS; i++) {
            if (_joints[i]) {
                _joints[i]->setSpeed(_feedrate);
                ESP_LOGD("GCODE", "  Joint %c speed set to %lu Hz", 'A' + i, _feedrate);
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
}
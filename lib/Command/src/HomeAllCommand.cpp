#include "HomeAllCommand.h"
#include "ESP_LOG.h"

HomeAllCommand::HomeAllCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                               const std::array<LimitSwitchBase*, NUM_JOINTS>& switches,
                               IHomingStrategy* homingStrategy)
    : _joints(joints), _switches(switches), _strategy(homingStrategy)
{}

void HomeAllCommand::execute() {
    ESP_LOGI("GCODE", "Executing HomeAllCommand");
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (_joints[i] && _switches[i]) {
            _strategy->home(*_joints[i], *_switches[i], i);
        }
    }
}
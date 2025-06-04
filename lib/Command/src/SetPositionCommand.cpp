#include "SetPositionCommand.h"
#include <config.h>
#include "ESP_LOG.h"

SetPositionCommand::SetPositionCommand(const std::array<IJoint*, NUM_JOINTS>& joints)
    : _joints(joints)
{}

void SetPositionCommand::execute() {
    ESP_LOGI("GCODE", "Executing SetPositionCommand");
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (_joints[i]) {
            _joints[i]->setCurrentPosition(0);
            ESP_LOGI("GCODE", "  Joint %d position set to 0", i);
        }
    }
}
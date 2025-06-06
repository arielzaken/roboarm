#include "HomeAllCommand.h"
#include "ESP_LOG.h"

HomeAllCommand::HomeAllCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                               const std::array<LimitSwitchBase*, NUM_JOINTS>& switches)
    : _joints(joints), _switches(switches)
{}

void HomeAllCommand::execute() {
    ESP_LOGI("GCODE", "Executing HomeAllCommand");
    // home Joint3
    if(_switches[3]->isPressed()){
        _joints[3]->move(-1000, true);
    }
    _joints[3]->runForward();
    while(_switches[3]->isPressed() == false) {
        yield();
    }
    _joints[3]->setCurrentPosition(-100); // Set current position to 0
    _joints[3]->moveTo(0);
}
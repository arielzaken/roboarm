#include "Stopper.h"
#include "RobotTypes.h"
#include "ESP_LOG.h"

void Stopper::stopAllJoints()
{
    for (auto joint : *_joints) {
        if (joint) {
            joint->stop();
        }
    }
}

Stopper::Stopper(const std::array<IJoint *, NUM_JOINTS> &joints, const std::array<LimitSwitchBase *, NUM_JOINTS> &switches) 
    : _joints(&joints)
    , _switches(&switches)
{
    for (auto &sw : switches) {
        if (sw) {
            sw->addObserver(this);
        }
    }
    ESP_LOGD("STOPPER", "Stopper initialized with %d joints and limit switches.", NUM_JOINTS);
}

Stopper::~Stopper()
{
    for (auto joint : *_joints) {
        if (joint) {
            joint->stop();  // Ensure all joints are stopped
        }
    }
    ESP_LOGW("STOPPER", "Stopper destroyed, all joints stopped.");
    
    // Remove observers from limit switches
    for (auto &sw : *_switches) {
        if (sw) {
            sw->removeObserver(this);
        }
    }
    ESP_LOGD("STOPPER", "Observers removed from limit switches.");
}

void Stopper::onNotify(const LimitSwitchEvent &event)
{
    if (event.state == SwitchState::SWITCH_PRESSED) {
        ESP_LOGD("STOPPER", "Limit switch %d pressed", event.id);
        switch (event.id)  // Use switch-case for clarity
        {
        case 0:
            (*_joints)[0]->stop();
            break;
        case 1:
        case 2:
            (*_joints)[1]->stop();
            (*_joints)[2]->stop();
            break;
        case 3:
            // (*_joints)[3]->stop();
            break;
        default:
            ESP_LOGW("STOPPER", "Unknown limit switch ID %d pressed, stopping all joints.", event.id);
            // If an unknown switch is pressed, stop all joints
            stopAllJoints();
            break;
        }
    } else if (event.state == SwitchState::SWITCH_RELEASED) {
        ESP_LOGD("STOPPER", "Limit switch %d released.", event.id);
    } else {
        ESP_LOGW("STOPPER", "Unknown state for limit switch %d: %d", event.id, static_cast<int>(event.state));
    }
}
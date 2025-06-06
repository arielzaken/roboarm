#include <Arduino.h>
#include "config.h"
#include "Observer/IObserver.h"
#include "Joint/IJoint.h"
#include "LimitSwitch/ILimitSwitch.h"


class Stopper : public IObserver<LimitSwitchEvent> {
private:
    const std::array<IJoint*, NUM_JOINTS>* _joints;

    void stopAllJoints() {
        for (auto& joint : *_joints) {
            if (joint) {
                joint->stop();
            }
        }
    }
public:
    Stopper(const std::array<IJoint*, NUM_JOINTS>& joints) :_joints(&joints) {}
    void onNotify(const LimitSwitchEvent& value) override {
        if (value.state == HIGH) {
            stopAllJoints();
        }
    }
};

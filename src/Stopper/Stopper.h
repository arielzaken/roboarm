#include <Arduino.h>
#include "config.h"
#include "Observer/IObserver.h"
#include "Joint/IJoint.h"
#include "LimitSwitch/LimitSwitchBase.h"


class Stopper : public IObserver<LimitSwitchEvent> {
private:
    const std::array<IJoint*, NUM_JOINTS>* _joints;
    const std::array<LimitSwitchBase*, NUM_JOINTS>* _switches;
    void stopAllJoints();
public:
    Stopper(const std::array<IJoint*, NUM_JOINTS>& joints, const std::array<LimitSwitchBase*, NUM_JOINTS>& switches);
    ~Stopper();
    void onNotify(const LimitSwitchEvent& value) override;
};

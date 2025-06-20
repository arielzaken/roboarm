#include "JointLimitProxy.h"
#include <Arduino.h>
#include <ESP_LOG.h>

static const char* TAG = "JointLimitProxy";


JointLimitProxy::JointLimitProxy(IJoint *joint, LimitSwitchBase *limitSwitch) : 
    _joint(joint), 
    _limitSwitch(limitSwitch)
{}

void JointLimitProxy::move(long step, bool blocking)
{
    if (_limitSwitch->isPressed()) {
        ESP_LOGW(TAG, "Move aborted: limit switch is pressed.");
        return;
    }
    _joint->move(step, blocking);
}

void JointLimitProxy::moveTo(long stepPosition, bool blocking) {
    if (_limitSwitch->isPressed()) {
        ESP_LOGW(TAG, "Move aborted: limit switch is pressed.");
        return;
    }
    _joint->moveTo(stepPosition, blocking);
}

void JointLimitProxy::runForward() {
    if (_limitSwitch->isPressed()) {
        ESP_LOGW(TAG, "Run forward aborted: limit switch is pressed.");
    }
    else{
        _joint->runForward();
    }
}

void JointLimitProxy::stop() {
    _joint->stop();
}

void JointLimitProxy::setCurrentPosition(long zeroPosition) {
    _joint->setCurrentPosition(zeroPosition);
}

long JointLimitProxy::getCurrentPosition() const {
    return _joint->getCurrentPosition();
}

void JointLimitProxy::setSpeed(unsigned long speed) {
    _joint->setSpeed(speed);
}

void JointLimitProxy::setAcceleration(unsigned long accel) {
    _joint->setAcceleration(accel);
}

bool JointLimitProxy::isRunning() const {
    return _joint->isRunning();
}

void JointLimitProxy::moveTimed(long stepPosition, unsigned long duration, uint32_t* actual, bool blocking)
{
    if (_limitSwitch->isPressed()) {
        ESP_LOGW(TAG, "Move timed aborted: limit switch is pressed.");
        return;
    }
    _joint->moveTimed(stepPosition, duration, actual, blocking);
}

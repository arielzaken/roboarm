#include "StepperJoint.h"
#include <FastAccelStepper.h>
#include "ESP_LOG.h"
static const char* TAG = "StepperJoint";
extern FastAccelStepperEngine engine;  // Defined in RobotController.cpp

StepperJoint::StepperJoint(int stepPin, int dirPin) {
    _stepper = engine.stepperConnectToPin(stepPin);
    if (_stepper) {
        _stepper->setDirectionPin(dirPin);
        ESP_LOGD(TAG, "StepperJoint created with step pin %d and direction pin %d", stepPin, dirPin);
        // Defaults will be overridden later by controller
    } 
    // else: failure to allocate, but RobotController checks for null.
}

void StepperJoint::moveTo(long stepPosition, bool blocking) {
    if (_stepper) {
        MoveResultCode res = _stepper->moveTo(stepPosition, blocking);
        if(moveIsOk(res)) {
            ESP_LOGD(TAG, "Joint %p moved to position %ld successfully", this, stepPosition);
        } else {
            ESP_LOGE(TAG, "Failed to move joint %p to position %ld (%s)", this, stepPosition, _stepper, toString(res));
        }
    }
}

void StepperJoint::runForward() {
    if (_stepper) {
        ESP_LOGD(TAG, "Running joint %p forward", this);
        _stepper->runForward();
    }
}

void StepperJoint::stop() {
    if (_stepper) {
        ESP_LOGD(TAG, "Stopping joint %p", this);
        _stepper->stopMove();
    }
}

void StepperJoint::setCurrentPosition(long zeroPosition) {
    if (_stepper) {
        ESP_LOGD(TAG, "Setting current position of joint %p to %ld", this, zeroPosition);
        _stepper->setCurrentPosition(zeroPosition);
    }
}

long StepperJoint::getCurrentPosition() const {
    ESP_LOGD(TAG, "Getting current position of joint %p", this);
    return _stepper ? _stepper->getCurrentPosition() : 0;
}

void StepperJoint::setSpeed(unsigned long speed) {
    if (_stepper) {
        ESP_LOGD(TAG, "Setting speed of joint %p to %lu Hz", this, speed);
        _stepper->setSpeedInHz(speed);
    }
}

void StepperJoint::setAcceleration(unsigned long accel) {
    if (_stepper) {
        ESP_LOGD(TAG, "Setting acceleration of joint %p to %lu steps/sec²", this, accel);
        _stepper->setAcceleration(accel);
    }
}

bool StepperJoint::isRunning() const {
    return _stepper ? _stepper->isRunning() : false;
}
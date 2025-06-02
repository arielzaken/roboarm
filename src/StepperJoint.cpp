#include "StepperJoint.h"
#include <FastAccelStepper.h>
#include "ESP_LOG.h"

extern FastAccelStepperEngine engine;  // Defined in RobotController.cpp

StepperJoint::StepperJoint(int stepPin, int dirPin) {
    _stepper = engine.stepperConnectToPin(stepPin);
    if (_stepper) {
        _stepper->setDirectionPin(dirPin);
        // Defaults will be overridden later by controller
    } 
    // else: failure to allocate, but RobotController checks for null.
}

void StepperJoint::moveTo(long stepPosition) {
    if (_stepper) {
        _stepper->moveTo(stepPosition);
    }
}

void StepperJoint::runForward() {
    if (_stepper) {
        _stepper->runForward();
    }
}

void StepperJoint::stop() {
    if (_stepper) {
        _stepper->stopMove();
    }
}

void StepperJoint::setCurrentPosition(long zeroPosition) {
    if (_stepper) {
        _stepper->setCurrentPosition(zeroPosition);
    }
}

long StepperJoint::getCurrentPosition() const {
    return _stepper ? _stepper->getCurrentPosition() : 0;
}

void StepperJoint::setSpeed(unsigned long speed) {
    if (_stepper) {
        _stepper->setSpeedInHz(speed);
    }
}

void StepperJoint::setAcceleration(unsigned long accel) {
    if (_stepper) {
        _stepper->setAcceleration(accel);
    }
}

bool StepperJoint::isRunning() const {
    return _stepper ? _stepper->isRunning() : false;
}
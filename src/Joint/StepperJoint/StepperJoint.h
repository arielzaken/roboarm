#pragma once
#include <FastAccelStepper.h>
#include "Joint/IJoint.h"

/// Concrete IJoint implementation using FastAccelStepper.
class StepperJoint : public IJoint {
public:
    StepperJoint(int stepPin, int dirPin);
    virtual ~StepperJoint() = default;

    // IJoint
    void moveTo(long stepPosition, bool blocking = false) override;
    void runForward() override;
    void stop() override;
    void setCurrentPosition(long zeroPosition) override;
    long getCurrentPosition() const override;
    void setSpeed(unsigned long speed) override;
    void setAcceleration(unsigned long accel) override;
    bool isRunning() const override;

private:
    FastAccelStepper* _stepper;
};
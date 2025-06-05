#pragma once
#include "IJoint.h"
#include <ILimitSwitch.h>
/**
 * @file JointLimitProxy.h
 * @brief Proxy class for a stepper joint with limit switch integration.
 * This class wraps an IJoint and an LimitSwitchBase to provide
 * stepper motor control with limit switch functionality.   
 * It allows moving to absolute step positions,
 * stepping forward, stopping, and querying position,
 * while also checking limit switch states.
 * @param joint Pointer to the underlying IJoint implementation.
 * @param limitSwitch Pointer to the LimitSwitchBase implementation.
 */
class JointLimitProxy : public IJoint {
private:
    IJoint* _joint;
    LimitSwitchBase* _limitSwitch;
public:
    JointLimitProxy(IJoint* joint, LimitSwitchBase* limitSwitch);

    /// Move to an absolute step position.
    void moveTo(long stepPosition, bool blocking = false) override;

    /// Step forward one time-step (no ramp).
    void runForward() override;

    /// Stop any ongoing move immediately (will decelerate to stop).
    void stop() override;

    /// Set current position as given step value.
    void setCurrentPosition(long zeroPosition) override;

    /// Get the current position in steps.
    long getCurrentPosition() const override;

    /// Set maximum speed (steps/sec).
    void setSpeed(unsigned long speed) override;

    /// Set acceleration (steps/sec²). 
    /// (If zero, future moves use no ramp.)
    void setAcceleration(unsigned long accel) override;

    /// Returns true if the joint is still moving.
    bool isRunning() const override;
};
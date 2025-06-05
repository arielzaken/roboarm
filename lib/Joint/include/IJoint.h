#pragma once

/// Interface for a generic joint (stepper). 
/// Defines methods for moving, stopping, and querying position.
class IJoint {
public:
    virtual ~IJoint() = default;

    /// Move to an absolute step position.
    virtual void moveTo(long stepPosition, bool blocking = false) = 0;

    /// Step forward one time-step (no ramp).
    virtual void runForward() = 0;

    /// Stop any ongoing move immediately (will decelerate to stop).
    virtual void stop() = 0;

    /// Set current position as given step value.
    virtual void setCurrentPosition(long zeroPosition) = 0;

    /// Get the current position in steps.
    virtual long getCurrentPosition() const = 0;

    /// Set maximum speed (steps/sec).
    virtual void setSpeed(unsigned long speed) = 0;

    /// Set acceleration (steps/sec²). 
    /// (If zero, future moves use no ramp.)
    virtual void setAcceleration(unsigned long accel) = 0;

    /// Returns true if the joint is still moving.
    virtual bool isRunning() const = 0;
};
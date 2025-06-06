#pragma once
#include <stdint.h>
/// Interface for a generic joint (stepper). 
/// Defines methods for moving, stopping, and querying position.
class IJoint {
public:
    virtual ~IJoint() = default;

    virtual void move(long step, bool blocking = false) = 0;

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

    /**
     * @brief Move to a step position over a specified duration.
     * @param stepPosition The target step position to move to.
     * @param duration The duration in hardware ticks to complete the move.
     * @param actual Pointer to store the actual step position reached.
     * @param blocking If true, the call blocks until the move is complete.
     */
    virtual void moveTimed(long stepPosition, unsigned long duration, uint32_t* actual, bool blocking = false)  = 0;
};
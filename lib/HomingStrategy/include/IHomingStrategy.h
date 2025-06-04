#pragma once
#include <IJoint.h>
#include <ILimitSwitch.h>

/// Defines an interface for homing a single joint.
/// Strategy Pattern: different joints or sensors might use different homing logic.
class IHomingStrategy {
public:
    virtual ~IHomingStrategy() = default;

    /// Perform a homing routine on one joint, using its associated switch.
    /// Should block until homing completes, then set joint position to zero.
    virtual void home(IJoint& joint, ILimitSwitch& limitSwitch, int jointIndex) = 0;
};
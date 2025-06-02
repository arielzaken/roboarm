#pragma once
#include "IHomingStrategy.h"

/// Homing by moving rapidly until switch fires, backing off, then approaching slowly.
class SwitchHomingStrategy : public IHomingStrategy {
public:
    virtual ~SwitchHomingStrategy() = default;
    void home(IJoint& joint, ILimitSwitch& limitSwitch, int jointIndex) override;
};
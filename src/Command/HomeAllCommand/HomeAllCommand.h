#pragma once
#include "../ICommand.h"
#include "Joint/IJoint.h"
#include "LimitSwitch/LimitSwitchBase.h"
#include <array>
#include <config.h>

/// G28: Homes all joints one by one using a provided HomingStrategy.
class HomeAllCommand : public ICommand {
public:
    HomeAllCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                   const std::array<LimitSwitchBase*, NUM_JOINTS>& switches);

    void execute() override; 

private:
    std::array<IJoint*, NUM_JOINTS> _joints;
    std::array<LimitSwitchBase*, NUM_JOINTS> _switches;
};
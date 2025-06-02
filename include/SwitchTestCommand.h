#pragma once
#include "ICommand.h"
#include "ILimitSwitch.h"
#include <array>
#include <config.h>

/// M119: Enter an interactive switch test mode.
/// Prints switch states on change, exits on typing "EXIT".
class SwitchTestCommand : public ICommand {
public:
    SwitchTestCommand(const std::array<ILimitSwitch*, NUM_JOINTS>& switches);

    void execute() override;

private:
    std::array<ILimitSwitch*, NUM_JOINTS> _switches;
};
#pragma once
#include "../ICommand.h"
#include "LimitSwitch/LimitSwitchBase.h"
#include <array>
#include <config.h>

class SwitchTestObserver;

/// M119: Enter an interactive switch test mode.
/// Prints switch states on change, exits on typing "EXIT".
class SwitchTestCommand : public ICommand, IObserver<LimitSwitchEvent> {
    public:
    SwitchTestCommand(const std::array<LimitSwitchBase*, NUM_JOINTS>& switches);
    ~SwitchTestCommand();
    void execute() override;
    
    private:
    std::array<LimitSwitchBase*, NUM_JOINTS> _switches;
    std::array<SwitchTestObserver*, NUM_JOINTS> _observers;  // store them
    std::array<bool, NUM_JOINTS> SwitchStates;
    friend class SwitchTestObserver;  // Allow observer to access private members
    void onNotify(const LimitSwitchEvent& event) override;  // Handle state changes
};
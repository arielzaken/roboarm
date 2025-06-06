#pragma once
#include "../ICommand.h"
#include "LimitSwitch/ILimitSwitch.h"
#include <array>
#include <config.h>

class SwitchTestObserver;

/// M119: Enter an interactive switch test mode.
/// Prints switch states on change, exits on typing "EXIT".
class SwitchTestCommand : public ICommand {
    public:
    SwitchTestCommand(const std::array<LimitSwitchBase*, NUM_JOINTS>& switches);
    ~SwitchTestCommand();  // <- Add this
    void execute() override;
    
    private:
    std::array<LimitSwitchBase*, NUM_JOINTS> _switches;
    std::array<SwitchTestObserver*, NUM_JOINTS> _observers;  // store them
    std::array<bool, NUM_JOINTS> SwitchStates;
    friend class SwitchTestObserver;  // Allow observer to access private members
};


class SwitchTestObserver : public IObserver<LimitSwitchEvent> {
private:
    SwitchTestCommand* _This;  // Pointer to the parent command
    int _SwitchIndex;  // Index of the switch this observer is for
public:
    SwitchTestObserver(SwitchTestCommand* This, int switchIndex):
        _This(This), _SwitchIndex(switchIndex) {}
    void onNotify(const LimitSwitchEvent& event) override;
};
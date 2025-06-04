#pragma once
#include "ICommand.h"
#include <IJoint.h>
#include <array>
#include <config.h>
/// G0/G1: Move specified joints to given step positions, optionally blocking until done.
class MoveCommand : public ICommand {
public:
    MoveCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                const std::array<long, NUM_JOINTS>& targets,
                const std::array<bool, NUM_JOINTS>& valid,
                unsigned long feedrate,
                bool blocking);

    void execute() override;

private:
    std::array<IJoint*, NUM_JOINTS> _joints;
    std::array<long, NUM_JOINTS> _targets;
    std::array<bool, NUM_JOINTS> _valid;
    unsigned long _feedrate;
    bool _blocking;
};
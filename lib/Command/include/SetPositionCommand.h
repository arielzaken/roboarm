#pragma once
#include <config.h>
#include <ICommand.h>
#include <IJoint.h>
#include <array>

/// G92: Set the current position as zero (no actual movement).
class SetPositionCommand : public ICommand {
public:
    SetPositionCommand(const std::array<IJoint*, NUM_JOINTS>& joints);

    void execute() override;

private:
    std::array<IJoint*, NUM_JOINTS> _joints;
};
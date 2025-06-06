#pragma once
#include <array>
#include "Command/ICommand.h"
#include "Joint/IJoint.h"
#include <config.h>
/// G0/G1: Move specified joints to given step positions, optionally blocking until done.
/**
 * This command moves multiple joints to specified target positions.
 * It supports both blocking and non-blocking execution.
 * If blocking is true, the command will wait until all joints have reached their target positions.
 * If blocking is false, the command will initiate the moves and return immediately.   
 * @param joints Array of joint pointers to move.
 * @param targets Array of target positions for each joint in steps.
 * @param valid Array indicating which joints are valid for movement.
 * @param feedrate Feedrate in steps/sec for the move.
 * @param blocking If true, the command will block until all moves are complete.
 */
class MoveCommand : public ICommand {
public:
    MoveCommand(const std::array<IJoint*, NUM_JOINTS>& joints,
                const std::array<long, NUM_JOINTS>& targets,
                const std::array<bool, NUM_JOINTS>& valid,
                unsigned long feedrate);

    void execute() override;

private:
    std::array<IJoint*, NUM_JOINTS> _joints;
    std::array<long, NUM_JOINTS> _targets;
    std::array<bool, NUM_JOINTS> _valid;
    unsigned long _feedrate;
};
#pragma once
#include <Arduino.h>
#include <config.h>
#include "Command/ICommand.h"
#include "Joint/IJoint.h"
#include "LimitSwitch/ILimitSwitch.h"
#include "HomingStrategy/IHomingStrategy.h"
#include <array>

/// Parses one line of G-code (as String) and returns a concrete ICommand.
/// Uses Factory Pattern: depending on prefix (G0,G1,G28,G92,M119), builds the right ICommand.
class GCodeParser {
public:
    GCodeParser(const std::array<IJoint*, NUM_JOINTS>& joints,
                const std::array<LimitSwitchBase*, NUM_JOINTS>& switches,
                IHomingStrategy* homingStrategy);

    /// Parse "line" and return a new ICommand*. Caller must delete after use.
    /// If unsupported or empty, returns nullptr.
    ICommand* parseLine(const String& line);

private:
    std::array<IJoint*, NUM_JOINTS> _joints;
    std::array<LimitSwitchBase*, NUM_JOINTS> _switches;
    IHomingStrategy* _homingStrategy;
};
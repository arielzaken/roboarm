#pragma once
#include <array>
#include "IJoint.h"
#include "StepperJoint.h"
#include "ILimitSwitch.h"
#include "LimitSwitch.h"
#include "IHomingStrategy.h"
#include "SwitchHomingStrategy.h"
#include "GCodeParser.h"

/// Encapsulates all robot-level objects: stepper joints, limit switches, parser, and main loop.
class RobotController {
public:
    RobotController();
    ~RobotController();

    /// Initialize hardware and objects.
    void begin();

    /// Must be called repeatedly from Arduino loop().
    void handleLoop();

private:
    std::array<IJoint*, NUM_JOINTS> _joints;
    std::array<ILimitSwitch*, NUM_JOINTS> _switches;
    IHomingStrategy* _homingStrategy;
    GCodeParser* _parser;

    // Interactive input buffer
    String _inputBuffer;
    bool _promptShown;

    void processInputChar(char c);
};
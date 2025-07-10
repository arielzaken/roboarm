#pragma once
#include <config.h>
#include "Sensors/PresenceSensor/PresenceSensor.h"
#include "CommandParser/ArmRobotParser/armRobotParser.h"

#include <staticTypes.h>

class Robot{
public:
    void begin();
protected:
    void startTasks();

    LIMIT_SWITCH_TYPE limitSwitchs[NUM_JOINTS];

    COMMAND_PARSER_TYPE commandParser;

    friend COMMAND_PARSER_TYPE;
private:
    static void serialLisener(void* arg);
};

extern Robot robot;
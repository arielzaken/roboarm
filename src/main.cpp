#include <Arduino.h>
#include <config.h>
#include <pins.h>
#include <FastAccelStepper.h>
#include <ESP_LOG.h>
#include "Joint/StepperJoint/StepperJoint.h"
#include "HomingStrategy/SwitchHomingStrategy/SwitchHomingStrategy.h"
#include "GCodeParser/GCodeParser.h"
#include "Command/MenuCommand/MenuCommand.h"
#include "Joint/JointLimitProxy/JointLimitProxy.h"
#include "Stopper/Stopper.h"
#include "LimitSwitch/LimitSwitch/LimitSwitch.h"

static const char* TAG = "MAIN";


std::array<IJoint*, NUM_JOINTS> _joints;
std::array<LimitSwitchBase*, NUM_JOINTS> _switches;
std::array<IJoint*, NUM_JOINTS> _jointProxies;
IHomingStrategy* _homingStrategy;
GCodeParser* _parser;
FastAccelStepperEngine engine;
Stopper* _stopper;

// Interactive input buffer
String _inputBuffer;
bool _promptShown;

void processInputChar(char c);

void setup() {
    Serial.begin(115200);
    engine.init();  // Initialize the FastAccelStepper engine
    const int pulsePins[NUM_JOINTS] = PULSE_PINS;
    const int dirPins[NUM_JOINTS] = DIR_PINS;
    const int jointSpeeds[NUM_JOINTS] = JOINT_SPEEDS;
    const int jointAccels[NUM_JOINTS] = JOINT_ACCELS;
    const int switchPins[NUM_JOINTS] = SWITCH_PINS;
    // 1) Create joints
    for (int i = 0; i < NUM_JOINTS; i++) {
        _joints[i] = new StepperJoint(
            pulsePins[i],
            dirPins[i]
        );
        // Set each joint’s rated speed/accel
        _joints[i]->setSpeed(jointSpeeds[i]);
        _joints[i]->setAcceleration(jointAccels[i]);
    }

    // 2) Create limit switches
    //    Use INPUT_PULLDOWN so idle=LOW, pressed=HIGH
    for (int i = 0; i < NUM_JOINTS; i++) {
        _switches[i] = new LimitSwitch(switchPins[i], CHANGE);
        _jointProxies[i] = new JointLimitProxy(_joints[i], _switches[i]);
    }

    // 3) Create homing strategy
    _homingStrategy = new SwitchHomingStrategy();

    // 4) Create G-code parser
    _parser = new GCodeParser(_jointProxies, _switches, _homingStrategy);

    ESP_LOGI(TAG, "Initialization complete. Ready for commands.");
    MenuCommand menuCmd;
    menuCmd.execute();  // Show the menu on startup
}

void loop() {
    // 1) If in switch-test mode, that command’s execute() is blocking,
    //    so handleLoop() won’t be called until it returns. We only do terminal input when not in test mode.

    // 2) Prompt if needed
    if (!_promptShown) {
        Serial.print("> ");
        _promptShown = true;
    }

    // 3) Read incoming chars
    while (Serial.available()) {
        char c = Serial.read();
        Serial.write(c);   // echo

        processInputChar(c);
    }
}

void processInputChar(char c) {
    // Handle backspace
    if (c == '\b' || c == 127) {
        if (_inputBuffer.length() > 0) {
            _inputBuffer.remove(_inputBuffer.length() - 1);
            Serial.print(" \b");
        } else {
            Serial.print(" ");
        }
        return;
    }

    // End of line
    if (c == '\r' || c == '\n') {
        Serial.println();

        if (_inputBuffer.length() > 0) {
            ICommand* cmd = _parser->parseLine(_inputBuffer);
            if (cmd) {
                cmd->execute();
                delete cmd;
            }
        }

        _inputBuffer = "";
        _promptShown = false;
        return;
    }

    // Normal character
    if (isPrintable(c)) {
        _inputBuffer += c;
    }
}
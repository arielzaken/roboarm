#include "RobotController.h"
#include <Arduino.h>
#include <config.h>
#include <pins.h>
#include <StepperJoint.h>
#include <LimitSwitch.h>
#include <SwitchHomingStrategy.h>
#include <FastAccelStepper.h>
#include "ESP_LOG.h"
static const char* TAG = "ROBOT";
/// Global engine for all StepperJoint instances
FastAccelStepperEngine engine;

RobotController::RobotController()
    : _homingStrategy(nullptr),
      _parser(nullptr),
      _inputBuffer(""),
      _promptShown(false)
{}

RobotController::~RobotController() {
    delete _homingStrategy;
    delete _parser;
    for (auto j : _joints) {
        delete j;
    }
    for (auto s : _switches) {
        delete s;
    }
}

void RobotController::begin() {
    Serial.begin(115200);
    ESP_LOGI(TAG, "Initializing RobotController…");
    const int pulsePins[4] = PULSE_PINS;
    const int dirPins[4] = DIR_PINS;
    const int jointSpeeds[4] = JOINT_SPEEDS;
    const int jointAccels[4] = JOINT_ACCELS;
    // 1) Create joints
    for (int i = 0; i < NUM_JOINTS; i++) {
        _joints[i] = new StepperJoint(
            pulsePins[i],
            dirPins[i]
        );
        // Set each joint’s rated speed/accel
        _joints[i]->setSpeed(jointSpeeds[i]);
        _joints[i]->setAcceleration(dirPins[i]);
    }

    // 2) Create limit switches
    //    Use INPUT_PULLDOWN so idle=LOW, pressed=HIGH
    for (int i = 0; i < NUM_JOINTS; i++) {
        int pin = 0;
        switch (i) {
            case 0: pin = LINK0_SWITCH_PIN; break;
            case 1: pin = LINK1_SWITCH_PIN; break;
            case 2: pin = LINK2_SWITCH_PIN; break;
            case 3: pin = LINK3_SWITCH_PIN; break;
        }
        _switches[i] = new LimitSwitch(pin, CHANGE);
    }

    // 3) Create homing strategy
    _homingStrategy = new SwitchHomingStrategy();

    // 4) Create G-code parser
    _parser = new GCodeParser(_joints, _switches, _homingStrategy);

    ESP_LOGI(TAG, "Initialization complete. Ready for commands.");
    Serial.println();
}

void RobotController::handleLoop() {
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

void RobotController::processInputChar(char c) {
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
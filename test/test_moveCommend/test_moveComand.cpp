#include <Arduino.h>
#include <array>
#include <unity.h>
#include <FastAccelStepper.h>
#include <MoveCommand.h>
#include <StepperJoint.h>
#include <config.h>

FastAccelStepperEngine engine;
StepperJoint* joint[NUM_JOINTS] = { nullptr };

void setUp(void) {
    // Optionally reset state before each test
}

void tearDown(void) {
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (joint[i]) joint[i]->stop();
    }
    for (int i = 0; i < NUM_JOINTS; i++) {
        if (joint[i]) joint[i]->moveTo(0); // Reset position to 0
    }
    delay(500);
}

void test_move_command_execute() {
    // Prepare joints
    std::array<IJoint*, NUM_JOINTS> joints;
    std::array<long, NUM_JOINTS> targets = { 500, 10000, 4000, 5000 }; // Example target positions
    std::array<bool, NUM_JOINTS> valid;

    for (int i = 0; i < NUM_JOINTS; i++) {
        joints[i] = joint[i];
        valid[i] = true;
    }

    MoveCommand cmd(joints, targets, valid, 5000, true);
    cmd.execute();

    // Check if joints moved to target (allowing some margin)
    for (int i = 0; i < NUM_JOINTS; i++) {
        long pos = joint[i]->getCurrentPosition();
        TEST_ASSERT_INT_WITHIN(20, targets[i], pos);
    }
}

void setup() {
    UNITY_BEGIN();
    engine.init();
    const int dirPins[4] = {13, 14, 26, 33}; // Example DIR pins    
    const int stepPins[4] = {12, 27, 25, 32}; // Example STEP pins
    for (size_t i = 0; i < NUM_JOINTS; i++) {
        joint[i] = new StepperJoint(stepPins[i], dirPins[i]);
        joint[i]->setSpeed(3000);
        joint[i]->setAcceleration(5000);
        joint[i]->setCurrentPosition(0);
    }
    RUN_TEST(test_move_command_execute);
    UNITY_END();
}

void loop(){}
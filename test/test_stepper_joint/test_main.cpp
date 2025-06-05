#include <Arduino.h>
#include <unity.h>
#include <StepperJoint.h>
#include <FastAccelStepper.h>

#define TEST_STEP_PIN 27
#define TEST_DIR_PIN 14

FastAccelStepperEngine engine;

StepperJoint* joint;

void setUp(void) {
    // Initialize engine and joint once
    engine.init();
    joint = new StepperJoint(TEST_STEP_PIN, TEST_DIR_PIN);
    joint->setSpeed(3000);       // 500 steps/sec
    joint->setAcceleration(5000); // 100 steps/sec^2
    joint->setCurrentPosition(0);
}

void tearDown(void) {
    joint->stop();
    delay(500);
}

void test_move_forward() {
    long target = 100;
    joint->moveTo(target);
    delay(5000);  // Wait for motion
    long pos = joint->getCurrentPosition();
    TEST_ASSERT_INT_WITHIN(10, target, pos);  // Allow margin
}

void test_run_and_stop() {
    joint->runForward();
    delay(300);
    TEST_ASSERT_TRUE(joint->isRunning());
    joint->stop();
    delay(3000);
    TEST_ASSERT_FALSE(joint->isRunning());
}

void test_set_position() {
    joint->setCurrentPosition(1234);
    TEST_ASSERT_EQUAL(1234, joint->getCurrentPosition());
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_move_forward);
    RUN_TEST(test_run_and_stop);
    RUN_TEST(test_set_position);
    UNITY_END();
}

void loop() {}

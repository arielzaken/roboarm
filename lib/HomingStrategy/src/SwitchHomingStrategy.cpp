#include <Arduino.h>
#include "SwitchHomingStrategy.h"
#include <config.h>
#include "ESP_LOG.h"
static const char* TAG = "HOMING";
void SwitchHomingStrategy::home(IJoint& joint, LimitSwitchBase& limitSwitch, int jointIndex) {
    ESP_LOGI(TAG, "Joint %d: Starting fast drive...", jointIndex);
    // Pull in the macro, then immediately put it into a real array:
    static const int accelArr[NUM_JOINTS] = JOINT_ACCELS;

    // Phase 1: fast ramped approach toward switch
    joint.setAcceleration(accelArr[jointIndex]);  
    joint.setSpeed(HOME_FAST_SPEED);
    joint.moveTo(-1000000);

    // Wait until switch pressed
    while (!limitSwitch.isPressed()) {
        yield();  // Yield to allow other tasks to run
    }

    // Immediately stop ramped motion
    joint.stop();
    while (joint.isRunning()) {
        yield();  // Yield to allow other tasks to run
    }

    ESP_LOGI(TAG, "Joint %d: Switch triggered, backing off %d steps at fast speed...", jointIndex, BACKOFF_STEPS);
    joint.setAcceleration(0);
    joint.setSpeed(HOME_FAST_SPEED);
    for (int i = 0; i < BACKOFF_STEPS; i++) {
        joint.runForward();
    }

    ESP_LOGI(TAG, "Joint %d: Approaching slowly...", jointIndex);
    joint.setAcceleration(0);
    joint.setSpeed(HOME_SLOW_SPEED);
    while (!limitSwitch.isPressed()) {
        joint.runForward();
    }

    joint.stop();
    joint.setCurrentPosition(0);
    ESP_LOGI(TAG, "Joint %d: Homed successfully, set position = 0", jointIndex);
}
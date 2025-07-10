#include "Robot.h"
#include <pins.h>

static const char* TAG = "Robot";

Robot robot;

void Robot::begin(){
    ESP_LOGI(TAG, "robot version %s", ROBOT_VERSION);
    
    limitSwitchs[0].begin(LINK0_SWITCH_PIN);
    limitSwitchs[1].begin(LINK1_SWITCH_PIN, LOW);
    limitSwitchs[2].begin(LINK2_SWITCH_PIN);
    limitSwitchs[3].begin(LINK3_SWITCH_PIN);

    commandParser.begin(this);

    startTasks();
}
#pragma once
// This file defines the pin assignments for the robotic arm's stepper motors and limit switches.
// The pin numbers are specific to the ESP32 microcontroller.

#define  LINK0_PULSE_PIN 32
#define  LINK0_DIR_PIN 33

#define  LINK1_PULSE_PIN 27
#define  LINK1_DIR_PIN 14

#define  LINK2_PULSE_PIN 12
#define  LINK2_DIR_PIN 13

#define  LINK3_PULSE_PIN 25
#define  LINK3_DIR_PIN 26

// Limit-switch pins for each joint
#define SWITCH_PORT_0_PIN 16
#define SWITCH_PORT_1_PIN 17
#define SWITCH_PORT_2_PIN 18
#define SWITCH_PORT_3_PIN 5

#define LINK0_SWITCH_PIN SWITCH_PORT_3_PIN
#define LINK1_SWITCH_PIN SWITCH_PORT_2_PIN
#define LINK2_SWITCH_PIN SWITCH_PORT_0_PIN
#define LINK3_SWITCH_PIN SWITCH_PORT_1_PIN
// Pulse (STEP) pins for each joint
#define PULSE_PINS {LINK0_PULSE_PIN, LINK1_PULSE_PIN, LINK2_PULSE_PIN, LINK3_PULSE_PIN}

// Direction (DIR) pins for each joint
#define DIR_PINS   {LINK0_DIR_PIN, LINK1_DIR_PIN, LINK2_DIR_PIN, LINK3_DIR_PIN}

// Limit switch pins for each joint
#define SWITCH_PINS {LINK0_SWITCH_PIN, LINK1_SWITCH_PIN, LINK2_SWITCH_PIN, LINK3_SWITCH_PIN}


#define MAX_GPIO_PIN 40

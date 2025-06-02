#pragma once

#include "esp32/clk.h"
// Stepper motor settings
#define STEPS_PER_DEG 100
#define DEFAULT_SPEED 2000
#define DEFAULT_ACCEL 1000
#define DEFAULT_DURATION_TICKS 100000
#define MAX_SAFE_STEPS 10000


// Joint speed (steps per second)
#define JOINT_SPEEDS {500, 500, 500, 500}

// Joint acceleration (steps per second squared)
#define JOINT_ACCELS {1000, 1000, 1000, 1000}

// Convert ms to hardware timer ticks based on runtime CPU frequency
#define DURATION_TICKS(ms) ((esp_clk_cpu_freq() / 1000) * (ms))
#define NUM_JOINTS 4
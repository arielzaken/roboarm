#pragma once

#define ROBOT_VERSION "V1.0.0"

// Number of joints
#define NUM_JOINTS        4

// Default (fallback) motion parameters
#define DEFAULT_SPEED     2000    // Steps/sec
#define DEFAULT_ACCEL     1000    // Steps/sec²

// Per-joint speed (steps/sec) and accel (steps/sec²)
#define JOINT_SPEEDS {500, 2000, 2000, 2000}
#define JOINT_ACCELS {1000, 3000, 3000, 5000}

// Homing parameters
#define HOME_FAST_SPEED   1000    // Steps/sec (fast until first switch trigger)
#define HOME_SLOW_SPEED    50     // Steps/sec (slow for fine approach)
#define BACKOFF_STEPS       10    // Steps to back off after first trigger

#define MAX_OBSERVERS_PER_SENSOR 4
#pragma once
/**
 * @file RobotTypes.h
 * @brief Common types and structures used in the robot control system.
 * This file defines the types used for limit switch events and other robot-related data structures.
 */
#include <stdint.h>


/**
 * @enum SwitchState
 * @brief Represents the state of a limit switch.
 * This enum defines the possible states of a limit switch, indicating whether it is pressed (active) or not pressed (inactive).
 * The values are used to represent the state in a boolean context, where `SWITCH_PRESSED` is true (1) and `SWITCH_RELEASED` is false (0).
 */
enum SwitchState {
    SWITCH_PRESSED = 1,   ///< The limit switch is pressed (active).
    SWITCH_RELEASED = 0   ///< The limit switch is not pressed (inactive).
};


/**
 * @brief Converts a pin value to a SwitchState.
 * This function takes an integer pin value (typically read from a digital input) and converts it to a SwitchState.
 * this is how you convert because of c++ nonsense that enum values are not implicitly convertible to bool.
 */
inline SwitchState toSwitchState(int pinValue) {
    return static_cast<SwitchState>(pinValue);
}



/**
 * @struct LimitSwitchEvent
 * @brief Represents an event from a limit switch.
 * This structure contains the state of the switch (pressed or not) and the ID of the pin where the switch is connected.
 */
struct LimitSwitchEvent {
    SwitchState state;
    uint8_t id;  // Pin number where the switch is connected
};
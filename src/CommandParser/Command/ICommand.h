#pragma once
#include <stdint.h>

/**
 * * @brief Represents the result of executing a command.
 */
typedef struct ExecuteResult {
    int16_t code = 0; ///< Result code of the command execution.
    const char* message = nullptr;       ///< Optional message providing additional information about the execution result.
} ExecuteResult;

/**
 * * @brief Interface for command pattern, if needed another command
 * *       inherit from this interface. and update the parser
 */
class ICommand {
public:
    virtual ~ICommand() = default;

    // Execute the command
    virtual ExecuteResult execute() = 0;
};
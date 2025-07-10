#pragma once
#include "Command/ICommand.h"

#include <staticTypes.h>
#include <StaticVirtual.h>

/**
 * * @brief Represents the result of parsing a command.
 */
typedef enum Parse_rct {
    PARSE_OK = 0,                       ///< Command parsed successfully.
    PARSE_INVALID_COMMAND_LEN = -1,     ///< the command buffer is not at the correct size
    PARSE_INVALID_COMMAND = -2,         ///< The command is invalid or not recognized.
    PARSE_OUT_OF_MEMORY = -3,
    PARSE_NOT_SUPPORTED = -4,
} Parse_rct;

class Robot;
/**
 * * @brief abstruct for parsing commands from a buffer and creating ICommand objects.
 * * This interface defines a method for parsing commands from a byte buffer and returning
 * * the result as an ICommand pointer. It is intended to be implemented by classes that
 * * handle specific command parsing logic.
 */
template <typename Derived>
class CommandParser {
protected:
    Robot* _robot = nullptr;
public:
    inline void begin(Robot* robot){ _robot = robot; }

    /**
     * @brief Parses a command from the provided buffer.
     * 
     * @param commandBuffer Pointer to the buffer containing the command data.
     * @param resCom Pointer to a pointer where the parsed command will be stored.
     * @return Parse_rct Result code indicating the outcome of the parsing operation.
     */
    SV_METHOD3(Derived, Parse_rct, parse, uint8_t*, commandBuffer, uint8_t, bufferLen, ICommand**, resCom); 
};

#pragma once
#include "../CommandParser.h"

class ArmRobotParser : public CommandParser<ArmRobotParser> {
public:
    /**
     * @brief Parses a command from the provided buffer.
     * 
     * @param commandBuffer Pointer to the buffer containing the command data.
     * @param resCom Pointer to a pointer where the parsed command will be stored.
     * @return Parse_rct Result code indicating the outcome of the parsing operation.
     */
    Parse_rct parse(uint8_t* commandBuffer, uint8_t bufferLen,  ICommand** resCom);
};
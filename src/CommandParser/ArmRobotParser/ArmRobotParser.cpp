#include "armRobotParser.h"
#include "Robot/Robot.h"

#include "CommandParser/Command/ViewSensors.h"

#include <esp_log.h>

static const char* LOG_TAG = "APCP";

Parse_rct ArmRobotParser::parse(uint8_t *commandBuffer, uint8_t bufferLen, ICommand **resCom)
{
    *resCom = nullptr;

    switch (commandBuffer[0]) {
        case 's':
            *resCom = new ViewSensors<LIMIT_SWITCH_TYPE>(robot.limitSwitchs);
            break;
        default:
            return PARSE_INVALID_COMMAND;
    }

    if (*resCom == nullptr) {
        ESP_LOGE(LOG_TAG,"allocation failed");
        return PARSE_OUT_OF_MEMORY;
    }

    return PARSE_OK;
}

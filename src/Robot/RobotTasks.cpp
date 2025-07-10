#include "Robot.h"
#include "CommandParser/Command/LogDecorator/LogDecorator.h"
#define COMM_SERIAL Serial

static const char* LOG_TAG = "Robot";

extern Robot robot;
static uint8_t buff[8];
/**
 * Task to listen for serial commands
 */
void Robot::serialLisener(void* arg) {
    COMMAND_PARSER_TYPE* parser = reinterpret_cast<COMMAND_PARSER_TYPE*>(arg);
    ESP_LOGI(LOG_TAG,"serialLisener activated");
    ICommand* cmd = nullptr;
    for (;;) {
        if (COMM_SERIAL.available()) {
            uint8_t bufflen = COMM_SERIAL.readBytes(buff, sizeof(buff));
            if (bufflen > 0) {
                ESP_LOGI(LOG_TAG,"got Command");
                Parse_rct parseRes = parser->parse(buff, bufflen, &cmd);
                if (parseRes == PARSE_OK) {
                    // Placement new copy of command
                    ICommand* cmd = new LogDecorator(cmd);
                    cmd->execute();
                    delete cmd;
                    cmd = nullptr;
                } else {
                    ESP_LOGE(LOG_TAG,"parser returned with %d", parseRes);
                    cmd = nullptr;
                }
            }
        }
        delay(10);
    }
}
#define SERIAL_LISENER_STACK_SIZE       2000
void Robot::startTasks(){
    xTaskCreate(
        serialLisener,
        "serialLisener",
        SERIAL_LISENER_STACK_SIZE,
        &commandParser,
        1,
        nullptr
    );
}
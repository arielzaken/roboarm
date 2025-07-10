#pragma once
#include "ICommand.h"
#include <esp_log.h>

template <typename LIMIT_SWITCH>
class ViewSensors : public ICommand{
    LIMIT_SWITCH* _limitSwitches;
public:
    ViewSensors(LIMIT_SWITCH* limitSwitches) : _limitSwitches(limitSwitches) {};
    ExecuteResult execute() override;
};

// implementaion        //////////////////////////////////////////////////////////////////////////////////////////////

template <typename LIMIT_SWITCH>
ExecuteResult ViewSensors<LIMIT_SWITCH>::execute(){
    ExecuteResult res;
    ESP_LOGI("ViewSensors", "s0: %d s1: %d s2: %d s3: %d", _limitSwitches[0].read(), _limitSwitches[1].read(), _limitSwitches[2].read(), _limitSwitches[3].read());
    return res;
}
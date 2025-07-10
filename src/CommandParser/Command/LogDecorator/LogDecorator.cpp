#include "LogDecorator.h"
#include <esp_log.h>
static const char* LOG_TAG = "LogDecorator";

LogDecorator::LogDecorator(ICommand *_decorable):
    decorable(_decorable)
{
}

LogDecorator::~LogDecorator()
{
    delete decorable;
    decorable = nullptr;
}

ExecuteResult LogDecorator::execute()
{
    ESP_LOGI(LOG_TAG,"executing");
    ExecuteResult exeRes = decorable->execute();
    if(exeRes.code == 0){
        ESP_LOGI(LOG_TAG,"executed successfuly!");
    }
    else if(exeRes.code > 0){
        ESP_LOGW(LOG_TAG,"execute succseded with warning: %d\n%s", exeRes.code, exeRes.message);
    }
    else{
        ESP_LOGE(LOG_TAG,"execute failed with: %d\n%s", exeRes.code, exeRes.message);
    }
    return exeRes;
}
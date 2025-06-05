// Observable.h
#pragma once

#include <stddef.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

// Generic observer interface
template<typename T>
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(const T& value) = 0;
};

// Observable class with internal queue and task
template<typename T, size_t MaxObservers = 4>
class ObservableISR {
public:
    ObservableISR(const char* taskName = "ObservableTask", uint32_t stackSize = 2048, UBaseType_t priority = 1, BaseType_t core = APP_CPU_NUM) {
        _queue = xQueueCreate(1, sizeof(T));
        xTaskCreatePinnedToCore(taskLoop, taskName, stackSize, this, priority, &_task, core);
    }

    virtual ~ObservableISR() {
        if (_queue) vQueueDelete(_queue);
        if (_task) vTaskDelete(_task);
    }

    void addObserver(IObserver<T>* obs) {
        for (size_t i = 0; i < MaxObservers; ++i) {
            if (!_observers[i]) {
                _observers[i] = obs;
                return;
            }
        }
    }

    void removeObserver(IObserver<T>* obs) {
        for (size_t i = 0; i < MaxObservers; ++i) {
            if (_observers[i] == obs) {
                _observers[i] = nullptr;
                return;
            }
        }
    }

protected:
    void notifyFromISR(const T& data) {
        BaseType_t hpw = pdFALSE;
        if (_queue) {
            xQueueSendFromISR(_queue, &data, &hpw);
            if (hpw) portYIELD_FROM_ISR();
        }
    }

private:
    IObserver<T>* _observers[MaxObservers] = {};
    QueueHandle_t _queue = nullptr;
    TaskHandle_t _task = nullptr;

    static void taskLoop(void* param) {
        auto* self = static_cast<ObservableISR*>(param);
        T evt;
        while (true) {
            if (xQueueReceive(self->_queue, &evt, portMAX_DELAY) == pdTRUE) {
                for (size_t i = 0; i < MaxObservers; ++i) {
                    if (self->_observers[i]) {
                        self->_observers[i]->onNotify(evt);
                    }
                }
            }
        }
    }
};
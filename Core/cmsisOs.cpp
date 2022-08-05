#include "cmsisOs.h"

namespace osWrapper {
    Task::Task(bool suspended, uint16_t stackDepth, osPriority priority)
    {
        isSuspended = suspended;
        this->stackDepth = stackDepth;
        this->priority = priority;
        handle = NULL;
        if (!suspended) {
            resume();
        }
    }

    void Task::suspend()
    {
        isSuspended = true;
        osThreadSuspend(handle);
    }

    void Task::terminate()
    {
        osThreadTerminate(handle);
    }

    void Task::resume()
    {
        if (handle != NULL) {
            osThreadResume(handle);
        } else {
            const osThreadAttr_t defaultTask_attributes = {
              .name = "",
              .stack_size = stackDepth,
              .priority = priority,
            };
            handle = osThreadNew(entry, this, &defaultTask_attributes);
        }
        isSuspended = false;
    }

    bool Task::suspended()
    {
        return isSuspended;
    }

    void Task::delay(uint32_t ms)
    {
        osDelay(ms / portTICK_PERIOD_MS);
    }

    osThreadId_t Task::getCurrentTaskHandle()
    {
        return osThreadGetId();
    }

    void Task::entry(void *params)
    {
        static_cast<Task *>(params)->exec();
        osThreadExit();
    }
}
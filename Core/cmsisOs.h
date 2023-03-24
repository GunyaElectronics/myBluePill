#pragma once

#include "cmsis_os.h"
#include <stddef.h>
#include <stdint.h>

namespace osWrapper
{
    template <typename T> class osQueue {
    public:
        bool put(T &item)
        {
            return osMessageQueuePut(pQueueHandle, &item, 0, 0) == osOK;
        }

        bool get(T *pItem, uint32_t timeoutMs = 0xFFFFFFFF)
        {
            return osMessageQueueGet(pQueueHandle, pItem, NULL, timeoutMs) == osOK;
        }

        osQueue(size_t maxCount)
        {
            pQueueHandle = osMessageQueueNew(maxCount, sizeof(T), NULL);
        }

        ~osQueue()
        {
            osMessageQueueDelete(pQueueHandle);
        }
    private:
        void *pQueueHandle = NULL;
        size_t elementsCount;
    };

    class Task {
    public:
        Task(bool suspended = true, uint16_t stackDepth = 256,
             osPriority priority = osPriorityNormal);
        ~Task()
        {
        }
        void suspend();
        void terminate();
        void resume();
        bool suspended();
        static void delay(uint32_t ms);
        static osThreadId_t getCurrentTaskHandle();

    public:
        virtual void exec() = 0;

    private:
        static void entry(void *params);

    private:
        uint16_t stackDepth;
        osPriority priority;
        osThreadId_t handle;
        bool isSuspended;
    };

    template <typename T, void (T::*Method)()> class MethodTask : public Task {
    public:
        MethodTask(T *object = NULL, bool suspended = true, uint16_t stackDepth = 256,
                   osPriority priority = osPriorityNormal)
            : Task(suspended, stackDepth, priority), tObject(object) {}

        void setObject(T *object) { tObject = object; }

        void exec() { (tObject->*Method)(); }

    private:
        T *tObject;
    };
}

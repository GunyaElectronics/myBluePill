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
        Task()
        {
        }

        ~Task()
        {
        }
    private:
    };
}

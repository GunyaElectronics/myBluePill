#pragma once

#include "BSP.h"

class GPIO {
    BSP_gpioHandle_t handle;

public:
    GPIO(uint32_t pin_number);
    ~GPIO();

    void modeInput();
    void modeOutput();

    inline void waitInterrupt();

    void togle(void);
    void operator= (bool value);
    bool operator== (bool value);
    bool operator!= (bool value);
};
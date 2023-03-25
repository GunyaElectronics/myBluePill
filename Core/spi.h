#pragma once

#include "BSP.h"

class SPI {
    BSP_spiHandle_t handle;

public:
    SPI(uint32_t pin_number);
    ~SPI();

};

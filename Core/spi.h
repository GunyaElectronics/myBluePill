#pragma once

#include "BSP.h"

class SPI {
    BSP_spiHandle_t handle;

public:
    SPI(BSP_spiNumber_t number, BSP_spiMode_t mode = BSP_SPI_MODE_0, BSP_spiRxTxMethod_t transferMethod = BSP_SPI_RTX_BLOCKING);
    ~SPI();

    void nssHigh()
    {
        BSP_spiSetNss(handle.number, BSP_GPIO_SET);
    }

    void nssLow()
    {
        BSP_spiSetNss(handle.number, BSP_GPIO_RESET);
    }

    virtual uint8_t readReg(uint8_t address)
    {
        const uint8_t dummy = 0;

        BSP_spiWriteReadByte(handle.number, address);
        return BSP_spiWriteReadByte(handle.number, dummy);
    }

    virtual void writeReg(uint8_t address, uint8_t value)
    {
        BSP_spiWriteReadByte(handle.number, address);
        BSP_spiWriteReadByte(handle.number, value);
    }
};

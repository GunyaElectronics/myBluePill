#include "gpio.h"
#include "spi.h"
#include "main.h"
#include "asserts.h"
#include "MCUPinout.h"

SPI::SPI(BSP_spiNumber_t number, BSP_spiMode_t mode, BSP_spiRxTxMethod_t transferMethod)
{
    handle.number = number;
    handle.mode = mode;
    handle.transferMethod = transferMethod;

    BSP_spiInit(&handle);
}

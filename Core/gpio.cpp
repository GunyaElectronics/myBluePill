#include "gpio.h"
#include "main.h"
#include "asserts.h"
#include "MCUPinout.h"

static const Io bluePill[] = {
    IO_INIT_VALUES
};

GPIO::GPIO(uint32_t pinNumber)
{
    ASSERT(pinNumber < PIN_COUNT);

    handle.pinNumber = pinNumber;
    handle.ioType = BSP_GPIO_INPUT;
    handle.io.inputType = BSP_GPIO_IN_NOPULL;

    BSP_gpioInit(&handle, BSP_GPIO_RESET);
}

void GPIO::modeOutput()
{
    handle.ioType = BSP_GPIO_OUTPUT;
    handle.io.outputType = BSP_GPIO_OUT_PP;

    BSP_gpioInit(&handle, BSP_GPIO_RESET);
}

void GPIO::operator= (bool value)
{
    if (value) {
        bluePill[handle.pinNumber].port->ODR |= bluePill[handle.pinNumber].mask;
    } else {
        bluePill[handle.pinNumber].port->ODR &= ~bluePill[handle.pinNumber].mask;
    }
}

bool GPIO::operator== (bool value)
{
    return ((bool) (bluePill[handle.pinNumber].port->IDR & bluePill[handle.pinNumber].mask)) ==
        value ? true : false;
}

bool GPIO::operator!= (bool value)
{
    return ((bool) (bluePill[handle.pinNumber].port->IDR & bluePill[handle.pinNumber].mask)) !=
        value ? true : false;
}
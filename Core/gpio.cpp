#include "gpio.h"
#include "main.h"
#include "asserts.h"
#include "MCUPinout.h"

static const Io bluePill[] = {
    IO_INIT_VALUES
};

GPIO::GPIO(uint32_t pinNumber)
{
    handle.pinNumber = pinNumber;

    ASSERT(pinNumber < PIN_COUNT);
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
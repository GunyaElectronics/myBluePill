#include "main.h"
#include "BSP.h"
#include "MCUPinout.h"
#include "gpio.h"
#include "spi.h"
#include "application.h"
#include "commandConsole.h"
#include "asserts.h"
#include "utils.h"

void application(void)
{
    SerialPortIo   uartPio = { 1, 115200 };
    CommandConsole console = { uartPio };

    SPI loraSpi = 0;
    loraSpi.nssHigh();

    GPIO led = PIN_PC13;
    led.modeOutput();

    while (true) {
        const uint32_t kBlinkPeriodMs = 500;
        led = true;
        osWrapper::Task::delay(kBlinkPeriodMs);
        led = false;
        osWrapper::Task::delay(kBlinkPeriodMs);
    }
}

bool CommandConsole::rebootCmd()
{
    if (handlerParamsCount) {
        io.putString(" System will be rebooted after ");
        io.putString(pParams[0]);
        io.putString(" second\r\n");
    } else {
        io.putString(" Rebooting...\r\n");
    }

    return false;
}

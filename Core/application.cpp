#include "cmsis_os.h"
#include "BSP.h"
#include "application.h"
#include "serialPort.h"
#include "asserts.h"

using namespace PeripheralNamespace;

void application(void)
{
    const BSP_uartNumber_t consoleUartNumber = 1;
    char str[] = "\r\n";

    SerialPort console = { consoleUartNumber, 115200 };
    console.open();

    while (true) {
        if (console.isByteReceived()) {

            BSP_greenLedToggle();

            str[0] = console.getChar();
            str[1] = (str[0] == '\r') ? '\n' : '\0';

            console.putString(str);
        }
    }
}
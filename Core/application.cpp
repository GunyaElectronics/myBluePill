#include "main.h"
#include "BSP.h"
#include "MCUPinout.h"
#include "gpio.h"
#include "spi.h"
#include "application.h"
#include "commandConsole.h"
#include "asserts.h"
#include "utils.h"
#include <stdio.h>

static bool helpCmd(const char **pParams, size_t paramCount);
static bool exitCmd(const char **pParams, size_t paramCount);
static bool rebootCmd(const char **pParams, size_t paramCount);

static const Command allCmds[] = {
    { "?",      "[command]",          "Show help", helpCmd },
    { "help",   "[command]",          "Show help", helpCmd },
    { "exit",   "",                   "Exit from the console", exitCmd },
    { "reboot", "[-d] [delay value]", "Example: reboot -d 2 "
                                      "(delay 2 seconds before reboot)", rebootCmd },
};

static ISerialInputOutput *pIo = NULL;
static CommandConsole *pConsole = NULL;

void application(void)
{
    CommandConsole console = { allCmds, COUNT_OF(allCmds) };
    pConsole = &console;
    const uint32_t kBlinkPeriodMs = 500;
    GPIO led = PIN_PC13;
    led.modeOutput();

    SerialPortIo uartPio = {1, 115200};

    SPI loraSpi = 0;

    loraSpi.nssHigh();

    pIo = &uartPio;
    console.start(pIo);

    while (true) {
        led = true;
        osWrapper::Task::delay(kBlinkPeriodMs);
        led = false;
        osWrapper::Task::delay(kBlinkPeriodMs);
    }
}

static bool helpCmd(const char **pParams, size_t paramCount)
{
    static char helpString[128];

    if (paramCount) {
        // if selected one command - view detail info.
        const Command *pCmd = pConsole->findCommand(pParams[0]);
        if (pCmd != NULL) {
            snprintf(helpString, sizeof(helpString),
                     " Syntax:\r\n"
                     "\t%s\t%s\r\n"
                     " Description:\r\n"
                     "\t%s\r\n",
                     pCmd->pCmd,
                     pCmd->pHelp,
                     pCmd->pReference);
            pIo->putString(helpString);
        } else {
            pIo->putString((char *)" Command not found\r\n");
        }
    } else {
        for (size_t i = 0; i < COUNT_OF(allCmds); i++) {
            snprintf(helpString, sizeof(helpString), "\t%s\t%s\r\n",
                     allCmds[i].pCmd,
                     allCmds[i].pHelp);
            pIo->putString(helpString);
        }
    }

    return false;
}

static bool exitCmd(const char **pParams, size_t paramCount)
{
    return true;
}

static bool rebootCmd(const char **pParams, size_t paramCount)
{
    if (paramCount) {
        pIo->putString((char *)" System will be reboted after ");
        pIo->putString((char *)pParams[1]);
        pIo->putString((char *)" second\r\n");
    } else {
        pIo->putString((char *)" Rebooting...\r\n");
    }
    return false;
}

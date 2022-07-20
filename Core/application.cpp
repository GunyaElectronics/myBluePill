#include "cmsis_os.h"
#include "BSP.h"
#include "application.h"
#include "serialPort.h"
#include "commandConsole.h"
#include "asserts.h"
#include "utils.h"
#include <stdio.h>

static bool helpCmd(const char *pParams, size_t paramCount);
static bool exitCmd(const char *pParams, size_t paramCount);
static bool rebootCmd(const char *pParams, size_t paramCount);

static const Command allCmds[] = {
    { "?",      "[command]",          "Show help", helpCmd },
    { "help",   "[command]",          "Show help", helpCmd },
    { "exit",   "",                   "Exit from the console", exitCmd },
    { "reboot", "[-p] [pause value]", "Example: -p 2 (pause 2 seconds before reboot)", rebootCmd },
};

static CommandConsole console = { &allCmds[0], COUNT_OF(allCmds) };
static SerialInputOutput *pIo = NULL;

void application(void)
{
    console.start(pIo);

    while (true) {
        BSP_greenLedToggle();
        osDelay(500);
    }
}

static bool helpCmd(const char *pParams, size_t paramCount)
{
    static char helpString[64];

    if (paramCount) {
        // if selected one command - view detail info.
    } else {
        for (size_t i = 0; i < COUNT_OF(allCmds); i++) {
            snprintf(helpString, sizeof(helpString), "%s %s\r\n",
                     allCmds[i].pCmd,
                     allCmds[i].pHelp);
            pIo->putString(helpString);
        }
    }

    return false;
}

static bool exitCmd(const char *pParams, size_t paramCount)
{
    return false;
}

static bool rebootCmd(const char *pParams, size_t paramCount)
{
    return false;
}

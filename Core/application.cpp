#include "cmsis_os.h"
#include "BSP.h"
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
    { "reboot", "[-p] [pause value]", "Example: -p 2 (pause 2 seconds before reboot)", rebootCmd },
};

static CommandConsole console = { &allCmds[0], COUNT_OF(allCmds) };
static ISerialInputOutput *pIo = NULL;

void application(void)
{
    SerialPortIo uartPio = {1, 115200};

    pIo = &uartPio;
    console.start(pIo);

    while (true) {
        console.exec();
    }
}

static bool helpCmd(const char **pParams, size_t paramCount)
{
    static char helpString[64];

    BSP_greenLedToggle();

    if (paramCount) {
        // if selected one command - view detail info.
        const Command *pCmd = console.findCommand(pParams[0]);
        if (pCmd != NULL) {
            snprintf(helpString, sizeof(helpString),
                     "Syntax:\r\n"
                     "\t%s %s\r\n"
                     "Description:\r\n"
                     "\t%s\r\n",
                     pCmd->pCmd,
                     pCmd->pHelp,
                     pCmd->pReference);
        } else {
            pIo->putString((char *)"Command not found\r\n");
        }
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

static bool exitCmd(const char **pParams, size_t paramCount)
{
    return true;
}

static bool rebootCmd(const char **pParams, size_t paramCount)
{
    return false;
}

#include "commandConsole.h"
#include "asserts.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>

typedef bool (CommandConsole::*Handler)();

struct Command {
    char const *pCmd;
    char const *pHelp;
    char const *pReference;
    Handler handler;
};

Command const *getCmd(size_t index)
{
    static const Command allCmds[] = {
        { "?",      "[command]",          "Show help", &CommandConsole::helpCmd },
        { "help",   "[command]",          "Show help", &CommandConsole::helpCmd },
        { "exit",   "",                   "Exit from the console", &CommandConsole::exitCmd },
        { "reboot", "[-d] [delay value]", "Example: reboot -d 2 "
                                          "(delay 2 seconds before reboot)", &CommandConsole::rebootCmd },
    };

    if (index < COUNT_OF(allCmds)) {
        return &allCmds[index];
    } else {
        return NULL;
    }
}

bool CommandConsole::hasParameter(size_t paramIndex, char const *pStr)
{
    return strcmp((char const*)pReceivedParams[paramIndex], pStr) == 0;
}

bool CommandConsole::isSymbolValid(char c)
{
#if 0 // TODO: add supporting to detect all symbols
    if (isalnum(c) ||
        isspace(c) ||
        isprint(c) ||
        c == 0x7F)
        return true;

    return false;
#else
return true;
#endif
}

void CommandConsole::readLine(void)
{
    const char kBackspaceSymbolCode = 0x7F;
    uint16_t symbolCounter = 0;

    while (true) {
        char c = io.getChar();

        if (!isSymbolValid(c)) {
            io.putChar(c);
            continue;
        }

        if (!(symbolCounter == 0 && c == kBackspaceSymbolCode)) {
            io.putChar(c);
        }

        if (c == '\r' || c == '\n') {
            c = '\0';
        }

        receivedCommandLine[symbolCounter] = c;

        if (c != kBackspaceSymbolCode) {
            symbolCounter++;
        } else if (symbolCounter) {
            symbolCounter--;
        }

        if (symbolCounter == sizeof(receivedCommandLine)) {
            symbolCounter--;
        }

        if (c == '\0' && symbolCounter) {
            return;
        }
    }
}

const Command *findCommand(const char *pParam)
{
    for (uint8_t i = 0; getCmd(i) != NULL; i++) {
        if (strcmp(getCmd(i)->pCmd, pParam) == 0) {
            return getCmd(i);
        }
    }

    return NULL;
}

bool CommandConsole::helpCmd()
{
    static char helpString[128];

    if (handlerParamsCount) {
        // if selected one command - view detail info.
        const Command *pCmd = findCommand(pParams[0]);
        if (pCmd != NULL) {
            snprintf(helpString, sizeof(helpString),
                     " Syntax:\r\n"
                     "\t%s\t%s\r\n"
                     " Description:\r\n"
                     "\t%s\r\n",
                     pCmd->pCmd,
                     pCmd->pHelp,
                     pCmd->pReference);
            io.putString(helpString);
        } else {
            io.putString((char *)" Command not found\r\n");
        }
    } else {
        for (size_t i = 0; getCmd(i) != NULL; i++) {
            snprintf(helpString, sizeof(helpString), "\t%s\t%s\r\n",
                     getCmd(i)->pCmd,
                     getCmd(i)->pHelp);
            io.putString(helpString);
        }
    }

    return false;
}

bool CommandConsole::exitCmd()
{
    return true;
}

void CommandConsole::parseParameters()
{
    enum {
        START_PARAM,
        END_PARAM
    } state = START_PARAM;

    paramsCount = 0;

    for (uint8_t i = 0; paramsCount < COUNT_OF(pReceivedParams); i++) {
        char *pParam = &receivedCommandLine[i];

        switch (state) {
        case START_PARAM:
            if (*pParam != SEPARATOR  && *pParam != '\0') {
                pReceivedParams[paramsCount++] = pParam;
                state = END_PARAM;
            } else {
                return;
            }
            break;
        case END_PARAM:
            if (*pParam == SEPARATOR) {
                *pParam = '\0';
                state = START_PARAM;
            } else if (*pParam == '\0') {
                return;
            }
            break;
        }
    }
}

bool CommandConsole::routeCommand()
{
    const uint8_t kCmdIndex = 0;
    const uint8_t kFirstParamIndex = 1;

    if (paramsCount) {
        for (uint8_t i = 0; getCmd(i) != NULL; i++) {
            if (hasParameter(kCmdIndex, getCmd(i)->pCmd)) {
                pParams = (const char **)&pReceivedParams[kFirstParamIndex];
                handlerParamsCount = paramsCount - 1;
                return (this->*(getCmd(i)->handler))();
            }
        }

        io.putString((char *)" Command not found\r\n");
    }

    return false;
}

void CommandConsole::exec(void)
{
    bool exitStatus;

    io.putString((char *)"Terminal version 1.0\r\n");

    do {
        io.putChar(LINE_START);
        readLine();
        io.putString((char *)"\r\n");
        parseParameters();
        exitStatus = routeCommand();
    } while (!exitStatus);
}

void CommandConsole::task(void)
{
    while (true) {
        exec();
    }
}

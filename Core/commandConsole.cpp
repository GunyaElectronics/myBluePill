#include "commandConsole.h"
#include "asserts.h"
#include "utils.h"
#include <ctype.h>

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
        char c = pIo->getChar();

        if (!isSymbolValid(c)) {
            pIo->putChar(c);
            continue;
        }

        if (!(symbolCounter == 0 && c == kBackspaceSymbolCode)) {
            pIo->putChar(c);
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
        for (uint8_t i = 0; i < hendlerCount; i++) {
            if (hasParameter(kCmdIndex, pAllCmds[i].pCmd)) {
                return pAllCmds[i].handler((const char **)
                    &pReceivedParams[kFirstParamIndex], paramsCount - 1);
            }
        }

        pIo->putString((char *)" Command not found\r\n");
    }

    return false;
}

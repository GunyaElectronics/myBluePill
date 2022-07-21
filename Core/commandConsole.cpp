#include "commandConsole.h"

bool CommandConsole::hasParameter(size_t paramIndex, char const *pStr)
{
    return strcmp((char const*)pReceivedParams[paramIndex], pStr) == 0;
}

void CommandConsole::readLine(void)
{
    uint16_t symbolCounter = 0;

    while (true) {
        char c = pIo->getChar();
        pIo->putChar(c);

        switch (c) {
        case '\r':
        case '\n':
            c = '\0';
            break;
        default:
            break;
        }

        receivedCommandLine[symbolCounter++] = c;

        if (symbolCounter == sizeof(receivedCommandLine)) {
            symbolCounter--;
        }

        if (c == '\0') {
            return;
        }
    }
}
#include "commandConsole.h"

bool CommandConsole::hasParameter(size_t paramIndex, char const *pStr)
{
    return strcmp((char const*)pReceivedParams[paramIndex], pStr) == 0;
}

void CommandConsole::readLine(void)
{
    const char kBackspaceSymbolCode = 0x7F;
    uint16_t symbolCounter = 0;

    while (true) {
        char c = pIo->getChar();

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
}

bool CommandConsole::routeCommand()
{
    return false;
}

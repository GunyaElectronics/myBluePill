#pragma once

#include "serialInputOutput.h"
#include <string.h>

#define MAX_COMMAND_LINE_SIZE_BYTES   (96)
#define MAX_COMMAND_PARAM_COUNT       (6)

typedef bool (*Handler)(const char *pParams, size_t paramCount);

struct Command {
    char const *pCmd;
    char const *pHelp;
    char const *pReference;
    Handler handler;
};

class CommandConsole {
public:
    CommandConsole(const Command *pAllCmds, size_t countOfCmds)
    {
        this->pAllCmds = pAllCmds;
        hendlerCount = countOfCmds;
    }

    void start(SerialInputOutput *pIo)
    {
        this->pIo = pIo;
    }

    bool hasParameter(size_t paramIndex, char const *pStr)
    {
        return strcmp((char const*)pReceivedParams[paramIndex], pStr) == 0;
    }

    ~CommandConsole()
    {
    }

private:
    char receivedCommandLine[MAX_COMMAND_LINE_SIZE_BYTES];
    char *pReceivedParams[MAX_COMMAND_PARAM_COUNT];
    const Command *pAllCmds = NULL;
    SerialInputOutput *pIo = NULL;
    size_t hendlerCount = 0;
    size_t paramsCount = 0;
};
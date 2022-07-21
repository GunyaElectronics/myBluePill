#pragma once

#include "serialInputOutput.h"
#include <string.h>

#define MAX_COMMAND_LINE_SIZE_BYTES   (96)
#define MAX_COMMAND_PARAM_COUNT       (6)

typedef bool (*Handler)(const char **pParams, size_t paramCount);

struct Command {
    char const *pCmd;
    char const *pHelp;
    char const *pReference;
    Handler handler;
};

class CommandConsole {
    bool hasParameter(size_t paramIndex, char const *pStr);
    void readLine(void);
public:
    CommandConsole(const Command *pAllCmds, size_t countOfCmds)
    {
        this->pAllCmds = pAllCmds;
        hendlerCount = countOfCmds;
    }

    void start(ISerialInputOutput *pIo)
    {
        this->pIo = pIo;
    }

    ~CommandConsole()
    {
    }

    const Command *findCommand(const char *pParam)
    {
        return NULL;
    }

    void exec(void)
    {
        while (true) {
            readLine();
            pIo->putString("\r\n");
            pIo->putString(receivedCommandLine);
            pIo->putString("\r\n");
        }
    }
private:
    char receivedCommandLine[MAX_COMMAND_LINE_SIZE_BYTES];
    char *pReceivedParams[MAX_COMMAND_PARAM_COUNT];
    const Command *pAllCmds = NULL;
    ISerialInputOutput *pIo = NULL;
    size_t hendlerCount = 0;
    size_t paramsCount = 0;
};
#pragma once

#include "serialInputOutput.h"
#include "cmsisOs.h"
#include <string.h>

using namespace osWrapper;

#define MAX_COMMAND_LINE_SIZE_BYTES   (96)
#define MAX_COMMAND_PARAM_COUNT       (6)
#define SEPARATOR                     (' ')
#define LINE_START                    ('>')

typedef bool (*Handler)(const char **pParams, size_t paramCount);

struct Command {
    char const *pCmd;
    char const *pHelp;
    char const *pReference;
    Handler handler;
};

class CommandConsole {
    bool hasParameter(size_t paramIndex, char const *pStr);
    void readLine();
    void parseParameters();
    bool routeCommand();
    bool isSymbolValid(char c);
public:
    CommandConsole(const Command *pAllCmds, size_t countOfCmds)
    {
        this->pAllCmds = pAllCmds;
        hendlerCount = countOfCmds;
    }

    void start(ISerialInputOutput *pIo)
    {
        this->pIo = pIo;
        pTask = new MethodTask<CommandConsole, &CommandConsole::task>(this, false, 1024);
    }

    ~CommandConsole()
    {
    }

    const Command *findCommand(const char *pParam)
    {
        for (uint8_t i = 0; i < hendlerCount; i++) {
            if (strcmp(pAllCmds[i].pCmd, pParam) == 0) {
                return &pAllCmds[i];
            }
        }

        return NULL;
    }

private:
    void exec(void)
    {
        bool exitStatus;

        pIo->putString((char *)"Terminal version 1.0\r\n");

        do {
            pIo->putChar(LINE_START);
            readLine();
            pIo->putString((char *)"\r\n");
            parseParameters();
            exitStatus = routeCommand();
        } while (!exitStatus);
    }

    void task(void)
    {
        while (true) {
            exec();
        }
    }

    char receivedCommandLine[MAX_COMMAND_LINE_SIZE_BYTES];
    char *pReceivedParams[MAX_COMMAND_PARAM_COUNT];
    const Command *pAllCmds = NULL;
    ISerialInputOutput *pIo = NULL;
    size_t hendlerCount = 0;
    size_t paramsCount = 0;
    Task* pTask = NULL;
};
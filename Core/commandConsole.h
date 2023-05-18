#pragma once

#include "serialInputOutput.h"
#include "cmsisOs.h"
#include <string.h>

using namespace osWrapper;

#define MAX_COMMAND_LINE_SIZE_BYTES   (96)
#define MAX_COMMAND_PARAM_COUNT       (6)
#define SEPARATOR                     (' ')
#define LINE_START                    ('>')

class CommandConsole {
public:
    CommandConsole(ISerialInputOutput& pIo) : io(pIo)
    {
        pTask = new MethodTask<CommandConsole, &CommandConsole::task>(this, false, 1024);
    }

    ~CommandConsole()
    {
    }

    //External commands, must be implemented on the user code space
    bool rebootCmd();

    //Built-in commands
    bool exitCmd();
    bool helpCmd();

private:
    bool hasParameter(size_t paramIndex, char const *pStr);
    void readLine();
    void parseParameters();
    bool routeCommand();
    bool isSymbolValid(char c);
    void exec(void);
    void task(void);

    char receivedCommandLine[MAX_COMMAND_LINE_SIZE_BYTES];
    char *pReceivedParams[MAX_COMMAND_PARAM_COUNT];
    ISerialInputOutput& io;
    size_t paramsCount = 0;
    Task* pTask = NULL;
    const char **pParams = NULL;
    size_t handlerParamsCount = 0;
};

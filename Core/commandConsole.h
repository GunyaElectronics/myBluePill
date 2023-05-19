#pragma once

#include "serialInputOutput.h"
#include "cmsisOs.h"
#include <string.h>

using namespace osWrapper;

class CommandConsole {
public:
    CommandConsole(ISerialInputOutput& pIo) : io(pIo)
    {
        static constexpr uint16_t taskStackSizeWords = 1024;
        pTask = new
            MethodTask<CommandConsole, &CommandConsole::task>(this, false, taskStackSizeWords);
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

    static constexpr uint8_t maxCommandLineSizeBytes = 96;
    static constexpr uint8_t maxCommandLineParamCount = 6;
    char receivedCommandLine[maxCommandLineSizeBytes];
    char *pReceivedParams[maxCommandLineParamCount];
    ISerialInputOutput& io;
    size_t paramsCount = 0;
    Task* pTask = NULL;
    const char **pParams = NULL;
    size_t handlerParamsCount = 0;
    static constexpr uint8_t separator = ' ';
    static constexpr uint8_t lineStart = '>';
};

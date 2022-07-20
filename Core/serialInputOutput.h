#pragma once

class SerialInputOutput {
public:
    SerialInputOutput() {}
    virtual void putChar(char c) = NULL;
    virtual void putString(char *pString) = NULL;
    virtual char getChar() = NULL;
    virtual ~SerialInputOutput() {}
};
#pragma once

#include "serialPort.h"
#include <string.h>

// SerialInputOutput interface
class ISerialInputOutput {
public:
    ISerialInputOutput() {}
    virtual void putChar(char c) = NULL;
    virtual void putString(char *pString) = NULL;
    virtual char getChar() = NULL;
    virtual ~ISerialInputOutput() {}
};

using namespace PeripheralNamespace;

class SerialPortIo: public ISerialInputOutput, public SerialPort
{
public:
    SerialPortIo(uint8_t serialNumber, uint32_t baudrate) : SerialPort(serialNumber, baudrate)
    {
        open();
    }

    ~SerialPortIo()
    {
    }

    void putChar(char c)
    {
        putBytesBlocking((uint8_t *)&c);
    }

    void putString(char *pString)
    {
        putBytesBlocking((uint8_t *)pString, strlen(pString));
    }

    char getChar()
    {
        return getByteBlocking();
    }
};
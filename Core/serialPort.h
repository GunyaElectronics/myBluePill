#pragma once

#include <stdint.h>
#include <stddef.h>

namespace PeripheralNamespace
{
    class SerialPort {
        uint8_t serialNumber;
        uint32_t baudrate;

        uint8_t indexIn;
        uint8_t indexOut;
        uint8_t charBuffer[2];
    public:
        SerialPort(uint8_t serialNumber, uint32_t baudrate = 9600)
        {
            this->serialNumber = serialNumber;
            this->baudrate = baudrate;
            indexIn = indexOut = 0;
        }

        void byteReceived(uint8_t byte);
        bool open();
        bool isByteReceived();
        char getChar();
        void putString(char *pLine);

        static SerialPort* pSerialInstance1;

        ~SerialPort()
        {
        }
    };
}
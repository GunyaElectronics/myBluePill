#pragma once

#include "cmsisOs.h"
#include <stdint.h>
#include <stddef.h>

namespace PeripheralNamespace
{
    class SerialPort {
    public:
        SerialPort(uint8_t serialNumber, uint32_t baudrate = 9600)
        {
            this->serialNumber = serialNumber;
            this->baudrate = baudrate;
        }

        void byteReceived(uint8_t byte);
        bool open();
        bool isByteReceived();
        char getChar();
        uint8_t getByteBlocking();
        void putBytesBlocking(uint8_t *pBytes, size_t count = 1);

        static SerialPort* pSerialInstance1;

        ~SerialPort()
        {
        }
    private:
        uint8_t lastRxByte;
        bool isByteAvaliable = false;
        uint8_t serialNumber = 0;
        uint32_t baudrate;
        osWrapper::osQueue<uint8_t> rxQueue = 2;
    };
}
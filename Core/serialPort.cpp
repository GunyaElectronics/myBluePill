#include "serialPort.h"
#include "BSP.h"
#include "asserts.h"

#include <string.h>

using namespace PeripheralNamespace;

SerialPort *SerialPort::pSerialInstance1 = NULL;

void BSP_serialPort1ReceivedByteCb(uint8_t byte)
{
    ASSERT(SerialPort::pSerialInstance1 != NULL);

    SerialPort::pSerialInstance1->byteReceived(byte);
}

bool SerialPort::open()
{
    BSP_uartReceivedByteInterrupt receivedByteCb = NULL;

    switch (serialNumber) {
    case 1:
        pSerialInstance1 = this;
        receivedByteCb = BSP_serialPort1ReceivedByteCb;
        break;
    default:
        break;
    }

    ASSERT(receivedByteCb != NULL);

    const BSP_uartHandle_t kSerialConfig = {
        .number           = serialNumber,
        .receivingType    = BSP_UART_RX_INTERRUPT,
        .transmittingType = BSP_UART_TX_BLOCKING,
        .receivedByteCb   = receivedByteCb,
    };

    return BSP_uartInit(&kSerialConfig, baudrate) == BSP_RESULT_OK &&
           BSP_uartStartReceive(serialNumber)     == BSP_RESULT_OK;
}

void SerialPort::byteReceived(uint8_t byte)
{
    charBuffer[indexIn] = byte;
    indexIn = indexIn ? 0 : 1;
}

bool SerialPort::isByteReceived()
{
    return indexIn != indexOut;
}

char SerialPort::getChar()
{
    char byte = charBuffer[indexOut];
    indexOut = indexOut ? 0 : 1;
    return byte;
}

void SerialPort::putString(char *pLine)
{
    BSP_uartSendBlocking(serialNumber,(uint8_t *) pLine, strlen(pLine));
}
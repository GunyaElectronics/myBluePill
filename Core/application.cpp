#include "cmsis_os.h"
#include "BSP.h"
#include "application.h"

uint32_t gRxCount = 0;
uint32_t gTxCount = 0;
uint8_t charBuffer[2];

void BSP_consoleReceivedByteCb(uint8_t byte)
{
    static uint8_t index = 0;
#if 0
    putToQueueFromIsr(byte);
#endif
    charBuffer[index] = byte;
    gRxCount++;
    index = index ? 0 : 1;
}

void application(void)
{
    static uint8_t index = 0;
    uint8_t receivedByte = 0;

    const BSP_uartNumber_t consoleUartNumber = 1;
    const BSP_uartHandle_t consoleConfig = {
        .number           = consoleUartNumber,
        .receivingType    = BSP_UART_RX_INTERRUPT,
        .transmittingType = BSP_UART_TX_BLOCKING,
        .receivedByteCb   = BSP_consoleReceivedByteCb,
    };

    BSP_uartInit(&consoleConfig, 115200);
    BSP_uartStartReceive(consoleUartNumber);

    while (true) {
#if 0
        if (getFromQueue(&receivedByte))
#endif
        if (gTxCount != gRxCount) {
            receivedByte = charBuffer[index];
            index = index ? 0 : 1;
            gTxCount++;
            BSP_greenLedToggle();
            if (receivedByte == '\r') {
                BSP_uartSendBlocking(consoleUartNumber,(uint8_t *) "\r\n", sizeof("\r\n"));
            } else {
                BSP_uartSendBlocking(consoleUartNumber, &receivedByte, sizeof(receivedByte));
            }
        } else {
            osDelay(2);
        }
    }
}
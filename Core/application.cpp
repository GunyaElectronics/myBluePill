#include "cmsis_os.h"
#include "BSP.h"
#include "application.h"
#include "asserts.h"

static const uint32_t kMaxAppTimeoutTicks = UINT32_MAX;
static osMessageQueueId_t gConsoleRxQueue = NULL;;

void BSP_consoleReceivedByteCb(uint8_t byte)
{
    if (gConsoleRxQueue != NULL) {
        osMessageQueuePut(gConsoleRxQueue, &byte, 0U, 0U);
    }
}

void application(void)
{
    const BSP_uartNumber_t consoleUartNumber = 1;
    const BSP_uartHandle_t consoleConfig = {
        .number           = consoleUartNumber,
        .receivingType    = BSP_UART_RX_INTERRUPT,
        .transmittingType = BSP_UART_TX_BLOCKING,
        .receivedByteCb   = BSP_consoleReceivedByteCb,
    };

    const uint8_t kQueueElementCount = 2U;
    uint8_t receivedByte = 0U;
    gConsoleRxQueue = osMessageQueueNew(kQueueElementCount, sizeof(receivedByte), NULL);

    ASSERT(gConsoleRxQueue != NULL);

    BSP_uartInit(&consoleConfig, 115200U);
    BSP_uartStartReceive(consoleUartNumber);

    while (true) {
        if (osMessageQueueGet(gConsoleRxQueue, &receivedByte, NULL, kMaxAppTimeoutTicks) == osOK) {
            BSP_greenLedToggle();
            if (receivedByte == '\r') {
                BSP_uartSendBlocking(consoleUartNumber,(uint8_t *) "\r\n", sizeof("\r\n"));
            } else {
                BSP_uartSendBlocking(consoleUartNumber, &receivedByte, sizeof(receivedByte));
            }
        }
    }
}
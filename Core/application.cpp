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
#define CARRIAGE_RETURN_STR    ("\r\n")

    const BSP_uartNumber_t consoleUartNumber = 1;
    const BSP_uartHandle_t consoleConfig = {
        .number           = consoleUartNumber,
        .receivingType    = BSP_UART_RX_INTERRUPT,
        .transmittingType = BSP_UART_TX_BLOCKING,
        .receivedByteCb   = BSP_consoleReceivedByteCb,
    };

    const uint8_t kQueueElementCount = 2U;
    uint8_t receivedBytes[] = CARRIAGE_RETURN_STR;
    gConsoleRxQueue = osMessageQueueNew(kQueueElementCount, sizeof(uint8_t), NULL);

    ASSERT(gConsoleRxQueue != NULL);

    BSP_uartInit(&consoleConfig, 115200U);
    BSP_uartStartReceive(consoleUartNumber);

    while (true) {
        if (osMessageQueueGet(gConsoleRxQueue, receivedBytes, NULL, kMaxAppTimeoutTicks) == osOK) {

            BSP_greenLedToggle();

            uint8_t size = (receivedBytes[0] == '\r') ?
                sizeof(CARRIAGE_RETURN_STR) - sizeof((uint8_t)'\0') : sizeof(uint8_t);

            BSP_uartSendBlocking(consoleUartNumber, receivedBytes, size);
        }
    }

#undef CARRIAGE_RETURN_STR
}
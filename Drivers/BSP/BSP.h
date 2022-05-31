#pragma once
#ifdef  __cplusplus
extern "C"
{
#endif

// -------------------------------- Include --------------------------------------

#include <stdint.h>

// -------------------------------- Typedefs -------------------------------------

typedef enum {
    BSP_RESULT_FAIL = 0,
    BSP_RESULT_OK
} BSP_Result_t;

typedef enum {
    BSP_UART_RX_BLOCKING,
    BSP_UART_RX_INTERRUPT,
    BSP_UART_RX_DMA
} BSP_uartReceivingType_t;

typedef enum {
    BSP_UART_TX_BLOCKING,
    BSP_UART_TX_INTERRUPT,
    BSP_UART_TX_DMA
} BSP_uartTransmittingType_t;

typedef uint8_t BSP_uartNumber_t;
typedef void (*BSP_uartReceivedByteInterrupt)(uint8_t byte);
typedef void (*BSP_uartTxReadyInterrupt)(void);

typedef struct {
    BSP_uartNumber_t number;
    BSP_uartReceivingType_t receivingType;
    BSP_uartTransmittingType_t transmittingType;
    uint8_t *pTxBuffer;
    uint8_t rxByte;
    uint8_t *pRxBuffer;
    BSP_uartReceivedByteInterrupt receivedByteCb;
    BSP_uartTxReadyInterrupt txReadyCb;
} BSP_uartHandle_t;

// ---------------------------- Function prototypes ------------------------------

void BSP_disableInterrupt(void);

void BSP_greenLedInit(void);
void BSP_greenLedToggle(void);

BSP_Result_t BSP_uartInit(const BSP_uartHandle_t *pUartHandle, uint32_t baudrate);
BSP_Result_t BSP_uartStartReceive(BSP_uartNumber_t uartNumber);
BSP_Result_t BSP_uartSendBlocking(BSP_uartNumber_t uartNumber, uint8_t *pData, uint16_t sizeBytes);

#ifdef  __cplusplus
}
#endif
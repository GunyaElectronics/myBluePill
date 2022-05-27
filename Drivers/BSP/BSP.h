#pragma once
#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>

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

void BSP_disableInterrupt(void);

void BSP_greenLedInit(void);
void BSP_greenLedToggle(void);

void BSP_uartInit(BSP_uartNumber_t uartNumber, uint32_t baudrate);
void BSP_uartSendBlocking(BSP_uartNumber_t uartNumber, uint8_t *pData, uint16_t sizeBytes);

#ifdef  __cplusplus
}
#endif
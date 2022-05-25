#pragma once
#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef uint8_t BSP_uartNumber_t;

void BSP_greenLedInit(void);
void BSP_greenLedToggle(void);

void BSP_uartInit(BSP_uartNumber_t uartNumber, uint32_t baudrate);

#ifdef  __cplusplus
}
#endif
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

// ------------------- UART Typedefs --------------------------

typedef enum {
    BSP_UART_RX_BLOCKING,
    BSP_UART_RX_INTERRUPT,
    BSP_UART_RX_DMA
} BSP_uartReceivingMethod_t;

typedef enum {
    BSP_UART_TX_BLOCKING,
    BSP_UART_TX_INTERRUPT,
    BSP_UART_TX_DMA
} BSP_uartTransmittingMethod_t;

typedef uint8_t BSP_uartNumber_t;
typedef void (*BSP_uartReceivedByteInterrupt)(uint8_t byte);
typedef void (*BSP_uartTxReadyInterrupt)(void);

typedef struct {
    BSP_uartNumber_t number;
    BSP_uartReceivingMethod_t receivingMethod;
    BSP_uartTransmittingMethod_t transmittingMethod;
    uint8_t *pTxBuffer;
    uint8_t rxByte;
    uint8_t *pRxBuffer;
    BSP_uartReceivedByteInterrupt receivedByteCb;
    BSP_uartTxReadyInterrupt txReadyCb;
} BSP_uartHandle_t;

// ---------------------- SPI typedefs ------------------------

typedef enum {
    BSP_SPI_RTX_BLOCKING,
    BSP_SPI_RTX_INTERRUPT,
    BSP_SPI_RTX_DMA
} BSP_spiRxTxMethod_t;

typedef enum {
    BSP_SPI_MODE_0,
    BSP_SPI_MODE_1,
    BSP_SPI_MODE_2,
    BSP_SPI_MODE_3
} BSP_spiMode_t;

typedef uint8_t BSP_spiNumber_t;

typedef struct {
    BSP_spiNumber_t number;
    BSP_spiMode_t mode;
    BSP_spiRxTxMethod_t transferMethod;
} BSP_spiHandle_t;

// --------------------- GPIO typedefs ------------------------

typedef enum {
    BSP_GPIO_RESET,
    BSP_GPIO_SET
} BSP_gpioPinState;

typedef enum {
    BSP_GPIO_INPUT,
    BSP_GPIO_OUTPUT
} BSP_gpioIoType_t;

typedef enum {
    BSP_GPIO_IN_NOPULL,
    BSP_GPIO_IN_PULLUP,
    BSP_GPIO_IN_PULLDOWN,
    BSP_GPIO_IN_ALTERNATE,
    BSP_GPIO_IN_ALTERNATE_PULLUP,
    BSP_GPIO_IN_ALTERNATE_PULLDOWN
} BSP_gpioInputType_t;

typedef enum {
    BSP_GPIO_OUT_PP, // Push-Pull
    BSP_GPIO_OUT_OD, // Open Drain
    BSP_GPIO_OUT_OD_PULLUP,
    BSP_GPIO_OUT_OD_PULLDOWN,
    BSP_GPIO_OUT_ALTERNATE_PP,
    BSP_GPIO_OUT_ALTERNATE_OD
} BSP_gpioOutputType_t;

typedef enum {
    BSP_GPIO_IT_NONE,
    BSP_GPIO_IT_FALLING,
    BSP_GPIO_IT_RISING,
    BSP_GPIO_IT_TOGGLE
} BSP_gpioInterruptType_t;

typedef uint32_t BSP_gpioNumber_t;
typedef void (*BSP_gpioInterruptCb_t)(void);

typedef struct {
    BSP_gpioInterruptType_t type;
    BSP_gpioInterruptCb_t callBack;
} BSP_gpioInterrupt_t;

typedef struct {
    BSP_gpioNumber_t pinNumber;
    BSP_gpioIoType_t ioType;
    union {
        BSP_gpioInputType_t inputType;
        BSP_gpioOutputType_t outputType;
    } io;
    BSP_gpioInterrupt_t it;
} BSP_gpioHandle_t;

// ---------------------------- Function prototypes ------------------------------

void BSP_disableInterrupt(void);

void BSP_greenLedInit(void);
void BSP_greenLedToggle(void);

BSP_Result_t BSP_uartInit(const BSP_uartHandle_t *pUartHandle, uint32_t baudrate);
BSP_Result_t BSP_uartStartReceive(BSP_uartNumber_t uartNumber);
BSP_Result_t BSP_uartSendBlocking(BSP_uartNumber_t uartNumber, uint8_t *pData, uint16_t sizeBytes);

BSP_Result_t BSP_spiInit(const BSP_spiHandle_t *pHandle);
uint8_t BSP_spiWriteReadByte(BSP_spiNumber_t number, uint8_t byte);
BSP_Result_t BSP_spiWrite(BSP_spiNumber_t number, const uint8_t *pData, uint16_t sizeBytes);
BSP_Result_t BSP_spiRead(BSP_spiNumber_t number, uint8_t *pData, uint16_t sizeBytes);
void BSP_spiSetNss(BSP_spiNumber_t number, BSP_gpioPinState state);

BSP_Result_t BSP_gpioInit(const BSP_gpioHandle_t *pHandle, BSP_gpioPinState initState);
BSP_gpioPinState BSP_gpioRead(BSP_gpioNumber_t pinNumber);
void BSP_gpioWrite(BSP_gpioNumber_t pinNumber, BSP_gpioPinState state);
void BSP_gpioToggle(BSP_gpioNumber_t pinNumber);

#ifdef  __cplusplus
}
#endif
#include "main.h"
#include "BSP.h"
#include "MCUPinout.h"
#include "asserts.h"
#include <stdbool.h>

UART_HandleTypeDef gUartConsole;
static BSP_uartHandle_t gUartConsoleHandle;

void BSP_disableInterrupt(void)
{
    __disable_irq();
}

void BSP_greenLedInit(void)
{
    GREEN_LED_CLK_ENABLE();

    GPIO_InitTypeDef led;

    led.Pin = GREEN_LED_PIN;
    led.Mode = GPIO_MODE_OUTPUT_PP;
    led.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GREEN_LED_GPIO, &led);
}

void BSP_greenLedToggle(void)
{
    HAL_GPIO_TogglePin(GREEN_LED_GPIO, GREEN_LED_PIN);
}

BSP_Result_t BSP_uartInit(const BSP_uartHandle_t *pUartHandle, uint32_t baudrate)
{
    BSP_Result_t result = BSP_RESULT_FAIL;

    ASSERT(pUartHandle != NULL);

    switch (pUartHandle->number) {
    case CONSOLE_UART_NUMBER:
        gUartConsoleHandle = *pUartHandle;

        gUartConsole.Instance = CONSOLE_UART;
        gUartConsole.Init.BaudRate = baudrate;
        gUartConsole.Init.WordLength = UART_WORDLENGTH_8B;
        gUartConsole.Init.StopBits = UART_STOPBITS_1;
        gUartConsole.Init.Parity = UART_PARITY_NONE;
        gUartConsole.Init.Mode = UART_MODE_TX_RX;
        gUartConsole.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        gUartConsole.Init.OverSampling = UART_OVERSAMPLING_16;
 
        ASSERT(HAL_UART_Init(&gUartConsole) == HAL_OK);
        result = BSP_RESULT_OK;
        break;
    default:
        break;
    }

    return result;
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (huart->Instance == CONSOLE_UART) {
        CONSOLE_UART_CLK_ENABLE();

        CONSOLE_UART_GPIO_CLK_ENABLE();

        GPIO_InitStruct.Pin = CONSOLE_UART_TX_GPIO;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(CONSOLE_UART_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = CONSOLE_UART_RX_GPIO;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(CONSOLE_UART_GPIO, &GPIO_InitStruct);

        if (gUartConsoleHandle.receivingMethod == BSP_UART_RX_INTERRUPT) {
            HAL_NVIC_SetPriority(CONSOLE_UART_IRQN,
                                 CONSOLE_IRQ_PRIORITY,
                                 CONSOLE_IRQ_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(CONSOLE_UART_IRQN);
        } else if (gUartConsoleHandle.receivingMethod == BSP_UART_RX_DMA) {
            // TODO: init DMA
        }
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == CONSOLE_UART) {
        CONSOLE_UART_CLK_DISABLE();

        HAL_GPIO_DeInit(CONSOLE_UART_GPIO, CONSOLE_UART_TX_GPIO | CONSOLE_UART_RX_GPIO);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uartHandle)
{
    if (uartHandle->Instance == CONSOLE_UART) {
        if (gUartConsoleHandle.receivedByteCb != NULL) {
            gUartConsoleHandle.receivedByteCb(gUartConsoleHandle.rxByte);
        }
        ASSERT(HAL_UART_Receive_IT(&gUartConsole, 
                                   &gUartConsoleHandle.rxByte,
                                   sizeof(gUartConsoleHandle.rxByte)) == HAL_OK);
    }
}

BSP_Result_t BSP_uartStartReceive(BSP_uartNumber_t uartNumber)
{
    BSP_Result_t result = BSP_RESULT_FAIL;

    switch (uartNumber) {
    case CONSOLE_UART_NUMBER:
        ASSERT(HAL_UART_Receive_IT(&gUartConsole, 
                                   &gUartConsoleHandle.rxByte,
                                   sizeof(gUartConsoleHandle.rxByte)) == HAL_OK);
        result = BSP_RESULT_OK;
        break;
    default:
        break;
    }

    return result;
}

BSP_Result_t BSP_uartSendBlocking(BSP_uartNumber_t uartNumber, uint8_t *pData, uint16_t sizeBytes)
{
    ASSERT(sizeBytes > 0);
    ASSERT(pData != NULL);

    BSP_Result_t result = BSP_RESULT_FAIL;

    switch (uartNumber) {
    case CONSOLE_UART_NUMBER:
        ASSERT(HAL_UART_Transmit(&gUartConsole, pData, sizeBytes, 0xFFFFFFFF) == HAL_OK);
        result = BSP_RESULT_OK;
        break;
    default:
        break;
    }

    return result;
}

BSP_Result_t BSP_spiInit(const BSP_spiHandle_t *pHandle)
{
    return BSP_RESULT_FAIL;
}

uint8_t BSP_spiWriteReadByte(BSP_spiNumber_t number, uint8_t byte)
{
    return 0;
}

BSP_Result_t BSP_spiWrite(BSP_spiNumber_t number, const uint8_t *pData, uint16_t sizeBytes)
{
    return BSP_RESULT_FAIL;
}

BSP_Result_t BSP_spiRead(BSP_spiNumber_t number, uint8_t *pData, uint16_t sizeBytes)
{
    return BSP_RESULT_FAIL;
}

void BSP_spiSetNss(BSP_spiNumber_t number, BSP_gpioPinState state)
{
}

static uint8_t getGpio(uint16_t *pPin, GPIO_TypeDef **ppPort, BSP_gpioNumber_t pinNumber)
{
    bool result = true;

    switch (pinNumber) {
    case PIN_PC13: *ppPort = GPIOC; *pPin = GPIO_PIN_13; break;
    case PIN_PC14: *ppPort = GPIOC; *pPin = GPIO_PIN_14; break;
    case PIN_PC15: *ppPort = GPIOC; *pPin = GPIO_PIN_15; break;

    case PIN_PD0:  *ppPort = GPIOD; *pPin = GPIO_PIN_0; break;
    case PIN_PD1:  *ppPort = GPIOD; *pPin = GPIO_PIN_1; break;

    case PIN_PA0:  *ppPort = GPIOA; *pPin = GPIO_PIN_0; break;
    case PIN_PA1:  *ppPort = GPIOA; *pPin = GPIO_PIN_1; break;
    case PIN_PA2:  *ppPort = GPIOA; *pPin = GPIO_PIN_2; break;
    case PIN_PA3:  *ppPort = GPIOA; *pPin = GPIO_PIN_3; break;
    case PIN_PA4:  *ppPort = GPIOA; *pPin = GPIO_PIN_4; break;
    case PIN_PA5:  *ppPort = GPIOA; *pPin = GPIO_PIN_5; break;
    case PIN_PA6:  *ppPort = GPIOA; *pPin = GPIO_PIN_6; break;
    case PIN_PA7:  *ppPort = GPIOA; *pPin = GPIO_PIN_7; break;

    case PIN_PB1:  *ppPort = GPIOB; *pPin = GPIO_PIN_1; break;
    case PIN_PB2:  *ppPort = GPIOB; *pPin = GPIO_PIN_2; break;
    case PIN_PB10: *ppPort = GPIOB; *pPin = GPIO_PIN_10; break;
    case PIN_PB11: *ppPort = GPIOB; *pPin = GPIO_PIN_11; break;
    case PIN_PB12: *ppPort = GPIOB; *pPin = GPIO_PIN_12; break;
    case PIN_PB13: *ppPort = GPIOB; *pPin = GPIO_PIN_13; break;
    case PIN_PB14: *ppPort = GPIOB; *pPin = GPIO_PIN_14; break;
    case PIN_PB15: *ppPort = GPIOB; *pPin = GPIO_PIN_15; break;

    case PIN_PA8:  *ppPort = GPIOA; *pPin = GPIO_PIN_8; break;
    case PIN_PA9:  *ppPort = GPIOA; *pPin = GPIO_PIN_9; break;
    case PIN_PA10: *ppPort = GPIOA; *pPin = GPIO_PIN_10; break;
    case PIN_PA11: *ppPort = GPIOA; *pPin = GPIO_PIN_11; break;
    case PIN_PA12: *ppPort = GPIOA; *pPin = GPIO_PIN_12; break;
    case PIN_PA13: *ppPort = GPIOA; *pPin = GPIO_PIN_13; break;
    case PIN_PA14: *ppPort = GPIOA; *pPin = GPIO_PIN_14; break;
    case PIN_PA15: *ppPort = GPIOA; *pPin = GPIO_PIN_15; break;

    case PIN_PB3:  *ppPort = GPIOB; *pPin = GPIO_PIN_3; break;
    case PIN_PB4:  *ppPort = GPIOB; *pPin = GPIO_PIN_4; break;
    case PIN_PB5:  *ppPort = GPIOB; *pPin = GPIO_PIN_5; break;
    case PIN_PB6:  *ppPort = GPIOB; *pPin = GPIO_PIN_6; break;
    case PIN_PB7:  *ppPort = GPIOB; *pPin = GPIO_PIN_7; break;
    case PIN_PB8:  *ppPort = GPIOB; *pPin = GPIO_PIN_8; break;
    case PIN_PB9:  *ppPort = GPIOB; *pPin = GPIO_PIN_9; break;

    default:
        result = false;
    }

    return result;
}

static void enableGpioClock(GPIO_TypeDef *pPort)
{
    if (pPort == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (pPort == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (pPort == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (pPort == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (pPort == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
}

static uint32_t getModeOut(BSP_gpioOutputType_t out)
{
    switch (out) {
    case BSP_GPIO_OUT_PP: return GPIO_MODE_OUTPUT_PP;
    case BSP_GPIO_OUT_OD: return GPIO_MODE_OUTPUT_OD;
    case BSP_GPIO_OUT_ALTERNATE_PP: return GPIO_MODE_AF_PP;
    case BSP_GPIO_OUT_ALTERNATE_OD: return GPIO_MODE_AF_OD;
    default:
        break;
    }

    ASSERT(0);
}

static uint32_t getPullUpDown(BSP_gpioInputType_t inputType)
{
    switch (inputType) {
    case BSP_GPIO_IN_NOPULL:
    case BSP_GPIO_IN_ALTERNATE:
        return GPIO_NOPULL;
    case BSP_GPIO_IN_PULLUP:
    case BSP_GPIO_IN_ALTERNATE_PULLUP:
        return GPIO_PULLUP;
    case BSP_GPIO_IN_PULLDOWN:
    case BSP_GPIO_IN_ALTERNATE_PULLDOWN:
        return GPIO_PULLDOWN;
    }

    ASSERT(0);
}

BSP_Result_t BSP_gpioInit(const BSP_gpioHandle_t *pHandle, BSP_gpioPinState initState)
{
    uint16_t pin;
    GPIO_TypeDef *pPort;
    GPIO_InitTypeDef config;

    if (!getGpio(&pin, &pPort, pHandle->pinNumber)) {
        return BSP_RESULT_FAIL;
    }

    config.Pin = pin;
    config.Speed = GPIO_SPEED_FREQ_HIGH;

    config.Mode = (pHandle->ioType == BSP_GPIO_OUTPUT) ? getModeOut(pHandle->io.outputType) :
                                                         GPIO_MODE_INPUT;
    config.Pull = (pHandle->ioType == BSP_GPIO_OUTPUT) ? GPIO_NOPULL :
                                                         getPullUpDown(pHandle->io.inputType);
    enableGpioClock(pPort);
    HAL_GPIO_Init(pPort, &config);

    BSP_gpioWrite(pHandle->pinNumber, initState);

    return BSP_RESULT_OK;
}

BSP_gpioPinState BSP_gpioRead(BSP_gpioNumber_t pinNumber)
{
    uint16_t pin;
    GPIO_TypeDef *pPort;

    if (!getGpio(&pin, &pPort, pinNumber)) {
        return BSP_GPIO_RESET;
    }

    if ((pPort->IDR & pin) != (uint32_t)GPIO_PIN_RESET) {
        return BSP_GPIO_SET;
    }

    return BSP_GPIO_RESET;
}

void BSP_gpioWrite(BSP_gpioNumber_t pinNumber, BSP_gpioPinState state)
{
    uint16_t pin;
    GPIO_TypeDef *pPort;

    if (!getGpio(&pin, &pPort, pinNumber)) {
        return;
    }

    if (state == BSP_GPIO_SET) {
        pPort->BSRR = pin;
    } else {
        pPort->BSRR = (uint32_t)pin << 16u;
    }
}

void BSP_gpioToggle(BSP_gpioNumber_t pinNumber)
{
    uint16_t pin;
    GPIO_TypeDef *pPort;

    if (!getGpio(&pin, &pPort, pinNumber)) {
        return;
    }

    HAL_GPIO_TogglePin(pPort, pin);
}

#include "main.h"
#include "BSP.h"
#include "MCUPinout.h"
#include "asserts.h"

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

        if (gUartConsoleHandle.receivingType == BSP_UART_RX_INTERRUPT) {
            HAL_NVIC_SetPriority(CONSOLE_UART_IRQN,
                                 CONSOLE_IRQ_PRIORITY,
                                 CONSOLE_IRQ_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(CONSOLE_UART_IRQN);
        } else if (gUartConsoleHandle.receivingType == BSP_UART_RX_DMA) {
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

BSP_Result_t BSP_gpioInit(const BSP_gpioHandle_t *pHandle, BSP_gpioPinState initState)
{
    return BSP_RESULT_FAIL;
}

BSP_gpioPinState BSP_gpioRead(BSP_gpioNumber_t pinNumber)
{
    return BSP_GPIO_RESET;
}

void BSP_gpioWrite(BSP_gpioNumber_t pinNumber, BSP_gpioPinState state)
{
}

void BSP_gpioToggle(BSP_gpioNumber_t pinNumber)
{
}
#include "main.h"
#include "BSP.h"
#include "MCUPinout.h"
#include "asserts.h"

static UART_HandleTypeDef gUartConsole;

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

void BSP_uartInit(BSP_uartNumber_t uartNumber, uint32_t baudrate)
{
    switch (uartNumber) {
    case CONSOLE_UART_NUMBER:
        gUartConsole.Instance = CONSOLE_UART;
        gUartConsole.Init.BaudRate = baudrate;
        gUartConsole.Init.WordLength = UART_WORDLENGTH_8B;
        gUartConsole.Init.StopBits = UART_STOPBITS_1;
        gUartConsole.Init.Parity = UART_PARITY_NONE;
        gUartConsole.Init.Mode = UART_MODE_TX_RX;
        gUartConsole.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        gUartConsole.Init.OverSampling = UART_OVERSAMPLING_16;
 
        ASSERT(HAL_UART_Init(&gUartConsole) == HAL_OK);
        break;
    default:
        break;
    }
}
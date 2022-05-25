#include "main.h"
#include "BSP.h"
#include "MCUPinout.h"

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
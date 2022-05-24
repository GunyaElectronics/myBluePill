#include "main.h"
#include "BSP.h"

void BSP_ledInit(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef led;

    led.Pin = GPIO_PIN_13;
    led.Mode = GPIO_MODE_OUTPUT_PP;
    led.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC, &led);
}

void BSP_ledToggle(void)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
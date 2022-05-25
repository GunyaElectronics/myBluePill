#pragma once

#ifdef BLUE_PILL

#define GREEN_LED_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define GREEN_LED_GPIO          GPIOC
#define GREEN_LED_PIN           GPIO_PIN_13

#define CONSOLE_UART            USART1
#define CONSOLE_UART_NUMBER     1
#define CONSOLE_UART_BAUDRATE   115200

#else
#   error "Please define your board"
#endif
#pragma once

#ifdef BLUE_PILL

// GPIO base pins
#define GREEN_LED_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()
#define GREEN_LED_GPIO                      GPIOC
#define GREEN_LED_PIN                       GPIO_PIN_13

// UARTs
#define CONSOLE_UART                        USART1
#define CONSOLE_UART_NUMBER                 1
#define CONSOLE_UART_BAUDRATE               115200

#define CONSOLE_UART_CLK_ENABLE()           __HAL_RCC_USART1_CLK_ENABLE()
#define CONSOLE_UART_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define CONSOLE_UART_RX_GPIO                GPIO_PIN_10
#define CONSOLE_UART_TX_GPIO                GPIO_PIN_9
#define CONSOLE_UART_GPIO                   GPIOA

#else
#   error "Please define your board"
#endif
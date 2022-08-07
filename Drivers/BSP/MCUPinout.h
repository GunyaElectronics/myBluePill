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
#define CONSOLE_UART_IRQN                   USART1_IRQn
#define CONSOLE_UART_IRQHandler             USART1_IRQHandler

extern UART_HandleTypeDef gUartConsole;

#define CONSOLE_UART_CLK_ENABLE()           __HAL_RCC_USART1_CLK_ENABLE()
#define CONSOLE_UART_CLK_DISABLE()          __HAL_RCC_USART1_CLK_DISABLE()
#define CONSOLE_UART_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define CONSOLE_UART_RX_GPIO                GPIO_PIN_10
#define CONSOLE_UART_TX_GPIO                GPIO_PIN_9
#define CONSOLE_UART_GPIO                   GPIOA

// IRQ Priorities
#define CONSOLE_IRQ_PRIORITY                (6)
#define CONSOLE_IRQ_SUB_PRIORITY            (0)

enum {
    PIN_PC13 = 0, PIN_PC14, PIN_PC15, PIN_PD0, PIN_PD1, PIN_PA0,
    PIN_PA1, PIN_PA2, PIN_PA3, PIN_PA4, PIN_PA5, PIN_PA6, PIN_PA7, PIN_PB0, 
    PIN_PB1, PIN_PB2, PIN_PB10, PIN_PB11, PIN_PB12, PIN_PB13, PIN_PB14, PIN_PB15,
    PIN_PA8, PIN_PA9, PIN_PA10, PIN_PA11, PIN_PA12, PIN_PA13, PIN_PA14, PIN_PA15,
    PIN_PB3, PIN_PB4, PIN_PB5, PIN_PB6, PIN_PB7, PIN_PB8, PIN_PB9
};

#else
#   error "Please define your board"
#endif
#ifndef __ICO_H
#define __ICO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include  "DataType.h"




#define BAT_USART                             USART6
#define BAT_USART_CLK_ENABLE()                __USART6_CLK_ENABLE();



#define BAT_USART_RX_GPIO_PORT                GPIOC
#define BAT_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define BAT_USART_RX_PIN                      GPIO_PIN_7
#define BAT_USART_RX_AF                       GPIO_AF8_USART6


#define BAT_USART_TX_GPIO_PORT                GPIOC
#define BAT_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define BAT_USART_TX_PIN                      GPIO_PIN_6
#define BAT_USART_TX_AF                       GPIO_AF8_USART6

#define BAT_USART_IRQHandler                  USART6_IRQHandler
#define BAT_USART_IRQ                         USART6_IRQn

/************************************************************/


//串口波特率
#define BAT_USART_BAUDRATE                    38400


void BAT_USART_Config(void);
extern UART_HandleTypeDef UartHandle6;
void FlashTuBiao(void);
extern OS_TCB ICOTaskTCB;
//任务堆栈
extern CPU_STK ICO_TASK_STK[ICO_STK_SIZE];
void ICO_task(void *p_arg);
void BluidICOTask(void);
#endif /* __ICO_H */

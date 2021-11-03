#ifndef __RS232_USART_H
#define __RS232_USART_H

#include "stm32f7xx.h"
#include <stdio.h>
#include "stm32f7xx_hal.h"
#include  "DataType.h"



//引脚定义
/*******************************************************/

//#define RS232_USART                             USART6
//#define RS232_USART_CLK_ENABLE()                __USART6_CLK_ENABLE();

//#define RCC_PERIPHCLK_UARTx              		RCC_PERIPHCLK_USART6
//#define RCC_UARTxCLKSOURCE_SYSCLK         		RCC_USART6CLKSOURCE_SYSCLK

//#define RS232_USART_RX_GPIO_PORT                GPIOC
//#define RS232_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
//#define RS232_USART_RX_PIN                      GPIO_PIN_6
//#define RS232_USART_RX_AF                       GPIO_AF8_USART6


//#define RS232_USART_TX_GPIO_PORT                GPIOC
//#define RS232_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
//#define RS232_USART_TX_PIN                      GPIO_PIN_7
//#define RS232_USART_TX_AF                       GPIO_AF8_USART6

//#define RS232_USART_IRQHandler                  USART6_IRQHandler
//#define RS232_USART_IRQ                         USART6_IRQn

//#define RS232_USART                             UART4
//#define RS232_USART_CLK_ENABLE()                __UART4_CLK_ENABLE();

//#define RCC_PERIPHCLK_UARTx              		RCC_PERIPHCLK_UART4
//#define RCC_UARTxCLKSOURCE_SYSCLK         		RCC_UART4CLKSOURCE_SYSCLK

//#define RS232_USART_RX_GPIO_PORT                GPIOC
//#define RS232_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
//#define RS232_USART_RX_PIN                      GPIO_PIN_11
//#define RS232_USART_RX_AF                       GPIO_AF8_UART4


//#define RS232_USART_TX_GPIO_PORT                GPIOC
//#define RS232_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
//#define RS232_USART_TX_PIN                      GPIO_PIN_10
//#define RS232_USART_TX_AF                       GPIO_AF8_UART4

//#define RS232_USART_IRQHandler                  UART4_IRQHandler
//#define RS232_USART_IRQ                         UART4_IRQn




#define RS232_USART                             USART3
#define RS232_USART_CLK_ENABLE()                __USART3_CLK_ENABLE();

#define RCC_PERIPHCLK_UARTx              		RCC_PERIPHCLK_USART3
#define RCC_UARTxCLKSOURCE_SYSCLK         		RCC_USART3CLKSOURCE_SYSCLK

#define RS232_USART_RX_GPIO_PORT                GPIOC
#define RS232_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define RS232_USART_RX_PIN                      GPIO_PIN_11
#define RS232_USART_RX_AF                       GPIO_AF7_USART3


#define RS232_USART_TX_GPIO_PORT                GPIOC
#define RS232_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define RS232_USART_TX_PIN                      GPIO_PIN_10
#define RS232_USART_TX_AF                       GPIO_AF7_USART3

#define RS232_USART_IRQHandler                  USART3_IRQHandler
#define RS232_USART_IRQ                         USART3_IRQn

/************************************************************/


//串口波特率
#define RS232_USART_BAUDRATE                    115200
//任务控制块
extern OS_TCB DoUartTaskTCB;
//任务堆栈
extern CPU_STK DoUart_TASK_STK[DoUart_STK_SIZE];
extern struct ChanalMData ChanMData;
void DoRx(struct ChanalData *pCmd,struct ChanalMData *pMD);
void DoUart_task(void *p_arg);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void RS232_USART_Config(void);
void FlashTimeVal(struct ChanalSet *p);
void Usart_SendStringL( USART_TypeDef * pUSARTx, uint8_t *str,uint8_t len);
uint32 GetTotalTime(struct ChanalSet *p);
void SetChanSetMod(void);
void SetChanSetMod2(void);
void SetChanSetTime(void);
uint8 FileInit(void);
void MovingAverage(const float  data[], int span,int Length);
float DoFiler(const float  val, int span,uint8 ch);
float DoSW(const float  val,uint8 i);
float DoSWHmd(const float  val,uint8 i);
extern uint8_t ScreenFlag;   //截屏标识
extern UART_HandleTypeDef UartHandle;
extern struct ChanalData ChanData;
extern struct ChanalSet ChanSet;
extern struct ChanalHSet ChanHSet;
extern uint8 ShowTimeflag;
extern OS_SEM  Uart_CMD;       //定义一个信号量，用于任务同步
extern union SD_U Dsd;//sd卡存储数据
extern union SD_U DsdR;//sd
extern union SD_U DsdFM[10];//sd卡存储数据快速
extern volatile uint8_t  UartCmd;
extern volatile uint8_t Rxflag;
extern struct SLData chTemp;//端子内部温度
extern uint8_t MDataF;//快采标志
extern uint32 FileSaveNum;
#endif /* __USART1_H */

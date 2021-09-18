#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H



#include "stm32f7xx.h"
#include "common8266.h"

#include <stdio.h>
#include <stdbool.h>


extern  UART_HandleTypeDef ESP8266_UartHandle;
#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;






/******************************** ESP8266 连接引脚定义 ***********************************/
#define      macESP8266_CH_PD_CLK_ENABLE()                    __HAL_RCC_GPIOA_CLK_ENABLE()
#define      macESP8266_CH_PD_PORT                            GPIOA
#define      macESP8266_CH_PD_PIN                             GPIO_PIN_10

#define      macESP8266_RST_CLK_ENABLE()                      __HAL_RCC_GPIOC_CLK_ENABLE()
#define      macESP8266_RST_PORT                              GPIOC
#define      macESP8266_RST_PIN                               GPIO_PIN_9

 

#define      macESP8266_USART_BAUD_RATE                       115200



#define      USART8266											UART5

#define      macESP8266_USARTx                                UART5
#define      macESP8266_USART_CLK_ENABLE()                    __UART5_CLK_ENABLE()

#define      macESP8266_USART_TX_GPIO_CLK()                    __HAL_RCC_GPIOB_CLK_ENABLE()  
#define      macESP8266_USART_TX_PORT                         GPIOB   
#define      macESP8266_USART_TX_PIN                          GPIO_PIN_9
#define      macESP8266_USART_TX_AF                           GPIO_AF7_UART5

#define      macESP8266_USART_RX_GPIO_CLK()                    __HAL_RCC_GPIOB_CLK_ENABLE() 
#define      macESP8266_USART_RX_PORT                         GPIOB
#define      macESP8266_USART_RX_PIN                          GPIO_PIN_8
#define      macESP8266_USART_RX_AF                           GPIO_AF7_UART5

#define      macESP8266_USART_IRQ                             UART5_IRQn
#define      macESP8266_USART_INT_FUN                         UART5_IRQHandler
      
/*********************************************** ESP8266 函数宏定义 *******************************************/
#define     macESP8266_Usart( fmt, ... )           USART_printf ( macESP8266_USARTx, fmt, ##__VA_ARGS__ )  

////使用了3级管高低电平是反的
//#define     macESP8266_RST_HIGH_LEVEL()            HAL_GPIO_WritePin( macESP8266_RST_PORT, macESP8266_RST_PIN,GPIO_PIN_RESET)
//#define     macESP8266_RST_LOW_LEVEL()             HAL_GPIO_WritePin( macESP8266_RST_PORT, macESP8266_RST_PIN,GPIO_PIN_SET)



#define     macESP8266_RST_HIGH_LEVEL()            HAL_GPIO_WritePin( macESP8266_RST_PORT, macESP8266_RST_PIN,GPIO_PIN_SET)
#define     macESP8266_RST_LOW_LEVEL()             HAL_GPIO_WritePin( macESP8266_RST_PORT, macESP8266_RST_PIN,GPIO_PIN_RESET)

#define     macESP8266_CH_ENABLE()                 HAL_GPIO_WritePin( macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN,GPIO_PIN_SET)
#define     macESP8266_CH_DISABLE()                HAL_GPIO_WritePin( macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN,GPIO_PIN_RESET)

/****************************************** ESP8266 函数声明 ***********************************************/
void                     ESP8266_Init                        ( void );
void                     ESP8266_Rst                         ( void );
void WIFI_delay_m(u32 delay);








#endif



/**
  ******************************************************************
  * @file    bsp_esp8266.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   WIFI测试
  ******************************************************************
  * @attention
  * 实验平台:野火 STM32F767 开发板
  ******************************************************************
  */
#include "include.h"


UART_HandleTypeDef ESP8266_UartHandle;
static void                   ESP8266_GPIO_Config                 ( void );
static void                   ESP8266_USART_Config                ( void );




/**
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */
void ESP8266_Init ( void )
{
    ESP8266_GPIO_Config ();
    macESP8266_CH_ENABLE();
    ESP8266_USART_Config ();
    macESP8266_RST_HIGH_LEVEL();
    //macESP8266_CH_DISABLE();
}

//使能已经被电源常拉高了
/**
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
static void ESP8266_GPIO_Config ( void )
{

    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*开启相关的GPIO外设时钟*/
    macESP8266_CH_PD_CLK_ENABLE();
    macESP8266_RST_CLK_ENABLE();

    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.Pin = macESP8266_CH_PD_PIN;

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;

    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*设置引脚速率为高速 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(macESP8266_CH_PD_PORT, &GPIO_InitStruct);


    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.Pin = macESP8266_RST_PIN;
    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(macESP8266_RST_PORT, &GPIO_InitStruct);
}


/**
  * @brief  初始化ESP8266用到的 USART
  * @param  无
  * @retval 无
  */
static void ESP8266_USART_Config ( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

    macESP8266_USART_TX_GPIO_CLK();
    macESP8266_USART_RX_GPIO_CLK();


    /* 配置串口5时钟源*/
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART5;
    RCC_PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    /* 使能串口5时钟 */
    macESP8266_USART_CLK_ENABLE();

    /* 配置Tx引脚为复用功能  */
    GPIO_InitStruct.Pin = macESP8266_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = macESP8266_USART_TX_AF;
    HAL_GPIO_Init(macESP8266_USART_TX_PORT, &GPIO_InitStruct);

    /* 配置Rx引脚为复用功能 */
    GPIO_InitStruct.Pin = macESP8266_USART_RX_PIN;
    GPIO_InitStruct.Alternate = macESP8266_USART_RX_AF;
    HAL_GPIO_Init(macESP8266_USART_RX_PORT, &GPIO_InitStruct);

    /* 配置串GPS_USART 模式 */
    ESP8266_UartHandle.Instance = macESP8266_USARTx;
    ESP8266_UartHandle.Init.BaudRate = macESP8266_USART_BAUD_RATE;
    ESP8266_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    ESP8266_UartHandle.Init.StopBits = UART_STOPBITS_1;
    ESP8266_UartHandle.Init.Parity = UART_PARITY_NONE;
    ESP8266_UartHandle.Init.Mode = UART_MODE_TX_RX;
    ESP8266_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(&ESP8266_UartHandle);


    /*配置中断优先级*/
    HAL_NVIC_SetPriority(macESP8266_USART_IRQ, 2, 0);
    /*使能DMA中断*/
    HAL_NVIC_EnableIRQ(macESP8266_USART_IRQ);
    /*配置中断条件*/
    __HAL_USART_ENABLE_IT(&ESP8266_UartHandle,USART_IT_RXNE);
    __HAL_USART_ENABLE_IT(&ESP8266_UartHandle,USART_IT_IDLE);


}

/*
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 ESP8266_AT_Test 调用
 */
void ESP8266_Rst ( void )
{
    OS_ERR err;
    macESP8266_CH_ENABLE();
    macESP8266_RST_LOW_LEVEL();

    OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );  //延时
    macESP8266_RST_HIGH_LEVEL();

}

void WIFI_delay_m(u32 delay)
{
    OS_ERR err;
    if(delay>=1000)
    {
        OSTimeDlyHMSM(0, 0,delay/1000,0,OS_OPT_TIME_DLY,&err);
    }
    else
    {
        OSTimeDly(delay, OS_OPT_TIME_DLY, &err );
    }
}






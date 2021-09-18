/**
  ******************************************************************
  * @file    bsp_esp8266.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   WIFI����
  ******************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ�� STM32F767 ������
  ******************************************************************
  */
#include "include.h"


UART_HandleTypeDef ESP8266_UartHandle;
static void                   ESP8266_GPIO_Config                 ( void );
static void                   ESP8266_USART_Config                ( void );




/**
  * @brief  ESP8266��ʼ������
  * @param  ��
  * @retval ��
  */
void ESP8266_Init ( void )
{
    ESP8266_GPIO_Config ();
    macESP8266_CH_ENABLE();
    ESP8266_USART_Config ();
    macESP8266_RST_HIGH_LEVEL();
    //macESP8266_CH_DISABLE();
}

//ʹ���Ѿ�����Դ��������
/**
  * @brief  ��ʼ��ESP8266�õ���GPIO����
  * @param  ��
  * @retval ��
  */
static void ESP8266_GPIO_Config ( void )
{

    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*������ص�GPIO����ʱ��*/
    macESP8266_CH_PD_CLK_ENABLE();
    macESP8266_RST_CLK_ENABLE();

    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.Pin = macESP8266_CH_PD_PIN;

    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;

    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*������������Ϊ���� */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(macESP8266_CH_PD_PORT, &GPIO_InitStruct);


    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.Pin = macESP8266_RST_PIN;
    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(macESP8266_RST_PORT, &GPIO_InitStruct);
}


/**
  * @brief  ��ʼ��ESP8266�õ��� USART
  * @param  ��
  * @retval ��
  */
static void ESP8266_USART_Config ( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

    macESP8266_USART_TX_GPIO_CLK();
    macESP8266_USART_RX_GPIO_CLK();


    /* ���ô���5ʱ��Դ*/
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART5;
    RCC_PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    /* ʹ�ܴ���5ʱ�� */
    macESP8266_USART_CLK_ENABLE();

    /* ����Tx����Ϊ���ù���  */
    GPIO_InitStruct.Pin = macESP8266_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = macESP8266_USART_TX_AF;
    HAL_GPIO_Init(macESP8266_USART_TX_PORT, &GPIO_InitStruct);

    /* ����Rx����Ϊ���ù��� */
    GPIO_InitStruct.Pin = macESP8266_USART_RX_PIN;
    GPIO_InitStruct.Alternate = macESP8266_USART_RX_AF;
    HAL_GPIO_Init(macESP8266_USART_RX_PORT, &GPIO_InitStruct);

    /* ���ô�GPS_USART ģʽ */
    ESP8266_UartHandle.Instance = macESP8266_USARTx;
    ESP8266_UartHandle.Init.BaudRate = macESP8266_USART_BAUD_RATE;
    ESP8266_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    ESP8266_UartHandle.Init.StopBits = UART_STOPBITS_1;
    ESP8266_UartHandle.Init.Parity = UART_PARITY_NONE;
    ESP8266_UartHandle.Init.Mode = UART_MODE_TX_RX;
    ESP8266_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(&ESP8266_UartHandle);


    /*�����ж����ȼ�*/
    HAL_NVIC_SetPriority(macESP8266_USART_IRQ, 2, 0);
    /*ʹ��DMA�ж�*/
    HAL_NVIC_EnableIRQ(macESP8266_USART_IRQ);
    /*�����ж�����*/
    __HAL_USART_ENABLE_IT(&ESP8266_UartHandle,USART_IT_RXNE);
    __HAL_USART_ENABLE_IT(&ESP8266_UartHandle,USART_IT_IDLE);


}

/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ���� ESP8266_AT_Test ����
 */
void ESP8266_Rst ( void )
{
    OS_ERR err;
    macESP8266_CH_ENABLE();
    macESP8266_RST_LOW_LEVEL();

    OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );  //��ʱ
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






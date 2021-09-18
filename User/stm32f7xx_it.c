/**
  ******************************************************************************
  * @file    Templates/stm32f7xx_it.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    18-November-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_it.h"
#include "main.h"
#include "include.h"
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}




extern uint8_t ucTemp;
extern uint8 testcur;
uint8_t ucTest;
uint8_t ucflag=0;



void DoRx(struct ChanalData *pCmd,struct ChanalMData *pMD);
void RS232_USART_IRQHandler(void)
{
    if(__HAL_UART_GET_IT( &UartHandle, UART_IT_RXNE ) != RESET)
    {
        
        ucTest = HAL_UART_Receive(&UartHandle, (uint8_t *)&ucTemp, 1, 1000);	
	    DoRx(&ChanData,&ChanMData);
			
	
    }
    if (__HAL_UART_GET_FLAG(&UartHandle, USART_FLAG_ORE) != RESET)
    {
        __HAL_UART_CLEAR_OREFLAG(&UartHandle);
    }
    HAL_UART_IRQHandler(&UartHandle);
}





void DoBat(uint8_t ucT);
uint8_t ucTestBAT;
void BAT_USART_IRQHandler(void)
{
	static uint8_t ucTempBAT;
    if(__HAL_UART_GET_IT( &UartHandle6, UART_IT_RXNE ) != RESET)
    {
        ucTestBAT = HAL_UART_Receive(&UartHandle6, (uint8_t *)&ucTempBAT, 1, 1000);	
        DoBat(ucTempBAT);
    }
    if (__HAL_UART_GET_FLAG(&UartHandle6, USART_FLAG_ORE) != RESET)
    {
        __HAL_UART_CLEAR_OREFLAG(&UartHandle6);
    }
    HAL_UART_IRQHandler(&UartHandle6);
}
/**
  * @brief  This function handles macESP8266_USARTx Handler.
  * @param  None
  * @retval None
  */
void macESP8266_USART_INT_FUN ( void )
{
    uint8_t ucCh;
    if ( __HAL_UART_GET_IT (&ESP8266_UartHandle, UART_IT_RXNE ) != RESET )
    {
        /*读取数据*/
        ucCh = ESP8266_UartHandle.Instance->RDR;
        UART8266_WriteRecBuf(ucCh ); //写一个字节数据到接收缓冲区
        Uart8266.RX_STA=1;       //接收状态标记
    }
    else if(ESP8266_UartHandle.Instance->ISR >> 4 & 1)
    {
        /*清除空闲标志位*/
        ESP8266_UartHandle.Instance->ICR |= ((1<<4));
    }

    //**************发送中断*************
//      if(__HAL_UART_GET_IT(&ESP8266_UartHandle, USART_IT_TC) != RESET)    //发送中断,开发送完中断后会进入一次，原因不详。
//      {
//          __HAL_UART_CLEAR_IT(&ESP8266_UartHandle, USART_FLAG_TC);
//          if(UART8266_SendBufEmpty())  //发送缓冲区非空
//          {
//              ESP8266_UartHandle.Instance->TDR=UART8266_ReadSendBuf();  //读发送缓冲区一个字节
//              Uart8266.TX_STA=0;  //发送状态标记
//          }
//          else
//          {
//              Uart8266.TX_STA=1;  //发送状态标记
//          }
//      }
    if (__HAL_UART_GET_FLAG(&ESP8266_UartHandle, USART_FLAG_ORE) != RESET)
    {
        __HAL_UART_CLEAR_OREFLAG(&ESP8266_UartHandle);
    }
    HAL_UART_IRQHandler(&ESP8266_UartHandle);
}




void SDMMC2_IRQHandler(void)
{
    BSP_SD_IRQHandler();
}

/**
* @brief This function handles DMA2 stream3 global interrupt.
*/
void DMA2_Stream0_IRQHandler(void)
{
    BSP_SD_DMA_Rx_IRQHandler();
}

/**
* @brief This function handles DMA2 stream6 global interrupt.
*/
void DMA2_Stream5_IRQHandler(void)
{
    BSP_SD_DMA_Tx_IRQHandler();
}

///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/**
  * @brief This function handles USB On The Go HS End Point 1 Out global interrupt.
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_HS_EP1_OUT_IRQn 0 */

  /* USER CODE END OTG_HS_EP1_OUT_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
  /* USER CODE BEGIN OTG_HS_EP1_OUT_IRQn 1 */

  /* USER CODE END OTG_HS_EP1_OUT_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go HS End Point 1 In global interrupt.
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_HS_EP1_IN_IRQn 0 */

  /* USER CODE END OTG_HS_EP1_IN_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
  /* USER CODE BEGIN OTG_HS_EP1_IN_IRQn 1 */

  /* USER CODE END OTG_HS_EP1_IN_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go HS global interrupt.
  */
static PCD_TypeDef Int;
void OTG_HS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_HS_IRQn 0 */
   
  /* USER CODE END OTG_HS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
	
 
	  /* USER CODE BEGIN OTG_HS_IRQn 1 */


  /* USER CODE END OTG_HS_IRQn 1 */
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void  BASIC_TIM_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure);	 	
}


void  BASIC_TIM7_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure7);	 	
}

void  ADVANCE_TIM_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructureAD);	 	
}


/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

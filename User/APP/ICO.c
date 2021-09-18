/*
  * @file  ICO.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   图标闪烁
 */

#include "include.h"
//任务控制块
OS_TCB ICOTaskTCB;
//任务堆栈
CPU_STK ICO_TASK_STK[ICO_STK_SIZE];

u8 BatFlag=0;
UART_HandleTypeDef UartHandle6;
/**
 * @brief  BAT_USART GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
 

void BAT_USART_Config(void)
{
	
	InitRes[13] = 0xFF;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

    BAT_USART_RX_GPIO_CLK_ENABLE();
    BAT_USART_TX_GPIO_CLK_ENABLE();

    /* 配置串口6时钟源*/
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART6;
    RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART6CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    /* 使能 UART 时钟 */
    BAT_USART_CLK_ENABLE();


    /* 配置Tx引脚为复用功能  */
    GPIO_InitStruct.Pin = BAT_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = BAT_USART_TX_AF;
    HAL_GPIO_Init(BAT_USART_TX_GPIO_PORT, &GPIO_InitStruct);

    /* 配置Rx引脚为复用功能 */
    GPIO_InitStruct.Pin = BAT_USART_RX_PIN;
    GPIO_InitStruct.Alternate = BAT_USART_RX_AF;
    HAL_GPIO_Init(BAT_USART_RX_GPIO_PORT, &GPIO_InitStruct);

    /* 配置串BAT_USART 模式 */
    UartHandle6.Instance = BAT_USART;
    UartHandle6.Init.BaudRate = BAT_USART_BAUDRATE;
    UartHandle6.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle6.Init.StopBits = UART_STOPBITS_1;
    UartHandle6.Init.Parity = UART_PARITY_NONE;
    UartHandle6.Init.Mode = UART_MODE_TX_RX;
    UartHandle6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle6.Init.OverSampling = UART_OVERSAMPLING_16;
    UartHandle6.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    UartHandle6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    HAL_UART_Init(&UartHandle6);

    /*串口中断初始化 */
    HAL_NVIC_SetPriority(BAT_USART_IRQ, 4, 0);
    HAL_NVIC_EnableIRQ(BAT_USART_IRQ);
    /*配置串口接收中断 */
    __HAL_UART_ENABLE_IT(&UartHandle6,UART_IT_RXNE);

}



void DoBat(uint8_t ucT)
{
    static uint8 uart6startbit;           //串口3接收到开始符0xAC标志为，1为收到，0为没收到
    static uint8 flag = 1;
//    OS_ERR err;
    uint8_t a;

    a= ucT;
    if(uart6startbit==0)
    {
        if(a==ChrStart)
        {
            uart6startbit=1;
         
        }
    }
    else
    {

        if(a!=ChrStart) //第2个字节
        {
          BatFlag = a; 
          uart6startbit=0;
		  if(flag ==1)
		  {
			 InitRes[13] = 0;
			 flag =0;
		  }
	 
        }
        
    }
}
void SetTuBiaoXY(uint16_t* x,uint16_t* y)
{
	  if(DataSave.Data_type.PageFlag==FPageDisplayData||
       DataSave.Data_type.PageFlag==FPageCh||
       DataSave.Data_type.PageFlag==FPageCh3||
       DataSave.Data_type.PageFlag==FPageHome||
       DataSave.Data_type.PageFlag==FPageHDATA||
       DataSave.Data_type.PageFlag==FPageOtur||
       DataSave.Data_type.PageFlag==FPageIF)
    {
        *x=10+95*5;
        *y=5;

    }
    else if(DataSave.Data_type.PageFlag==FPageFile)
    {
        *x=10+95*2;
        *y=5;

    }
    else if(DataSave.Data_type.PageFlag==FPageGraph)
    {
        *x=490;
        *y=0;

    }


}	
void FlashTuBiao(void)
{
    static uint16_t x;
    static uint16_t y;
    static uint8 PageFlag=0xff;
    static uint8 CurCh3COL=0xff;
  
   if(DataSave.Data_type.PageFlag!=FPageRecord)
   {


		if(PageFlag!=DataSave.Data_type.PageFlag||
			(CurCh3.COL!=CurCh3COL&&DataSave.Data_type.PageFlag==FPageCh3))//页面变了
		{
			SetTuBiaoXY(&x,&y);
			DrawUdisk(x-20,y,usbstatus,ON);
			SetTuBiaoXY(&x,&y);
			DrawWIFI(x+10,y,DataSave.Data_type.parameters.SW,ON);
			SetTuBiaoXY(&x,&y);
			DrawHid(x+40,y,HidFlag,ON);
			SetTuBiaoXY(&x,&y);
			DrawSave(x+70, y,FileRes ,ON);
			SetTuBiaoXY(&x,&y);
			DrawBat(x+100,y,BatFlag,ON);
			PageFlag=DataSave.Data_type.PageFlag;
		    CurCh3COL=CurCh3.COL;
		}
		else
		{
            SetTuBiaoXY(&x,&y);
			DrawUdisk(x-20,y,usbstatus,OFF);
			SetTuBiaoXY(&x,&y);
			DrawWIFI(x+10,y,DataSave.Data_type.parameters.SW,OFF);
			SetTuBiaoXY(&x,&y);
			DrawHid(x+40,y,HidFlag,OFF);
			SetTuBiaoXY(&x,&y);
			DrawSave(x+70, y,FileRes,OFF);
			SetTuBiaoXY(&x,&y);
			DrawBat(x+100,y,BatFlag,OFF);			
		}	
		
	}

}

//设置RTC 任务500ms刷新一次
void ICO_task(void *p_arg)
{
    OS_ERR  err;
    static uint32 starttick=0;
    uint32 Endtick=0;
    uint32 Divtick=0;
    static uint8 flagInit = 0;
    while(1)
    {   
		FlashTuBiao();
		if(flagInit == 0)
		{
			starttick = HAL_GetTick(); 
			flagInit = 1;
			OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );  //延时20ms
		}
		else
		{
			Endtick = HAL_GetTick(); 
			Divtick  = Endtick-starttick;
			starttick= Endtick;
			if(Divtick <305)
				OSTimeDly ( 305-Divtick, OS_OPT_TIME_DLY, & err );  //
			else
				OSTimeDly (5, OS_OPT_TIME_DLY, & err );  //延时20ms
			
		}
    }
}
void BluidICOTask(void)
{
	OS_ERR  err;
  /* 创建RTC任务 时间刷新500ms 一次 */
    OSTaskCreate((OS_TCB     *)&ICOTaskTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"ICO Task",                             //任务名称
                 (OS_TASK_PTR ) ICO_task,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) ICO_TASK_PRIO,                          //任务的优先级
                 (CPU_STK    *)&ICO_TASK_STK[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) ICO_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) ICO_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);
}

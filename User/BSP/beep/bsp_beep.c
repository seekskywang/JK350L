/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   蜂鸣器函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32F767 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
 #include  "include.h"


OS_SEM  Beep_SEM;        //定义一个信号量，用于任务同步

//任务控制块
OS_TCB BeepTaskTCB;
//任务堆栈
CPU_STK Beep_TASK_STK[Beep_STK_SIZE];



 /**
  * @brief  初始化控制蜂鸣器的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{		
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启控制蜂鸣器的GPIO的端口时钟*/
    BEEP_GPIO_CLK_ENABLE();   

    /*选择要控制蜂鸣器的GPIO*/															   
    GPIO_InitStructure.Pin = BEEP_GPIO_PIN;	

    /*设置GPIO模式为通用推挽输出*/
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   

    GPIO_InitStructure.Pull = GPIO_PULLDOWN;

    /*设置GPIO速率为50MHz */   
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; 

    /*调用库函数，初始化控制蜂鸣器的GPIO*/
    HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 

    /*  关闭蜂鸣器*/
    HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN,GPIO_PIN_RESET);	 
}




 //KEY0任务
void Beep_task(void *p_arg)
{
    OS_ERR err;
//    uint16 i;
    //创建一个信号量
    OSSemCreate ((OS_SEM*   )&Beep_SEM,
                 (CPU_CHAR* )"Beep_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
    while(1)
    {
             OSSemPend(&Beep_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
		HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructureAD);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	    //关闭定时器
	    HAL_TIM_Base_Stop_IT(&TIM_TimeBaseStructureAD);
//        for(i=0;i<20;i++)
//		{
//			BEEP_ON;
//			OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_PERIODIC,&err);//延时ms
//			BEEP_OFF;
//			OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_PERIODIC,&err);//延时1ms
//			
//			
//        }
      
    }
}

/*********************************************END OF FILE**********************/

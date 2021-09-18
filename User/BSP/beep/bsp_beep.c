/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   �����������ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32F767 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
 #include  "include.h"


OS_SEM  Beep_SEM;        //����һ���ź�������������ͬ��

//������ƿ�
OS_TCB BeepTaskTCB;
//�����ջ
CPU_STK Beep_TASK_STK[Beep_STK_SIZE];



 /**
  * @brief  ��ʼ�����Ʒ�������IO
  * @param  ��
  * @retval ��
  */
void BEEP_GPIO_Config(void)
{		
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*�������Ʒ�������GPIO�Ķ˿�ʱ��*/
    BEEP_GPIO_CLK_ENABLE();   

    /*ѡ��Ҫ���Ʒ�������GPIO*/															   
    GPIO_InitStructure.Pin = BEEP_GPIO_PIN;	

    /*����GPIOģʽΪͨ���������*/
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   

    GPIO_InitStructure.Pull = GPIO_PULLDOWN;

    /*����GPIO����Ϊ50MHz */   
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; 

    /*���ÿ⺯������ʼ�����Ʒ�������GPIO*/
    HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 

    /*  �رշ�����*/
    HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN,GPIO_PIN_RESET);	 
}




 //KEY0����
void Beep_task(void *p_arg)
{
    OS_ERR err;
//    uint16 i;
    //����һ���ź���
    OSSemCreate ((OS_SEM*   )&Beep_SEM,
                 (CPU_CHAR* )"Beep_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
    while(1)
    {
             OSSemPend(&Beep_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
		HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructureAD);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	    //�رն�ʱ��
	    HAL_TIM_Base_Stop_IT(&TIM_TimeBaseStructureAD);
//        for(i=0;i<20;i++)
//		{
//			BEEP_ON;
//			OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_PERIODIC,&err);//��ʱms
//			BEEP_OFF;
//			OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_PERIODIC,&err);//��ʱ1ms
//			
//			
//        }
      
    }
}

/*********************************************END OF FILE**********************/

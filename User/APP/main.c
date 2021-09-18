 /*
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   uCOS-III ϵͳ��ֲ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F767 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/

/*
*********************************************************************************************************
*                                            �������ļ�
*********************************************************************************************************
*/
#include "include.h"
/*
*********************************************************************************************************
*                                         ������ƿ�TCB
*********************************************************************************************************
*/

static  OS_TCB       AppTaskStartTCB;
//static  OS_TCB       AppTaskLightTCB;

/*
*********************************************************************************************************
*                                            �����ջ
*********************************************************************************************************
*/
static  CPU_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];
//static  CPU_STK      AppTaskLightStk[APP_TASK_Light_STK_SIZE];
/*
*********************************************************************************************************
*                                            ����ԭ��
*********************************************************************************************************
*/
static  void  AppTaskStart (void  *p_arg);

OS_SEM  Page_SEM;        //����һ���ź�������������ͬ��

//������ƿ�
extern OS_TCB USBTaskTCB;
//�����ջ
extern CPU_STK USB_TASK_STK[USB_STK_SIZE];
void USB_task(void *p_arg);
extern OS_SEM  USB_SEM;        //����һ���ź�������������ͬ��
/*
*********************************************************************************************************
* ������ : main
* ����   : ��׼��C�������
* �β�   : ��
* ����ֵ : ��
* ע��   : 1) HAL��ʼ��:
*             a) ����FlashԤȡ��ָ������ݸ��ٻ��档
*             b) ����Systick�������жϡ�HAL_InitTick()�����Ѿ���ϵͳ��д��
*                ϵͳ���Լ���Systick��ʼ�������������ڶ���������֮���ʼ����
*********************************************************************************************************
   */
int main(void)
	{
    OS_ERR   err;
//  SCB->VTOR = FLASH_BASE | 0x10000;//����ƫ����
//  __set_PRIMASK(0);
    HAL_Init();                                                             //HAL��ʼ��,��ע�� 1

    BSP_SystemClkCfg();                                                     //��ʼ��CPUƵ��Ϊ 216Mhz

    CPU_Init();                                                             //��ʼ�� CPU �����ʱ��������ж�ʱ���������������

    Mem_Init();                                                           //��ʼ���ڴ������������ڴ�غ��ڴ�ر�

    CPU_IntDis();                                                           //��ֹ�����ж�

    OSInit(&err);                                                           //��ʼ��uC/OS-IIIϵͳ

    App_OS_SetAllHooks();

    /* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) AppTaskStart,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    OSStart(&err);                                                          //�����������������uC/OS-III���ƣ�
}
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

void InitData_type(void)
{
    DataSave.Data_type.PageFlag = FPageCh;//����С��������չʾ
    DataSave.Data_type.LANG = CHI ;  //����ѡ��Ĭ������
    DataSave.Data_type.BeepF = SETON ;  //����ѡ��Ĭ������
    DataSave.Data_type.parameters.SW = OFF;   //wifi Ĭ���ǹص�
    DataSave.Data_type.FormatF= OFF;//��ʽ�����ܲ���
    DataSave.Data_type.ShotF= OFF;//��ͼ���ܲ���
    DataSave.Data_type.SYNF= OFF;//��ͬ�����ܲ���
}

void SelfTest(void)
{
    uint8 i;
	OS_ERR  err;
	LCD_Clear(LCD_COLOR_BLACK);  //����
    for(i=0;i<10;i++)
		DrawIniALL(i,200);
	InitRes[12] = FileRes;
	for(i=10;i<14;i++)
        DrawIniALL(i,200);
	OSTimeDly ( 800, OS_OPT_TIME_DLY, & err );
}
/*
*********************************************************************************************************
* ������ ��AppTaskStart
* ����   : ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(�� BSP_Init ��ʵ��)��
* �β�   : p_arg   ��OSTaskCreate()�ڴ���������ʱ���ݹ������βΡ�
* ����ֵ : ��
* ע��   : 1) ��һ�д��� (void)p_arg; ��Ϊ�˷�ֹ������������Ϊ�β�p_arg��û���õ�
*********************************************************************************************************
*/

void EndSD(void);
static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;

    (void)p_arg;
   
    BSP_Init();                                    //�弶��ʼ��
    InitRes[9] = ReadAllPara();               //���籣������

#if OS_CFG_STAT_TASK_EN > 0u                       //���ʹ�ܣ�Ĭ��ʹ�ܣ���ͳ������
    OSStatTaskCPUUsageInit(&err);                  //����û��Ӧ������ֻ�п�����������ʱ CPU �ģ����
#endif                                             //���������� OS_Stat_IdleCtrMax ��ֵ��Ϊ������� CPU
    //ʹ����ʹ��
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                   //��λ�����㣩��ǰ�����ж�ʱ��
#endif
    InitData_type();
    InitTigT();
    /* ����RTC���� ʱ��ˢ��500ms һ�� */
    OSTaskCreate((OS_TCB     *)&RTCTaskTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"RTC Task",                             //��������
                 (OS_TASK_PTR ) RTC_task,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) RTC_TASK_PRIO,                          //��������ȼ�
                 (CPU_STK    *)&RTC_TASK_STK[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) RTC_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) RTC_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 1u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ����USB���� ʱ��ˢ��ms һ�� */
//    OSTaskCreate((OS_TCB     *)&USBTaskTCB,                             //������ƿ��ַ
//                 (CPU_CHAR   *)"USB Task",                             //��������
//                 (OS_TASK_PTR ) USB_task,                                //������
//                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
//                 (OS_PRIO     ) USB_TASK_PRIO,                          //��������ȼ�
//                 (CPU_STK    *)&USB_TASK_STK[0],                          //�����ջ�Ļ���ַ
//                 (CPU_STK_SIZE) USB_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
//                 (CPU_STK_SIZE) USB_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
//                 (OS_MSG_QTY  ) 1u,                                         //����ɽ��յ������Ϣ��
//                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
//                 (void       *) 0,                                          //������չ��0����չ��
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
//                 (OS_ERR     *)&err);

    /* ����ǰ�����ݲɼ����� ����ǰ�˵Ĵ������� 4��ָ�����ݵĴ��� */
    OSTaskCreate((OS_TCB     *)&DoUartTaskTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"DoUart Task",                             //��������
                 (OS_TASK_PTR ) DoUart_task,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) DoUart_TASK_PRIO,                          //��������ȼ�
                 (CPU_STK    *)&DoUart_TASK_STK[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) DoUart_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) DoUart_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ��������10 msһ�� */
    OSTaskCreate((OS_TCB     *)&AppTaskLightTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Light",                            //��������
                 (OS_TASK_PTR ) AppTaskLight,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_Light_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskLightStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_Light_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_Light_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 1u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
    //����������
    OSTaskCreate((OS_TCB*     )&BeepTaskTCB,
                 (CPU_CHAR*   )"Beep task",
                 (OS_TASK_PTR )Beep_task,
                 (void*       )0,
                 (OS_PRIO     )Beep_TASK_PRIO,
                 (CPU_STK*    )&Beep_TASK_STK[0],
                 (CPU_STK_SIZE)Beep_STK_SIZE/10,
                 (CPU_STK_SIZE)Beep_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void*       )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);

    /* ����FLASH ������˸���� ʱ��ˢ��500ms һ�� */
    OSTaskCreate((OS_TCB     *)&FLASHTaskTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"FLASH Task",                             //��������
                 (OS_TASK_PTR ) FLASH_task,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) FLASH_TASK_PRIO,                          //��������ȼ�
                 (CPU_STK    *)&FLASH_TASK_STK[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) FLASH_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) FLASH_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 1u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    //����һ���ź���
    OSSemCreate ((OS_SEM*   )&Page_SEM,
                 (CPU_CHAR* )"Page_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
	OSSemPend(&Page_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	SelfTest();
    BluidICOTask();

    
    while(1)
    {
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            switch(PageFlagold)
            {
                case FPageCh ://����С��������չʾ
                    OSTaskDel ( &DisplayChTaskTCB, & err );
                    break;
                case FPageHome://�������ý���
                    OSTaskDel ( &DoWorkTaskTCB, & err );
                    break;
                case FPageDisplayData://����չʾ����
                    OSTaskDel ( &DisplayDataTaskTCB, & err );
                    break;
                case FPageHDATA://������������
                    OSTaskDel ( &DoHDataTaskTCB, & err );
                    break;
                case FPageCh3 ://�������������չʾ
                    OSTaskDel ( &DisplayCh3TaskTCB, & err );
                    break;
                case FPageOtur :
                    OSTaskDel ( &DoOturTaskTCB, & err );
                    break;
                case FPageIF :
                    OSTaskDel ( &DoIFTaskTCB, & err );
                    break;
                case FPageFile :
                    OSTaskDel ( &DoFileTaskTCB, & err );
                    break;
                case FPageGraph :
                    OSTaskSuspend((OS_TCB*)&FLASHTaskTCB,&err);   //��������
                    OSTaskDel ( &GraphTaskTCB, & err );
                    break;
                case FPageRecord :
                    OSTaskDel ( &DoFileTaskTCB, & err );
                    break;
            }
			if(DataSave.Data_type.PageFlag==FPageHome)
			{
				if(PageFlagold==FPageCh || PageFlagold==FPageCh3 || FPageDisplayData==PageFlagold
					||PageFlagold==FPageGraph)  //һ���˵�
				{
					PageFlagSetRe=PageFlagold;
				}
			}
            PageFlagold=DataSave.Data_type.PageFlag;
	        
            switch(DataSave.Data_type.PageFlag)
            {
                case FPageCh ://����С��������չʾ
                    /* ����DisplayCh С������������ʵʱ��ʾ */
                    OSTaskCreate((OS_TCB     *)&DisplayChTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DisplayCh Task",                             //��������
                                 (OS_TASK_PTR ) DisplayCh_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DisplayCh_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DisplayCh_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DisplayCh_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DisplayCh_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    break;
                case FPageHome://�������ý���
                    /* ����Homework   �����ò˵��пؼ�ѡ������ */
                    OSTaskCreate((OS_TCB     *)&DoWorkTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DoWork Task",                             //��������
                                 (OS_TASK_PTR ) DoWork_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DoWork_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DoWork_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DoWork_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DoWork_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);                                       //���ش�������
                    break;
                case FPageDisplayData://����չʾ����
                    /* ����DisplayData ��������ʵʱ��ʾ */
                    OSTaskCreate((OS_TCB     *)&DisplayDataTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DisplayData Task",                             //��������
                                 (OS_TASK_PTR ) DisplayData_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DisplayData_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DisplayData_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DisplayData_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DisplayData_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);                                       //���ش�������
                    break;
                case FPageHDATA://������������
                    /* ����data  �����ò˵��пؼ�ѡ������ */
                    OSTaskCreate((OS_TCB     *)&DoHDataTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DoHData Task",                             //��������
                                 (OS_TASK_PTR ) DoHData_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DoHData_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DoHData_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DoHData_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DoHData_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    break;
                case FPageFile://�����ļ�����
                    /* ����d�ļ�  �����ò˵��пؼ�ѡ������ */
                    OSTaskCreate((OS_TCB     *)&DoFileTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DoFile Task",                             //��������
                                 (OS_TASK_PTR ) DoFile_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DoFile_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DoFile_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DoFile_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DoFile_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    break;
                case FPageCh3 ://�������������չʾ
                    /* ����DisplayCh3 ��������������ʵʱ��ʾ */
                    OSTaskCreate((OS_TCB     *)&DisplayCh3TaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DisplayCh3 Task",                             //��������
                                 (OS_TASK_PTR ) DisplayCh3_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DisplayCh3_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DisplayCh3_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DisplayCh3_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DisplayCh3_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    break;
                case FPageOtur :
                    /* ����othr  �����ò˵��пؼ�ѡ������ */
                    OSTaskCreate((OS_TCB     *)&DoOturTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DoOtur Task",                             //��������
                                 (OS_TASK_PTR ) DoOtur_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DoOtur_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DoOtur_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DoOtur_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DoOtur_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    break;
                case FPageIF :
                    /* ����IF  �����ò˵��пؼ�ѡ������ */
                    OSTaskCreate((OS_TCB     *)&DoIFTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"DoIF Task",                             //��������
                                 (OS_TASK_PTR ) DoIF_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) DoIF_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&DoIF_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) DoIF_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) DoIF_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    break;
                case FPageGraph :
                    OSTaskCreate((OS_TCB     *)&GraphTaskTCB,                             //������ƿ��ַ
                                 (CPU_CHAR   *)"Graph Task",                             //��������
                                 (OS_TASK_PTR ) Graph_task,                                //������
                                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                                 (OS_PRIO     ) Graph_TASK_PRIO,                          //��������ȼ�
                                 (CPU_STK    *)&Graph_TASK_STK[0],                          //�����ջ�Ļ���ַ
                                 (CPU_STK_SIZE) Graph_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                                 (CPU_STK_SIZE) Graph_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                                 (OS_MSG_QTY  ) 2u,                                         //����ɽ��յ������Ϣ��
                                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                                 (void       *) 0,                                          //������չ��0����չ��
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                                 (OS_ERR     *)&err);
                    OSTaskResume((OS_TCB*)&FLASHTaskTCB,&err);    //
                    break;
            }
        }
        OSSemPend(&Page_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
    }
}


 /*
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   uCOS-III 系统移植
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F767 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/

/*
*********************************************************************************************************
*                                            包含的文件
*********************************************************************************************************
*/
#include "include.h"
/*
*********************************************************************************************************
*                                         任务控制块TCB
*********************************************************************************************************
*/

static  OS_TCB       AppTaskStartTCB;
//static  OS_TCB       AppTaskLightTCB;

/*
*********************************************************************************************************
*                                            任务堆栈
*********************************************************************************************************
*/
static  CPU_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];
//static  CPU_STK      AppTaskLightStk[APP_TASK_Light_STK_SIZE];
/*
*********************************************************************************************************
*                                            函数原型
*********************************************************************************************************
*/
static  void  AppTaskStart (void  *p_arg);

OS_SEM  Page_SEM;        //定义一个信号量，用于任务同步

//任务控制块
extern OS_TCB USBTaskTCB;
//任务堆栈
extern CPU_STK USB_TASK_STK[USB_STK_SIZE];
void USB_task(void *p_arg);
extern OS_SEM  USB_SEM;        //定义一个信号量，用于任务同步
/*
*********************************************************************************************************
* 函数名 : main
* 描述   : 标准的C函数入口
* 形参   : 无
* 返回值 : 无
* 注意   : 1) HAL初始化:
*             a) 配置Flash预取，指令和数据高速缓存。
*             b) 配置Systick以生成中断。HAL_InitTick()函数已经被系统重写，
*                系统有自己的Systick初始化函数，建议在多任务启动之后初始化。
*********************************************************************************************************
   */
int main(void)
	{
    OS_ERR   err;
//  SCB->VTOR = FLASH_BASE | 0x10000;//设置偏移量
//  __set_PRIMASK(0);
    HAL_Init();                                                             //HAL初始化,见注意 1

    BSP_SystemClkCfg();                                                     //初始化CPU频率为 216Mhz

    CPU_Init();                                                             //初始化 CPU 组件（时间戳、关中断时间测量和主机名）

    Mem_Init();                                                           //初始化内存管理组件（堆内存池和内存池表）

    CPU_IntDis();                                                           //禁止所有中断

    OSInit(&err);                                                           //初始化uC/OS-III系统

    App_OS_SetAllHooks();

    /* 创建起始任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Start",                            //任务名称
                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    OSStart(&err);                                                          //启动多任务管理（交由uC/OS-III控制）
}
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

void InitData_type(void)
{
    DataSave.Data_type.PageFlag = FPageCh;//进入小字体数据展示
    DataSave.Data_type.LANG = CHI ;  //语言选择默认中文
    DataSave.Data_type.BeepF = SETON ;  //语言选择默认中文
    DataSave.Data_type.parameters.SW = OFF;   //wifi 默认是关的
    DataSave.Data_type.FormatF= OFF;//格式化功能不开
    DataSave.Data_type.ShotF= OFF;//截图功能不开
    DataSave.Data_type.SYNF= OFF;//外同步功能不开
}

void SelfTest(void)
{
    uint8 i;
	OS_ERR  err;
	LCD_Clear(LCD_COLOR_BLACK);  //清屏
    for(i=0;i<10;i++)
		DrawIniALL(i,200);
	InitRes[12] = FileRes;
	for(i=10;i<14;i++)
        DrawIniALL(i,200);
	OSTimeDly ( 800, OS_OPT_TIME_DLY, & err );
}
/*
*********************************************************************************************************
* 函数名 ：AppTaskStart
* 描述   : 这是一个启动任务，在多任务系统启动后，必须初始化滴答计数器(在 BSP_Init 中实现)。
* 形参   : p_arg   是OSTaskCreate()在创建该任务时传递过来的形参。
* 返回值 : 无
* 注意   : 1) 第一行代码 (void)p_arg; 是为了防止编译器报错，因为形参p_arg并没有用到
*********************************************************************************************************
*/

void EndSD(void);
static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;

    (void)p_arg;
   
    BSP_Init();                                    //板级初始化
    InitRes[9] = ReadAllPara();               //掉电保持数据

#if OS_CFG_STAT_TASK_EN > 0u                       //如果使能（默认使能）了统计任务
    OSStatTaskCPUUsageInit(&err);                  //计算没有应用任务（只有空闲任务）运行时 CPU 的（最大）
#endif                                             //容量（决定 OS_Stat_IdleCtrMax 的值，为后面计算 CPU
    //使用率使用
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                   //复位（清零）当前最大关中断时间
#endif
    InitData_type();
    InitTigT();
    /* 创建RTC任务 时间刷新500ms 一次 */
    OSTaskCreate((OS_TCB     *)&RTCTaskTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"RTC Task",                             //任务名称
                 (OS_TASK_PTR ) RTC_task,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) RTC_TASK_PRIO,                          //任务的优先级
                 (CPU_STK    *)&RTC_TASK_STK[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) RTC_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) RTC_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);

    /* 创建USB任务 时间刷新ms 一次 */
//    OSTaskCreate((OS_TCB     *)&USBTaskTCB,                             //任务控制块地址
//                 (CPU_CHAR   *)"USB Task",                             //任务名称
//                 (OS_TASK_PTR ) USB_task,                                //任务函数
//                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
//                 (OS_PRIO     ) USB_TASK_PRIO,                          //任务的优先级
//                 (CPU_STK    *)&USB_TASK_STK[0],                          //任务堆栈的基地址
//                 (CPU_STK_SIZE) USB_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
//                 (CPU_STK_SIZE) USB_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
//                 (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
//                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
//                 (void       *) 0,                                          //任务扩展（0表不扩展）
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
//                 (OS_ERR     *)&err);

    /* 创建前端数据采集任务 处理前端的串口数据 4种指令数据的处理 */
    OSTaskCreate((OS_TCB     *)&DoUartTaskTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"DoUart Task",                             //任务名称
                 (OS_TASK_PTR ) DoUart_task,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) DoUart_TASK_PRIO,                          //任务的优先级
                 (CPU_STK    *)&DoUart_TASK_STK[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) DoUart_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) DoUart_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);

    /* 亮度任务10 ms一次 */
    OSTaskCreate((OS_TCB     *)&AppTaskLightTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Light",                            //任务名称
                 (OS_TASK_PTR ) AppTaskLight,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_Light_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskLightStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_Light_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_Light_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
    //蜂鸣器任务
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

    /* 创建FLASH 超出闪烁任务 时间刷新500ms 一次 */
    OSTaskCreate((OS_TCB     *)&FLASHTaskTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"FLASH Task",                             //任务名称
                 (OS_TASK_PTR ) FLASH_task,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) FLASH_TASK_PRIO,                          //任务的优先级
                 (CPU_STK    *)&FLASH_TASK_STK[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) FLASH_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) FLASH_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    //创建一个信号量
    OSSemCreate ((OS_SEM*   )&Page_SEM,
                 (CPU_CHAR* )"Page_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
	OSSemPend(&Page_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	SelfTest();
    BluidICOTask();

    
    while(1)
    {
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            switch(PageFlagold)
            {
                case FPageCh ://进入小字体数据展示
                    OSTaskDel ( &DisplayChTaskTCB, & err );
                    break;
                case FPageHome://进入设置界面
                    OSTaskDel ( &DoWorkTaskTCB, & err );
                    break;
                case FPageDisplayData://数据展示界面
                    OSTaskDel ( &DisplayDataTaskTCB, & err );
                    break;
                case FPageHDATA://进入数据设置
                    OSTaskDel ( &DoHDataTaskTCB, & err );
                    break;
                case FPageCh3 ://进入大字体数据展示
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
                    OSTaskSuspend((OS_TCB*)&FLASHTaskTCB,&err);   //挂起任务
                    OSTaskDel ( &GraphTaskTCB, & err );
                    break;
                case FPageRecord :
                    OSTaskDel ( &DoFileTaskTCB, & err );
                    break;
            }
			if(DataSave.Data_type.PageFlag==FPageHome)
			{
				if(PageFlagold==FPageCh || PageFlagold==FPageCh3 || FPageDisplayData==PageFlagold
					||PageFlagold==FPageGraph)  //一级菜单
				{
					PageFlagSetRe=PageFlagold;
				}
			}
            PageFlagold=DataSave.Data_type.PageFlag;
	        
            switch(DataSave.Data_type.PageFlag)
            {
                case FPageCh ://进入小字体数据展示
                    /* 创建DisplayCh 小字体任务数据实时显示 */
                    OSTaskCreate((OS_TCB     *)&DisplayChTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DisplayCh Task",                             //任务名称
                                 (OS_TASK_PTR ) DisplayCh_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DisplayCh_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DisplayCh_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DisplayCh_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DisplayCh_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    break;
                case FPageHome://进入设置界面
                    /* 创建Homework   主设置菜单中控件选择任务 */
                    OSTaskCreate((OS_TCB     *)&DoWorkTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DoWork Task",                             //任务名称
                                 (OS_TASK_PTR ) DoWork_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DoWork_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DoWork_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DoWork_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DoWork_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);                                       //返回错误类型
                    break;
                case FPageDisplayData://数据展示界面
                    /* 创建DisplayData 任务数据实时显示 */
                    OSTaskCreate((OS_TCB     *)&DisplayDataTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DisplayData Task",                             //任务名称
                                 (OS_TASK_PTR ) DisplayData_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DisplayData_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DisplayData_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DisplayData_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DisplayData_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);                                       //返回错误类型
                    break;
                case FPageHDATA://进入数据设置
                    /* 创建data  主设置菜单中控件选择任务 */
                    OSTaskCreate((OS_TCB     *)&DoHDataTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DoHData Task",                             //任务名称
                                 (OS_TASK_PTR ) DoHData_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DoHData_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DoHData_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DoHData_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DoHData_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    break;
                case FPageFile://进入文件储存
                    /* 创建d文件  主设置菜单中控件选择任务 */
                    OSTaskCreate((OS_TCB     *)&DoFileTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DoFile Task",                             //任务名称
                                 (OS_TASK_PTR ) DoFile_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DoFile_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DoFile_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DoFile_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DoFile_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    break;
                case FPageCh3 ://进入大字体数据展示
                    /* 创建DisplayCh3 大字体任务数据实时显示 */
                    OSTaskCreate((OS_TCB     *)&DisplayCh3TaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DisplayCh3 Task",                             //任务名称
                                 (OS_TASK_PTR ) DisplayCh3_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DisplayCh3_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DisplayCh3_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DisplayCh3_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DisplayCh3_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    break;
                case FPageOtur :
                    /* 创建othr  主设置菜单中控件选择任务 */
                    OSTaskCreate((OS_TCB     *)&DoOturTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DoOtur Task",                             //任务名称
                                 (OS_TASK_PTR ) DoOtur_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DoOtur_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DoOtur_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DoOtur_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DoOtur_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    break;
                case FPageIF :
                    /* 创建IF  主设置菜单中控件选择任务 */
                    OSTaskCreate((OS_TCB     *)&DoIFTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"DoIF Task",                             //任务名称
                                 (OS_TASK_PTR ) DoIF_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) DoIF_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&DoIF_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) DoIF_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) DoIF_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    break;
                case FPageGraph :
                    OSTaskCreate((OS_TCB     *)&GraphTaskTCB,                             //任务控制块地址
                                 (CPU_CHAR   *)"Graph Task",                             //任务名称
                                 (OS_TASK_PTR ) Graph_task,                                //任务函数
                                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                 (OS_PRIO     ) Graph_TASK_PRIO,                          //任务的优先级
                                 (CPU_STK    *)&Graph_TASK_STK[0],                          //任务堆栈的基地址
                                 (CPU_STK_SIZE) Graph_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                 (CPU_STK_SIZE) Graph_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                 (OS_MSG_QTY  ) 2u,                                         //任务可接收的最大消息数
                                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                 (void       *) 0,                                          //任务扩展（0表不扩展）
                                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                 (OS_ERR     *)&err);
                    OSTaskResume((OS_TCB*)&FLASHTaskTCB,&err);    //
                    break;
            }
        }
        OSSemPend(&Page_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
    }
}


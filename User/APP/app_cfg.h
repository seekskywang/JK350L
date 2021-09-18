/*
  ******************************************************************************
  * @file    app_cfg.h
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   uCOS-III应用配置头文件
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F767 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

/*
*********************************************************************************************************
*                                           模块禁能/使能
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            任务优先级
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                2u
#define  APP_TASK_Light_PRIO                   3u

//设置任务优先级
#define KEY0_TASK_PRIO 				              5u

#define Beep_TASK_PRIO 				              8u

//设置任务优先级
#define DoWork_TASK_PRIO 				           11u
#define DisplayData_TASK_PRIO 				 16u
#define DoUart_TASK_PRIO 				          4u
#define Graph_TASK_PRIO 				          10u
#define RTC_TASK_PRIO 				                   15u
#define DisplayCh_TASK_PRIO 				            17u
#define DisplayCh3_TASK_PRIO 				            18u
#define DoOtur_TASK_PRIO 				            12u
#define DoHData_TASK_PRIO 				            13u
#define DoIF_TASK_PRIO 				            14u
#define FLASH_TASK_PRIO 				            19u
#define WIFI_TASK_PRIO 				                   9u
#define WIFICheck_TASK_PRIO 				                   7u
#define USB_TASK_PRIO 				                   6u
#define DoFile_TASK_PRIO 				            13u
#define ICO_TASK_PRIO 				                   20u
//任务堆栈大小

//#define  APP_TASK_LED1_PRIO                 3u
//#define  APP_TASK_LED2_PRIO                 3u
//#define  APP_TASK_LED3_PRIO                 3u
/*
*********************************************************************************************************
*                                            任务堆栈大小
*********************************************************************************************************
*/
#define  APP_TASK_START_STK_SIZE            2560u
#define  APP_TASK_Light_STK_SIZE               64u
#define Beep_STK_SIZE				               128u

#define KEY0_STK_SIZE				               128u
#define DoWork_STK_SIZE				               1024u
#define DisplayData_STK_SIZE				       1152u
#define DoUart_STK_SIZE				               6144u
#define Graph_STK_SIZE				               1664u
#define RTC_STK_SIZE				               128u
#define USB_STK_SIZE				               768u
#define FLASH_STK_SIZE				               128u
#define DisplayCh_STK_SIZE				           1024u
#define DisplayCh3_STK_SIZE				           512u
#define DoOtur_STK_SIZE				              512u
#define DoHData_STK_SIZE				            4096u
#define DoFile_STK_SIZE				              7138u
#define ICO_STK_SIZE				               128u
#define DoIF_STK_SIZE				              1280u
#define WIFICheck_STK_SIZE				              128u
#define WIFI_STK_SIZE				               256u
//#define  APP_TASK_LED1_STK_SIZE             512u
//#define  APP_TASK_LED2_STK_SIZE             512u
//#define  APP_TASK_LED3_STK_SIZE             512u
/*
*********************************************************************************************************
*                                           跟踪/调试配置
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                        0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                       1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                        2u
#endif

#define  APP_TRACE_LEVEL                        TRACE_LEVEL_OFF
#define  APP_TRACE                              printf

#define  APP_TRACE_INFO(x)               ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)

#endif

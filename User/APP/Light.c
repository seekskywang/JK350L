/*
  * @file    Light.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   屏幕亮度设置
 */

#include "include.h"
OS_TCB       AppTaskLightTCB;
CPU_STK      AppTaskLightStk[APP_TASK_Light_STK_SIZE];

//设置亮度 任务10ms刷新一次
void AppTaskLight(void *p_arg)
{
    OS_ERR  err;
    while(1)
    {
         //TrigFile();//启动文件 
          OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );  //延时
    }
}

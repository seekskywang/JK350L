/*
  * @file    Light.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   ��Ļ��������
 */

#include "include.h"
OS_TCB       AppTaskLightTCB;
CPU_STK      AppTaskLightStk[APP_TASK_Light_STK_SIZE];

//�������� ����10msˢ��һ��
void AppTaskLight(void *p_arg)
{
    OS_ERR  err;
    while(1)
    {
         //TrigFile();//�����ļ� 
          OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );  //��ʱ
    }
}

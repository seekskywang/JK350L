

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include  "DataType.h"
//������ƿ�
extern OS_TCB FLASHTaskTCB;
//�����ջ
extern CPU_STK FLASH_TASK_STK[FLASH_STK_SIZE];
void FLASH_task(void *p_arg);
void FLASH_GraphWork(const struct CUR CURV,uint8 Flag);
void Display_Tip(uint16_t x,uint16_t y,const char *ptr,uint8 Flag);

#endif /* __FLASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

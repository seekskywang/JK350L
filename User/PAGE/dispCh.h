
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAYCH_H
#define __DISPLAYCH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include  "DataType.h"


extern OS_TCB DisplayChTaskTCB;
//ÈÎÎñ¶ÑÕ»
extern CPU_STK DisplayCh_TASK_STK[DisplayCh_STK_SIZE];

void Num2String(uint8 i,char *str);
void page_DisplayCh(void);
uint8 page_DisplayCh_cycle(uint8 flag);
void DisplayCh_task(void *p_arg);
void LCD_DisplayNumLine_EN_CH_LJ_CH(struct RDispPara *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType,uint32_t TextC,uint32_t BackC);
#endif /* __DISPLAYCH_H */

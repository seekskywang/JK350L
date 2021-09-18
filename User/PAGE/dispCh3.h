/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Header for main.c module
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAYCH3_H
#define __DISPLAYCH3_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include  "DataType.h"


extern struct CUR CurCh3;
extern OS_TCB DisplayCh3TaskTCB;
//ÈÎÎñ¶ÑÕ»
extern CPU_STK DisplayCh3_TASK_STK[DisplayCh3_STK_SIZE];

uint8 ModifyCh3(uint8 keytmp,struct CUR* cur,struct CUR* curold,char MinCol,char MaxCol);
void page_DisplayCh3(uint8 page);
uint8 page_DisplayCh3_cycle(uint8 page,uint8 flag);
void DisplayCh3_task(void *p_arg);
void LCD_DisplayStringLineLJ_WithC(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t *prt,uint32_t TextC,uint32_t BackC);
#endif /* __DISPLAYCH3_H */

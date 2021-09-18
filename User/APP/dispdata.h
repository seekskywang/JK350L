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
#ifndef __DISPLAYDATA_H
#define __DISPLAYDATA_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include  "DataType.h"

extern uint8 PageFlagold;


extern const   uint32 colors[13];
  //任务控制块
extern OS_TCB DisplayDataTaskTCB;
//任务堆栈
extern CPU_STK DisplayData_TASK_STK[DisplayData_STK_SIZE];
extern uint8 PageFlagSetRe;
extern const struct UintARR ConstUint[VALCIND];
extern const struct UintARR ConstUintF[VALCIND];
extern struct RDispData  DDa[DCOL+2][ DCAM];
extern struct RDispData  DFrq[2];
extern struct RDispData  DHmd[3];
extern struct RDispData  DTemp;
void page_DisplayData_cycle(void);
void page_DisplayData(void);
void DisplayData_task(void *p_arg);
void AddUint2Char(const struct RDispData RD,char* ptr,uint8 setoff) ;
void Stu_DisplayData_Init(void);
void Stu_DisplayDataPCha_Init(void);
void page_FlashP(void);
void AddUint2CharUnit(const struct RDispData RD,char* ptr,uint8 setoff);
#endif /* __DISPLAYDATA_H */

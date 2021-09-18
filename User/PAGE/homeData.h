
#ifndef  HOMEDATA_H_
#define  HOMEDATA_H_
#include  "DataType.h"

/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

void page_HData_cycle(void);
void page_HData(void);

extern const struct HDataStr HDataCh[HDataNUM];
extern const struct HDataStr HDataEn[HDataNUM];

void Stu_HData_Init(void);
void Stu_HData_cycle(struct CUR *sle,struct CUR *sleold);

void FileInForDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void LCD_DisplayStringLineLJ_HData(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType,uint32_t SetTextColor);
void DoHData_task(void *p_arg);
void Stu_MinHData_Init(void);
void page_HData_cycle(void);
void InitTigT(void);
void ModifyHS(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol);
void CompSample(void);
extern RTC_DateTypeDef DateStart;  //   4个字节日期时间  
extern RTC_TimeTypeDef TimeStart;   //20个字节日期时间 
extern RTC_DateTypeDef DateEnd;  //   4个字节日期时间  
extern RTC_TimeTypeDef TimeEnd;   //20个字节日期时间
extern uint8 trigTflag;

extern uint8   SaveCnt;
//游标  分行和列
extern struct CUR CurHData;
extern struct CUR CurHDataOld;


//任务控制块
extern OS_TCB DoHDataTaskTCB;
//任务堆栈
extern CPU_STK DoHData_TASK_STK[DoHData_STK_SIZE];


//其它设置的下拉菜单
extern struct RDispPara DisHData[HDataCOL];
#endif                                                          /* End of module include.                               */


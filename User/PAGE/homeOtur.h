/*
  ******************************************************************************
  * @file    bsp.h
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ϵͳ��ʼ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F767 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/

#ifndef  OTUR_H_
#define  OTUR_H_
#include  "DataType.h"
/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/
void page_Otur_cycle(void);
void page_Otur(void);
void ComBoxS(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol);
void Stu_Otur_Init(void);
void Stu_Otur_cycle(struct CUR *sle,struct CUR *sleold);
uint8 ModifyOtur(uint8 keytmp,struct CUR* cur,struct CUR* curold);             // ��ֵƥ�亯������while��ѭ���з���ƥ��
void LCD_DisplayStringLineLJ_Otur(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType,uint32_t SetTextColor);
void DoOtur_task(void *p_arg);
void Stu_MinOtur_Init(void);
void Stu_HomeManu_Init(uint8 a);
void Disp_Head(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint8_t *ptr,uint8_t FontType,sFONT fonts);
void ModifySaver(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol);
void RTC_TimeAndDate_Set_Otur(RTC_DateTypeDef  Date,RTC_TimeTypeDef  Time);
u32 SetSaveStamp(void);
void ReturnST_Init(void);
void F2UintChar(char* ptr,uint8 setoff);

//�α�  ���к���
extern struct CUR CurOtur;
extern struct CUR CurOturOld;
extern u32 SaveStamp;
extern u32 SaveFlag; 
//������ƿ�
extern OS_TCB DoOturTaskTCB;
//�����ջ
extern CPU_STK DoOtur_TASK_STK[DoOtur_STK_SIZE];

//�������õ������˵�
extern struct RDispPara DisOtur[OturCOL];
extern const struct RStr OturCh[OturNUM ];
extern const struct RStr OturEn[OturNUM ];
#endif                                                          /* End of module include.                               */


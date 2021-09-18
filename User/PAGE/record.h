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

#ifndef  RECORD_H_
#define  RECORD_H_
//#include  "DateType.h"

/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/
//#define  SampleMax 18
//#define  PointsNum  4000    //�ܵ���
//#define  PointsPix  400      //��Ч����
//#define  GIXStartY   382
//#define  GIXStartX   53
//#define  SampleMaxCam  18  //SampleMaxCam 18 ��ѡ��
//#define  ZoneMaxCam  4  // 
#define  DCOL 11    //��
#define  DCAMR 1    
void page_Record(const struct CUR CURV);
uint8 ModifyRecord(uint8 keytmp,struct CUR* cur,struct CUR* curold,struct CUR* curchan,char MinCol,char MaxCol) ;
void page_Record_cycle(struct CUR *pCUR);
int CompFloat(float a,float b) ;
int CompNumStru(struct NumStru *pNum) ;
void Record_task(void *p_arg);
void page_Record_init(const struct CUR CURV);
//����������
void lcd_Grids(u16 x1,u16 y1,u16 HSpace,u16 VSpace,u8 NumOfRows,u8 NumOfColumns,u32 BorderColor,u8 span);
void lcd_line(u16 x1, u16 y1, u16 x2, u16 y2,u8 Span,u32 BorderColor); //��ʵ�߻�����
uint8 DrawRecord(const struct CUR CURV,uint16_t PointCount,uint16_t MaxCount);
void LCD_FillRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor);
void LCD_DrawRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor);
void LCD_DisplayStringLine_EN_CH_LJ_WithFont(uint16_t x,uint16_t y, uint8_t *ptr,sFONT *fonts);
void LCD_DrawRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor);
void LCD_DrawPolygonLJ(pPoint Points, uint16_t PointCount,uint32 SetColor);
void LCD_DrawHLineLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Length,uint32 SetColor);
void DrawRecord_Init(void);
void page_CH(const struct CUR CURV);
void ShowDateTime(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos,sFONT *fonts);
void ShowTimeLine(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos, uint16 xW,sFONT *fonts,uint32 devtime);
//�ֲ���� ֻ������߲���
void DrawRecord_Flash(const struct CUR CURV,uint16_t PointCount);
void Record_Work(void);
void LCD_DisplayStringLine_EN_CH_LJ_WithFC(uint16_t x,uint16_t y, uint8_t *ptr,sFONT *fonts,uint32 SetColor);
void LCD_DrawPolygonLJ2(pPoint Points, uint16_t PointCount,uint32 SetColor);
void page_Protect(void);
void DrawRecord_FlashZone(void);
void LCD_DisplayStringLine_EN_CH_LJ_Time(uint16_t x,uint16_t y, uint8_t *ptr);
void Record_Page(void);
void Stu_Manu_Init(uint8 a);
uint8 RereadSD(uint32 RCNT);
extern struct CUR CurRecord;//�Ҳ�����ѡ����α�
extern struct CUR CurRecordOld;//�Ҳ�����ѡ����α��ϴε�λ��
extern struct CUR CurChan;
extern struct CUR SampleTimeCur; //SampleTime����ѡ����α�
extern struct CUR ZoneCur;//����ѡ����α�

extern const char  SampleTime[SampleMaxCam][7];
extern const uint32 SampletimeMs[SampleMaxCam];
extern const uint32 ZoneData[ZoneMaxCam];
extern uint16 Linenum;
extern RTC_TimeTypeDef RTC_TimeStrStart;
extern RTC_DateTypeDef RTC_DateStrStart;
#endif                                                          /* End of module include.                               */


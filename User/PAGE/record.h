/*
  ******************************************************************************
  * @file    bsp.h
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   系统初始化相关
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F767 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
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
//#define  PointsNum  4000    //总点数
//#define  PointsPix  400      //有效点数
//#define  GIXStartY   382
//#define  GIXStartX   53
//#define  SampleMaxCam  18  //SampleMaxCam 18 个选项
//#define  ZoneMaxCam  4  // 
#define  DCOL 11    //行
#define  DCAMR 1    
void page_Record(const struct CUR CURV);
uint8 ModifyRecord(uint8 keytmp,struct CUR* cur,struct CUR* curold,struct CUR* curchan,char MinCol,char MaxCol) ;
void page_Record_cycle(struct CUR *pCUR);
int CompFloat(float a,float b) ;
int CompNumStru(struct NumStru *pNum) ;
void Record_task(void *p_arg);
void page_Record_init(const struct CUR CURV);
//绘制网格线
void lcd_Grids(u16 x1,u16 y1,u16 HSpace,u16 VSpace,u8 NumOfRows,u8 NumOfColumns,u32 BorderColor,u8 span);
void lcd_line(u16 x1, u16 y1, u16 x2, u16 y2,u8 Span,u32 BorderColor); //画实线或虚线
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
//局部清除 只清除曲线部分
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
extern struct CUR CurRecord;//右侧框框体选择的游标
extern struct CUR CurRecordOld;//右侧框框体选择的游标上次的位置
extern struct CUR CurChan;
extern struct CUR SampleTimeCur; //SampleTime横向选择的游标
extern struct CUR ZoneCur;//横向选择的游标

extern const char  SampleTime[SampleMaxCam][7];
extern const uint32 SampletimeMs[SampleMaxCam];
extern const uint32 ZoneData[ZoneMaxCam];
extern uint16 Linenum;
extern RTC_TimeTypeDef RTC_TimeStrStart;
extern RTC_DateTypeDef RTC_DateStrStart;
#endif                                                          /* End of module include.                               */


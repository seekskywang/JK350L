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

#ifndef  GRAPH_H_
#define  GRAPH_H_
//#include  "DateType.h"

/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/
#define  SampleMax 18
#define  PointsNum  440    //总点数
#define  PointsPix  400      //x轴有效点数
#define  PointsPiy  400      //y轴有效点数
#define  PointsPiy1  402      //y轴有效点数
#define  PointsPiy2  (PointsPiy+4)      //y轴有效点数
#define  StrPiX  	320      //字符显示位置
#define  GIXStartY   422
#define  GIXEndY   (GIXStartY-PointsPiy)
#define  GIXStartX   53
#define  GIXEndX   (GIXStartX+PointsPix)
#define  SampleMaxCam  18  //SampleMaxCam 18 个选项
#define  ZoneMaxCam  4  // 

void page_graph(const struct CUR CURV);
uint8 ModifyGraph(uint8 keytmp,struct CUR* cur,struct CUR* curold,struct CUR* curchan,char MinCol,char MaxCol) ;
void page_graph_cycle(struct CUR *pCUR);
int CompFloat(float a,float b) ;
int CompNumStru(struct NumStru *pNum) ;
void Graph_task(void *p_arg);
void page_graph_init(const struct CUR CURV);
//绘制网格线
void lcd_Grids(u16 x1,u16 y1,u16 HSpace,u16 VSpace,u8 NumOfRows,u8 NumOfColumns,u32 BorderColor,u8 span);
void lcd_line(u16 x1, u16 y1, u16 x2, u16 y2,u8 Span,u32 BorderColor); //画实线或虚线
uint8 DrawGraph(const struct CUR CURV,uint16_t PointCount);
void LCD_FillRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor);
void LCD_DrawRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor);
void LCD_DisplayStringLine_EN_CH_LJ_WithFont(uint16_t x,uint16_t y, uint8_t *ptr,sFONT *fonts);
void LCD_DrawRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor);
void LCD_DrawPolygonLJ(pPoint Points, uint16_t PointCount,uint32 SetColor);
void LCD_DrawHLineLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Length,uint32 SetColor);
void DrawGraph_Init(void);
void page_CH(const struct CUR CURV);
void ShowDateTime(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos,sFONT *fonts);
void ShowTimeLine(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos, uint16 xW,sFONT *fonts,uint32 devtime);
//局部清除 只清除曲线部分
void DrawGraph_Flash(const struct CUR CURV,uint16_t PointCount);
void Graph_Work(void);
void LCD_DisplayStringLine_EN_CH_LJ_WithFC(uint16_t x,uint16_t y, uint8_t *ptr,sFONT *fonts,uint32 SetColor);
void LCD_DrawPolygonLJ2(pPoint Points, uint16_t PointCount,uint32 SetColor);
void page_Protect(void);
void DrawGraph_FlashZone(const struct CUR CURV,uint16_t PointCount);
void LCD_DisplayStringLine_EN_CH_LJ_Time(uint16_t x,uint16_t y, uint8_t *ptr);
int CompFloat(float a,float b);
int CompNumStru(struct NumStru *pNum);
void Stu_Manu_Init(uint8 a);
void ShowTimeLineFM(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos, uint16 xW,sFONT *fonts);
void page_string(struct RDispData  *p,uint16 xR,uint16 yR,sFONT FontType);

extern struct CUR CurGraph;//右侧框框体选择的游标
extern struct CUR CurGraphOld;//右侧框框体选择的游标上次的位置
extern struct CUR CurChan;
extern struct CUR SampleTimeCur; //SampleTime横向选择的游标
extern struct CUR ZoneCur;//横向选择的游标
extern OS_TCB GraphTaskTCB;
//任务堆栈
extern CPU_STK Graph_TASK_STK[Graph_STK_SIZE];
extern const char  SampleTime[SampleMaxCam][7];
extern const uint32 SampletimeMs[SampleMaxCam];
extern const uint32 ZoneData[ZoneMaxCam];
extern uint16 Linenum;
extern uint16 LinenumFM;
extern RTC_TimeTypeDef RTC_TimeStrStart;
extern RTC_DateTypeDef RTC_DateStrStart;
extern Point Lines[DCOL-1][PointsNum];
extern Point Lines2[2][PointsNum];//通道11-12曲线的矩阵
extern const char Sampletime[SampleMaxCam][7];
extern const uint32 SampletimeMs[SampleMaxCam];
extern const char  ZoneC[ZoneMaxCam][8];
extern const uint32 ZoneData[ZoneMaxCam];
extern const uint32 ZoneData[ZoneMaxCam];
#endif                                                          /* End of module include.                               */


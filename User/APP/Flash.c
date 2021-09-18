/*
  * @file    Flash.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   闪烁功能闪烁任务不是存储功能
 */


#include "include.h"
//任务控制块 屏幕刷新任务 不是存储
OS_TCB FLASHTaskTCB;
//任务堆栈
CPU_STK FLASH_TASK_STK[FLASH_STK_SIZE];

const  char AlarmCh[4] = "\0HLE";
//帮助菜单显示
static const struct TipStr TipS[1]=
{
    {"数据采集\0","Running\0"},
};


void Display_Tip(uint16_t x,uint16_t y,const char *ptr,uint8 Flag)
{

    uint32 BackColor,TextColor;
    if(Flag==1)
    {
		TextColor=LCD_GetTextColor();
        BackColor=LCD_GetBackColor();
		LCD_SetColors(LCD_COLOR_VERYDARKGREEN, LCD_COLOR_BLACK);
        if(CHI == DataSave.Data_type.LANG )
        {
            LCD_DisplayStringLineLJ(x, y, (uint8_t *)ptr,FONTTYPE16);
        }
        else
        {
            LCD_SetFont(&Font16);
            LCD_DisplayStringLine_EN_CH_LJ(x, y+2, (uint8_t *)ptr);
        }
        LCD_SetColors(TextColor, BackColor);
    }
    else
    {
        LCD_FillRectLJ(x-2, 0, 9*8+6,20,LCD_COLOR_BLACK); //清除下边框
    }
}



//设置RTC 任务500ms刷新一次
void FLASH_task(void *p_arg)
{
    OS_ERR  err;

    while(1)
    {
        if(DataSave.Data_type.PageFlag==FPageGraph)
            FLASH_GraphWork(CurGraph,0);

        OSTimeDly ( 500, OS_OPT_TIME_DLY, & err );  //延时90ms

        if(DataSave.Data_type.PageFlag==FPageGraph)
            FLASH_GraphWork(CurGraph,1);

        OSTimeDly ( 500, OS_OPT_TIME_DLY, & err );  //延时90ms



    }
}






//Flag 1 时显示0 时灭掉
void FLASH_GraphWork(const struct CUR CURV,uint8 Flag)
{
    OS_ERR  err;

    uint16 xR=10;
    uint16 yR=39;
    uint16 HdivR=90;
    uint16 HdivR_Sel=90;

    uint16 i;
    char str[10];
    uint32 BackColor,TextColor;

    xR=460;
    yR=4+20+24;
    HdivR=26;
    HdivR_Sel=HdivR*2;
    OSSchedLock(&err);
    Display_Tip(52,0,TipS[0].Ch[DataSave.Data_type.LANG],Flag);
    OSSchedUnlock(&err);
    for(i=1; i<13; i++)
    {
        if(ChanData.chalarm[i-1]==0)
        {
            if( CURV.COL==i)
            {
                yR+=HdivR_Sel;
            }
            else
            {
                yR+=HdivR;
            }
            continue;
        }
        //显示右边框体
        if(i>0&&i<10&&CURV.COL!=i)
        {
            sprintf(str,"%d%c ",i,AlarmCh[ChanData.chalarm[i-1]]);

            BackColor=LCD_GetBackColor();
            if(Flag==1)
            {
                OSSchedLock(&err);
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
                OSSchedUnlock(&err);
            }
            else
            {
                TextColor=LCD_GetTextColor();
                OSSchedLock(&err);
                LCD_SetColors(LCD_COLOR_BLACK, LCD_COLOR_BLACK);

                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
                LCD_SetColors(TextColor, BackColor);
                OSSchedUnlock(&err);
            }
        }
        else if(i==10&&CURV.COL!=i)
        {
            sprintf(str,"%d%c",i,AlarmCh[ChanData.chalarm[i-1]]);
            BackColor=LCD_GetBackColor();
            if(Flag==1)
            {
                OSSchedLock(&err);
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
                OSSchedUnlock(&err);
            }
            else
            {
                TextColor=LCD_GetTextColor();
                OSSchedLock(&err);
                LCD_SetColors(LCD_COLOR_BLACK, LCD_COLOR_BLACK);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
                LCD_SetColors(TextColor, BackColor);
                OSSchedUnlock(&err);
            }
        }
        else  if(i>0&&i<10&&CURV.COL==i)
        {
            TextColor=LCD_GetTextColor();
            OSSchedLock(&err);
            LCD_SetTextColor(LCD_COLOR_BLACK);
            LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //标题框填色
            if(Flag==1)
            {
                LCD_SetTextColor( TextColor);
                sprintf(str,"%d%c %6s",i,AlarmCh[ChanData.chalarm[i-1]],DataSave.Data_type.ChName[i-1]);
                BackColor=LCD_GetBackColor();
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
            }
            else
            {
                sprintf(str,"%d%c %6s",i,AlarmCh[ChanData.chalarm[i-1]],DataSave.Data_type.ChName[i-1]);
                BackColor=LCD_GetBackColor();
                LCD_SetColors(LCD_COLOR_BLACK, LCD_COLOR_BLACK);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
                LCD_SetColors(TextColor, BackColor);
            }
            OSSchedUnlock(&err);
        }
        else if(i==10&&CURV.COL==i)
        {
            TextColor=LCD_GetTextColor();
            OSSchedLock(&err);
            LCD_SetTextColor(LCD_COLOR_BLACK);
            LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //标题框填色
            if(Flag==1)
            {
                LCD_SetTextColor( TextColor);

                sprintf(str,"%d%c %6s",i,AlarmCh[ChanData.chalarm[i-1]],DataSave.Data_type.ChName[i-1]);
                BackColor=LCD_GetBackColor();
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
            }
            else
            {

                sprintf(str,"%d%c %6s",i,AlarmCh[ChanData.chalarm[i-1]],DataSave.Data_type.ChName[i-1]);
                BackColor=LCD_GetBackColor();
                LCD_SetColors(LCD_COLOR_BLACK, LCD_COLOR_BLACK);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
                LCD_SetColors(TextColor, BackColor);
            }
            OSSchedUnlock(&err);
        }
        if( CURV.COL==i)
        {
            yR+=HdivR_Sel;
        }
        else
        {
            yR+=HdivR;
        }
    }
}







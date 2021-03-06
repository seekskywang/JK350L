/*
  * @file    Power_on.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   ????????????
 */

#include "include.h"
uint8 InitRes[14]= {0};
void DrawLogo(u16 x,u16 y);
void Delay(__IO uint32_t nCount);
void LCD_DrawLineLJ(uint16_t x1, uint16_t y1, uint16_t Length,uint8_t flag)
{
    if(flag==LCD_DIR_HORIZONTAL)
    {
        LCD_DrawHLine(x1,y1, Length);
    }
    else if(flag==LCD_DIR_VERTICAL)
    {
        LCD_DrawVLine(x1,y1, Length);
    }
}
void power_on(void)
{

    LCD_Clear(LCD_COLOR_BLACK);


    DrawLogo(140,250);


    LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BLACK);
    LCD_DisplayStringLineLJ(0,0, "JKOS",FONTTYPE24);

    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_SetFont(&Font24);
    LCD_DisplayStringLine_EN_CH_LJ(0,40, "REV 1.0");
    LCD_DisplayStringLine_EN_CH_LJ(0,80,"JK350L Multi-channel Meter");

//    Delay(0xffffff);
//    Delay(0xffffff);



}


void DrawIniALL(uint8 i,uint32 time)
{
    OS_ERR  err;
	if(InitRes[i]==0)
    {

        OSTimeDly ( time, OS_OPT_TIME_DLY, & err );
    }
    else
    {

		OSTimeDly ( time*3, OS_OPT_TIME_DLY, & err );
    }
	OSSchedLock(&err);
    LCD_SetFont(&Font20);
    if(InitRes[i]==0)
    {
        LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);

    }
    else
    {
        LCD_SetColors(LCD_COLOR_DARKRED,LCD_COLOR_BLACK);

    }
    switch(i)
    {
        case 0 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing RTC------------ OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing RTC------------ ERROR!");
            break;
        case 1 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing LCD------------ OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing LCD------------ ERROR!");
            break;
        case 2 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Reset Beep------------------ OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Reset Beep------------------ ERROR!");
            break;
        case 3 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Reset ESP8266--------------- OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Reset ESP8266--------------- ERROR!");
            break;
        case 4 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing USB HID-------- OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing USB HID-------- ERROR!");
            break;
        case 5 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing FILESYS-------- OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing FILESYS-------- ERROR!");
            break;
        case 6 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing ChanS Com------ OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing ChanS Com------ ERROR!");
            break;
        case 7 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing BAT Com-------- OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing BAT Com-------- ERROR!");
            break;
        case 8 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing USB CH376------ OK.");
            else
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing USB CH376------ ERROR!");
            break;
        case 9 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Read Datas------------------ OK.");
            else
            {
                LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Factory datas  recovery----- OK.");
            }
        case 10 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing ChanS Mode----- OK.");
            else
            {
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing ChanS Mode----- ERROR!");
            }
            break;
        case 11 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing ChanS Time----- OK.");
            else
            {
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing ChanS Time----- ERROR!");
            }
            break;
        case 12 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing TF card-------- OK.");
            else
            {
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing TF card-------- ERROR!");
            }
            break;
        case 13 :
            if(InitRes[i]==0)
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing Battery-------- OK.");
            else
            {
                LCD_DisplayStringLine_EN_CH_LJ(5,5+i*24,"Initializing Battery-------- ERROR!");
            }
            break;
    }
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	OSSchedUnlock(&err);
}
void DrawLogo(u16 x,u16 y)
{

    LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BLACK);
    /*J*/
    LCD_DrawLineLJ(x,y,41,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x-1,y-1,44,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x-1,y-2,46,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x,y-3,46,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+2,y-4,46,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+35,y-5,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+35,y-6,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+35,y-7,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+36,y-8,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+36,y-9,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+37,y-10,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+37,y-11,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+37,y-12,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+38,y-13,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+37,y-14,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+38,y-15,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+38,y-16,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+38,y-17,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+39,y-18,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+39,y-19,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+39,y-20,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+40,y-21,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+40,y-22,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+40,y-23,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+40,y-24,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+41,y-25,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+41,y-26,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+41,y-27,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+42,y-28,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+42,y-29,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+42,y-30,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+42,y-31,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+42,y-32,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+43,y-33,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+43,y-34,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+44,y-35,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+44,y-36,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+44,y-37,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+44,y-38,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+44,y-39,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+45,y-40,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+45,y-41,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+45,y-42,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+45,y-43,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+46,y-44,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+46,y-45,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+46,y-46,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+47,y-47,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+47,y-48,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+47,y-49,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+47,y-50,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+48,y-51,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+48,y-52,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+48,y-53,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+49,y-54,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+49,y-55,14,LCD_DIR_HORIZONTAL);

    /*i*/
    LCD_FillRect(x+72,y-54,12,3);
    LCD_DrawLineLJ(x+71,y-52,13,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+71,y-51,12,4);
    LCD_FillRect(x+70,y-47,12,3);

    LCD_DrawLineLJ(x+68,y-44+4,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+69,y-43+4,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+68,y-42+4,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+68,y-41+4,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+68-1,y-40+4,13,3);
    LCD_DrawLineLJ(x+68-1,y-37+4,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+67-1,y-36+4,13,3);
    LCD_DrawLineLJ(x+67-1,y-33+4,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+66-1,y-32+4,13,2);
    LCD_DrawLineLJ(x+66-1,y-30+4,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+65-1,y-29+4,13,3);
    LCD_DrawLineLJ(x+64-1,y-26+4,14,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+64-1,y-25+4,13,2);
    LCD_FillRect(x+64-1,y-23+4,12,2);
    LCD_DrawLineLJ(x+63-1,y-21+4,13,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+63-1,y-20+4,12,2);
    LCD_FillRect(x+62-1,y-18+4,13,3);
    LCD_DrawLineLJ(x+62-1,y-15+4,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+61-1,y-14+4,13,3);
    LCD_FillRect(x+60-1,y-11+4,13,2);
    LCD_FillRect(x+59-1,y-5,13,2);
    LCD_DrawLineLJ(x+59-1,y-3,14,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+59-1,y-2,13,2);
    LCD_DrawLineLJ(x+59-1,y,12,LCD_DIR_HORIZONTAL);

    /*n*/
    LCD_DrawLineLJ(x+88,y-40,23,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+85,y-39,27,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+84,y-38,30,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+83,y-37,32,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+83,y-36,3,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+104,y-36,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+83,y-35,1,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+104,y-35,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+104,y-34,12,3);
    LCD_FillRect(x+103,y-31,13,3);
    LCD_DrawLineLJ(x+102,y-28,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+103,y-27,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+102,y-26,13,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+101,y-25,13,2);
    LCD_FillRect(x+101,y-23,12,2);
    LCD_FillRect(x+100,y-21,13,3);
    LCD_FillRect(x+99,y-18,13,3);
    LCD_DrawLineLJ(x+99,y-15,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+98,y-14,13,3);
    LCD_DrawLineLJ(x+98,y-11,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+97,y-10,13,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+97,y-9,12,4);
    LCD_FillRect(x+96,y-5,12,3);
    LCD_FillRect(x+95,y-2,13,3);

    /*k*/
    LCD_DrawLineLJ(x+127,y-56,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+126,y-55,13,3);
    LCD_FillRect(x+126,y-52,12,2);
    LCD_FillRect(x+125,y-50,12,2);
    LCD_DrawLineLJ(x+124,y-48,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+124,y-47,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+124,y-46,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+124,y-45,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+123,y-44,13,2);
    LCD_FillRect(x+123,y-42,12,3);
    LCD_DrawLineLJ(x+153,y-41,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+151,y-40,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+122,y-39,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+150,y-39,12,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+122,y-38,12,2);
    LCD_DrawLineLJ(x+148,y-38,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+147,y-37,12,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+121,y-36,13,3);
    LCD_DrawLineLJ(x+146,y-36,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+145,y-35,11,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+143,y-34,12,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+120,y-33,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+141,y-33,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+121,y-32,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+140,y-32,12,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+120,y-31,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-31,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+120,y-30,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+137,y-30,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+119,y-29,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+135,y-29,15,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+119,y-28,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+134,y-28,16,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+119,y-27,31,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+118,y-26,31,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+118,y-25,32,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+118,y-24,32,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+117,y-23,33,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+118,y-22,19,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-22,12,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+117,y-21,19,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-21,12,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+117,y-20,18,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-20,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+116,y-19,17,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-19,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+116,y-18,16,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-18,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+116,y-17,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-17,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+116,y-16,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+138,y-16,14,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+115,y-15,13,3);
    LCD_DrawLineLJ(x+115,y-12,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+114,y-11,13,3);
    LCD_DrawLineLJ(x+114,y-8,12,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+113,y-7,13,3);
    LCD_DrawLineLJ(x+112,y-4,14,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+112,y-3,13,3);
    LCD_DrawLineLJ(x+112,y,12,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+139,y-15,13,3);
    LCD_FillRect(x+139,y-12,14,2);
    LCD_FillRect(x+140,y-10,13,3);
    LCD_FillRect(x+140,y-7,14,3);
    LCD_FillRect(x+140,y-4,15,2);
    LCD_DrawLineLJ(x+141,y-2,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+143,y-1,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+145,y,10,LCD_DIR_HORIZONTAL);

    /*o*/
    LCD_DrawLineLJ(x+173,y-41,35,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+171,y-40,39,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+169,y-39,41,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+168,y-38,43,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+167,y-37,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+198,y-37,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+166,y-36,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+199,y-36,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+166,y-35,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+198,y-35,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+165,y-34,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+198,y-34,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+165,y-33,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+198,y-33,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+165,y-32,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+198,y-32,13,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+164,y-31,14,2);
    LCD_FillRect(x+197,y-31,14,2);
    LCD_DrawLineLJ(x+164,y-29,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+197,y-29,13,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+163,y-28,14,2);
    LCD_FillRect(x+196,y-28,14,2);
    LCD_DrawLineLJ(x+163,y-26,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+196,y-26,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+162,y-25,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+196,y-25,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+162,y-25,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+195,y-25,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+162,y-24,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+195,y-24,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+162,y-23,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+195,y-23,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+161,y-22,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+194,y-22,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+161,y-21,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+194,y-21,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+161,y-20,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+194,y-20,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+160,y-19,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+194,y-19,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+160,y-18,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+193,y-18,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+160,y-17,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+193,y-17,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+159,y-16,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+193,y-16,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+160,y-15,13,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+193,y-15,13,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+159,y-14,13,4);
    LCD_DrawLineLJ(x+192,y-14,14,LCD_DIR_HORIZONTAL);
    LCD_FillRect(x+192,y-13,13,3);

    LCD_FillRect(x+159,y-10,12,3);
    LCD_FillRect(x+191,y-10,13,2);
    LCD_DrawLineLJ(x+190,y-8,13,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+159,y-7,14,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+188,y-7,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+159,y-6,43,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+160,y-5,41,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+161,y-4,39,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+162,y-3,36,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+163,y-2,34,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+164,y-1,32,LCD_DIR_HORIZONTAL);

    /*??*/
    LCD_DrawLineLJ(x+224,y-36,18,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+223,y-35,22,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+222,y-34,24,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+221,y-33,25,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+221,y-32,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+238,y-32,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+220,y-31,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+238,y-31,9,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+219,y-30,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+238,y-30,9,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+218,y-29,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+239,y-29,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+218,y-28,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+239,y-28,9,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+217,y-27,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+226,y-27,22,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+215,y-26,33,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+214,y-25,35,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+213,y-24,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+227,y-24,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+232,y-24,9,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+213,y-23,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+227,y-23,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+232,y-23,9,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+227,y-22,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+226,y-21,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+226,y-20,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+213,y-19,34,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+212,y-18,35,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+212,y-17,35,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+225,y-16,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+213,y-15,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+225,y-15,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+238,y-15,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-14,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+225,y-14,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+237,y-14,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-13,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+224,y-13,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+237,y-13,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-12,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+224,y-12,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+236,y-12,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-11,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+223,y-11,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+235,y-11,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-10,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+223,y-10,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+235,y-10,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-9,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+223,y-9,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+235,y-9,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-8,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+222,y-8,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+234,y-8,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+212,y-7,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+222,y-7,19,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+214,y-6,16,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+231,y-6,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+217,y-5,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+222,y-5,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+232,y-5,4,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+222,y-4,8,LCD_DIR_HORIZONTAL);

    LCD_FillRect(x+206,y-3,39,2);

    /*??*/
    LCD_DrawLineLJ(x+269,y-38,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+285,y-38,6,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+267,y-37,9,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+285,y-37,6,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+265,y-36,11,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+284,y-36,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+256,y-35,18,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+275,y-35,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+284,y-35,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+256,y-34,17,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+275,y-34,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+283,y-34,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+256,y-33,15,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-33,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+283,y-33,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+261,y-32,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-32,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+283,y-32,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+261,y-31,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-31,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+282,y-31,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+261,y-30,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-30,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+282,y-30,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+260,y-29,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-29,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+282,y-29,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+253,y-28,20,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+275,y-28,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+282,y-28,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+253,y-27,19,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-27,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+282,y-27,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+259,y-26,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-26,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+282,y-26,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+258,y-25,8,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-25,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+259,y-24,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+277,y-24,11,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+253,y-23,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+258,y-23,10,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+271,y-23,5,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+281,y-23,6,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+253,y-22,16,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+270,y-22,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+280,y-22,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+253,y-21,16,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+270,y-21,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+280,y-21,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+252,y-20,24,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+280,y-20,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+252,y-19,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+257,y-19,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+270,y-19,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+280,y-19,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+252,y-18,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+257,y-18,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+270,y-18,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+279,y-18,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+251,y-17,18,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+270,y-17,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+279,y-17,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+251,y-16,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+264,y-16,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+269,y-16,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+279,y-16,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+251,y-15,17,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+269,y-15,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+278,y-15,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+251,y-14,11,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+263,y-14,5,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+269,y-14,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+278,y-14,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+250,y-13,17,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+270,y-13,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+278,y-13,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+250,y-12,17,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+271,y-12,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+278,y-12,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+250,y-11,11,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+262,y-11,5,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+273,y-11,12,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+249,y-10,18,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+277,y-10,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+249,y-9,5,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+255,y-9,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+273,y-9,14,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+248,y-8,39,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+248,y-7,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+260,y-7,22,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+248,y-6,12,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+260,y-6,5,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+275,y-6,7,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+248,y-5,18,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-5,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+248,y-4,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+253,y-4,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+260,y-4,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-4,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+247,y-3,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+253,y-3,6,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+260,y-3,5,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+274,y-3,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+247,y-2,4,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+253,y-2,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+262,y-2,2,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+273,y-2,9,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+247,y-1,1,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+252,y-1,7,LCD_DIR_HORIZONTAL);
    LCD_DrawLineLJ(x+273,y-1,8,LCD_DIR_HORIZONTAL);

    LCD_DrawLineLJ(x+273,y,2,LCD_DIR_HORIZONTAL);
}

/*
* @file    Common.c
* @author  JK LJ
* @version V1.0.0
* @date    18-6-2020
* @brief   通用显示自编函数
*/
#include  "include.h"
uint8 savedata[200];
//画矩形外框    bevel_edge  斜角边
void Disp_Box_Zhi2(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t Biancolor)
{
    Print_Box_Zhi(x1,y1,x2,y2,Biancolor);
    Print_Box_Zhi(x1-1,y1-1,x2+1,y2+1,Biancolor);

}
void Print_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t color)
{
    LCD_SetColors(color,LCD_COLOR_BLACK);
    LCD_DrawLine(x1+bevel_edge-1,y1,x2-bevel_edge+1,y1);    //画水平线
    LCD_DrawLine(x1+bevel_edge-1,y2,x2-bevel_edge+1,y2);
    LCD_DrawLine(x1,y1+bevel_edge-2,x1,y2-bevel_edge+2);    //画垂直线
    LCD_DrawLine(x2,y1+bevel_edge-2,x2,y2-bevel_edge+2);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}
void Print_Box_Zhi(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color)
{
    LCD_SetColors(color,LCD_COLOR_BLACK);
    LCD_DrawLine(x1,y1,x2,y1);  //画水平线
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x1,y1,x1,y2);  //画垂直线
    LCD_DrawLine(x2,y1,x2,y2);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}


//填充方块
void Clear_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color)
{
    uint16_t i;
    LCD_SetColors(color,LCD_COLOR_BLACK);
    for(i=1; i <= y2-y1; i++) //填充
    {
        LCD_DrawLine(x1+1,y1+i,x2-1,y1+i);
    }
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}


//显示直方框
void Disp_Box_Zhi(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t Biancolor,uint32_t color)
{
    uint16_t i;
    Print_Box_Zhi(x1,y1,x2,y2,Biancolor);
    Print_Box_Zhi(x1-1,y1-1,x2+1,y2+1,Biancolor);
    LCD_SetColors(color,LCD_COLOR_BLACK);
    for(i=1; i <= y2-y1; i++) //填充
    {
        LCD_DrawLine(x1+1,y1+i,x2-1,y1+i);
    }
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}
//显示方框
void Disp_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color)
{
    uint8_t i;
    Print_Box(x1,y1,x2,y2,bevel_edge,Biancolor);
    Print_Box(x1-1,y1-1,x2+1,y2+1,bevel_edge+2,Biancolor);
    for(i=1; i < bevel_edge; i++) //填充斜边有左上角
    {
        LCD_DrawPixel(x1+1,y1+i,Biancolor);
        LCD_DrawPixel(x1+2,y1+i-1,Biancolor);
        LCD_DrawPixel(x1+3,y1+i-2,Biancolor);

        LCD_DrawPixel(x2-1,y1+i,Biancolor);
        LCD_DrawPixel(x2-2,y1+i-1,Biancolor);
        LCD_DrawPixel(x2-3,y1+i-2,Biancolor);

        LCD_DrawPixel(x1+1,y2+i-4,Biancolor);
        LCD_DrawPixel(x1+2,y2+i-3,Biancolor);
        LCD_DrawPixel(x1+3,y2+i-2,Biancolor);

        LCD_DrawPixel(x2-1,y2+i-4,Biancolor);
        LCD_DrawPixel(x2-2,y2+i-3,Biancolor);
        LCD_DrawPixel(x2-3,y2+i-2,Biancolor);
    }
    LCD_SetColors(color,LCD_COLOR_BLACK);
    for(i=1+bevel_edge; i <= y2-y1-bevel_edge; i++) //填充
    {
        LCD_DrawLine(x1+2,y1+i,x2-2,y1+i);
    }
    for(i=2; i <= bevel_edge; i++)
        LCD_DrawLine(x1+bevel_edge-i+3,y1+i,x2-bevel_edge+i-3,y1+i);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}
void Disp_Box_Sel(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t colorsel,struct RDispPara *sel)
{
    if(sel->sel!=sel->selold)
    {
        if(sel->sel==1)
        {
            Disp_Box(x1,y1,x2,y2,bevel_edge,Biancolor,colorsel);
        }
        else
        {
            Disp_Box(x1,y1,x2,y2,bevel_edge,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
        }
    }
}

void Disp_Box_Manu(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint8_t flag)
{
    if(flag==0)
    {

        Disp_Box(x1,y1,x2,y2,bevel_edge,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    }
    else
    {

        Disp_Box(x1,y1,x2,y2,bevel_edge,LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
    }
}

//底菜单通用函数
void COMmenuInit(char chs,uint8_t *ptr1,uint8_t *ptr2,uint8_t *ptr3,uint8_t *ptr4,uint8_t *ptr5,uint8_t sle)
{
    uint16_t y1;
	uint16_t yd;
    y1=446;
	yd=2;
    if(sle==0)
        Disp_Box_Manu(1,y1,127,y1+50,3,1);
    else
        Disp_Box_Manu(1,y1,127,y1+50,3,0);
    if(sle==1)
        Disp_Box_Manu(129,y1,255,y1+50,3,1);
    else
        Disp_Box_Manu(129,y1,255,y1+50,3,0);
    if(sle==2)
        Disp_Box_Manu(257,y1,383,y1+50,3,1);
    else
        Disp_Box_Manu(257,y1,383,y1+50,3,0);
    if(sle==3)
        Disp_Box_Manu(385,y1,511,y1+50,3,1);
    else
        Disp_Box_Manu(385,y1,511,y1+50,3,0);
    if(sle==4)
        Disp_Box_Manu(513,y1,639,y1+50,3,1);
    else
        Disp_Box_Manu(513,y1,639,y1+50,3,0);

    if(CHI == chs)
    {
        if(sle==0)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(28,y1+yd,ptr1,FONTTYPE24);
        if(sle==1)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(144,y1+yd,ptr2,FONTTYPE24);
        if(sle==2)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(295,y1+yd,ptr3,FONTTYPE24);
        if(sle==3)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(406,y1+yd,ptr4,FONTTYPE24);//21
        if(sle==4)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(552,y1+yd,ptr5,FONTTYPE24);//37
    }
    else if(ENG == chs)
    {
        if(sle==0)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(20,y1+yd, ptr1,FONTTYPE24);
        if(sle==1)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(153,y1+yd,ptr2,FONTTYPE24);
        if(sle==2)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(280,y1+yd,ptr3,FONTTYPE24);
        if(sle==3)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(412,y1+yd,ptr4,FONTTYPE24);
        if(sle==4)
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
        else
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
        LCD_DisplayStringLineLJ(550,y1+yd,ptr5,FONTTYPE24);
    }

    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}
//设置底菜单通用函数
void SetmenuInitP(char chs,uint8_t *ptr1,uint8_t *ptr2,uint8_t *ptr3,uint8_t *ptr4,uint8_t *ptr5
	,uint16 x1,uint16 x2,uint16 x3,uint16 x4,uint16 x5)
{
    uint16_t y1;
	uint16_t yd=2;
    y1=446;
    Disp_Box(1,y1,127,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(129,y1,255,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(257,y1,383,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(385,y1,511,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(513,y1,639,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
    if(CHI == chs)
    {
        LCD_DisplayStringLineLJ(x1,y1+yd,ptr1,FONTTYPE24);
        LCD_DisplayStringLineLJ(x2,y1+yd,ptr2,FONTTYPE24);
        LCD_DisplayStringLineLJ(x3,y1+yd,ptr3,FONTTYPE24);
        LCD_DisplayStringLineLJ(x4,y1+yd,ptr4,FONTTYPE24);
        LCD_DisplayStringLineLJ(x5,y1+yd,ptr5,FONTTYPE24);
    }
    else if(ENG == chs)
    {
        LCD_DisplayStringLineLJ(35,y1+5, ptr1,FONTTYPE24);
        LCD_DisplayStringLineLJ(153,y1+5,ptr2,FONTTYPE24);
        LCD_DisplayStringLineLJ(290,y1+5,ptr3,FONTTYPE24);
        LCD_DisplayStringLineLJ(412,y1+5,ptr4,FONTTYPE24);
        LCD_DisplayStringLineLJ(550,y1+5,ptr5,FONTTYPE24);
    }

    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}

//设置底菜单通用函数
void SetmenuInit(char chs,uint8_t *ptr1,uint8_t *ptr2,uint8_t *ptr3,uint8_t *ptr4,uint8_t *ptr5)
{
    uint16_t y1;
	uint16_t yd=2;
    y1=446;
    Disp_Box(1,y1,127,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(129,y1,255,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(257,y1,383,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(385,y1,511,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    Disp_Box(513,y1,639,y1+50,3,LCD_COLOR_MIDGRAY,LCD_COLOR_DARKBLUE);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
    if(CHI == chs)
    {
        LCD_DisplayStringLineLJ(35,y1+yd,ptr1,FONTTYPE24);
        LCD_DisplayStringLineLJ(144,y1+yd,ptr2,FONTTYPE24);
        LCD_DisplayStringLineLJ(295,y1+yd,ptr3,FONTTYPE24);
        LCD_DisplayStringLineLJ(406,y1+yd,ptr4,FONTTYPE24);
        LCD_DisplayStringLineLJ(552,y1+yd,ptr5,FONTTYPE24);
    }
    else if(ENG == chs)
    {
        LCD_DisplayStringLineLJ(35,y1+yd, ptr1,FONTTYPE24);
        LCD_DisplayStringLineLJ(153,y1+yd,ptr2,FONTTYPE24);
        LCD_DisplayStringLineLJ(290,y1+yd,ptr3,FONTTYPE24);
        LCD_DisplayStringLineLJ(412,y1+yd,ptr4,FONTTYPE24);
        LCD_DisplayStringLineLJ(550,y1+yd,ptr5,FONTTYPE24);
    }

    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}
void LCD_DisplayStringLineLJmenu(uint16_t x,uint16_t y,uint8_t FontType,struct RDispPara *sel,uint32_t colorsel)
{
    if(sel->sel!=sel->selold)
    {
        if(sel->sel==1)
        {
            LCD_SetColors(LCD_COLOR_WHITE,colorsel);
            if(sel->lang==CHI)
                LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FONTTYPE24);
            else
                LCD_DisplayStringLineLJ(x,y,(uint8_t *)HEn[sel->cind].Ch[sel->index[sel->cind]],FONTTYPE24);
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        }
        else
        {
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_DARKBLUE);
            if(sel->lang==CHI)
                LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FONTTYPE24);
            else
                LCD_DisplayStringLineLJ(x,y,(uint8_t *)HEn[sel->cind].Ch[sel->index[sel->cind]],FONTTYPE24);
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);

        }
        sel->selold = sel->sel; //要放最后不然前面做不了
    }
}
//底菜单通用函数
void COMmenu(struct RDispPara *sel1,struct RDispPara *sel2,struct RDispPara *sel3,struct RDispPara *sel4,struct RDispPara *sel5)
{
    uint32_t colorsel = LCD_COLOR_DARKGREEN;
	uint16_t y1;
	uint16_t yd=2;
	y1=446;
    Disp_Box_Sel(1,y1,127,490,3,LCD_COLOR_WHITE,colorsel,sel1);
    Disp_Box_Sel(129,y1,255,490,3,LCD_COLOR_WHITE,colorsel,sel2);
    Disp_Box_Sel(257,y1,383,490,3,LCD_COLOR_WHITE,colorsel,sel3);
    Disp_Box_Sel(385,y1,511,490,3,LCD_COLOR_WHITE,colorsel,sel4);
    Disp_Box_Sel(513,y1,639,490,3,LCD_COLOR_WHITE,colorsel,sel5);



    if(sel1->lang==CHI)
    {
//        LCD_DisplayStringLineLJ(28,y1+5,ptr1,FONTTYPE24);
//        LCD_DisplayStringLineLJ(144,y1+5,ptr2,FONTTYPE24);
//        LCD_DisplayStringLineLJ(295,y1+5,ptr3,FONTTYPE24);
//        LCD_DisplayStringLineLJ(422,y1+5,ptr4,FONTTYPE24);
//        LCD_DisplayStringLineLJ(534,y1+5,ptr5,FONTTYPE24);
        LCD_DisplayStringLineLJmenu(35,y1+yd,FONTTYPE24,sel1,colorsel);
        LCD_DisplayStringLineLJmenu(144,y1+yd,FONTTYPE24,sel2,colorsel);
        LCD_DisplayStringLineLJmenu(295,y1+yd,FONTTYPE24,sel3,colorsel);
        LCD_DisplayStringLineLJmenu(422,y1+yd,FONTTYPE24,sel4,colorsel);
        LCD_DisplayStringLineLJmenu(550,y1+yd,FONTTYPE24,sel5,colorsel);
    }
    else if(sel1->lang==ENG)
    {
//        LCD_DisplayStringLineLJ(20,y1+5, ptr1,FONTTYPE24);
//        LCD_DisplayStringLineLJ(153,y1+5,ptr2,FONTTYPE24);
//        LCD_DisplayStringLineLJ(280,y1+5,ptr3,FONTTYPE24);
//        LCD_DisplayStringLineLJ(420,y1+5,ptr4,FONTTYPE24);
//        LCD_DisplayStringLineLJ(540,y1+5,ptr5,FONTTYPE24);
        LCD_DisplayStringLineLJmenu(35,y1+yd,FONTTYPE24,sel1,colorsel);
        LCD_DisplayStringLineLJmenu(153,y1+yd,FONTTYPE24,sel2,colorsel);
        LCD_DisplayStringLineLJmenu(290,y1+yd,FONTTYPE24,sel3,colorsel);
        LCD_DisplayStringLineLJmenu(420,y1+yd,FONTTYPE24,sel4,colorsel);
        LCD_DisplayStringLineLJmenu(545,y1+yd,FONTTYPE24,sel5,colorsel);
    }
//    LCD_DisplayStringLineLJmenu(35,450,FONTTYPE24,sel1,colorsel);
//    LCD_DisplayStringLineLJmenu(153,450,FONTTYPE24,sel2,colorsel);
//    LCD_DisplayStringLineLJmenu(293,450,FONTTYPE24,sel3,colorsel);
//    LCD_DisplayStringLineLJmenu(420,450,FONTTYPE24,sel4,colorsel);
//    LCD_DisplayStringLineLJmenu(528,450,FONTTYPE24,sel5,colorsel);
}


uint32_t RGBCOLOR(uint8_t red,uint8_t green,uint8_t blue)
{
    uint32_t color;
    color=(uint32_t)red<<16|(uint32_t)green<<8|(uint32_t)blue;
    return color;
}

void LCD_DrawRectShade( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint8_t red,uint8_t green,uint8_t blue,uint8_t big_small,uint8_t colorflag )
{
    unsigned long i,j;
    uint32_t color;
    for(j=0; j<(y1-y0); j++)
    {
        if(colorflag==1)
            red-=big_small;
        else if(colorflag==2)
            green-=big_small;
        else if(colorflag==3)
            blue-=big_small;
        else if(colorflag==4)
        {
            red-=big_small;
            green-=big_small;
        }
        else if(colorflag==5)
        {
            red-=big_small;
            blue-=big_small;
        }
        else if(colorflag==6)
        {
            green-=big_small;
            blue-=big_small;
        }
        else if(colorflag==7)
        {
            red-=big_small;
            green-=big_small;
            blue-=big_small;
        }
        color=RGBCOLOR(red,green,blue);
        for( i = 0; ((x1-x0) ) > i; i++)
            LCD_DrawPixel(x0+i,y0+j,color);
    }
}


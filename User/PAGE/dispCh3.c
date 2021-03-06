/*
 * @file    dispCh3.c
 * @author  JK LJ
 * @version V1.0.0
 * @date    18-6-2020
 * @brief   三通道展示的界面大字体
*/
#include "include.h"
//任务控制块
OS_TCB DisplayCh3TaskTCB;
//任务堆栈
CPU_STK DisplayCh3_TASK_STK[DisplayCh3_STK_SIZE];
struct CUR CurCh3 = {0};
struct CUR CurCh3Old= {0};
void WriteString_Big(uint16_t x0, uint16_t y0, const uint8_t *pcStr);
//设置数据展示任务
void DisplayCh3_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    static uint8 page = 0;
    static uint8 pageold = 0xff;
    static uint8 flag=1;
    static RTC_TimeTypeDef TimeStr;
    //主页
    flag=1;
    Stu_DisplayData_Init(); //数据展示页面数据初始化直接用数据展示页面的结构体初始化page_DisplayCh3(uint8 page)page_DisplayCh3(uint8 page)
    page_DisplayCh3(page);
    pageold=page;
    page_DisplayCh3_cycle(page,flag); //数据展示页面处理
    FlashTuBiao();//刷一下图标
    while(1)
    {
        //任务
        keytmp = KeyTaskCreate(ALL_ReTurn);//20ms返回键值 不堵塞
        keytmp=ModifyCh3(keytmp,&CurCh3,&CurCh3Old,0,3) ;       //处理键值   0 至3页 0页是ch1 ch2 ch3 一共4页
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
        }
        page  = CurCh3.COL;
        if(page!=pageold)
        {
            pageold=page;
            flag=1;
            page_DisplayCh3(page);//数据展示页面初始化
        }
        flag=page_DisplayCh3_cycle(page,flag); //数据展示页面处理
        if(TimeStr.Seconds!=RTC_TimeStr.Seconds)
        {
            ShowDateTime(RTC_DateStr,RTC_TimeStr,2,5,&Font16);   //显示时间
            TimeStr=RTC_TimeStr;
        }
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );
    }
}


//画图的键值处理
uint8 ModifyCh3(uint8 keytmp,struct CUR* cur,struct CUR* curold,char MinCol,char MaxCol)                 // 键值匹配函数，在while大循环中反复匹配
{
    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    switch(cur->OPN)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
                ScreenFlag=ON;
            break;
        case KEY_UP://上
            if(cur->COL>MinCol)
                cur->COL--;
            else
                cur->COL=MaxCol;
            break;
        case KEY_DOWN:
            if(cur->COL<MaxCol) //最后一行
                cur->COL++;
            else
                cur->COL=MinCol;
            break;
        case KEY_ACC:
            cur->COL+=2;
            if(cur->COL>MaxCol)
                cur->COL=MinCol;
            break;
        case KEY_DOT:
            if(FileRes ==FILEReady)//未创建文件态
                SaveTrig = TRIGStart;
            else if(FileRes ==FILETFOpen)//创建tf卡文件
                SaveTrig = TRIGStopTF;
            else if(FileRes ==FILEUSBOpen)//创建usb卡文件
                SaveTrig = TRIGStopUSB;
            break;
        case KEY_LEFT://
            break;
        case KEY_FUNC1://进入10ch
            DataSave.Data_type.PageFlag =  FPageCh ;
            break;
        case KEY_FUNC2://进入data
            DataSave.Data_type.PageFlag =  FPageDisplayData ;
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageGraph ;
            break;
        case KEY_FUNC4://进入3ch

            break;
        case KEY_FUNC5://进入文件
            if(FileRes ==FILEReady)
                DataSave.Data_type.PageFlag =  FPageFile  ;
            break;
        case KEY_MANU://进入设置
            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
    }
    return keytmp;
}



void LCD_DisplayStringLineLJ_WithC(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t *prt,uint32_t TextC,uint32_t BackC)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontCH.flag ==0)
    {
        sel->FontCH.x1=x;
        sel->FontCH.y1=y;
        sel->FontCH.flag = 1;
    }
    BackColor=LCD_GetBackColor();
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_SetBackColor(BackC);
        WriteString_Big(x,y,prt);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                if(sel->sel==1)
                {
                    LCD_SetBackColor(BackC);
                    WriteString_Big(x,y,prt);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(BackC);
                    WriteString_Big(x,y,prt);
                    LCD_SetBackColor(BackColor);
                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(BackC,BackC);
            WriteString_Big(x,y,prt);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(TextC,BackC);
            WriteString_Big(x,y,prt);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}

void page_DisplayCh3(uint8 page)
{
    uint16 x=10;
    uint16 y=39;

    uint16 Ydiv=90;
    uint16 i;
    char str[12];
    uint32 BackColor,TextColor;
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(LCD_COLOR_MIDGRAY);

    LCD_SetTextColor(TextColor);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);//标题框
    LCD_DrawRect(0, 2, 639, 434);//全框

    if(CHI == DataSave.Data_type.LANG )
    {
        COMmenuInit(DataSave.Data_type.LANG,"全通道","数据显示","曲线","三通道","文件",3);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "CH:");
        LCD_DisplayStringLineLJ(x+320,y, "数值",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        COMmenuInit(DataSave.Data_type.LANG,"10CH","DATA","GRAPH","3CH","FILE",3);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+320,y, "Value");
    }
    LCD_SetFont(&Font20);
    y = 64;
    Ydiv =64*2;
    BackColor=LCD_GetBackColor();
    for(i=1; i<5-1; i++)
    {
        Num2String(i+page*3,str);

        LCD_SetFont(&Font20);
        LCD_SetBackColor(colors[i+page*3-1]);
        LCD_DisplayStringLine_EN_CH_LJ(x-8,y+Ydiv*(i-1)-3,(uint8_t* )str);
        if(i<5-2)
        {
            TextColor=LCD_GetTextColor();
            LCD_SetTextColor(LCD_COLOR_MIDGRAY);
            LCD_DrawHLine(0, y+Ydiv*(i)-5, 639);
            LCD_SetTextColor(TextColor);
        }

    }
    LCD_SetBackColor(BackColor);
}

void SWUint2Char(const struct RDispData RD,char* ptr)
{
    if(RD.vcind==VDC)//此单位是电压单位电压单位有5种
        strncpy(ptr,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VT)//
        strncpy(ptr,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VI)//
        strncpy(ptr,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VG)//
        strncpy(ptr,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VFRQ)//
        strncpy(ptr,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else  //此单位只有1种
        strncpy(ptr,RD.Uint[RD.vcind].Ch[0],sizeof(RD.Uint[RD.vcind].Ch[0]));
    ptr[sizeof(RD.Uint[RD.vcind].Ch[0])]=0;
}

float  DiplayCh3Big(struct RDispData *p,uint16_t x1, uint16_t y1,uint8 i,float Oldfval)
{
    char ptr[11];
    char Uptr[5] = {0};
    u8 len=0;
    float fval;
    u8 j;
    fval = p->DP.Num.fval;
    if(fabs(fval-Oldfval)<EPSF0)
    {
        return fval;
    }
    else
    {
        p->DP.Box.x1 =x1;
        p->DP.Box.y1 =y1;
        memset(ptr,0,sizeof(ptr));
        p->DP.dispold = 0;
        NumFloat2Char(p,&p->DP.Num,ptr);  //接收数据浮点型转换为字符串
        len = strlen(ptr);
        for(j=len; j<9; j++)
        {
            ptr[j] = ' ';
        }
        LCD_DisplayStringLineLJ_WithC(&p->DP,p->DP.Box.x1,p->DP.Box.y1-5,(uint8_t *)ptr,colors[i],LCD_COLOR_BLACK);
        memset(Uptr,0,sizeof(Uptr));
        SWUint2Char(*p,Uptr);
        p->DP.dispold = 0;
        LCD_DisplayNumLine_EN_CH_LJ_CH(&p->DP,p->DP.Box.x1+450,p->DP.Box.y1+65,Uptr,Font24,colors[i],LCD_COLOR_BLACK);
    }
    return fval;
}

uint8 page_DisplayCh3_cycle(uint8 page,uint8 flag)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    static float DHmdfval[2] = {0};
    static float DDafval[10] = {0};
    Ydiv =64*2;
    y = 64;
    if(flag==1)
    {
        for(i=0; i<10; i++)//
        {
            DDafval[i] = EPSFMAX;
        }
        for(i=0; i<2; i++)//
        {
            DHmdfval[i] = EPSFMAX;
        }
        flag=0;
    }
    if(page<3)
    {
        for(i=0; i<3; i++)//+1是两个频率参数
        {
            DDafval[i]=DiplayCh3Big(&DDa[i+page*3][0],x-5+Xdiv, y+i*Ydiv+24,i+page*3,DDafval[i]);
        }
    }
    else
    {
        i=0;
        DDafval[i]=DiplayCh3Big(&DDa[i+page*3][0],x-5+Xdiv, y+i*Ydiv+24,i+page*3,DDafval[i]);
        for(i=1; i<3; i++)  //+1是两个频率参数
        {
            DHmdfval[i-1]=DiplayCh3Big(&DDa[i+10][0],x-5+Xdiv, y+i*Ydiv+24,i+page*3,DHmdfval[i-1]);
        }
    }
    return flag;
}

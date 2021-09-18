/*
 * @file    dispCh.c
 * @author  JK LJ
 * @version V1.0.0
 * @date    18-6-2020
 * @brief   全通道展示的界面
*/

#include "include.h"
//任务控制块
OS_TCB DisplayChTaskTCB;
//任务堆栈
CPU_STK DisplayCh_TASK_STK[DisplayCh_STK_SIZE];

void SWUint2Char(const struct RDispData RD,char* ptr);
uint8 ModifyCh(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // 键值匹配函数，在while大循环中反复匹配
{
    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    switch(cur->OPN)
    {
        case KEY_FUNC1://进入10ch
            break;
        case KEY_FUNC2://进入data
            DataSave.Data_type.PageFlag =  FPageDisplayData ;
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageGraph ;
            break;
        case KEY_FUNC4://进入3ch
            DataSave.Data_type.PageFlag =  FPageCh3 ;
            break;
        case KEY_FUNC5://进入文件
            if(FileRes ==FILEReady)
                DataSave.Data_type.PageFlag =  FPageFile  ;
            break;
        case KEY_MANU://进入设置
            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
        case KEY_DOT:
            if(FileRes ==FILEReady)//未创建文件态
                SaveTrig = TRIGStart;
            else if(FileRes ==FILETFOpen)//创建tf卡文件
                SaveTrig = TRIGStopTF;
            else if(FileRes ==FILEUSBOpen)//创建usb卡文件
                SaveTrig = TRIGStopUSB;
            break;
        case KEY_0:
            if(ScreenFlag==OFF)
                ScreenFlag=ON;
            break;
    }
    return keytmp;
}

//设置数据展示任务
void DisplayCh_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    static uint8 flag=1;
    static RTC_TimeTypeDef TimeStr;
    //主页
    flag=1;
    Stu_DisplayData_Init(); //数据展示页面数据初始化直接用数据展示页面的结构体初始化
    page_DisplayCh();//数据展示页面初始化
    FlashTuBiao();//刷一下图标
    while(1)
    {
        //任务
        keytmp = KeyTaskCreate(ALL_ReTurn);//20ms返回键值 不堵塞
        keytmp=ModifyCh(keytmp,&CurDisp,&CurDispOld);     //处理键值
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
        }
//  NumTest();
        flag = page_DisplayCh_cycle(flag); //数据展示页面处理
        if(TimeStr.Seconds!=RTC_TimeStr.Seconds)
        {
            ShowDateTime(RTC_DateStr,RTC_TimeStr,2,5,&Font16);   //显示时间
            TimeStr=RTC_TimeStr;
        }
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );
//  FlashTuBiao();
        //      OSTimeDlyHMSM(0,0,0,900,OS_OPT_TIME_PERIODIC,&err);//延时200ms

    }
}

void Num2String(uint8 i,char *str)
{
    if(i>9)
	{
		if(i > 10)
		{
			sprintf(str,"%d   CH%d",i,i);
		}else{
			sprintf(str,"%d %6s",i,DataSave.Data_type.ChName[i-1]);
		}
		
	}
    else{
        sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
	}


}

void LCD_DisplayNumLine_EN_CH_LJ_CH_M(struct RDispPara *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType,uint32_t TextC,uint32_t BackC)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontEN.flag ==0)
    {
        sel->FontEN.x1=x;
        sel->FontEN.y1=y;
        sel->FontEN.flag = 1;
    }
    BackColor=LCD_GetBackColor();
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_SetFont(&FontType);
        LCD_SetBackColor(LCD_COLOR_HUI);

        DISP_TEMP_M(x,y,(uint8_t* )ptr,ch_on);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                LCD_SetFont(&FontType);
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    DISP_TEMP_M(x,y,(uint8_t* )ptr,ch_on);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    DISP_TEMP_M(x,y,(uint8_t* )ptr,ch_on);
                    LCD_SetBackColor(BackColor);
                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(TextC,BackC);
            DISP_TEMP_M(x,y,(uint8_t* )ptr,ch_on);

            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(TextC,BackC);
            DISP_TEMP_M(x,y,(uint8_t* )ptr,ch_on);

            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
            sel->Num.fvalold = sel->Num.fval;
        }
    }
}

void LCD_DisplayNumLine_EN_CH_LJ_CH(struct RDispPara *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType,uint32_t TextC,uint32_t BackC)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontEN.flag ==0)
    {
        sel->FontEN.x1=x;
        sel->FontEN.y1=y;
        sel->FontEN.flag = 1;
    }
    BackColor=LCD_GetBackColor();
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_SetFont(&FontType);
        LCD_SetBackColor(LCD_COLOR_HUI);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                LCD_SetFont(&FontType);
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);

                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);

                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
                    LCD_SetBackColor(BackColor);

                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(TextC,BackC);

            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(TextC,BackC);

            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
            sel->Num.fvalold = sel->Num.fval;
        }
    }
}

void page_DisplayCh(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    char str[12];
    uint32 BackColor,TextColor;
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(LCD_COLOR_MIDGRAY);
    LCD_DrawVLine(320, 58, 430-52);//画线
    LCD_SetTextColor(TextColor);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);//标题框
    LCD_DrawRect(0, 2, 639, 434);//全框
    if(CHI == DataSave.Data_type.LANG )
    {
        COMmenuInit(DataSave.Data_type.LANG,"全通道","数据显示","曲线","三通道","文件",0);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+320,y, "CH:");
        LCD_DisplayStringLineLJ(x+Xdiv+30,y, "数值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+320+Xdiv+30,y, "数值",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        COMmenuInit(DataSave.Data_type.LANG,"10CH","DATA","GRAPH","3CH","FILE",0);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv+30,y, "Value");
        LCD_DisplayStringLine_EN_CH_LJ(x+320,y, "CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+320+Xdiv+30,y, "Value");
    }
    LCD_SetFont(&Font20);
    y = 64;
    Ydiv =64;
    BackColor=LCD_GetBackColor();
    for(i=1; i<8-1; i++)
    {
        Num2String(i*2-1,str);
        LCD_SetFont(&Font20);
        LCD_SetBackColor(colors[i*2-1-1]);
        LCD_DisplayStringLine_EN_CH_LJ(x-8,y+Ydiv*(i-1)-3,(uint8_t* )str);
        if(i<8-2)
        {
            TextColor=LCD_GetTextColor();
            LCD_SetTextColor(LCD_COLOR_MIDGRAY);

            LCD_DrawHLine(0, y+Ydiv*(i)-5, 639);
            LCD_SetTextColor(TextColor);
        }
        Num2String(i*2,str);
        LCD_SetFont(&Font20);
        LCD_SetBackColor(colors[i*2-1]);
        LCD_DisplayStringLine_EN_CH_LJ(x-8+320,y+Ydiv*(i-1)-3,(uint8_t* )str);
    }
    LCD_SetBackColor(BackColor);
    LCD_SetFont(&Font20);
    LCD_DisplayStringLine_EN_CH_LJ(260,10,  "Tin:");

}


//
float DiplayChBig(struct RDispData* p,uint16_t x1, uint16_t y1,uint8 i,uint8 dev,float Oldfval)
{
    static char ptr[11];
    static char Uptr[5] = {0};
    float fval;
    u8 len=0;
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
//      p->DP.Num.fval=9.998;
//      p->DP.Num.Decimalbit = 2;
        NumFloat2Char(p,&p->DP.Num,ptr);  //接收数据浮点型转换为字符串
        len = strlen(ptr);
        for(j=len; j<9; j++)
        {
            ptr[j] = ' ';
        }
        LCD_DisplayNumLine_EN_CH_LJ_CH_M(&p->DP,p->DP.Box.x1,p->DP.Box.y1-5,ptr,Font24,colors[i],LCD_COLOR_BLACK);
        memset(Uptr,0,sizeof(Uptr));
        SWUint2Char(*p,Uptr);
        p->DP.dispold = 0;
        LCD_DisplayNumLine_EN_CH_LJ_CH(&p->DP,p->DP.Box.x1+dev*16,p->DP.Box.y1,Uptr,Font24,colors[i],LCD_COLOR_BLACK);
        return fval;
    }
}

float DiplayChTemp(struct RDispData* p,uint16_t x1, uint16_t y1,uint8 i,uint8 dev,float Oldfval)
{
    static char ptr[11];
    float fval;
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
        AddUint2Char(*p,ptr,7);    // 字符串尾巴上加单位
        LCD_DisplayNumLine_EN_CH_LJ_Home(&(p->DP),p->DP.Box.x1,p->DP.Box.y1+2,ptr,Font20);
        p->DP.dispold = 0;
        return fval;
    }
}

uint8 page_DisplayCh_cycle(uint8 flag)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    static float DTempfval=0;
    static float DFrqfval[2] = {0};
	static float DHmdfval[3] = {0};
    static float DDafval[12] = {0};
    if(flag==1)
    {
        DTempfval = EPSFMAX;
        for(i=0; i<12; i++)//
        {
            DDafval[i] = EPSFMAX;
        }
        for(i=0; i<2; i++)//
        {
            DFrqfval[i] = EPSFMAX;
        }
        flag=0;
        COMmenu(&DDa[10][0].DP,&DDa[10][1].DP,&DDa[10][2].DP,&DDa[10][3].DP,&DDa[10][4].DP);
    }
    Ydiv =64;
    y = 64;
//    for(i=0; i<5; i++)//+1是两个频率参数
//    {
//        DDafval[i*2] = DiplayChBig(&DDa[i*2][0],x-5+Xdiv,y+i*Ydiv+24,i*2,9,DDafval[i*2]);
//		if(i<4)
//		{
//			DDafval[i*2+1] = DiplayChBig(&DDa[i*2+1][0],x-5+Xdiv+320,y+i*Ydiv+24,i*2+1,9,DDafval[i*2+1]);
//		}else{
//			DHmd[0].DP.Num.fval += 0.01;
//			DHmdfval[0] = DiplayChBig(&DHmd[0],x-5+Xdiv+320,y+i*Ydiv+24,i*2+1,9,DHmdfval[0]);
//		}
//    }
	for(i=0; i<6; i++)//+1是两个频率参数
    {
		if(i == 5)
		{
			DDafval[i*2] = DiplayChBig(&DDa[i*2+1][0],x-5+Xdiv,y+i*Ydiv+24,i*2,9,DDafval[i*2]);
			DDafval[i*2+1] = DiplayChBig(&DDa[i*2+2][0],x-5+Xdiv+320,y+i*Ydiv+24,i*2+1,9,DDafval[i*2+1]);
		}else{
			DDafval[i*2] = DiplayChBig(&DDa[i*2][0],x-5+Xdiv,y+i*Ydiv+24,i*2,9,DDafval[i*2]);
			DDafval[i*2+1] = DiplayChBig(&DDa[i*2+1][0],x-5+Xdiv+320,y+i*Ydiv+24,i*2+1,9,DDafval[i*2+1]);
		}
    }
//    DFrqfval[0] = DiplayChBig(&DFrq[0],x-5+Xdiv,y+i*Ydiv+24,i*2,9,DFrqfval[0]);
//    DFrqfval[1] = DiplayChBig(&DFrq[1],x-5+Xdiv+320,y+i*Ydiv+24,i*2+1,9,DFrqfval[1]);
//	DHmd[1].DP.Num.fval += 0.01;
//	DHmd[2].DP.Num.fval += 0.01;
//	DHmdfval[1] = DiplayChBig(&DHmd[1],x-5+Xdiv,y+6*Ydiv+24,6*2,9,DHmdfval[1]);
//    DHmdfval[2] = DiplayChBig(&DHmd[2],x-5+Xdiv+320,y+6*Ydiv+24,6*2+1,9,DHmdfval[2]);
    DTempfval =DiplayChTemp(&DTemp,220+Xdiv,8,12,8,DTempfval);
    return flag;
}

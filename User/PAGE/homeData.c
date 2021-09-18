/*
  * @file    homeData.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   数据储存设置界面的业务
 */
//本文件为其它选项的设置页面
#include  "include.h"
//任务控制块
OS_TCB DoHDataTaskTCB;
//任务堆栈
CPU_STK DoHData_TASK_STK[DoHData_STK_SIZE];
void Stu_MinHData_Init(void);
//游标  分行和列
struct CUR CurHData = {0};
struct CUR CurHDataOld= {0};
//其它设置的下拉菜单
struct RDispPara DisHData[HDataCOL]= {0};
uint8   SaveCnt=1;
static void ModifyHS(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol);
static void ComBoxData(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol);
//10个通道里的设置
static const struct HDataStr HDataCh[HDataNUM] =
{
    {"50ms \0","100ms\0","150ms\0","200ms\0","500ms\0","1s   \0","2s   \0","5s   \0","10s  \0","20s  \0","30s  \0","1min \0","2min \0","5min \0","10min\0","20min\0","30min\0","1h   \0"}, //
    {">"},  //储存设置
    {"关","开"},//格式化文件
    {"关","开"},//截图功能
    {">"},  //
    {">"},  //
    {"关","开"},//触发记录
};    //

static const struct HDataStr HDataEn[HDataNUM]=
{
    {"50ms \0","100ms\0","150ms\0","200ms\0","500ms\0","1s   \0","2s   \0","5s   \0","10s  \0","20s  \0","30s  \0","1min \0","2min \0","5min \0","10min\0","20min\0","30min\0","1h   \0"}, //
    {">"},  //储存设置
    {"Off","On "},//格式化文件
    {"Off","On "},//截图功?    {">"},  //
    {">"},  //
    {">"},  //
    {"Off","On "},//触发记录
};    //
uint32 SampleTimeS[18]=
{
    50,
    100,
    150,
    200,
    500,
    1000,
    2000,
    5000,
    10000,
    20000,
    30000,
    60000,
    120000,
    300000,
    600000,
    1200000,
    1800000,
    3600000
};
static const struct HearStr HDataHear[HDataNUM]=
{
    {"保存间隔:","SAVE SAMPLING:"},   //保存间隔
    {"文件信息:","FILE INFOR:"},  //文件信息
    {"格式化文件:","FORMAT FILES:"},//外部取样
    {"截图功能:","SCREENSHOT:"},//截图功能
    {"起始时间:","START TIME:"},
    {"结束时间:","END TIME:"},
    {"触发收录:","TRIGGER RECORD:"},

};


//帮助菜单显示
static const struct HelpStr helpS[HDataNUM]=
{
    {"帮助:设定采样间隔。\0","Please set the sampling interval.\0"},
    {"帮助:设定文件收录地址。\0","Please set the address of document collection.\0"},
    {"帮助:将格式化全部收录文件。\0","All record files will be formatted!!!\0"},
    {"帮助:设置截图功能是否有效。","Set whether the screenshot function is valid.\0"},
    {"帮助:设定设备记录起始时间，年份仅后两位。\0","Set the device record start time,year is only two digits.\0"},
    {"帮助:设定设备记录结束时间，年份仅后两位。\0","Set the device record end time,year is only two digits.\0"},
    {"帮助:设定触发收录功能。\0","Please Set trigger recording function.\0"},
};

struct RDispPara DisDTS[DiaDTCOL][DiaDTCAM] = {0};//子对话框电压对话框的控键矩阵
struct RDispPara DisDTE[DiaDTCOL][DiaDTCAM] = {0};//子对话框电压对话框的控键矩阵

RTC_DateTypeDef DateStart;  //   4个字节日期时间
RTC_TimeTypeDef TimeStart;   //20个字节日期时间
RTC_DateTypeDef DateEnd;  //   4个字节日期时间
RTC_TimeTypeDef TimeEnd;   //20个字节日期时间
uint8 trigTflag=0;
void NumFrameHData(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void Stu_HData_Init(void)
{
    uint8 m,n;

    for(m=0; m<HDataCOL; m++)
    {
        DisHData[m].col = m;//行
        DisHData[m].cam = 0;//列
        DisHData[m].lang = DataSave.Data_type.LANG;
        DisHData[m].flag = 1;  //初始化第一显示标志
        DisHData[m].selold = 1;//以前全选中现在全没选中 刷新
        DisHData[m].sel= 0;     //当前没有选中
        DisHData[m].disp=1;  // 是否显示1 显示 0不显示
        DisHData[m].dispold=1; //之前是否显示
        DisHData[m].Box.flag = 0;  //字符串是否有框 0 没有框
        DisHData[m].FontCH.flag = 0;  //中文标志
        DisHData[m].FontEN.flag = 0;  //英文标志
        DisHData[m].cind=m;//类型
        DisHData[m].Ceng= 0;
        DisHData[m].index[DisHData[m].cind] =0;
        if(DisHData[m].cind==0)
        {
            DisHData[m].Ceng=18;
        }

        else if(DisHData[m].cind==2)
        {
            DisHData[m].Ceng=2;//子下拉框有几个选项 2个
        }
        else if(DisHData[m].cind==3)
        {
            DisHData[m].Ceng=2;
        }
        else if(DisHData[m].cind==6)
        {
            DisHData[m].Ceng=2;
        }
    }
    DisHData[0].flag = 0;  //初始化第一显示标志
    DisHData[0].sel= 1;//当前为选中状态
    DisHData[0].selold = 0;//上一次为选中状态
    for(n=0; n<HCAM; n++)
    {
        DisLog[12][n].sel= 0;
        DisLog[12][n].selold = 1;
        DisLog[12][n].lang = DataSave.Data_type.LANG;
    }
    DisLog[12][1].sel= 1;
    DisLog[12][1].selold = 2;

    //之前设置值还原
    DisHData[0].index[DisHData[0].cind] =    DataSave.Data_type.SaveSample;
    DisHData[2].index[DisHData[2].cind] =    DataSave.Data_type.FormatF;
    DisHData[3].index[DisHData[3].cind] =    DataSave.Data_type.ShotF;

    CurHData.COL = 0;
    CurHData.CAM = 0;


}

//中框数据的初始化
void Stu_MinHData_Init(void)
{
    uint8 m,n;
    for(m=0; m<HDataCOL ; m++)
    {
        DisHData[m].lang = DataSave.Data_type.LANG;
        DisHData[m].flag = 1;
        DisHData[m].selold = 1;//以前全选中现在全没选中 刷新
        DisHData[m].sel= 0;
        if(m==0&&n==0)
        {
            DisHData[m].selold = 0;
        }
    }
    DisHData[CurHData.COL].sel= 1;
    DisHData[CurHData.COL].selold = 0;
    DisHData[CurHData.COL].flag = 0;

}

//整个设置界面的初始化
void Stu_HData_cycle(struct CUR *sle,struct CUR *sleold)
{
    uint8 m;
    for(m=0; m<HDataCOL; m++)
    {
        DisHData[m].flag = 0;
        DisHData[m].selold = 0;
        DisHData[m].sel= 0;
    }
    if(sleold->COL!=sle->COL)
    {
        DisHData[sle->COL].selold = 0;//刷新
        DisHData[sle->COL].sel= 1;
        DisHData[sleold->COL].selold = 1;//刷新
        DisHData[sleold->COL].sel= 0;
    }
}

//初始化
void page_HData(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);
    LCD_DrawRect(0, 2, 639, 430);
    LCD_DrawHLine(0, 380, 639);
    Stu_HomeManu_Init(1);  //第2个按钮高亮
    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//此处调用主界面
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(2,5, "数据处理",FONTTYPE24);
        LCD_DisplayStringLineLJ(x,y, "设置项",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+4*Xdiv,y, "参数",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_DisplayStringLineLJ(3,5, "DATA",FONTTYPE24);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "Settings:");
        LCD_DisplayStringLine_EN_CH_LJ(x+4*Xdiv,y, "Parameter");
    }

    page_HData_cycle();
}

void LCD_DisplayStringLine_EN_CH_LJ_HData(struct RDispPara *sel,uint16_t x,uint16_t y,uint32_t SetTextColor)
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
        LCD_SetFont(&Font20);
        LCD_SetBackColor(SetTextColor);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HDataEn[sel->cind].Ch[sel->index[sel->cind]]);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                LCD_SetFont(&Font20);
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HDataEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HDataEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HDataEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HDataEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}


//中文
void LCD_DisplayStringLineLJ_HData(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType,uint32_t SetTextColor)
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
        LCD_SetBackColor(SetTextColor);
        LCD_DisplayStringLineLJ(x,y,(uint8_t *)HDataCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HDataCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HDataCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(x,y,(uint8_t *)HDataCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HDataEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}


//时间下拉框的键值处理函数
void ModifyDTS(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    OS_ERR err;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时200ms
            }
            break;
        case KEY_UP://上
            DisDTS[cur->COL][cur->CAM].selold=1;
            DisDTS[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisDTS[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTS[cur->COL][cur->CAM].selold=0;
            DisDTS[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisDTS[cur->COL][cur->CAM].selold=1;
            DisDTS[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDTS[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTS[cur->COL][cur->CAM].selold=0;
            DisDTS[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisDTS[cur->COL][cur->CAM].selold=1;
            DisDTS[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisDTS[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTS[cur->COL][cur->CAM].selold=0;
            DisDTS[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisDTS[cur->COL][cur->CAM].selold=1;
            DisDTS[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisDTS[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTS[cur->COL][cur->CAM].selold=0;
            DisDTS[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisDTS[cur->COL][cur->CAM].selold=1;
            DisDTS[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDTS[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTS[cur->COL][cur->CAM].selold=0;
            DisDTS[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->COL<3)
            {
                NumFrameHData(keytmp,&DisDTS[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisDTS[0][0].index[DisDTS[0][0].cind];
            }
            else if(cur->CAM==1&&cur->COL==3)
            {

                RD->index[RD->cind] = RDOld->index[RDOld->cind];
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }


}
void DTDiaInitS(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(m=0; m<DiaDTCOL; m++)
        for(n=0; n<DiaDTCAM; n++)
        {
            DisDTS[m][n].flag = 1;
            DisDTS[m][n].disp=0;
            DisDTS[m][n].dispold=0;
            DisDTS[m][n].lang =DataSave.Data_type.LANG;
            DisDTS[m][n].Num.bit = 2;
            DisDTS[m][n].Num.dot= 2;
            DisDTS[m][n].Num.Decimalbit= 0;
        }
    DisDTS[0][0].cind=17;
    DisDTS[0][0].disp=1;
    DisDTS[0][1].cind=17;
    DisDTS[0][1].disp=1;
    DisDTS[1][0].cind=17;
    DisDTS[1][0].disp=1;
    DisDTS[1][1].cind=17;
    DisDTS[1][1].disp=1;
    DisDTS[2][0].cind=17;
    DisDTS[2][0].disp=1;
    DisDTS[2][1].cind=17;
    DisDTS[2][1].disp=1;
    DisDTS[3][0].cind=16;
    DisDTS[3][0].index[DisDTS[3][0].cind] = 0;
    DisDTS[3][0].disp=1;
    DisDTS[3][1].cind=16;
    DisDTS[3][1].index[DisDTS[3][1].cind] = 1;
    DisDTS[3][1].disp=1;



    DisDTS[0][0].Num.fval = (float)DateStart.Year ; //年
    DisDTS[0][1].Num.fval = (float)DateStart.Month ;//月
    DisDTS[1][0].Num.fval = (float)DateStart.Date ;//日
    DisDTS[1][1].Num.fval = (float)TimeStart.Hours ;
    DisDTS[2][0].Num.fval = (float)TimeStart.Minutes ;
    DisDTS[2][1].Num.fval = (float)TimeStart.Seconds ;
    x1y1.Box.x1 =  100;//取点
    x1y1.Box.y1 =  100;//取?
    x2y2.Box.x2=  500;//取点
    x2y2.Box.y2 = 400 ;//取点
    Xdiv=80;
    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//标题
    // LCD_DrawRect(x1y1.Box.x1+2,x1y1.Box.y1+Ydiv*2,x2y2.Box.x2-x1y1.Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=24;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "时间设置",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "年",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+Ydiv, "月",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "日",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*4, "时",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "分",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*7, "秒",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Time Setting");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "Year");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv, "Mon");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "Date");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*4, "Hour");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "Min");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*7, "Sec");
    }
}

//时间对话框
void DateTimeDiaS(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    OS_ERR  err;
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.CAM=0;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  100;//取点
    x1y1.Box.y1 =  100;//
    x2y2.Box.x2=  500;//取点
    x2y2.Box.y1 = 376;//取点
    x2y2.Box.y2 = 400;//取点
    DTDiaInitS(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisDTS[0][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[0][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[1][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[1][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[2][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[2][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[3][0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTS[3][1],x1y1.Box.x1+Xdiv*3,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*3+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        for(m=0; m<DiaDTCOL; m++)
            for(n=0; n<DiaDTCAM; n++)
            {

                if(m<3)
                {
                    NumFloat2CharS(&DisDTS[m][n].Num,ptr);
                    F2UintChar(ptr,4);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisDTS[m][n],DisDTS[m][n].Box.x1+2,DisDTS[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisDTS[m][n],DisDTS[m][n].Box.x1+15,DisDTS[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisDTS[m][n],DisDTS[m][n].Box.x1+15,DisDTS[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyDTS(keytmp,RD,&DisLogOld,&Cur,DiaDCCOL,DiaDCCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC); //DC的对话框第4行是确定和取消按钮
    OSSchedLock(&err);
    DateStart.Year = (uint8)DisDTS[0][0].Num.fval; //年
    DateStart.Month = (uint8)DisDTS[0][1].Num.fval ;//月
    DateStart.Date =(uint8)DisDTS[1][0].Num.fval;//日
    TimeStart.Hours =  (uint8)DisDTS[1][1].Num.fval;
    TimeStart.Minutes = (uint8)DisDTS[2][0].Num.fval ;
    TimeStart.Seconds = (uint8)DisDTS[2][1].Num.fval ;
    Stu_MinHData_Init();
    ModifyDTS(keytmp,RD,&DisLogOld,&Cur,DiaDTCOL,DiaDTCAM);
    page_HData();
    OSSchedUnlock(&err);
}
//时间下拉框的键值处理函数
void ModifyDTE(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    OS_ERR err;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时200ms
            }
            break;
        case KEY_UP://上
            DisDTE[cur->COL][cur->CAM].selold=1;
            DisDTE[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisDTE[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTE[cur->COL][cur->CAM].selold=0;
            DisDTE[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisDTE[cur->COL][cur->CAM].selold=1;
            DisDTE[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDTE[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTE[cur->COL][cur->CAM].selold=0;
            DisDTE[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisDTE[cur->COL][cur->CAM].selold=1;
            DisDTE[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisDTE[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTE[cur->COL][cur->CAM].selold=0;
            DisDTE[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisDTE[cur->COL][cur->CAM].selold=1;
            DisDTE[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisDTE[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTE[cur->COL][cur->CAM].selold=0;
            DisDTE[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisDTE[cur->COL][cur->CAM].selold=1;
            DisDTE[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDTE[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDTE[cur->COL][cur->CAM].selold=0;
            DisDTE[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->COL<3)
            {
                NumFrameHData(keytmp,&DisDTE[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisDTE[0][0].index[DisDTE[0][0].cind];
            }
            else if(cur->CAM==1&&cur->COL==3)
            {

                RD->index[RD->cind] = RDOld->index[RDOld->cind];
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }


}

void InitTigT(void)
{
    trigTflag=0;
    DateStart.Year = 20.0; //年
    DateStart.Month = 6.0;//月
    DateStart.Date =16.0;//日
    TimeStart.Hours = 15.0;
    TimeStart.Minutes = 0.0;
    TimeStart.Seconds = 0.0;
    DateEnd.Year = 20.0; //年
    DateEnd.Month = 6.0;//月
    DateEnd.Date =16.0;//日
    TimeEnd.Hours = 15.0;
    TimeEnd.Minutes = 0.0;
    TimeEnd.Seconds = 0.0;
}
void DTDiaInitE(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(m=0; m<DiaDTCOL; m++)
        for(n=0; n<DiaDTCAM; n++)
        {
            DisDTE[m][n].flag = 1;
            DisDTE[m][n].disp=0;
            DisDTE[m][n].dispold=0;
            DisDTE[m][n].lang =DataSave.Data_type.LANG;
            DisDTE[m][n].Num.bit = 2;
            DisDTE[m][n].Num.dot= 2;
            DisDTE[m][n].Num.Decimalbit= 0;
        }
    DisDTE[0][0].cind=17;
    DisDTE[0][0].disp=1;
    DisDTE[0][1].cind=17;
    DisDTE[0][1].disp=1;
    DisDTE[1][0].cind=17;
    DisDTE[1][0].disp=1;
    DisDTE[1][1].cind=17;
    DisDTE[1][1].disp=1;
    DisDTE[2][0].cind=17;
    DisDTE[2][0].disp=1;
    DisDTE[2][1].cind=17;
    DisDTE[2][1].disp=1;
    DisDTE[3][0].cind=16;
    DisDTE[3][0].index[DisDTE[3][0].cind] = 0;
    DisDTE[3][0].disp=1;
    DisDTE[3][1].cind=16;
    DisDTE[3][1].index[DisDTE[3][1].cind] = 1;
    DisDTE[3][1].disp=1;


    DisDTE[0][0].Num.fval = (float)DateEnd.Year ; //年
    DisDTE[0][1].Num.fval = (float)DateEnd.Month ;//月
    DisDTE[1][0].Num.fval = (float)DateEnd.Date ;//日
    DisDTE[1][1].Num.fval = (float)TimeEnd.Hours ;
    DisDTE[2][0].Num.fval = (float)TimeEnd.Minutes ;
    DisDTE[2][1].Num.fval = (float)TimeEnd.Seconds ;
    x1y1.Box.x1 =  100;//取点
    x1y1.Box.y1 =  100;//取
    x2y2.Box.x2=  500;//取点
    x2y2.Box.y2 = 400 ;//取点
    Xdiv=80;
    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//标题
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=24;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "时间设置",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "年",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+Ydiv, "月",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "日",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*4, "时",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "分",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*7, "秒",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Time Setting");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "Year");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv, "Mon");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "Date");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*4, "Hour");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "Min");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*7, "Sec");
    }
}





//时间对话框
void DateTimeDiaE(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    OS_ERR  err;
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.CAM=0;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  100;//取点
    x1y1.Box.y1 =  100;//
    x2y2.Box.x2=  500;//取点
    x2y2.Box.y1 = 376;//取点
    x2y2.Box.y2 = 400;//取点
    DTDiaInitE(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisDTE[0][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[0][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[1][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[1][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[2][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[2][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[3][0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDTE[3][1],x1y1.Box.x1+Xdiv*3,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*3+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        for(m=0; m<DiaDTCOL; m++)
            for(n=0; n<DiaDTCAM; n++)
            {

                if(m<3)
                {
                    NumFloat2CharS(&DisDTE[m][n].Num,ptr);
                    F2UintChar(ptr,4);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisDTE[m][n],DisDTE[m][n].Box.x1+2,DisDTE[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisDTE[m][n],DisDTE[m][n].Box.x1+15,DisDTE[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisDTE[m][n],DisDTE[m][n].Box.x1+15,DisDTE[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyDTE(keytmp,RD,&DisLogOld,&Cur,DiaDCCOL,DiaDCCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC); //DC的对话框第4行是确定和取消按钮
    OSSchedLock(&err);
    DateEnd.Year = (uint8)DisDTE[0][0].Num.fval; //年
    DateEnd.Month = (uint8)DisDTE[0][1].Num.fval ;//月
    DateEnd.Date =(uint8)DisDTE[1][0].Num.fval;//日
    TimeEnd.Hours =  (uint8)DisDTE[1][1].Num.fval;
    TimeEnd.Minutes = (uint8)DisDTE[2][0].Num.fval ;
    TimeEnd.Seconds = (uint8)DisDTE[2][1].Num.fval ;
    Stu_MinHData_Init();
    ModifyDTE(keytmp,RD,&DisLogOld,&Cur,DiaDTCOL,DiaDTCAM);
    page_HData();
    OSSchedUnlock(&err);

}




void page_HData_cycle(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=90;
    uint16 Ydiv=90;
    uint16 i;
    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//此处调用主界面
    y = 60;
    Ydiv =28;
    Display_Help(x-2,y+Ydiv*11+20,helpS[CurHData.COL].Ch[DataSave.Data_type.LANG]);
    for(i=0; i<HDataCOL; i++)
    {
        Disp_Box_Home(&DisHData[i],x+Xdiv*4+20,y+i*Ydiv+2,x-15+Xdiv*4+130,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
    }
    for(i=0; i<HDataNUM; i++)
    {
        if(CHI == DataSave.Data_type.LANG )
        {
            Disp_Head(&DisHData[i],x-5,y+Ydiv*i,(uint8_t* )HDataHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24,Font20);
        }
        else if(ENG == DataSave.Data_type.LANG)
        {
            Disp_Head(&DisHData[i],x-5,y+Ydiv*i,(uint8_t* )HDataHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24,Font20);
        }
    }
    y = 64;
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
    for(i=0; i<HDataCOL; i++)
    {
        if(i==1||i==0||i==4||i==5)
        {
            LCD_DisplayStringLine_EN_CH_LJ_HData(&DisHData[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
        else
        {
            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_HData(&DisHData[i],x+30+Xdiv*4,y+i*Ydiv,FONTTYPE16,LCD_COLOR_HUI );
            else
                LCD_DisplayStringLine_EN_CH_LJ_HData(&DisHData[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
}


//第一列下拉框的键值处理函数
void ModifyHS(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)
{
    OS_ERR err;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时200ms
            }
            break;
        case KEY_UP://上
            if((RD->index[RD->cind])==0)
                (RD->index[RD->cind]) = MaxCol-1;
            else
                RD->index[RD->cind]--;
            break;
        case KEY_DOWN:
            if( ++(RD->index[RD->cind])>MaxCol-1)
                (RD->index[RD->cind]) =0 ;
            break;
        case KEY_ACC:
            (RD->index[RD->cind])+=2;
            if((RD->index[RD->cind])>MaxCol-1)
                (RD->index[RD->cind]) =0 ;
            break;
        case KEY_ENTER:
            if(sel->COL<HDataCOL-1)
            {
                DisHData[sel->COL].disp=1;
                DisHData[sel->COL].dispold=0;
                DisHData[sel->COL].index[DisHData[sel->COL].cind] = RD->index[RD->cind];

            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
    }
}

void Disp_Box_HData(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    if(sel->disp==1)
    {
        if(sel->sel!=sel->selold)
        {
            if(sel->sel==1)
            {
                Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_DARKGREEN);
            }
            else
            {
                Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            }
        }
    }
    else if(sel->disp==0&&sel->dispold==1)
    {
        Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    }
}

//Sampling下拉框可通用
void ComBoxSHData(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[SaverOnOffCOL] = {0};   //选最大的
    static struct RDispPara DisLogOld;
    for(m=0; m<MaxCol; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=RD->cind;
        DisInPut[m].index[DisInPut[m].cind]=m;
    }
    DisLogOld = *RD;
    do
    {
        box=RD->Box;
        Ceng = RD->Ceng;
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        for(m=0; m<MaxCol; m++)
        {
            DisInPut[m].flag = 1;
            DisInPut[m].disp=1;
            DisInPut[m].dispold=0;
            DisInPut[m].sel = 0;
            DisInPut[m].selold=1;
            DisInPut[m].Box.x1 = RD->Box.x1;
            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1);
            DisInPut[m].Box.x2 = RD->Box.x2 ;
            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1);
            DisInPut[m].FontCH.x1 = RD->FontCH.x1;
            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
            DisInPut[m].FontEN.x1 = RD->FontEN.x1;
            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
            {
                DisInPut[m].sel = 1;
                DisInPut[m].selold=0;
                DisInPut[m].flag = 0;
            }
            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
            if(cur->COL==1||cur->COL==4|cur->COL==5||cur->COL==8)
            {
                LCD_DisplayStringLine_EN_CH_LJ_HData(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }
            else
            {
                if(CHI == DataSave.Data_type.LANG )
                    LCD_DisplayStringLineLJ_HData(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16,LCD_COLOR_BLACK);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_HData(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifySaver(keytmp,RD,&DisLogOld,cur,MaxCol);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_MinHData_Init();
    ModifySaver(keytmp,RD,&DisLogOld,cur,MaxCol);
    page_HData();
}


void NumFrameHData(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    char  ptr[5] = "     ";
    static struct RDispPara DisLogOld;
    DisLogOld = *RD;
    RD->Num.fval=0;
    RD->Num.Decimalbit = 0;
    RD->Num.Changflag = 0;
    RD->Num.bit = 0;
    RD->Num.dot=0;
    RD->sel = 1;
    RD->selold = 0;
    LCD_DisplayNumLine_EN_CH_LJ_Home(RD,RD->Box.x1+2,RD->Box.y1+2,ptr,Font20);
    do
    {
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyTEMPNum(keytmp,RD,&DisLogOld,cur,ptr);
        NumFloat2CharS(&RD->Num,ptr);
        LCD_DisplayNumLine_EN_CH_LJ(RD,ptr,LCD_COLOR_WHITE,LCD_COLOR_HUI,Font20);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    RD->sel = 1;
    RD->selold = 0;
    LCD_DisplayNumLine_EN_CH_LJ_Home(RD,RD->Box.x1+2,RD->Box.y1+2,ptr,Font20);
}


//lj  为了提速18项下拉菜单按键响应变慢做了更改
static void ComBoxData(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[SamplingCOL] = {0};   //选最大的
    static struct RDispPara DisLogOld;
    box=RD->Box;
    Ceng = RD->Ceng;
    for(m=0; m<MaxCol; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=RD->cind;
        DisInPut[m].index[DisInPut[m].cind]=m;
        //lj  为了提速18项下拉菜单按键响应变慢做了更改
        if(m <9)
        {
            DisInPut[m].Box.x1 = RD->Box.x1;
            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1);
            DisInPut[m].Box.x2 = RD->Box.x2 ;
            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1);
            DisInPut[m].FontCH.x1 = RD->FontCH.x1;
            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
            DisInPut[m].FontEN.x1 = RD->FontEN.x1;
            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
        }
        else
        {
            DisInPut[m].Box.x1 = RD->Box.x1+box.w;
            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1-9);
            DisInPut[m].Box.x2 = RD->Box.x2 +box.w;
            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1-9);
            DisInPut[m].FontCH.x1 = RD->FontCH.x1+box.w;
            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1-9);
            DisInPut[m].FontEN.x1 = RD->FontEN.x1+box.w;
            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1-9);
        }
    }
    DisLogOld = *RD;
    //lj  为了提速18项下拉菜单按键响应变慢做了更改
    if(Ceng <9)
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    else
        Disp_Box_Zhi( box.x1,box.y2,box.x2+box.w,box.y2+(box.h+8)*(Ceng/2),LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    do
    {
        for(m=0; m<MaxCol; m++)
        {
            DisInPut[m].flag = 1;
            DisInPut[m].disp=1;
            DisInPut[m].dispold=0;
            DisInPut[m].sel = 0;
            DisInPut[m].selold=1;
            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
            {
                DisInPut[m].sel = 1;
                DisInPut[m].selold=0;
                DisInPut[m].flag = 0;
            }
            //lj  为了提速18项下拉菜单按键响应变慢做了更改
            //     Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
            if(cur->COL==0)
            {
                LCD_DisplayStringLine_EN_CH_LJ_HData(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }
            else
            {
                //lj  为了提速18项下拉菜单按键响应变慢做了更改
                Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
                if(CHI == DataSave.Data_type.LANG )
                    LCD_DisplayStringLineLJ_HData(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16,LCD_COLOR_BLACK);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_HData(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }
        }
        if(Ceng <9)
            Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        else
            Disp_Box_Zhi2( box.x1,box.y2,box.x2+box.w,box.y2+(box.h+8)*(Ceng/2),LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyHS(keytmp,RD,&DisLogOld,cur,MaxCol);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_MinHData_Init();
    ModifyHS(keytmp,RD,&DisLogOld,cur,MaxCol);
    page_HData();
}
void CompSample(void)
{
    uint8 max;
    uint8 i;
    if(DataSave.Data_type.SaveSample<DataSave.Data_type.SampleCAM)
    {
        DataSave.Data_type.SaveSample = DataSave.Data_type.SampleCAM;
        SaveCnt=1;
    }
    else
    {
        max=DataSave.Data_type.SaveSample;
        DataSave.Data_type.SaveSample=DataSave.Data_type.SampleCAM;
        SaveCnt=1;
        for(i=max; i>DataSave.Data_type.SampleCAM; i--)
        {
            if((SampleTimeS[i]%SampleTimeS[DataSave.Data_type.SampleCAM])==0)
            {
                SaveCnt=(SampleTimeS[i]/SampleTimeS[DataSave.Data_type.SampleCAM]);
                DataSave.Data_type.SaveSample= i;
                break;
            }
        }
    }
    DisHData[0].index[DisHData[0].cind] =    DataSave.Data_type.SaveSample;
}


//提示对话框
static uint8 TiShiDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 type)
{
    OS_ERR  err;
    uint8 n;
    uint16 Xdiv=60;
//    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    static struct CUR Cur;
//    char ptr[10] = {0};
    Cur.CAM=1;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  100;//取点
    x1y1.Box.y1 =  100;//
    x2y2.Box.x2=  500;//取点
    x2y2.Box.y1 = 276;//取点
    x2y2.Box.y2 = 300;//取点
    TiShiDiaInit(keytmp,RD, cur,type) ;
    Xdiv=80;
  //  Ydiv=24;

    do
    {
      //  Ydiv=24;
        Disp_Box_Home(&DisTiShi[0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisTiShi[1],x1y1.Box.x1+Xdiv*3,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*3+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
      //  Ydiv=32;
        LCD_SetFont(&Font20);
        for(n=0; n<DisTiShiCAM; n++)
        {

            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_Home(&DisTiShi[n],DisTiShi[n].Box.x1+15,DisTiShi[n].Box.y1+2,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Home(&DisTiShi[n],DisTiShi[n].Box.x1+15,DisTiShi[n].Box.y1+2);
        }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyTiShi(keytmp,RD,&DisLogOld,&Cur,DisTiShiCAM);
    }
    while(!(keytmp==KEY_ENTER)&&keytmp!=KEY_ESC);  //
    OSSchedLock(&err);
    Stu_MinHData_Init();
    ModifyTiShi(keytmp,RD,&DisLogOld,&Cur,DisTiShiCAM);
    if(Cur.CAM==0)
    	FormatSD();
    page_HData();
    OSSchedUnlock(&err);
    return Cur.CAM;
}

uint8 ModifyHData(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // 键值匹配函数，在while大循环中反复匹配
{
    OS_ERR err;


    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    switch(cur->OPN)
    {
        case KEY_FUNC1://进入显示设置
            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
        case KEY_FUNC2://进入data 处理

            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageFile ;
            break;
        case KEY_FUNC4:// 接口
            DataSave.Data_type.PageFlag =  FPageIF  ;
            break;
        case KEY_FUNC5://进入设置
            DataSave.Data_type.PageFlag =  FPageOtur ;
            break;
        case KEY_ESC:
            DataSave.Data_type.PageFlag =  PageFlagSetRe  ;
            break;
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时200ms
            }
            break;
        case KEY_UP://上
            if(cur->COL>0)
                cur->COL--;
            else
                cur->COL=HDataCOL-1;
            break;
        case KEY_DOWN:
            if(cur->COL<HDataCOL-1) //最后一行是功能键
                cur->COL++;
            else
                cur->COL=0;
            break;
        case KEY_ACC:
            cur->COL+=2;
            if(cur->COL>HDataCOL-1)
                cur->COL=1;
            break;
        case KEY_ENTER:
            if(cur->COL==0)  //第一行
            {
                ComBoxData(keytmp,&DisHData[cur->COL],cur,SamplingCOL);
                DataSave.Data_type.SaveSample=DisHData[cur->COL].index[DisHData[cur->COL].cind] ;

                CompSample();
                SaveAllPara();

            }
            else if(cur->COL==1)//文件格式存储
            {
                FileInForDia(keytmp,&DisHData[cur->COL],cur);
            }
            else if(cur->COL==2)
            {
                ComBoxData(keytmp,&DisHData[cur->COL],cur,ExternCOL);
                DataSave.Data_type.FormatF=DisHData[cur->COL].index[DisHData[cur->COL].cind] ;
                if(DataSave.Data_type.FormatF==ON)  //格式化SD
                {
                   TiShiDia(keytmp,&DisHData[cur->COL],cur,2);//格式化函数放在里面
                }
                DataSave.Data_type.FormatF=OFF;
                DisHData[cur->COL].index[DisHData[cur->COL].cind] =DataSave.Data_type.FormatF;
            }
            else if(cur->COL==3)
            {
                ComBoxData(keytmp,&DisHData[cur->COL],cur,BackUpCOL);
                DataSave.Data_type.ShotF=DisHData[cur->COL].index[DisHData[cur->COL].cind] ;

            }
            else if(cur->COL==4)
            {
                DateTimeDiaS(keytmp,&DisHData[cur->COL],cur);
            }
            else if(cur->COL==5)
            {
                DateTimeDiaE(keytmp,&DisHData[cur->COL],cur);
            }
            else if(cur->COL==6)
            {
                ComBoxData(keytmp,&DisHData[cur->COL],cur,2);
                trigTflag=DisHData[cur->COL].index[DisHData[cur->COL].cind] ;
            }
            break;
    }
    Stu_HData_cycle(cur,curold);
    return keytmp;
}


//温度对话框初始化
void FileInForDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{

    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    uint32 BackColor;
    char str[20]= {0};
    FatReadAllDir("0:",str);


    x1y1.Box.x1 =  100;//取点
    x1y1.Box.y1 =  100;//取
    x2y2.Box.x2=  500;//取点
    x2y2.Box.y2 = 400 ;//取点

    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//标题
    LCD_DrawRect(x1y1.Box.x1+2,x1y1.Box.y1+Ydiv*2,x2y2.Box.x2-x1y1.Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+2+1,x1y1.Box.y1+Ydiv*2+1,x2y2.Box.x2-x1y1.Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, (uint8_t *)"信息",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"类型",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*3, (uint8_t *)"总容量",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, (uint8_t *)"已用容量",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, (uint8_t *)"Information");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"Tpye:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*3, (uint8_t *)"Total:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, (uint8_t *)"Used:");
    }
    LCD_SetFont(&Font20);
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+Ydiv, (uint8_t *)"TF Card");
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*3, (uint8_t *)"32GB");
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*4, (uint8_t *)str);
    Disp_Box(260,374,340,396,0,LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
    Ydiv=32;
    LCD_SetFont(&Font20);
    BackColor=LCD_GetBackColor();
    LCD_SetFont(&Font20);
    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
    LCD_DisplayStringLine_EN_CH_LJ(285,376,(uint8_t* )"OK");
    LCD_SetBackColor(BackColor);
}

//信息对话框
void FileInForDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{


    FileInForDiaInit(keytmp,RD, cur) ;

    do
    {
        keytmp = KeyTaskCreate(NoNull_ReTurn);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_MinHData_Init();
    page_HData();
}

//设置菜单任务
void DoHData_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //主页
    Stu_HData_Init();
    page_HData();
    while(1)
    {
        //DoHData任务
//        BluidICOTask();
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //   OSTaskDel(&ICOTaskTCB,&err);
        keytmp=ModifyHData(keytmp,&CurHData,&CurHDataOld);     //处理键值
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
            //删除自己任务
        }
        page_HData_cycle();
//        FlashTuBiao();
    }
}

/*
  * @file    homeIF.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   接口设置界面的业务
 */
//本文件为其它选项的设置页面
#include  "include.h"
#define IsDigit(c)  (((c)>='0')&&((c)<='9'))
//任务控制块
OS_TCB DoIFTaskTCB;
//任务堆栈
CPU_STK DoIF_TASK_STK[DoIF_STK_SIZE];
void Stu_MinIF_Init(void);
//游标  分行和列
struct CUR CurIF = {0};
struct CUR CurIFOld= {0};
//其它设置的下拉菜单
struct RDispPara DisIF[IFCOL]= {0};
//10个通道里的设置
static const struct RStr IFCh[IFNUM] =
{
    {"0\0","1\0","2\0","3\0","4\0","5\0","6\0","7\0","8\0","9\0"}, //
    {">"},//物联网平台接口
    {"关","开"},//wifi 上传
    {"关","开"},//报警输出
    {"累计","实时"},
    {"累计","实时"},
    {"关","开"},//同步触发

};    //
static const struct RStr IFEn[IFNUM]=
{
    {"0\0","1\0","2\0","3\0","4\0","5\0","6\0","7\0","8\0","9\0"}, //
    {">"},//物联网平台接口
    {"Off","On "},//wifi 上传
    {"Off","On "},//报警输出
    {"Total","Actual "},//脉冲1 输出
    {"Total","Actual "},//脉冲2 输出
    {"Off","On "},//同步触发
};

static const struct HearStr IFHear[IFNUM]=
{
    {"接口编号:","USB ID:"},   //usb 编号
    {"云平台参数:","CLOUD PLATFORM:"},   //云平台参数
    {"云平台上传:","UPLOAD:"},//wifi 上传
    {"报警输出:","ALARM OUTPUT:"},//报警输出
    {"脉冲一:","PLS1:"},//脉冲1 输出
    {"脉冲二:","PLS2:"},//脉冲2 输出
    {"同步触发:","SYNC TRIGGER:"},//同步触发
};    //
//帮助菜单显示
static const struct HelpStr helpS[IFNUM]=
{
    {"帮助:设定接口编号。\0","Please set USB ID.\0"},
    {"帮助:设定云平台参数。\0","Please set cloud platform parameters.\0"},
    {"帮助:设定云平台数据上传。\0","Please set cloud platform data upload.\0"},
    {"帮助:设定报警输出。\0","Please set cloud alarm output.\0"},
    {"帮助:设定脉冲一输出。\0","Please set PLS1 output.\0"},
    {"帮助:设定脉冲二输出。\0","Please set PLS2 output.\0"},
    {"帮助:设定同步触发。\0","Please set synchronous trigger.\0"},
};

static const struct HearStr ParaS[DiaWifiCOL-1][DiaWifiCAM]=
{
    {{"热点密码\0","PassW\0"},{"热点名称","Name\0"}},
    {{"目标端口\0","DestP\0"},{"域名","Domain\0"}},
    {{"本地端口\0","LocolP\0"},{"本地编号","ID\0"}},

};

const char FuHao[DiaSoftKeyCAM] ="-/_()<>:*#%~\\";

struct RDispStr DisWifi[DiaWifiCOL][DiaWifiCAM] = {0};//子对话框电压对话框的控键矩阵

struct RDispStr DisSoftKey[DiaSoftKeyCOL][DiaSoftKeyCAM] = {0};//子对话框电压对话框的控键矩阵


static void ModifyHS(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol);
static void ComBoxData(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol);
void Stu_IF_Init(void)
{
    uint8 m,n;
    for(m=0; m<IFCOL; m++)
    {
        DisIF[m].col = m;//行
        DisIF[m].cam = 0;//列
        DisIF[m].lang = DataSave.Data_type.LANG;
        DisIF[m].flag = 1;  //初始化第一显示标志
        DisIF[m].selold = 1;//以前全选中现在全没选中 刷新
        DisIF[m].sel= 0;     //当前没有选中
        DisIF[m].disp=1;  // 是否显示1 显示 0不显示
        DisIF[m].dispold=1; //之前是否显示
        DisIF[m].Box.flag = 0;  //字符串是否有框 0 没有框
        DisIF[m].FontCH.flag = 0;  //中文标志
        DisIF[m].FontEN.flag = 0;  //英文标志
        DisIF[m].cind=m;//类型
        DisIF[m].Ceng= 0;
        DisIF[m].index[DisIF[m].cind] =0;
        if(DisIF[m].cind==0)
        {
            DisIF[m].Ceng=10;
        }
        else if(DisIF[m].cind==2||DisIF[m].cind==3||DisIF[m].cind==4
                ||DisIF[m].cind==5||DisIF[m].cind==6)
        {
            DisIF[m].Ceng=2;//子下拉框有几个选项 2个
        }
    }
    DisIF[0].flag = 0;  //初始化第一显示标志
    DisIF[0].sel= 1;//当前为选中状态
    DisIF[0].selold = 0;//上一次为选中状态
    for(n=0; n<HCAM; n++)
    {
        DisLog[12][n].sel= 0;
        DisLog[12][n].selold = 1;
        DisLog[12][n].lang = DataSave.Data_type.LANG;
    }
    DisLog[12][3].sel= 1;
    DisLog[12][3].selold = 2;
//之前设置值还原
    DisIF[0].index[DisIF[0].cind] =DataSave.Data_type.USBId;
    DisIF[2].index[DisIF[2].cind] = DataSave.Data_type.parameters.SW;
    DisIF[3].index[DisIF[3].cind] = DataSave.Data_type.AlarmOutF;
    DisIF[4].index[DisIF[4].cind] = DataSave.Data_type.PLSF[0];
    DisIF[5].index[DisIF[5].cind] = DataSave.Data_type.PLSF[1];
    DisIF[6].index[DisIF[6].cind] = DataSave.Data_type.SYNF;
    CurIF.COL = 0;
    CurIF.CAM = 0;
}

//中框数据的初始化
void Stu_MinIF_Init(void)
{
    uint8 m,n;
    for(m=0; m<IFCOL ; m++)
    {
        DisIF[m].lang = DataSave.Data_type.LANG;
        DisIF[m].flag = 1;
        DisIF[m].selold = 1;//以前全选中现在全没选中 刷新
        DisIF[m].sel= 0;
        if(m==0&&n==0)
        {
            DisIF[m].selold = 0;
        }
    }
    DisIF[CurIF.COL].sel= 1;
    DisIF[CurIF.COL].selold = 0;
    DisIF[CurIF.COL].flag = 0;
}

//整个设置界面的初始化
void Stu_IF_cycle(struct CUR *sle,struct CUR *sleold)
{
    uint8 m;
    for(m=0; m<IFCOL; m++)
    {
        DisIF[m].flag = 0;
        DisIF[m].selold = 0;
        DisIF[m].sel= 0;
    }
    if(sleold->COL!=sle->COL)
    {
        DisIF[sle->COL].selold = 0;//刷新
        DisIF[sle->COL].sel= 1;
        DisIF[sleold->COL].selold = 1;//刷新
        DisIF[sleold->COL].sel= 0;
    }
}

//初始化
void page_IF(void)
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
    Stu_HomeManu_Init(3);  //第4个按钮高亮
    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//此处调用主界面
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(2,5, "接口",FONTTYPE24);
        LCD_DisplayStringLineLJ(x,y, "设置项",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+4*Xdiv,y, "参数",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_DisplayStringLineLJ(3,5, "I/F",FONTTYPE24);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "Settings:");
        LCD_DisplayStringLine_EN_CH_LJ(x+4*Xdiv,y, "Parameter");
    }
    LCD_SetFont(&Font20);
    page_IF_cycle();
}

void LCD_DisplayStringLine_EN_CH_LJ_IF(struct RDispPara *sel,uint16_t x,uint16_t y,uint32_t SetTextColor)
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
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )IFEn[sel->cind].Ch[sel->index[sel->cind]]);
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
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )IFEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )IFEn[sel->cind].Ch[sel->index[sel->cind]]);
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
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )IFEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )IFEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}


//中文
void LCD_DisplayStringLineLJ_IF(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType,uint32_t SetTextColor)
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
        LCD_DisplayStringLineLJ(x,y,(uint8_t *)IFCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)IFCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)IFCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
            LCD_DisplayStringLineLJ(x,y,(uint8_t *)IFCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )IFEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}



void page_IF_cycle(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=90;
    uint16 Ydiv=90;
    uint16 i;

    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//此处调用主界面
    y = 60;
    Ydiv =28;
    Display_Help(x-2,y+Ydiv*11+20,helpS[CurIF.COL].Ch[DataSave.Data_type.LANG]);
    for(i=0; i<IFCOL; i++)
    {
        Disp_Box_Home(&DisIF[i],x+Xdiv*4+20,y+i*Ydiv+2,x-15+Xdiv*4+130,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
    }
    for(i=0; i<IFNUM; i++)
    {
        if(CHI == DataSave.Data_type.LANG )
        {
            Disp_Head(&DisIF[i],x-5,y+Ydiv*i,(uint8_t* )IFHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24,Font20);
        }
        else if(ENG == DataSave.Data_type.LANG)
        {
            Disp_Head(&DisIF[i],x-5,y+Ydiv*i,(uint8_t* )IFHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24,Font20);
        }
    }
    y = 64;
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
    for(i=0; i<IFCOL; i++)
    {
        if(i==1||i==0)
        {
            LCD_DisplayStringLine_EN_CH_LJ_IF(&DisIF[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
        else
        {
            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_IF(&DisIF[i],x+30+Xdiv*4,y+i*Ydiv,FONTTYPE16,LCD_COLOR_HUI );
            else
                LCD_DisplayStringLine_EN_CH_LJ_IF(&DisIF[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
}


//第一列下拉框的键值处理函数
static void ModifyHS(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)
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

        {
            DisIF[sel->COL].disp=1;
            DisIF[sel->COL].dispold=0;
            DisIF[sel->COL].index[DisIF[sel->COL].cind] = RD->index[RD->cind];

        }
        break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
    }
}


// wifi 对话框初始化
void WifiDiaFlash(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    for(m=0; m<DiaWifiCOL; m++)
        for(n=0; n<DiaWifiCAM; n++)
        {
            DisWifi[m][n].flag = 1;
            DisWifi[m][n].disp=1;
            DisWifi[m][n].dispold=0;
            DisWifi[m][n].lang =DataSave.Data_type.LANG;
        }
    DisWifi[0][0].cind =  CindStr; //密码
    DisWifi[0][1].cind =  CindStr ;//名称
    DisWifi[1][0].cind  = CindNum;//目标端口
    DisWifi[1][1].cind  = CindStr ;
    DisWifi[2][0].cind  = CindNum;
    DisWifi[2][1].cind  = CindNum;
    DisWifi[3][0].cind=16;
    DisWifi[3][0].index[DisWifi[3][0].cind] = 0;

    DisWifi[3][1].cind=16;
    DisWifi[3][1].index[DisWifi[3][1].cind] = 1;
    x1y1.Box.x1 =  30;//取点
    x1y1.Box.y1 =  60;//取?    x2y2.Box.x2=  630;//取点
    x2y2.Box.y2 = 360 ;//取点
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
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "云平台设置",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "热点密码",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+Ydiv, "热点名称",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "目标端口",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*4, "域名",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "本地端口",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*7, "本地编号",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Cloud Platform  Setting");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "WiFiP");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv, "Name");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "DestP");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*4, "Domain");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "LocolP");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*7, "ID");
    }
}
// wifi 对话框初始化
void WifiDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(m=0; m<DiaWifiCOL; m++)
        for(n=0; n<DiaWifiCAM; n++)
        {
            memset(DisWifi[m][m].Str,0,sizeof(DisWifi[m][m].Str));
            DisWifi[m][n].flag = 1;
            DisWifi[m][n].disp=1;
            DisWifi[m][n].dispold=0;
            DisWifi[m][n].lang =DataSave.Data_type.LANG;
        }
    DisWifi[0][0].cind =  CindStr; //密码
    DisWifi[0][1].cind =  CindStr ;//名称
    DisWifi[1][0].cind  = CindNum;//目标端口
    DisWifi[1][1].cind  = CindStr ;
    DisWifi[2][0].cind  = CindNum;
    DisWifi[2][1].cind  = CindNum;
    DisWifi[3][0].cind=16;
    DisWifi[3][0].index[DisWifi[3][0].cind] = 0;

    DisWifi[3][1].cind=16;
    DisWifi[3][1].index[DisWifi[3][1].cind] = 1;
    strncpy(DisWifi[0][0].Str,DataSave.Data_type.parameters.Wifi_PW,sizeof(DataSave.Data_type.parameters.Wifi_PW)); //密码
    strncpy(DisWifi[0][1].Str,DataSave.Data_type.parameters.Wifi_Name,sizeof(DataSave.Data_type.parameters.Wifi_Name));   //名称
    u16toa(DataSave.Data_type.parameters.Ser_Port,DisWifi[1][0].Str,10);
    strncpy(DisWifi[1][1].Str,DataSave.Data_type.parameters.YuMing,sizeof(DataSave.Data_type.parameters.YuMing));
    u16toa(DataSave.Data_type.parameters.Port,DisWifi[2][0].Str,10);
    u16toa(DataSave.Data_type.parameters.modbus_id,DisWifi[2][1].Str,10);
    x1y1.Box.x1 =  30;//取点
    x1y1.Box.y1 =  60;//取?    x2y2.Box.x2=  630;//取点
    x2y2.Box.y2 = 360 ;//取点
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
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "云平台设置",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "热点密码",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+Ydiv, "热点名称",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "目标端口",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*4, "域名",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "本地端口",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*7, "本地编号",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Cloud Platform  Setting");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "WiFiP");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv, "Name");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "DestP");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*4, "Domain");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "LocolP");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*3+15, x1y1.Box.y1+2+Ydiv*7, "ID");
    }
}

void SoftKeyDiaInit(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(m=0; m<DiaSoftKeyCOL; m++)
        for(n=0; n<DiaSoftKeyCAM; n++)
        {
            memset(DisSoftKey[m][m].Str,0,sizeof(DisSoftKey[m][m].Str));
            DisSoftKey[m][n].flag = 1;
            DisSoftKey[m][n].disp=0;
            DisSoftKey[m][n].dispold=0;
            DisSoftKey[m][n].lang =DataSave.Data_type.LANG;
        }
    DisSoftKey[0][0].disp=1;
    DisSoftKey[0][0].cind =  RD->cind  ;//名称

    strncpy(DisSoftKey[0][0].Str,RD->Str,sizeof(RD->Str)); //
    for(n=0; n<DiaSoftKeyCAM; n++)
    {
        DisSoftKey[1][n].disp=1;
        DisSoftKey[1][n].cind  = CindStr ;
        DisSoftKey[1][n].Str[0]  = 'A'+n ;
        DisSoftKey[2][n].disp=1;
        DisSoftKey[2][n].cind  = CindStr ;
        DisSoftKey[2][n].Str[0]  = 'A'+n +DiaSoftKeyCAM;
        DisSoftKey[3][n].disp=1;
        DisSoftKey[3][n].cind  = CindStr ;
        DisSoftKey[3][n].Str[0]  = 'a'+n ;
        DisSoftKey[4][n].disp=1;
        DisSoftKey[4][n].cind  = CindStr ;
        DisSoftKey[4][n].Str[0]  = 'a'+n +DiaSoftKeyCAM;
	DisSoftKey[5][n].disp=1;
        DisSoftKey[5][n].cind  = CindStr ;
        DisSoftKey[5][n].Str[0]  = FuHao[n];
    }


    DisSoftKey[6][0].disp=1;
    DisSoftKey[6][0].cind=16;
    DisSoftKey[6][0].index[DisSoftKey[6][0].cind] = 0;
    DisSoftKey[6][1].disp=1;
    DisSoftKey[6][1].cind=16;
    DisSoftKey[6][1].index[DisSoftKey[6][1].cind] = 1;

    x1y1.Box.x1 =  30;//取点
    x1y1.Box.y1 =  65;//取
    x2y2.Box.x2=  630;//取点
    x2y2.Box.y2 = 420 ;//取点

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
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "英文软件盘",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)ParaS[cur->COL][cur->CAM].Ch[DataSave.Data_type.LANG],FONTTYPE16);

    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Soft Key");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv,(uint8_t *)ParaS[cur->COL][cur->CAM].Ch[DataSave.Data_type.LANG]);
    }
}

//局部从新定义
static void Disp_Box_Home_Wifi(struct RDispStr *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color)
{
    char flag = sel->flag;
    if(sel->Box.flag ==0)
    {
        sel->Box.x1=x1;
        sel->Box.y1=y1;
        sel->Box.x2=x2;
        sel->Box.y2=y2;
        sel->Box.h=y2-y1;
        sel->Box.w=x2-x1;
        sel->Box.flag = 1;
    }
    if(sel->disp==1&&flag==1)
    {
        Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                if(sel->sel==1)
                {
                    Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
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
        else if(sel->disp==2&&sel->dispold==0)
        {
            Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
        }
    }
}


//中文
//局部从新定义
static void LCD_DisplayStringLineLJ_Home_Wifi(struct RDispStr* sel,uint16_t x,uint16_t y,uint8_t FontType)
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
        LCD_SetBackColor(LCD_COLOR_HUI);

        LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
            LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}

//大字体英文
//局部从新定义
void LCD_DisplayStringLineLJ_Home_SofeKey(struct RDispStr* sel,uint16_t x,uint16_t y,uint8_t FontType)
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
        LCD_SetBackColor(LCD_COLOR_HUI);

        LCD_DisplayStringLineLJ(x,y,(uint8_t *)sel->Str,FontType);
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
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)sel->Str,FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)sel->Str,FontType);
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
            LCD_DisplayStringLineLJ(x,y,(uint8_t *)sel->Str,FontType);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t *)sel->Str);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}


//局部从新定义
static void LCD_DisplayStringLine_EN_CH_LJ_Home_Wifi(struct RDispStr *sel,uint16_t x,uint16_t y)
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
        LCD_SetBackColor(LCD_COLOR_HUI);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
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
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
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
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}


//Wifi 下拉框的键值处理函数
void ModifyWifi(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
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
            DisWifi[cur->COL][cur->CAM].selold=1;
            DisWifi[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisWifi[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisWifi[cur->COL][cur->CAM].selold=0;
            DisWifi[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisWifi[cur->COL][cur->CAM].selold=1;
            DisWifi[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisWifi[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisWifi[cur->COL][cur->CAM].selold=0;
            DisWifi[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisWifi[cur->COL][cur->CAM].selold=1;
            DisWifi[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisWifi[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisWifi[cur->COL][cur->CAM].selold=0;
            DisWifi[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisWifi[cur->COL][cur->CAM].selold=1;
            DisWifi[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisWifi[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisWifi[cur->COL][cur->CAM].selold=0;
            DisWifi[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisWifi[cur->COL][cur->CAM].selold=1;
            DisWifi[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisWifi[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisWifi[cur->COL][cur->CAM].selold=0;
            DisWifi[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if((cur->COL<2)||(cur->CAM==0&&cur->COL==2))
            {
                SoftKeyDia(keytmp,&DisWifi[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisWifi[0][0].index[DisWifi[0][0].cind];
            }
            else if(cur->CAM==1&&cur->COL==3)
            {

                RD->index[RD->cind] = RDOld->index[RDOld->cind];
            }
            break;
	case KEY_HIDE1:
	    if(cur->CAM==1&&cur->COL==2)
            {
                SoftKeyDia(keytmp,&DisWifi[cur->COL][cur->CAM],cur);
            }
	break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }


}

void ModifySoftKey(uint8 keytmp,struct RDispStr* RD,struct RDispStr* RDOld,struct CUR* cur,char MaxCol,char MaxCam,uint8 MaxNum)
{
    switch(keytmp)
    {

        case KEY_UP://上
            DisSoftKey[cur->COL][cur->CAM].selold=1;
            DisSoftKey[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==1)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisSoftKey[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisSoftKey[cur->COL][cur->CAM].selold=0;
            DisSoftKey[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisSoftKey[cur->COL][cur->CAM].selold=1;
            DisSoftKey[cur->COL][cur->CAM].sel=0;
            do
            {
                ++cur->COL;
                if(cur->COL>MaxCol-1)
                    cur->COL =1 ;
                else if(cur->COL==MaxCol-1&&DisSoftKey[cur->COL][cur->CAM].disp!=1)
                {
                    cur->CAM = 0;
                }
            }
            while(DisSoftKey[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisSoftKey[cur->COL][cur->CAM].selold=0;
            DisSoftKey[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisSoftKey[cur->COL][cur->CAM].selold=1;
            DisSoftKey[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisSoftKey[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisSoftKey[cur->COL][cur->CAM].selold=0;
            DisSoftKey[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisSoftKey[cur->COL][cur->CAM].selold=1;
            DisSoftKey[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisSoftKey[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisSoftKey[cur->COL][cur->CAM].selold=0;
            DisSoftKey[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisSoftKey[cur->COL][cur->CAM].selold=1;
            DisSoftKey[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =1 ;
                else if(cur->COL==MaxCol-1&&DisSoftKey[cur->COL][cur->CAM].disp!=1)
                {
                    cur->CAM = 0;
                }
            }
            while(DisSoftKey[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisSoftKey[cur->COL][cur->CAM].selold=0;
            DisSoftKey[cur->COL][cur->CAM].sel=1;
            break;
        case  KEY_1:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"1\0");
            break;
        case  KEY_2:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"2\0");
            break;
        case  KEY_3:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"3\0");
            break;
        case  KEY_4:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"4\0");
            break;
        case  KEY_5:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"5\0");
            break;
        case  KEY_6:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"6\0");
            break;
        case  KEY_7:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"7\0");
            break;
        case  KEY_8:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"8\0");
            break;
        case  KEY_9:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"9\0");
            break;
        case  KEY_0:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,"0\0");
            break;
        case  KEY_BACK:
            memset(DisSoftKey[0][0].Str,0,sizeof(DisSoftKey[0][0].Str));
            break;
        case  KEY_DOT:
            if(strlen(DisSoftKey[0][0].Str)<MaxNum)
                strcat(DisSoftKey[0][0].Str,".\0");
            break;
        case KEY_ENTER:
            if(cur->COL<MaxCol-1&&cur->COL>0)
            {
                if(strlen(DisSoftKey[0][0].Str)<MaxNum&&RD->cind==CindStr )
                    strcat(DisSoftKey[0][0].Str,DisSoftKey[cur->COL][cur->CAM].Str);
            }
            else if(cur->CAM==0&&cur->COL==MaxCol-1)
            {
                memset(RD->Str,0,sizeof(RD->Str));
                strcpy(RD->Str, DisSoftKey[0][0].Str);
            }
            else if(cur->CAM==1&&cur->COL==MaxCol-1)
            {
                memset(RD->Str,0,sizeof(RD->Str));
                strcpy(RD->Str, RDOld->Str);
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }


}
int Myatoi(const char *str)
{

    int flg;
    int tmp;
    if(str == NULL)
    {
        return 0;
    }
    while(*str==' ')
    {
        str++;
    }
    flg=1;
    if(*str=='-')
    {
        flg=-1;
        str++;
    }
    else if(*str=='+')
    {
        str++;
    }
    tmp=0;
    while(isdigit(*str))
    {
        tmp=tmp*10+*str-'0';
        str++;
    }
    return flg*tmp;
}
uint16 Myatou16(const char *str)
{

    uint16 flg;
    uint16 tmp;
    if(str == NULL)
    {
        return 0;
    }
    while(*str==' ')
    {
        str++;
    }
    flg=1;
    if(*str=='-')
    {
        flg=-1;
        str++;
    }
    else if(*str=='+')
    {
        str++;
    }
    tmp=0;

    while(isdigit(*str))
    {
        tmp=tmp*10+*str-'0';
        str++;
    }
    return flg*tmp;
}
void LCD_DisplayNumLine_EN_CH_LJ_Home_Wifi(struct RDispStr *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType)
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
            LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
            //  sel->Num.fvalold = sel->Num.fval;
        }
    }
}
//wifi   对话框
static void wifiDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    OS_ERR  err;
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    int tempval=0;
    static struct RDispPara DisLogOld;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    static struct CUR Cur;
    Cur.CAM=0;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  30;//取点
    x1y1.Box.y1 =  60;//
    x2y2.Box.x2=  630;//取点
    x2y2.Box.y1 = 336;//取点
    x2y2.Box.y2 = 360;//取点
    WifiDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home_Wifi(&DisWifi[0][0],x1y1.Box.x1+Xdiv+10,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+160,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[0][1],x1y1.Box.x1+Xdiv*4+26,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv*4+210,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[1][0],x1y1.Box.x1+Xdiv+10,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv+160,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[1][1],x1y1.Box.x1+Xdiv*4+26,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv*4+210,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[2][0],x1y1.Box.x1+Xdiv+10,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv+160,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[2][1],x1y1.Box.x1+Xdiv*4+26,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv*4+210,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[3][0],x1y1.Box.x1+Xdiv+10,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+70,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisWifi[3][1],x1y1.Box.x1+Xdiv*4+26,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*4+140,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        for(m=0; m<DiaWifiCOL; m++)
            for(n=0; n<DiaWifiCAM; n++)
            {
                if(m<3)
                {

                    LCD_DisplayNumLine_EN_CH_LJ_Home_Wifi(&DisWifi[m][n],DisWifi[m][n].Box.x1+2,DisWifi[m][n].Box.y1+2,DisWifi[m][n].Str,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home_Wifi(&DisWifi[m][n],DisWifi[m][n].Box.x1+15,DisWifi[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home_Wifi(&DisWifi[m][n],DisWifi[m][n].Box.x1+15,DisWifi[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyWifi(keytmp,RD,&DisLogOld,&Cur,DiaWifiCOL,DiaWifiCAM);
        if((keytmp==KEY_ENTER&&Cur.COL<DiaWifiCOL-1)||(keytmp==KEY_HIDE1&&Cur.COL<DiaWifiCOL-1))
            WifiDiaFlash(keytmp,RD, cur) ;
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==DiaWifiCOL-1)&&keytmp!=KEY_ESC&&!(keytmp==KEY_HIDE1&&Cur.COL==DiaWifiCOL-1)); //DC的对话框第4行是确定和取消按钮
    OSSchedLock(&err);
    strncpy(DataSave.Data_type.parameters.Wifi_PW,DisWifi[0][0].Str,sizeof(DataSave.Data_type.parameters.Wifi_PW)); //密码
    strncpy(DataSave.Data_type.parameters.Wifi_Name,DisWifi[0][1].Str,sizeof(DataSave.Data_type.parameters.Wifi_Name));  //名称
    tempval=Myatoi(DisWifi[1][0].Str);  //目标端口
    if( tempval<=0xffff&&tempval>=0x0)
        DataSave.Data_type.parameters.Ser_Port=Myatou16(DisWifi[1][0].Str);  //目标端口
    strncpy(DataSave.Data_type.parameters.YuMing,DisWifi[1][1].Str,sizeof(DataSave.Data_type.parameters.YuMing));
    tempval=Myatoi(DisWifi[2][0].Str);  //本地端口
    if( tempval<=0xffff&&tempval>=0x0)
        DataSave.Data_type.parameters.Port=Myatou16(DisWifi[2][0].Str);//本地端口
    tempval=Myatoi(DisWifi[2][1].Str);  //本地id
    if( tempval<=0xffff&&tempval>=0x0)
        DataSave.Data_type.parameters.modbus_id=Myatou16(DisWifi[2][1].Str ); //本地id
    Stu_MinIF_Init();
    ModifyWifi(keytmp,RD,&DisLogOld,&Cur,DiaWifiCOL,DiaWifiCAM);
    page_IF();
    SaveAllPara();
    OSSchedUnlock(&err);
}


//软件键盘  对话框
static void SoftKeyDia(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
{
    OS_ERR  err;
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    uint16 HChar=40;
    static struct RDispStr DisLogOld= {0};
    struct RDispStr x1y1;
    struct RDispStr x2y2;
    static struct CUR Cur;
    Cur.CAM=0;
    Cur.COL=0;
    DisLogOld = *RD;
    strncpy(DisLogOld.Str,RD->Str,sizeof(RD->Str)) ;
    x1y1.Box.x1 =  30;//取点
    x1y1.Box.y1 =  65;//
    x2y2.Box.x2=  630;//取点
    x2y2.Box.y1 = 396;//取点
    x2y2.Box.y2 = 420;//取点
    SoftKeyDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home_Wifi(&DisSoftKey[0][0],x1y1.Box.x1+Xdiv+10,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+290,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        for(n=0; n<DiaSoftKeyCAM; n++)
        {
            Disp_Box_Home_Wifi(&DisSoftKey[1][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*3,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*3+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_Wifi(&DisSoftKey[2][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*5,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*5+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_Wifi(&DisSoftKey[3][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*7+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_Wifi(&DisSoftKey[4][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*9,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*9+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
	    Disp_Box_Home_Wifi(&DisSoftKey[5][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*11,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*11+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        }
        Disp_Box_Home_Wifi(&DisSoftKey[6][0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_Wifi(&DisSoftKey[6][1],x1y1.Box.x1+Xdiv*4+16,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*4+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);

        for(n=0; n<DiaSoftKeyCAM; n++)
            for(m=0; m<DiaSoftKeyCOL; m++)
            {
                if(m==0&&n==0)
                {

                    LCD_DisplayNumLine_EN_CH_LJ_Home_Wifi(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+2,DisSoftKey[m][n].Box.y1+2,DisSoftKey[m][n].Str,Font20);
                    DisSoftKey[m][n].sel=1;
                    DisSoftKey[m][n].selold=0;

                }
                else if(m==6)
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home_Wifi(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+15,DisSoftKey[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home_Wifi(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+15,DisSoftKey[m][n].Box.y1+2);
                }
                else if(m>=1&&m<=5)
                {

                    LCD_DisplayNumLine_EN_CH_LJ_Home_Wifi(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+6,DisSoftKey[m][n].Box.y1+8,DisSoftKey[m][n].Str,Font24);

                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==DiaSoftKeyCOL-1)&&keytmp!=KEY_ESC); //DC的对话框第4行是确定和取消按钮
    OSSchedLock(&err);
    Stu_MinIF_Init();
    ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    page_IF();
    OSSchedUnlock(&err);
}



void Disp_Box_IF(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
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

void NumFrameIF(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
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
    struct RDispPara DisInPut[USBIDCOL] = {0};   //选最大的
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
        if(m <(MaxCol>>1))
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
            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1-(MaxCol>>1));
            DisInPut[m].Box.x2 = RD->Box.x2 +box.w;
            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1-(MaxCol>>1));
            DisInPut[m].FontCH.x1 = RD->FontCH.x1+box.w;
            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1-(MaxCol>>1));
            DisInPut[m].FontEN.x1 = RD->FontEN.x1+box.w;
            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1-(MaxCol>>1));
        }
    }
    DisLogOld = *RD;
    //lj  为了提速18项下拉菜单按键响应变慢做了更改
    if(Ceng <(MaxCol>>2))
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
            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
            if(cur->COL==0)
            {
                LCD_DisplayStringLine_EN_CH_LJ_IF(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }

        }
        if(Ceng <(MaxCol>>1))
            Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        else
            Disp_Box_Zhi2( box.x1,box.y2,box.x2+box.w,box.y2+(box.h+8)*(Ceng/2),LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyHS(keytmp,RD,&DisLogOld,cur,MaxCol);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_MinIF_Init();
    ModifyHS(keytmp,RD,&DisLogOld,cur,MaxCol);
    page_IF();
}
static void ComBoxS(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol)
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



            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_IF(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16,LCD_COLOR_BLACK);
            else
                LCD_DisplayStringLine_EN_CH_LJ_IF(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);

        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyHS(keytmp,RD,&DisLogOld,cur,MaxCol);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_MinIF_Init();
    ModifyHS(keytmp,RD,&DisLogOld,cur,MaxCol);
    page_IF();
}
uint8 ModifyIF(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // 键值匹配函数，在while大循环中反复匹配
{
    OS_ERR err;
    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    switch(cur->OPN)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时200ms
            }
            break;
        case KEY_FUNC1://进入显示设置
            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
        case KEY_FUNC2://进入data 处理
            DataSave.Data_type.PageFlag =  FPageHDATA ;
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageFile ;
            break;
        case KEY_FUNC4:// 接口

            break;
        case KEY_FUNC5://进入其它
            DataSave.Data_type.PageFlag =  FPageOtur  ;
            break;
        case KEY_ESC:
            DataSave.Data_type.PageFlag =  PageFlagSetRe  ;
            break;
        case KEY_UP://上
            if(cur->COL>0)
                cur->COL--;
            else
                cur->COL=IFCOL-1;
            break;
        case KEY_DOWN:
            if(cur->COL<IFCOL-1) //最后一行是功能键
                cur->COL++;
            else
                cur->COL=0;
            break;
        case KEY_ACC:
            cur->COL+=2;
            if(cur->COL>IFCOL-1)
                cur->COL=1;
            break;
        case KEY_ENTER:
            if(cur->COL==0)  //第一行
            {
                ComBoxData(keytmp,&DisIF[cur->COL],cur,USBIDCOL);
                DataSave.Data_type.USBId = DisIF[cur->COL].index[DisIF[cur->COL].cind] ;
                SaveAllPara();
            }
            else if(cur->COL==1)
            {
                wifiDia(keytmp,&DisIF[cur->COL],cur);
            }
            else if(cur->COL==2)
            {
                ComBoxS(keytmp,&DisIF[cur->COL],cur,WifiSWCOL);
                if(DataSave.Data_type.parameters.SW!= DisIF[cur->COL].index[DisIF[cur->COL].cind])
                {
                    DataSave.Data_type.parameters.SW= DisIF[cur->COL].index[DisIF[cur->COL].cind];

                    if(DataSave.Data_type.parameters.SW==ON)
                    {
                        /* 创建WIFI任务 时间刷新1ms 一次 */
                        OSTaskCreate((OS_TCB     *)&WIFICheckTaskTCB,                             //任务控制块地址
                                     (CPU_CHAR   *)"WIFICheck Task",                             //任务名称
                                     (OS_TASK_PTR ) WIFICheck_task,                                //任务函数
                                     (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                     (OS_PRIO     ) WIFICheck_TASK_PRIO,                          //任务的优先级
                                     (CPU_STK    *)&WIFICheck_TASK_STK[0],                          //任务堆栈的基地址
                                     (CPU_STK_SIZE) WIFICheck_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                     (CPU_STK_SIZE) WIFICheck_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                     (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
                                     (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                     (void       *) 0,                                          //任务扩展（0表不扩展）
                                     (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                     (OS_ERR     *)&err);
                        /* 创建WIFI任务 时间2s 上传一次 */
                        OSTaskCreate((OS_TCB     *)&WIFITaskTCB,                             //任务控制块地址
                                     (CPU_CHAR   *)"WIFI Task",                             //任务名称
                                     (OS_TASK_PTR ) WIFI_task,                                //任务函数
                                     (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                                     (OS_PRIO     ) WIFI_TASK_PRIO,                          //任务的优先级
                                     (CPU_STK    *)&WIFI_TASK_STK[0],                          //任务堆栈的基地址
                                     (CPU_STK_SIZE) WIFI_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                                     (CPU_STK_SIZE) WIFI_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                                     (OS_MSG_QTY  ) 1u,                                         //任务可接收的最大消息数
                                     (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                                     (void       *) 0,                                          //任务扩展（0表不扩展）
                                     (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                                     (OS_ERR     *)&err);

                    }
                    else
                    {
                        macESP8266_RST_LOW_LEVEL();
                        OSTaskDel ( &WIFITaskTCB, & err );
                        OSTaskDel ( &WIFICheckTaskTCB, & err );
						macESP8266_RST_LOW_LEVEL();
                    }
                }
            }
            else if(cur->COL==3)
            {
                DisIF[cur->COL].index[DisIF[cur->COL].cind] =DataSave.Data_type.AlarmOutF;
                ComBoxS(keytmp,&DisIF[cur->COL],cur,2);
                DataSave.Data_type.AlarmOutF = DisIF[cur->COL].index[DisIF[cur->COL].cind] ;
                SaveAllPara();
            }
            else if(cur->COL==4)
            {
                DisIF[cur->COL].index[DisIF[cur->COL].cind] =   DataSave.Data_type.PLSF[0] ;
                ComBoxS(keytmp,&DisIF[cur->COL],cur,2);
                DataSave.Data_type.PLSF[0] = DisIF[cur->COL].index[DisIF[cur->COL].cind] ;
                SaveAllPara();
            }
            else if(cur->COL==5)
            {
                DisIF[cur->COL].index[DisIF[cur->COL].cind]  =  DataSave.Data_type.PLSF[1];
                ComBoxS(keytmp,&DisIF[cur->COL],cur,2);
                DataSave.Data_type.PLSF[1] = DisIF[cur->COL].index[DisIF[cur->COL].cind] ;
                SaveAllPara();
            }
            else if(cur->COL==6)
            {
                DisIF[cur->COL].index[DisIF[cur->COL].cind] =DataSave.Data_type.SYNF;
                ComBoxS(keytmp,&DisIF[cur->COL],cur,2);
                if(DataSave.Data_type.SYNF != DisIF[cur->COL].index[DisIF[cur->COL].cind])
                {
                    DataSave.Data_type.SYNF = DisIF[cur->COL].index[DisIF[cur->COL].cind] ;
                    OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
                    ChanSet.SYNC = DataSave.Data_type.SYNF+1;
                    SendSetSYNCTime(ChanSet);
                    ChanSet.SYNC = 0;
                    OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
                    SaveAllPara();
                    OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
                }

            }
            break;
    }
    Stu_IF_cycle(cur,curold);
    return keytmp;
}

//设置菜单任务
void DoIF_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //主页
    Stu_IF_Init();


    page_IF();
    while(1)
    {
        //DoIF任务
//        BluidICOTask();


        keytmp = KeyTaskCreate(NoNull_ReTurn);
     //   OSTaskDel(&ICOTaskTCB,&err);
        keytmp=ModifyIF(keytmp,&CurIF,&CurIFOld);     //处理键值

        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
            //删除自己任务

        }
        page_IF_cycle();

    }
}

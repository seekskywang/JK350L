/*
  * @file    Dispdata.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   数据展示的界面
 */
#include "include.h"
//任务控制块
OS_TCB DisplayDataTaskTCB;
//任务堆栈
CPU_STK DisplayData_TASK_STK[DisplayData_STK_SIZE];
struct RDispData  DDa[DCOL+2][ DCAM];
struct RDispData  DDaP[DCOL+2][ DCAMP];
uint8 CAM1=CAMMax;
uint8 CAM2=CAMMin;
float DSum[CHANNUM+2] = {0};
struct RDispData  DFrq[2];//+是放两个频率值
struct RDispData  DHmd[3];//3通道湿度
struct RDispData  DTemp;//内部温度值
uint8 PageFlagold=0;
uint8 PageFlagSetRe=FPageCh; //设置返回
const struct  UintARR ConstUint[VALCIND]=
{
    {"  \0"},//off 状态
    {"MV\0","kV\0"," V\0","mV\0","uV\0"},//电压显示单位
    {"$\0","&\0"},//摄氏度
    {"%\0"},//湿度
    {"mA\0"," A\0"},//电流
    {" g\0","kg\0"},//重力
    {"  \0"," k\0"," M\0"," G\0"},//脉冲

};
const struct  UintARR ConstUintF[VALCIND]=
{
    {"  \0"},//off 状态
    {"MV\0","kV\0"," V\0","mV\0","uV\0"},//电压显示单位
    {"℃\0","℉\0"},//摄氏度
    {"%\0"},//湿度
    {"mA\0"," A\0"},//电流
    {" g\0","kg\0"},//重力
    {"  \0"," k\0"," M\0"," G\0"},//脉冲

};
//10个通道的曲线颜色 10种 有些是自定义的   + 两个通道频率
const   uint32 colors[13]=
{
    LCD_COLOR_BLUE,
    LCD_COLOR_VERYDARKGREEN,
    LCD_COLOR_RED,
    LCD_COLOR_LITTLEDARKCYAN,
    LCD_COLOR_MAGENTA,
    LCD_COLOR_VERYDARKYELLOW,
    LCD_COLOR_ORANGE,
    LCD_COLOR_BROWN,
    LCD_COLOR_VERYDARKRED,
    LCD_COLOR_VERYDARKCYAN,
    LCD_COLOR_DARKRED,
    LCD_COLOR_DARKYELLOW,
    LCD_COLOR_WHITE
};
//帮助菜单显示
static const struct HelpStr helpS[1]=
{
    {"提示:显示运行周期内的参数值。\0"," Displag parameter values in operation cycle.\0"},
};
static struct PStr PS[DCAMP]=
{
    {"最大值\0"," Max\0"},
    {"最小值\0"," Min\0"},
    {"平均值\0"," Ave\0"},
    {"峰峰值\0"," P-P\0"},
    {"峰叼值\0"," PV \0"},
    {"谷叼值\0"," VV \0"},
};

void Stu_DisplayDataFrq_Init(void)
{
    uint8 m,n,i,j;
    for(m=0; m<2; m++)
    {
        DFrq[m].DP.col = m;
        DFrq[m].DP.cam = n;
        DFrq[m].DP.lang = DataSave.Data_type.LANG;

        DFrq[m].vcind = VFRQ ;//类型
        for(i=0; i<VALCIND; i++)
            for(j=0; j<UintCOL; j++)
            {
                strncpy(DFrq[m].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //电压类型 下面的单位
                DFrq[m].Uint[i].Col =0;   // 频率种类为0
            }
        DFrq[m].DP.Num.fval = 0.0;//先给个默认值1.1v 方便调试 温度时默认是1.1摄氏度
        DFrq[m].DP.Num.bit =1;//整数位1位
        DFrq[m].DP.Num.dot = 1;//小数点一位后
        DFrq[m].DP.Num.Decimalbit = 1;//小数位1位

        DFrq[m].DP.flag = 1;
        DFrq[m].DP.disp=2;
        DFrq[m].DP.dispold=0;

        DFrq[m].DP.Box.flag = 0;
        DFrq[m].DP.FontEN.flag = 0;
        //脉冲单位
        ChanData.frq[m].vcind =DFrq[m].vcind;
    }
}



void Stu_DisplayDataPCha_Init(void)
{
    uint8 m,n,i,j;

    for(m=0; m<DCOL; m++)
    {
        for(n=0; n<DCAMP; n++)
        {
            DDaP[m][n].DP.col = m;
            DDaP[m][n].DP.cam = n;
            DDaP[m][n].DP.lang = DataSave.Data_type.LANG;
            DDaP[m][n].vcind = DisLog[m+2][0].index[1];//前两行 第一行是切换按键 第二行是ALL 都不用 故要加2
            //   DDaP[m][n].vcind = VT;//类型
            for(i=0; i<VALCIND; i++)
                for(j=0; j<UintCOL; j++)
                {
                    strncpy(DDaP[m][n].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //电压类型 下面的单位
                    if(DDaP[m][n].vcind == VDC)   //电压类型默认选为单位是v 方便调试
                        DDaP[m][n].Uint[i].Col = 2;  //0 MV 1KV 2 V 3 mV 4 uV
                    else  if(DDaP[m][n].vcind == VI)   //电流
                        DDaP[m][n].Uint[i].Col = 0;  //0
                    else  if(DDaP[m][n].vcind == VT)   //温度单位类型
                        DDaP[m][n].Uint[i].Col = DataSave.Data_type.TUnit; //两种摄氏华氏
                    else  if(DDaP[m][n].vcind == VG)   //重力
                        DDaP[m][n].Uint[i].Col = 0;  //0
                    else
                        DDaP[m][n].Uint[i].Col =0;   // 其它种类只有1种 故为0
                }
            DDaP[m][n].DP.Num.fval = 0.0;//先给个默认值1.1v 方便调试 温度时默认是1.1摄氏度
            DDaP[m][n].DP.Num.bit =1;//整数位1位
            DDaP[m][n].DP.Num.dot = 1;//小数点一位后
            DDaP[m][n].DP.Num.Decimalbit = 1;//小数位1位
            if(DDaP[m][n].vcind==0)//off 状态下不显示
            {
                DDaP[m][n].DP.flag = 0;
                DDaP[m][n].DP.disp=0;
                DDaP[m][n].DP.dispold=0;
            }
            else//设有参数状态下 1 列是最大值  2列是最小值 都要初次赋值
            {
                DDaP[m][n].DP.flag = 1;
                DDaP[m][n].DP.disp=2;
                DDaP[m][n].DP.dispold=0;
            }
            DDaP[m][n].DP.Box.flag = 0;
            DDaP[m][n].DP.FontEN.flag = 0;
        }
    }
}

void Stu_DisplayDataHmd_Init(void)
{
	 uint8 m,n,i,j;
}


void Stu_DisplayDataCha_Init(void)
{
    uint8 m,n,i,j;
    for(m=0; m<DCOL+2; m++)
    {
        for(n=0; n<DCAM; n++)
        {
            DDa[m][n].DP.col = m;
            DDa[m][n].DP.cam = n;
            DDa[m][n].DP.lang = DataSave.Data_type.LANG;
            DDa[m][n].vcind = DisLog[m+2][0].index[1];//前两行 第一行是切换按键 第二行是ALL 都不用 故要加2
            //   DDa[m][n].vcind = VT;//类型
            for(i=0; i<VALCIND; i++)
                for(j=0; j<UintCOL; j++)
                {
                    strncpy(DDa[m][n].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //电压类型 下面的单位
                    if(DDa[m][n].vcind == VDC)   //电压类型默认选为单位是v 方便调试
                        DDa[m][n].Uint[i].Col = 2;  //0 MV 1KV 2 V 3 mV 4 uV
                    else  if(DDa[m][n].vcind == VI)   //电流
                        DDa[m][n].Uint[i].Col = 0;  //0
                    else  if(DDa[m][n].vcind == VT)   //温度单位类型
                        DDa[m][n].Uint[i].Col = DataSave.Data_type.TUnit; //两种摄氏华氏
                    else  if(DDa[m][n].vcind == VG)   //重力
                        DDa[m][n].Uint[i].Col = 0;  //0
                    else
                        DDa[m][n].Uint[i].Col =0;   // 其它种类只有1种 故为0
                }
            //单位类型初始化
            if(n==0)
            {
                ChanData.chd[m].vcind =DDa[m][n].vcind;  //10个通道的解析数据类型定义 1为电压
				ChanData.hmd[1].vcind =3;  //10个通道的解析数据类型定义 1为电压
				ChanData.hmd[3].vcind =3;  //10个通道的解析数据类型定义 1为电压
				ChanData.hmd[5].vcind =3;  //10个通道的解析数据类型定义 1为电压
                for(j=0; j<UintCOL; j++)
                    strncpy(ChanData.chd[m].UintOne.Ch[j],ConstUint[ChanData.chd[m].vcind ].Ch[j],sizeof(ConstUint[ChanData.chd[m].vcind ].Ch[j]));//10个通道的解析数据显示单位种类后可能调整
            }
            if( DDa[m][n].vcind ==VDC ) //电压类型
            {
                //如果当前是电压操作就更新

                DDa[m][n].Up[DDa[m][n].vcind]=DataSave.Data_type.DCMax[m] ;
                DDa[m][n].Up[DDa[m][n].vcind].dot=3;  //小数点在1位之后
                DDa[m][n].Up[DDa[m][n].vcind].bit=3;//整数位个数
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=3;//小数位个数

                //值不在区间范围内

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.DCMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=3;  //小数点在1位之后
                DDa[m][n].Low[DDa[m][n].vcind].bit=3;//整数位个数
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=3;//小数位个数


            }
            if( DDa[m][n].vcind ==VI ) //电流类型
            {
                //如果当前是电压操作就更新

                DDa[m][n].Up[DDa[m][n].vcind]=DataSave.Data_type.DIMax[m] ;
                DDa[m][n].Up[DDa[m][n].vcind].dot=6;  //小数点在1位之后
                DDa[m][n].Up[DDa[m][n].vcind].bit=6;//整数位个数
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=2;//小数位个数

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.DIMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=6;  //小数点在1位之后
                DDa[m][n].Low[DDa[m][n].vcind].bit=6;//整数位个数
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=2;//小数位个数


            }
            if( DDa[m][n].vcind ==VG ) //重力类型
            {
                //如果当前是电压操作就更新

                DDa[m][n].Up[DDa[m][n].vcind]=DataSave.Data_type.VGMax[m] ;
                DDa[m][n].Up[DDa[m][n].vcind].dot=6;  //小数点在1位之后
                DDa[m][n].Up[DDa[m][n].vcind].bit=6;//整数位个数
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=3;//小数位个数

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.VGMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=6;  //小数点在1位之后
                DDa[m][n].Low[DDa[m][n].vcind].bit=6;//整数位个数
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=3;//小数位个数


            }
            else if(DDa[m][n].vcind ==VT)//温度类型
            {

                //如果当前是温度操作就更新

                DDa[m][n].Up[DDa[m][n].vcind]= DataSave.Data_type.TempMax[m];
                DDa[m][n].Up[DDa[m][n].vcind].dot=4;  //小数点在4位之后
                DDa[m][n].Up[DDa[m][n].vcind].bit=4;//整数位个数
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=1;//小数位个数

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.TempMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=4;  //小数点在4位之后
                DDa[m][n].Low[DDa[m][n].vcind].bit=4;//整数位个数
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=1;//小数位个数

            }
            else if(DDa[m][n].vcind ==VR)//湿度类型
            {

                DDa[m][n].Up[DDa[m][n].vcind]= DataSave.Data_type.VRMax[m];
                DDa[m][n].Up[DDa[m][n].vcind].dot=3;  //小数点在3位之后
                DDa[m][n].Up[DDa[m][n].vcind].bit=3;//整数位个数
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=1;//小数位个数
                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.VRMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=3;  //小数点在3位之后
                DDa[m][n].Low[DDa[m][n].vcind].bit=3;//整数位个数
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=1;//小数位个数
            }

            DDa[m][n].DP.Num.fval = 0.0;//先给个默认值1.1v 方便调试 温度时默认是1.1摄氏度
            DDa[m][n].DP.Num.bit =1;//整数位1位
            DDa[m][n].DP.Num.dot = 1;//小数点一位后
            DDa[m][n].DP.Num.Decimalbit = 1;//小数位1位

            if(DDa[m][n].vcind==0)//off 状态下不显示
            {
                DDa[m][n].DP.flag = 0;
                DDa[m][n].DP.disp=0;
                DDa[m][n].DP.dispold=0;
            }
            else//设有参数状态下 1 列是最大值  2列是最小值 都要初次赋值
            {
                DDa[m][n].DP.flag = 1;
                DDa[m][n].DP.disp=2;
                DDa[m][n].DP.dispold=0;
            }
            DDa[m][n].DP.Box.flag = 0;
            DDa[m][n].DP.FontEN.flag = 0;


            if(n==3||n==4)
            {
                DDa[m][n].DP.flag = 0;
                DDa[m][n].DP.disp=0;
                DDa[m][n].DP.dispold=0;
            }

            DDa[9][n].Up[VT].dot=4;  //小数点在4位之后
            DDa[9][n].Up[VT].bit=4;//整数位个数
            DDa[9][n].Up[VT].Decimalbit=1;//小数位个数
            DDa[9][n].Low[VT].dot=4;  //小数点在4位之后
            DDa[9][n].Low[VT].bit=4;//整数位个数
            DDa[9][n].Low[VT].Decimalbit=1;//小数位个数
        }
    }
}


void DisplayDataTemp_Init(void)
{
    uint8 i,j;
    DTemp.DP.lang = DataSave.Data_type.LANG;
    DTemp.vcind = VT ;//类型
    for(i=0; i<VALCIND; i++)
        for(j=0; j<UintCOL; j++)
        {
            strncpy(DTemp.Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //电压类型 下面的单位
            DTemp.Uint[i].Col =0;   // 频率种类为0
        }
    DTemp.DP.Num.fval = 0.0;//先给个默认值1.1v 方便调试 温度时默认是1.1摄氏度
    DTemp.DP.Num.bit =1;//整数位1位
    DTemp.DP.Num.dot = 1;//小数点一位后
    DTemp.DP.Num.Decimalbit = 2;//小数位2位
    DTemp.DP.flag = 1;
    DTemp.DP.disp=2;
    DTemp.DP.dispold=0;
    DTemp.DP.Box.flag = 0;
    DTemp.DP.FontEN.flag = 0;
    //温度单位
    chTemp.vcind =DTemp.vcind;
}

void Stu_DisplayData_Init(void)
{
    Stu_DisplayDataCha_Init();
    Stu_DisplayDataFrq_Init();
    DisplayDataTemp_Init();

}
uint8 ModifyData(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // 键值匹配函数，在while大循环中反复匹配
{
    PageFlagold=DataSave.Data_type.PageFlag ;
    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    switch(cur->OPN)
    {
        case KEY_FUNC1://进入10ch
            DataSave.Data_type.PageFlag =  FPageCh ;
            break;
        case KEY_FUNC2://进入data
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageGraph  ;
            break;

        case KEY_FUNC4://进入3ch
            DataSave.Data_type.PageFlag =  FPageCh3 ;
            break;
        case KEY_FUNC5://进入文件
            if(FileRes ==0)
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
        case KEY_LEFT://
            if(++CAM1>=DCAMP)
                CAM1=0;
            page_FlashP();
            break;
        case KEY_RIGHT:
            if(++CAM2>=DCAMP)
                CAM2=0;
            page_FlashP();
            break;
    }
    return keytmp;
}

//设置数据展示任务
void DisplayData_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    static RTC_TimeTypeDef TimeStr;
    //主页
    Stu_DisplayData_Init(); //数据展示页面数据初始化
    page_DisplayData();//数据展示页面初始化
    while(1)
    {
        //任务
        keytmp = KeyTaskCreate(ALL_ReTurn);//20ms返回键值 不堵塞
        keytmp=ModifyData(keytmp,&CurDisp,&CurDispOld);     //处理键值
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
            //删除自己任务
        }
        page_DisplayData_cycle(); //数据展示页面处理
        if(TimeStr.Seconds!=RTC_TimeStr.Seconds)
        {
            ShowDateTime(RTC_DateStr,RTC_TimeStr,2,5,&Font16);   //显示时间
            TimeStr=RTC_TimeStr;
        }
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );
    }
}

void page_FlashP(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;

    if(CHI == DataSave.Data_type.LANG )
    {


        LCD_DisplayStringLineLJ(x+Xdiv,y, (uint8_t *)"数值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*3,y, (uint8_t *)PS[CAM1].Ch[DataSave.Data_type.LANG],FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*5,y, (uint8_t *)PS[CAM2].Ch[DataSave.Data_type.LANG],FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetFont(&Font16);

        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv,y, (uint8_t *)"Value");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*3,y, (uint8_t *)PS[CAM1].Ch[DataSave.Data_type.LANG]);
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*5,y, (uint8_t *)PS[CAM2].Ch[DataSave.Data_type.LANG]);
    }

}
void page_DisplayData(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    char str[5];
    uint32 BackColor,TextColor;
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(LCD_COLOR_MIDGRAY);
    LCD_DrawVLine(x+Xdiv*3-40, 58, 384);
    LCD_DrawVLine(x+Xdiv*5-40, 58, 384);
    LCD_SetTextColor(TextColor);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);
    LCD_DrawRect(0, 2, 639, 442);
//    LCD_DrawHLine(0, 380, 639);

    if(CHI == DataSave.Data_type.LANG )
    {
        COMmenuInit(DataSave.Data_type.LANG,(uint8_t *)"全通道",(uint8_t *)"数据显示",(uint8_t *)"曲线",(uint8_t *)"三通道",(uint8_t *)"文件",1);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLineLJ(x+Xdiv,y, (uint8_t *)"数值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*3,y, (uint8_t *)PS[CAM1].Ch[DataSave.Data_type.LANG],FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*5,y, (uint8_t *)PS[CAM2].Ch[DataSave.Data_type.LANG],FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        COMmenuInit(DataSave.Data_type.LANG,(uint8_t *)"10CH",(uint8_t *)"DATA",(uint8_t *)"GRAPH",(uint8_t *)"3CH",(uint8_t *)"FILE",1);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv,y, (uint8_t *)"Value");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*3,y, (uint8_t *)PS[CAM1].Ch[DataSave.Data_type.LANG]);
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*5,y, (uint8_t *)PS[CAM2].Ch[DataSave.Data_type.LANG]);
    }
//    y = 60;
//    Ydiv =28;
//    Display_Help(x-2,y+Ydiv*11+20,helpS[0].Ch[DataSave.Data_type.LANG]);
    LCD_SetFont(&Font20);
    y = 64;
    Ydiv =32;
    BackColor=LCD_GetBackColor();
    for(i=1; i<DCOL-1+2; i++)
    {

        sprintf(str,"%6s",DataSave.Data_type.ChName[i-1]);
        LCD_SetFont(&Font20);
        LCD_SetBackColor(colors[i-1]);
        LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*(i-1),(uint8_t* )str);
        TextColor=LCD_GetTextColor();
        LCD_SetTextColor(LCD_COLOR_MIDGRAY);
        LCD_DrawHLine(0, y+Ydiv*(i-1)+26, 639);
        LCD_SetTextColor(TextColor);
    }

    sprintf(str,"%6s",DataSave.Data_type.ChName[i-1]);
    LCD_SetFont(&Font20);
    LCD_SetBackColor(colors[i-1]);
    LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*(i-1),(uint8_t* )str);
    LCD_SetBackColor(BackColor);
    page_DisplayData_cycle();


}
//加单位
void AddUint2Char(const struct RDispData RD,char* ptr,uint8 setoff)
{
    uint8 i;
    uint8 j=setoff;
    if(RD.vcind==VDC)//此单位是电压单位电压单位有5种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VT)//此单位是温度单位2种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VI)//此单位是温度单位2种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VG)//此单位是重力单位2种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else  //此单位只有1种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[0],sizeof(RD.Uint[RD.vcind].Ch[0]));//湿度
    for(i=0; i<setoff; i++)
    {
        if(ptr[i]=='\0')
        {
            ptr[i]=' ';
            j=i;
        }
        if(i>j)
            ptr[i]=' ';
    }
}

void AddUint2CharUnit(const struct RDispData RD,char* ptr,uint8 setoff)
{
    uint8 i;
    uint8 j=setoff;
    if(RD.vcind==VDC)//此单位是电压单位电压单位有5种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[2],sizeof(RD.Uint[RD.vcind].Ch[2]));
    else if(RD.vcind==VT)//此单位是温度单位2种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VI)//此单位是温度单位2种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VG)//此单位是重力单位2种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else  //此单位只有1种
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[0],sizeof(RD.Uint[RD.vcind].Ch[0]));
    for(i=0; i<setoff; i++)
    {
        if(ptr[i]=='\0')
        {
            ptr[i]=' ';
            j=i;
        }
        if(i>j)
            ptr[i]=' ';
    }
}

void ComputerP(uint8 i)
{
    uint8 n;
    static struct SLData  DDaSL[DCOL+2][ DCAMP];
    for(n=0; n<DCAMP; n++)
    {
		if(i > 9)
		{
			if(DDaP[i+1][n].vcind != DDa[i+1][0].vcind)
			{
				DSum[i]=0;
			}
			if(DSum[i]==0)   //重新计数给初值
			{
				if(i == 10)
				{
					DDaSL[i+1][n].fStr= ChanData.hmd[3].fStr;
				}else if(i == 11){
					DDaSL[i+1][n].fStr= ChanData.hmd[5].fStr;
				}
			}
			DDaSL[i+1][n].vcind= DDa[i+1][0].vcind;
			DDaP[i+1][n].vcind = DDa[i+1][0].vcind;
		}else if(i == 9){
			if(DDaP[i][n].vcind != DDa[i][0].vcind)
			{
				DSum[i]=0;
			}
			if(DSum[i]==0)   //重新计数给初值
				DDaSL[i][n].fStr= ChanData.hmd[1].fStr;
			DDaSL[i][n].vcind= DDa[i][0].vcind;
			DDaP[i][n].vcind = DDa[i][0].vcind;
		}else{
			if(DDaP[i][n].vcind != DDa[i][0].vcind)
			{
				DSum[i]=0;
			}
			if(DSum[i]==0)   //重新计数给初值
				DDaSL[i][n].fStr= ChanData.chd[i].fStr;
			DDaSL[i][n].vcind= DDa[i][0].vcind;
			DDaP[i][n].vcind = DDa[i][0].vcind;
		}
    }
    

	if(i > 9)
	{
		if(i==10)
		{
			if(DDaSL[i+1][CAMMax].fStr.fval<=ChanData.hmd[3].fStr.fval)
				DDaSL[i+1][CAMMax] = ChanData.hmd[3];
			if(DDaSL[i+1][CAMMin].fStr.fval>=ChanData.hmd[3].fStr.fval)
				DDaSL[i+1][CAMMin] = ChanData.hmd[3];
		}else if(i==11){
			if(DDaSL[i+1][CAMMax].fStr.fval<=ChanData.hmd[5].fStr.fval)
				DDaSL[i+1][CAMMax] = ChanData.hmd[5];
			if(DDaSL[i+1][CAMMin].fStr.fval>=ChanData.hmd[5].fStr.fval)
				DDaSL[i+1][CAMMin] = ChanData.hmd[5];
		}
	}else if(i==9){
		if(DDaSL[i][CAMMax].fStr.fval<=ChanData.hmd[1].fStr.fval)
			DDaSL[i][CAMMax] = ChanData.hmd[1];
		if(DDaSL[i][CAMMin].fStr.fval>=ChanData.hmd[1].fStr.fval)
			DDaSL[i][CAMMin] = ChanData.hmd[1];
	}else{
		if(DDaSL[i][CAMMax].fStr.fval<=ChanData.chd[i].fStr.fval)
			DDaSL[i][CAMMax] = ChanData.chd[i];
		if(DDaSL[i][CAMMin].fStr.fval>=ChanData.chd[i].fStr.fval)
			DDaSL[i][CAMMin] = ChanData.chd[i];
	}
	if(i > 9)
	{
		//平均值
		DDaSL[i+1][CAMAV].fStr.fval =  DDaSL[i+1][CAMAV].fStr.fval-DDaSL[i+1][CAMAV].fStr.fval/DSum[i]+DDaSL[i+1][0].fStr.fval/DSum[i];
		//峰峰值
		DDaSL[i+1][CAMPP].fStr.fval = DDaSL[i+1][CAMMax].fStr.fval -DDaSL[i+1][CAMMin].fStr.fval;
		//峰值
		DDaSL[i+1][CAMP].fStr.fval = DDaSL[i+1][CAMMax].fStr.fval -DDaSL[i+1][CAMAV].fStr.fval;
		//谷值
		DDaSL[i+1][CAMG].fStr.fval = -DDaSL[i][CAMMin].fStr.fval+DDaSL[i+1][CAMAV].fStr.fval;
	}else{
		//平均值
		DDaSL[i][CAMAV].fStr.fval =  DDaSL[i][CAMAV].fStr.fval-DDaSL[i][CAMAV].fStr.fval/DSum[i]+DDaSL[i][0].fStr.fval/DSum[i];
		//峰峰值
		DDaSL[i][CAMPP].fStr.fval = DDaSL[i][CAMMax].fStr.fval -DDaSL[i][CAMMin].fStr.fval;
		//峰值
		DDaSL[i][CAMP].fStr.fval = DDaSL[i][CAMMax].fStr.fval -DDaSL[i][CAMAV].fStr.fval;
		//谷值
		DDaSL[i][CAMG].fStr.fval = -DDaSL[i][CAMMin].fStr.fval+DDaSL[i][CAMAV].fStr.fval;
	}
    for(n=0; n<DCAMP; n++)
    {
		if(i > 9)
		{
			fStr2DfStr(&DDaSL[i+1][n]);
			DDaP[i+1][n].DP.Num = DDaSL[i+1][n].DfStr;//接收到数据更新
			DDaP[i+1][n].Uint[DDaP[i+1][n].vcind].Col = DDaSL[i+1][n].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
		}else{
			fStr2DfStr(&DDaSL[i][n]);
			DDaP[i][n].DP.Num = DDaSL[i][n].DfStr;//接收到数据更新
			DDaP[i][n].Uint[DDaP[i][n].vcind].Col = DDaSL[i][n].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
		}
    }
    /*
      DSum+=1;
      if(DDaP[i][CAMMax].DP.Num.fval<DDa[i][0].DP.Num.fval)
          DDaP[i][CAMMax].DP.Num = DDa[i][0].DP.Num;
      if(DDaP[i][CAMMin].DP.Num.fval>DDa[i][0].DP.Num.fval)
          DDaP[i][CAMMin].DP.Num = DDa[i][0].DP.Num;
      //平均值
      DDaP[i][CAMAV].DP.Num.fval =  DDaP[i][CAMAV].DP.Num.fval-DDaP[i][CAMAV].DP.Num.fval/DSum+DDa[i][0].DP.Num.fval/DSum;
      //峰峰值
      DDaP[i][CAMPP].DP.Num.fval = DDaP[i][CAMMax].DP.Num.fval -DDaP[i][CAMMin].DP.Num.fval;
      //峰值
      DDaP[i][CAMP].DP.Num.fval = DDaP[i][CAMMax].DP.Num.fval -DDaP[i][CAMAV].DP.Num.fval;
      //谷值
      DDaP[i][CAMG].DP.Num.fval = -DDaP[i][CAMMin].DP.Num.fval +DDaP[i][CAMAV].DP.Num.fval;
    */
}


void page_DisplayData_cycle(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=90;
    uint16 Ydiv=90;
    uint16 i;
    char ptr[11];
    COMmenu(&DDa[10][0].DP,&DDa[10][1].DP,&DDa[10][2].DP,&DDa[10][3].DP,&DDa[10][4].DP);
    Ydiv =32;
    y = 64;
    // DSum+=1;  //计数
    for(i=0; i<DCOL+2-1; i++)
    {
        
		if(i>9)
		{
			DDa[i+1][0].DP.dispold = 0;//刷新要更新标志
			DDa[i+1][1].DP.dispold = 0;
			DDa[i+1][2].DP.dispold = 0;
			DDa[i+1][0].DP.Box.x1 =x-5+Xdiv;
			DDa[i+1][0].DP.Box.y1 =y+i*Ydiv;
		}else{
			DDa[i][0].DP.dispold = 0;//刷新要更新标志
			DDa[i][1].DP.dispold = 0;
			DDa[i][2].DP.dispold = 0;
			DDa[i][0].DP.Box.x1 =x-5+Xdiv;
			DDa[i][0].DP.Box.y1 =y+i*Ydiv;
		}
        memset(ptr,0,sizeof(ptr));
        
		if(i>9)
		{
			 if(i==7&&DDa[i+1][0].DP.Num.fval<1)//数据显示
				i=i;
			 NumFloat2Char(&DDa[i+1][0],&DDa[i+1][0].DP.Num,ptr);  //接收数据浮点型转换为字符串
			 AddUint2Char(DDa[i+1][0],ptr,9);    // 字符串尾巴上加单位
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+1][0].DP,DDa[i+1][0].DP.Box.x1,DDa[i+1][0].DP.Box.y1+2,ptr,Font20);
		}else{
			 if(i==7&&DDa[i][0].DP.Num.fval<1)//数据显示
				i=i;
			 NumFloat2Char(&DDa[i][0],&DDa[i][0].DP.Num,ptr);  //接收数据浮点型转换为字符串
			 AddUint2Char(DDa[i][0],ptr,9);    // 字符串尾巴上加单位
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][0].DP,DDa[i][0].DP.Box.x1,DDa[i][0].DP.Box.y1+2,ptr,Font20);
		}
       

        ComputerP(i);
        //最大值的操作
//        DDa[i][1].DP.Num = DDaP[i][CAM1].DP.Num;
//        DDa[i][1].DP.Num.fval = DDaP[i][CAM1].DP.Num.fval;
//        DDa[i][1].vcind = DDaP[i][CAM1].vcind;
//        DDa[i][1].Uint[DDa[i][1].vcind].Col = DDaP[i][CAM1].Uint[DDaP[i][CAM1].vcind].Col;
		if(i>9)
		{
			DDa[i+1][1].DP.Num = DDaP[i+1][CAM1].DP.Num;
			DDa[i+1][1].DP.Num.fval = DDaP[i+1][CAM1].DP.Num.fval;
			DDa[i+1][1].vcind = DDaP[i+1][CAM1].vcind;
			DDa[i+1][1].Uint[DDa[i+1][1].vcind].Col = DDaP[i+1][CAM1].Uint[DDaP[i+1][CAM1].vcind].Col;
			DDa[i+1][1].DP.Box.x1 =x-5+Xdiv*3;
			DDa[i+1][1].DP.Box.y1 =y+i*Ydiv;
		}else{
			DDa[i][1].DP.Num = DDaP[i][CAM1].DP.Num;
			DDa[i][1].DP.Num.fval = DDaP[i][CAM1].DP.Num.fval;
			DDa[i][1].vcind = DDaP[i][CAM1].vcind;
			DDa[i][1].Uint[DDa[i][1].vcind].Col = DDaP[i][CAM1].Uint[DDaP[i][CAM1].vcind].Col;
			DDa[i][1].DP.Box.x1 =x-5+Xdiv*3;
			DDa[i][1].DP.Box.y1 =y+i*Ydiv;
		}
        memset(ptr,0,sizeof(ptr));
        //  if(DDa[i][1].DP.Num.fval<DDa[i][0].DP.Num.fval)
        //    DDa[i][1].DP.Num = DDa[i][0].DP.Num;



//        if(DDa[i][1].DP.flag==1)
//        {
//            DDa[i][1].DP.Num = DDa[i][0].DP.Num;
//            DDa[i][1].DP.flag=0;
//        }
//        NumFloat2Char(&DDa[i][1],&DDa[i][1].DP.Num,ptr);
//        AddUint2Char(DDa[i][1],ptr,9);
        if(i>9)
		{
			if(DDa[i+1][1].DP.flag==1)
			{
				DDa[i+1][1].DP.Num = DDa[i+1][0].DP.Num;
				DDa[i+1][1].DP.flag=0;
			}
			NumFloat2Char(&DDa[i+1][1],&DDa[i+1][1].DP.Num,ptr);
			AddUint2Char(DDa[i+1][1],ptr,9);
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+1][1].DP,DDa[i+1][1].DP.Box.x1,DDa[i+1][1].DP.Box.y1+2,ptr,Font20);
		}else{
			if(DDa[i][1].DP.flag==1)
			{
				DDa[i][1].DP.Num = DDa[i][0].DP.Num;
				DDa[i][1].DP.flag=0;
			}
			NumFloat2Char(&DDa[i][1],&DDa[i][1].DP.Num,ptr);
			AddUint2Char(DDa[i][1],ptr,9);
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][1].DP,DDa[i][1].DP.Box.x1,DDa[i][1].DP.Box.y1+2,ptr,Font20);
		}
        //最小值的操作

//        DDa[i][2].DP.Num.fval = DDaP[i][CAM2].DP.Num.fval;
//        DDa[i][2].vcind = DDaP[i][CAM1].vcind;
//        DDa[i][2].Uint[DDa[i][2].vcind].Col = DDaP[i][CAM2].Uint[DDaP[i][CAM2].vcind].Col;
		if(i>9)
		{
			DDa[i+1][2].DP.Num = DDaP[i+1][CAM2].DP.Num;
			DDa[i+1][2].vcind = DDaP[i+1][CAM2].vcind;
			DDa[i+1][2].Uint[DDa[i+1][2].vcind].Col = DDaP[i+1][CAM2].Uint[DDaP[i+1][CAM2].vcind].Col;
			DDa[i+1][2].DP.Box.x1 =x-5+Xdiv*5;
			DDa[i+1][2].DP.Box.y1 =y+i*Ydiv;
		}else{
			DDa[i][2].DP.Num = DDaP[i][CAM2].DP.Num;
			DDa[i][2].vcind = DDaP[i][CAM2].vcind;
			DDa[i][2].Uint[DDa[i][2].vcind].Col = DDaP[i][CAM2].Uint[DDaP[i][CAM2].vcind].Col;
			DDa[i][2].DP.Box.x1 =x-5+Xdiv*5;
			DDa[i][2].DP.Box.y1 =y+i*Ydiv;
		}
        
        memset(ptr,0,sizeof(ptr));
        //   if(DDa[i][2].DP.Num.fval>DDa[i][0].DP.Num.fval)
        //       DDa[i][2].DP.Num = DDa[i][0].DP.Num;
//        if(DDa[i][2].DP.flag==1)
//        {
//            DDa[i][2].DP.Num = DDa[i][0].DP.Num;
//            DDa[i][2].DP.flag=0;
//        }
//        NumFloat2Char(&DDa[i][2],&DDa[i][2].DP.Num,ptr);
//        AddUint2Char(DDa[i][2],ptr,9);
       
		if(i>9)
		{
			if(DDa[i+1][2].DP.flag==1)
			{
				DDa[i+1][2].DP.Num = DDa[i+1][0].DP.Num;
				DDa[i+1][2].DP.flag=0;
			}
			NumFloat2Char(&DDa[i+1][2],&DDa[i+1][2].DP.Num,ptr);
			AddUint2Char(DDa[i+1][2],ptr,9);
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+1][2].DP,DDa[i+1][2].DP.Box.x1,DDa[i+1][2].DP.Box.y1+2,ptr,Font20);
		}else{
			if(DDa[i][2].DP.flag==1)
			{
				DDa[i][2].DP.Num = DDa[i][0].DP.Num;
				DDa[i][2].DP.flag=0;
			}
			NumFloat2Char(&DDa[i][2],&DDa[i][2].DP.Num,ptr);
			AddUint2Char(DDa[i][2],ptr,9);
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][2].DP,DDa[i][2].DP.Box.x1,DDa[i][2].DP.Box.y1+2,ptr,Font20);
		}
    }
    //不需要加单位
//    for(i=0; i<DCOL-1; i++)
//    {
//        LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][0].DP,DDa[i][0].DP.Box.x1+100,DDa[i][0].DP.Box.y1+2,DDa[i][0].Uint[DDa[i][0].vcind].Ch[DDa[i][0].Uint[DDa[i][0].vcind].Col],Font20);
//        LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][1].DP,DDa[i][1].DP.Box.x1+100,DDa[i][1].DP.Box.y1+2,DDa[i][1].Uint[DDa[i][1].vcind].Ch[DDa[i][1].Uint[DDa[i][1].vcind].Col],Font20);
//        LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][2].DP,DDa[i][2].DP.Box.x1+100,DDa[i][2].DP.Box.y1+2,DDa[i][2].Uint[DDa[i][2].vcind].Ch[DDa[i][1].Uint[DDa[i][1].vcind].Col],Font20);
//    }

}

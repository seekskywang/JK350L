/*
  * @file    record.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   回看画图界面
 */
#include  "include.h"
extern FIL fnew;
struct CUR CurRecord = {0};//右侧框框体选择的游标
struct CUR CurRecordOld = {0};//右侧框框体选择的游标上次的位置
struct CUR CurChanR; //仅仅用在1 至 10通道
struct CUR SampleTimeCurR= {0};  //SampleTime横向选择的游标
struct CUR ZoneCurR= {0};//横向选择的游标
Point LinesR[DCOL-1][PointsNum] = {0};//通道曲线的矩阵
float BYTE3toFloat(uint8 *buf);
uint16 DDaRDPDxyXold=0;
uint16 LinenumR=0;
uint8 ShowDateTimeFlagR = 0;
uint8 RightframeR = 0; //左侧指针栏刷新标志 1刷新
RTC_TimeTypeDef RTC_TimeStrStartR;//时间
RTC_DateTypeDef RTC_DateStrStartR;//日期
struct ChanalData ChanDataR;
// 总计数
uint32_t RCNTALL = 0;

uint32_t RCNTALLold = 100;
uint32_t   RCNTCUR = 0;
uint32_t   RCNTCURflag = 0;
uint32_t   RCNTCURold = 0;
//段计数
uint16 RCNT = 0;
static struct RDispData  DDaR[DCOL][ DCAMR];
static struct RDispData  DFrqR[2];//+是放两个频率值
void page_Record_F(const struct CUR CURV);






void Stu_DisplayDataFrq_InitR(void)
{
    uint8 m,n,i,j;
    for(m=0; m<2; m++)
    {
        DFrqR[m].DP.col = m;
        DFrqR[m].DP.cam = n;
        DFrqR[m].DP.lang = DataSave.Data_type.LANG;
        DFrqR[m].vcind = VFRQ ;//类型
        for(i=0; i<VALCIND; i++)
            for(j=0; j<UintCOL; j++)
            {
                strncpy(DFrqR[m].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //
                DFrqR[m].Uint[i].Col =0;   // 其它种类只有1种 故为0
            }

//        DFrqR[m].Up[DFrqR[m].vcind].fval=10000;
//        DFrqR[m].Up[DFrqR[m].vcind].dot=3;  //小数点在3位之后
//        DFrqR[m].Up[DFrqR[m].vcind].bit=3;//整数位个数
//        DFrqR[m].Up[DFrqR[m].vcind].Decimalbit=1;//小数位个数
//        DFrqR[m].Low[DFrqR[m].vcind].fval=0;
//        DFrqR[m].Low[DFrqR[m].vcind].dot=1;  //小数点在3位之后
//        DFrqR[m].Low[DFrqR[m].vcind].bit=1;//整数位个数
//        DFrqR[m].Low[DFrqR[m].vcind].Decimalbit=1;//小数位个数
        DFrqR[m].DP.Num.fval = 0.0;//先给个默认值1.1v 方便调试 温度时默认是1.1摄氏度
        DFrqR[m].DP.Num.bit =1;//整数位1位
        DFrqR[m].DP.Num.dot = 1;//小数点一位后
        DFrqR[m].DP.Num.Decimalbit = 1;//小数位1位
      
        DFrqR[m].DP.flag = 1;
        DFrqR[m].DP.disp=2;
       
        DFrqR[m].DP.Box.flag = 0;
        DFrqR[m].DP.FontEN.flag = 0;
    }
}






void Stu_DisplayDataCha_InitR(void)
{
    uint8 m,n,i,j;
    for(m=0; m<DCOL; m++)
    {
        for(n=0; n<DCAMR; n++)
        {
            DDaR[m][n].DP.col = m;
            DDaR[m][n].DP.cam = n;
            DDaR[m][n].DP.lang = DataSave.Data_type.LANG;

            for(i=0; i<VALCIND; i++)
                for(j=0; j<UintCOL; j++)
                {
                    strncpy(DDaR[m][n].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //电压类型 下面的单位
                    if(DDaR[m][n].vcind == VDC)   //电压类型默认选为单位是v 方便调试
                        DDaR[m][n].Uint[i].Col = 2;  //0 MV 1KV 2 V 3 mV 4 uV
                    else  if(DDaR[m][n].vcind == VT)   //温度单位类型
                        DDaR[m][n].Uint[i].Col = DataSave.Data_type.TUnit; //两种摄氏华氏
                    else  if(DDaR[m][n].vcind == VI)   //电流
                        DDaR[m][n].Uint[i].Col = 0; //
					else  if(DDaR[m][n].vcind == VG)   //重力
                        DDaR[m][n].Uint[i].Col = 0; //
                    else
                        DDaR[m][n].Uint[i].Col =0;   // 其它种类只有1种 故为0
                }
            //单位类型初始化
            if( DDaR[m][n].vcind ==VDC ) //电压类型
            {
                DDaR[m][n].Up[DDaR[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaR[m][n].Up[DDaR[m][n].vcind].dot=1;  //小数点在1位之后
                DDaR[m][n].Up[DDaR[m][n].vcind].bit=1;//整数位个数
                DDaR[m][n].Up[DDaR[m][n].vcind].Decimalbit=1;//小数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaR[m][n].Low[DDaR[m][n].vcind].dot=1;  //小数点在1位之后
                DDaR[m][n].Low[DDaR[m][n].vcind].bit=1;//整数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].Decimalbit=1;//小数位个数

            }
            else if(DDaR[m][n].vcind ==VT)//温度类型
            {
                DDaR[m][n].Up[DDaR[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaR[m][n].Up[DDaR[m][n].vcind].dot=4;  //小数点在4位之后
                DDaR[m][n].Up[DDaR[m][n].vcind].bit=4;//整数位个数
                DDaR[m][n].Up[DDaR[m][n].vcind].Decimalbit=1;//小数位个数

                DDaR[m][n].Low[DDaR[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaR[m][n].Low[DDaR[m][n].vcind].dot=4;  //小数点在4位之后
                DDaR[m][n].Low[DDaR[m][n].vcind].bit=4;//整数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].Decimalbit=1;//小数位个数

            }
            else if(DDaR[m][n].vcind ==VR)//湿度类型
            {
                DDaR[m][n].Up[DDaR[m][n].vcind].fval=100;
                DDaR[m][n].Up[DDaR[m][n].vcind].dot=3;  //小数点在3位之后
                DDaR[m][n].Up[DDaR[m][n].vcind].bit=3;//整数位个数
                DDaR[m][n].Up[DDaR[m][n].vcind].Decimalbit=1;//小数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].fval=0;
                DDaR[m][n].Low[DDaR[m][n].vcind].dot=1;  //小数点在3位之后
                DDaR[m][n].Low[DDaR[m][n].vcind].bit=1;//整数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].Decimalbit=1;//小数位个数
            }
            else if( DDaR[m][n].vcind ==VI ) //电流类型
            {
                DDaR[m][n].Up[DDaR[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaR[m][n].Up[DDaR[m][n].vcind].dot=1;  //小数点在1位之后
                DDaR[m][n].Up[DDaR[m][n].vcind].bit=1;//整数位个数
                DDaR[m][n].Up[DDaR[m][n].vcind].Decimalbit=1;//小数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaR[m][n].Low[DDaR[m][n].vcind].dot=1;  //小数点在1位之后
                DDaR[m][n].Low[DDaR[m][n].vcind].bit=1;//整数位个数
                DDaR[m][n].Low[DDaR[m][n].vcind].Decimalbit=1;//小数位个数

            }

            DDaR[m][n].DP.Num.fval = 0.0;//先给个默认值1.1v 方便调试 温度时默认是1.1摄氏度
            DDaR[m][n].DP.Num.bit =1;//整数位1位
            DDaR[m][n].DP.Num.dot = 1;//小数点一位后
            DDaR[m][n].DP.Num.Decimalbit = 1;//小数位1位

            if(DDaR[m][n].vcind==0)//off 状态下不显示
            {
                DDaR[m][n].DP.flag = 0;
                DDaR[m][n].DP.disp=0;
                DDaR[m][n].DP.dispold=0;
            }
            else//设有参数状态下 1 列是最大值  2列是最小值 都要初次赋值
            {
                DDaR[m][n].DP.flag = 1;
                DDaR[m][n].DP.disp=2;
                DDaR[m][n].DP.dispold=0;
            }
            DDaR[m][n].DP.Box.flag = 0;
            DDaR[m][n].DP.FontEN.flag = 0;
            if(n==3||n==4)
            {
                DDaR[m][n].DP.flag = 0;
                DDaR[m][n].DP.disp=0;
                DDaR[m][n].DP.dispold=0;
            }
            DDaR[9][n].Up[VT].dot=4;  //小数点在4位之后
            DDaR[9][n].Up[VT].bit=4;//整数位个数
            DDaR[9][n].Up[VT].Decimalbit=1;//小数位个数

            DDaR[9][n].Low[VT].dot=4;  //小数点在4位之后
            DDaR[9][n].Low[VT].bit=4;//整数位个数
            DDaR[9][n].Low[VT].Decimalbit=1;//小数位个数
        }
    }
}
void Stu_DisplayData_InitR(void)
{
    Stu_DisplayDataCha_InitR();
    Stu_DisplayDataFrq_InitR();
}

//显示时间
void ShowDateTimeR(uint32 starttick,uint32 endtick,uint32 samp,uint16_t Xpos, uint16_t Ypos,sFONT *fonts)
{
    char str[50];
    RTC_TimeTypeDef RTC_TimeStrTemp;
    RTC_DateTypeDef RTC_DateStrTemp;

    uint32 s= samp+(endtick-starttick)/1000;
    stamp2Time(s,&RTC_DateStrTemp,&RTC_TimeStrTemp) ;
    //显示时间
    sprintf(str,"20%0.2d-%0.2d-%0.2d-%0.2d:%0.2d:%0.2d.%0.3dms",
            RTC_DateStrTemp.Year,
            RTC_DateStrTemp.Month,
            RTC_DateStrTemp.Date,
            RTC_TimeStrTemp.Hours,
            RTC_TimeStrTemp.Minutes,
            RTC_TimeStrTemp.Seconds,
            (endtick-starttick)%1000);
    LCD_DisplayStringLine_EN_CH_LJ_WithFont(Xpos-100, Ypos, (uint8_t* )str,fonts);
}


void Record_work(void)
{
    OS_ERR err;
    uint16 i,j,r = 0;
    uint8 derect =0;//方向标志 0 是前进 1是后退

//发现总的打点位置发生了变化
    if(RCNTALLold != RCNTALL||RCNTCURflag == 1)
    {
        if(RCNTALLold == (RCNTALL+RCNT))  //倒退了重新刷有些设置刷新了或者游标移动
        {
            if(RCNTCUR==RCNTCURold&&RCNTCURflag == 0)
            {
                for(i=0; i<DCOL-1; i++)
                {
                    LCD_DrawPolygonLJ(LinesR[i],RCNT,LCD_COLOR_BLACK);//清曲线
                }
            }
            derect = 1;  //后退
        }
        else  //往前进  往前进一般是刷到下一页
        {
            //取第一个点进行曲线页面的初始化刷下一页也要做
            RereadSD(RCNTALL);
            page_Record_init(CurRecord);
            DrawRecord_Init();
            Stu_DisplayData_InitR();
            derect = 0;  //前进
        }
        OSSchedLock(&err);
        for(j=0; j<DCOL-1; j++)
        {
            //清理曲线
            memset(LinesR[j],0,PointsNum);
        }
        //连续读一页曲线0
        for(i=0; i<PointsPix; i++)
        {

            if(RereadSD(RCNTALL+i)==0)
                break;
            r=DrawRecord(CurChanR,i+1,PointsPix);
            if(r==1)   //到龙头了
            {
                break;//
            }
        }
        RCNT = i;//曲线结束点 结束点就是400
		RCNTALL+=RCNT;
        if(RCNTCUR!=RCNTCURold||RCNTCURflag == 1)    //竖线游标走动
        {
            lcd_line(DDaRDPDxyXold,GIXEndY,DDaRDPDxyXold,GIXStartY,1,LCD_COLOR_BLACK );
            lcd_Grids(5-2+50,GIXEndY,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
        }
        if(RCNT>0)   //只有页面切换到画图页面时才画
        {
            for(j=0; j<DCOL-1; j++)
            {
                LCD_DrawPolygonLJ(LinesR[j],RCNT,colors[j]);
            }
        }
        if(derect == 0)   //新刷了一页
        {
            RCNTCUR =RCNTALL-RCNT;  //只有在新刷一页画面是才会跳到第一个点
        }
        RereadSD(RCNTCUR);    //
        for(i=0; i<CHANNUM; i++)
        {
            if(DDaR[i][0].DP.Dxy.X!=0)
            {
                lcd_line(DDaR[i][0].DP.Dxy.X,GIXEndY,DDaR[i][0].DP.Dxy.X,GIXStartY,1,LCD_COLOR_WHITE);
                DDaRDPDxyXold= DDaR[i][0].DP.Dxy.X;
                if((DsdR.Data.DataType&0xf)==(CmdR_DATA&0xf))
                    ShowDateTimeR(DsdR.Data.starttick,DsdR.Data.CHD[i].SDData.endtick,DsdR.Data.Timestamp,150,2,&Font16);   //显示时间
                else if((DsdR.Data.DataType&0xf)==(CmdR_FDATA&0xf))
                    ShowDateTimeR(DsdR.Data.starttick,DsdR.Data.CHD[i].SDData.endtick,DsdR.Data.Timestamp,150,2,&Font16);   //显示时间
                break;
            }
        }
        OSSchedUnlock(&err);
        if(RCNT>=0&&derect == 0)//第一个点
        {
            if((DsdR.Data.DataType&0xf)==(CmdR_DATA&0xf))
                ShowTimeLine(RTC_DateStrStartR,RTC_TimeStrStartR,10, 403, 100,&Font16,SampletimeMs[DsdR.Data.SampleTimeCurCAM]/10);// 显示画图下方的时间
            else if((DsdR.Data.DataType&0xf)==(CmdR_FDATA&0xf))
                ShowTimeLineFM(RTC_DateStrStartR,RTC_TimeStrStartR,10, 403, 100,&Font16);// 显示画图下方的时间
        }
    }
    if(RCNTCUR!=RCNTCURold||RCNTCURflag == 1)
        page_Record_cycle(&CurRecord);
    else
    {

        page_Record(CurRecord);// 全刷新
        if(RCNT>0)   //只有页面切换到画图页面时才画
        {
            for(j=0; j<DCOL-1; j++)
            {
                LCD_DrawPolygonLJ(LinesR[j],RCNT,colors[j]);
            }
        }
        RightframeR = 1;
    }
    RCNTALLold = RCNTALL;
    RCNTCURold = RCNTCUR;
    RCNTCURflag = 0; // 强行刷新清0
}

//设置数据展示任务
void Record_Page(void)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //主页
    DataSave.Data_type.PageFlag = FPageRecord;
    PageFlagold =  DataSave.Data_type.PageFlag;
    CurRecord.COL=11;
    CurRecord.CAM=0;
    CurChanR.COL =11;
    CurRecordOld = CurRecord;
    ZoneCurR.CAM  = DataSave.Data_type.ZoneCAM;
    RightframeR=1;
    RCNTCUR = 0;
    RCNTCURold = 0;
    RCNTALL = 0;
    RCNTALLold = 100;
    page_Record_init(CurRecord);
    while(1)
    {
        //画曲 线
        Record_work();
        //任务
        keytmp = KeyTaskCreate(NoNull_ReTurn);//键盘扫描15毫秒
        keytmp=ModifyRecord(keytmp,&CurRecord,&CurRecordOld,&CurChanR,1,12) ;       //处理键值
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err); //删除自己任务
        }
    }
}

//左边曲线静态控制的初始化62 62+20*16
void DrawRecord_Init(void)
{
    uint16 x=5;
    uint16 y=20;
//    uint16 xW=PointsPix;
    LCD_FillRectLJ(x-1, y-1, PointsPix+50,PointsPiy2+1,LCD_COLOR_BLACK);
    lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
  //  LCD_DrawRectLJ(x-2+50,y-20,  xW, 1,LCD_COLOR_WHITE);
}

void DrawRecord_FlashZone(void)
{
    uint16 x=5;
    uint16 y=20;
//    uint16 xW=PointsPix;
    LCD_FillRectLJ(x-2+50,y-1, PointsPix,PointsPiy2+1,LCD_COLOR_BLACK);
    lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
   // LCD_DrawRectLJ(x-2+50,y-20,  xW, 1,LCD_COLOR_WHITE);
}

//局部清除 只清除曲线部分
void DrawRecord_Flash(const struct CUR CURV,uint16_t PointCount)
{
    uint16 x=5;
    uint16 y=20;
    uint8 i;
    uint16 Yoff=0;
    uint16 Yoffup=0;
    char ptr[10];
    static uint8 COLold = 0;
    uint32 BackColor;
    if(DataSave.Data_type.PageFlag != FPageRecord)//不在画图页面直接返回
    {
        return;
    }
    for(i=0; i<DCOL-1; i++)
    {
        LCD_DrawPolygonLJ(LinesR[i],PointCount-1,LCD_COLOR_BLACK);
    }
    lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
    if(CURV.COL<11)//只有在画完一帧时才更新
        COLold =CURV.COL-1;//lj 0610改
    Yoff =  ((COLold)%ZoneData[ ZoneCurR.CAM])*(PointsPiy/ ZoneData[ ZoneCurR.CAM]);
    Yoffup =  (PointsPiy/ ZoneData[ ZoneCurR.CAM]);
    memset(ptr,0,sizeof(ptr));
    NumFloat2Char(&DDaR[COLold][0],&DDaR[COLold][0].Up[DDaR[COLold][0].vcind],ptr);//显示上限
    AddUint2Char(DDaR[COLold][0],ptr,7);
    BackColor=LCD_GetBackColor();
    LCD_SetBackColor(LCD_COLOR_BLACK);
    if(DataSave.Data_type.PageFlag == FPageRecord)
    {
        LCD_DisplayStringLine_EN_CH_LJ_WithFC(45+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[COLold]);
    }
    LCD_SetBackColor(BackColor);
    memset(ptr,0,sizeof(ptr));
    NumFloat2Char(&DDaR[COLold][0],&DDaR[COLold][0].Low[DDaR[COLold][0].vcind],ptr);//显示下限
    AddUint2Char(DDaR[COLold][0],ptr,7);
    BackColor=LCD_GetBackColor();
    LCD_SetBackColor(LCD_COLOR_BLACK);
    if(DataSave.Data_type.PageFlag == FPageRecord)
    {
        LCD_DisplayStringLine_EN_CH_LJ_WithFC(45+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[COLold]);
    }
    LCD_SetBackColor(BackColor);
}


void page_Record(const struct CUR CURV)
{
    uint16 x=5;
    uint16 y=20;
    uint16 xR=10;
    uint16 yR=39;
    uint16 HdivR=90;
    uint16 HdivR_Sel=90;
    uint16 Yoff=0;
    uint16 Yoffup=0;
    uint16 i;
    char str[10];
    uint32 BackColor,TextColor;
    char ptr[10];
    static  uint8 COLOld;
    static  uint8 ZoneCAMold;

    page_CH(CURV);
    xR=460;
    yR=4+20+24;
    HdivR=26;
    HdivR_Sel=HdivR*2;
    LCD_FillRectLJ(xR-1, yR-1, 639-460, 26*13,LCD_COLOR_BLACK); //清除右边框
    BackColor=LCD_GetBackColor();
    for(i=1; i<13; i++)
    {
        if( CURV.COL==i)
        {
            LCD_DrawRect(xR,  yR, 639-460, 20);
            LCD_DrawRect(xR,  yR, 639-460, HdivR_Sel);
        }
        else
        {
            LCD_DrawRect(xR,  yR, 639-460, HdivR);
        }
        if((COLOld!=CURV.COL&&CURV.COL>0&&CURV.COL<=10)||ZoneCAMold!=ZoneCurR.CAM)
        {
            memset(ptr,0,sizeof(ptr));
            strncpy(ptr,"         \0",sizeof(ptr)) ;
            Yoff =  ((COLOld-1)%ZoneData[ ZoneCAMold ])*(PointsPiy/ ZoneData[ ZoneCAMold ]);
            Yoffup =  (PointsPix/ ZoneData[ ZoneCAMold ]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[COLOld-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[COLOld-1]);
            lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
            if(CURV.COL>0&&CURV.COL<=10)
                COLOld = CURV.COL;
            ZoneCAMold = ZoneCurR.CAM;
           // if(PointsPiy-17-Yoff-Yoffup<0)
           //     LCD_DrawRectLJ(x-2+50, y-20, PointsPix, 1,LCD_COLOR_WHITE);
        }
        DDaR[i-1][0].DP.dispold = 0;      //数据 在 .DP.dispold == 0  且disp== 1才会刷新
//显示右边框体
        if(i>0&&i<10&&CURV.COL!=i)
        {
            sprintf(str,"%d  ",i);
            BackColor=LCD_GetBackColor();
            if(ChanDataR.chalarm[i-1]==0)
            {
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
            }
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i -1][0].DP,xR+1+50,yR+5,ptr,Font16);
        }
        else if(i==10&&CURV.COL!=i)
        {
            sprintf(str,"%d ",i);
            if(ChanDataR.chalarm[i-1]==0)
            {
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
            }
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
        }
        else  if(i>0&&i<10&&CURV.COL==i)
        {
            TextColor=LCD_GetTextColor();
            LCD_SetTextColor(LCD_COLOR_BLACK);

            LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //标题框填色
            LCD_SetTextColor( TextColor);
            sprintf(str,"%d    CH%d ",i,i);
            BackColor=LCD_GetBackColor();
            if(ChanDataR.chalarm[i-1]==0)
            {
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
            }
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);

            Yoff =  ((i-1)%ZoneData[ ZoneCurR.CAM])*(PointsPiy/ ZoneData[ ZoneCurR.CAM]);
            Yoffup =  (PointsPiy/ ZoneData[ ZoneCurR.CAM]);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].Up[DDaR[i-1][0].vcind],ptr);//显示上限
            AddUint2Char(DDaR[i-1][0],ptr,7);
            LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[i-1]);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].Low[DDaR[i-1][0].vcind],ptr);//显示下限
            AddUint2Char(DDaR[i-1][0],ptr,7);
            LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[i-1]);
        }
        else if(i==10&&CURV.COL==i)
        {
            TextColor=LCD_GetTextColor();
            LCD_SetTextColor(LCD_COLOR_BLACK);
            LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //标题框填色
            LCD_SetTextColor( TextColor);
            sprintf(str,"%d   CH%d",i,i);
            BackColor=LCD_GetBackColor();
            if(ChanDataR.chalarm[i-1]==0)
            {
                LCD_SetBackColor(colors[i-1]);
                LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
                LCD_SetBackColor(BackColor);
            }
            Yoff =  ((i-1)%ZoneData[ ZoneCurR.CAM])*(PointsPiy/ ZoneData[ ZoneCurR.CAM]);
            Yoffup =  (PointsPiy/ ZoneData[ ZoneCurR.CAM]);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].Up[DDaR[i-1][0].vcind],ptr);//显示上限
            AddUint2Char(DDaR[i-1][0],ptr,7);
            LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[i-1]);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].Low[DDaR[i-1][0].vcind],ptr);//显示下限
            AddUint2Char(DDaR[i-1][0],ptr,7);
            LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[i-1]);
        }
        else if(i==11&&CURV.COL!=i) //SAMPLE
        {
            sprintf(str,"%d  ",i);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
			if((DsdR.Data.DataType&0xf)==(CmdR_FDATA&0xf))//快速模式
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )("5ms"),&Font20);
			else
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCurR.CAM]),&Font20);
        }

        else  if(i==11&&CURV.COL==i)
        {

            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//标题框填色
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
			if((DsdR.Data.DataType&0xf)==(CmdR_FDATA&0xf))//快速模式
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )("5ms"),&Font24);
			else
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCurR.CAM]),&Font24);
        }
        else if(i==12&&CURV.COL!=i)//ZONE
        {
            sprintf(str,"%d  ",i);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCurR.CAM]),&Font20);
        }
        else  if(i==12&&CURV.COL==i)
        {

            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//标题框填色
            TextColor=LCD_GetTextColor();
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCurR.CAM]),&Font24);
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



uint8 DrawRecord(const struct CUR CURV,uint16_t PointCount,uint16_t MaxCount)
{
    uint32 TextColor,BackColor;
    uint16 x=5;
    uint16 y=19;
    uint16 i;
    char str[5];
    uint8 r;
    uint8 flag = 0;
    uint8 lastnum= 0;
    static uint8 col;
    r = 0;
    for(i=DCOL-2; i>0; i--)
    {
        if(DDaR[i][0].vcind == VT||DDaR[i][0].vcind  == VDC||DDaR[i][0].vcind  == VI||DDaR[i][0].vcind  == VG||DDaR[i][0].vcind  == VR)
        {
            lastnum = i;
            break;
        }
    }
    for(i=0; i<DCOL-1; i++)
    {
        if(PointCount>0)
        {
            if(DDaR[i][0].DP.dispold==1)
                continue;

            LinesR[i][PointCount-1] =DDaR[i][0].DP.Dxy;

            if(DDaR[i][0].DP.Dxy.X>GIXStartX+MaxCount)    //超出图框
            {
                //DDaR[i][0].DP.Dxy.X = GIXStartX+400; //会产生误差
                r =1;
                continue;
            }

            if(!(DDaR[i][0].vcind == VT||DDaR[i][0].vcind  == VDC||DDaR[i][0].vcind  == VI||DDaR[i][0].vcind  == VG||DDaR[i][0].vcind  == VR)) //off状态下跳出
            {
                continue;
            }


            if(PointCount==1||RightframeR==1)
            {

                if(i<DCOL-2)
                    sprintf(str," %d ",i+1);
                else
                    sprintf(str," %d",i+1);
                TextColor=LCD_GetTextColor();
                BackColor=LCD_GetBackColor();
                if(CURV.COL-1>10)  //越出
                {
                }
                else
                {
                    col = CURV.COL-1;
                }
                if(i== col )
                    LCD_SetColors(colors[i],LCD_COLOR_WHITE);
                else
                    LCD_SetColors(LCD_COLOR_WHITE,colors[i]);
                if(flag==0)
                {

                  //  LCD_FillRectLJ(x-4-1, y,52,324,LCD_COLOR_BLACK);   // 擦完马上画

		  LCD_FillRectLJ(x-4-1, y,52,PointsPiy2,LCD_COLOR_BLACK);   
                    flag = 1;
                }

                LCD_DrawHLineLJ(8+30, LinesR[i][0].Y, 12,colors[i]);
		if((LinesR[i][0].Y+8)>GIXStartY)
			 LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  LinesR[i][0].Y-15, (uint8_t* )str,&Font16);
		else if((LinesR[i][0].Y-8)<GIXEndY)
			LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  LinesR[i][0].Y, (uint8_t* )str,&Font16);
		else					
               		 LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  LinesR[i][0].Y-8, (uint8_t* )str,&Font16);
                LCD_SetColors(TextColor,BackColor);
                if(i== lastnum)
                    RightframeR = 0;  //画完把标志清0
            }
        }
    }
    return r;
}

//画图的键值处理
uint8 ModifyRecord(uint8 keytmp,struct CUR* cur,struct CUR* curold,struct CUR* CurChanR,char MinCol,char MaxCol)                 // 键值匹配函数，在while大循环中反复匹配
{
    char MaxCam = 0;
    uint32 starttickold=0;
    uint16 i;
    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    if(cur->COL==11)
    {
        MaxCam = SampleMaxCam;
        cur->CAM = SampleTimeCurR.CAM;
    }
    else if(cur->COL==12)
    {
        MaxCam = ZoneMaxCam;
        cur->CAM = ZoneCurR.CAM;
    }
    switch(cur->OPN)
    {
        case KEY_FUNC1://进入10ch
            if(RCNTALL <= (RCNT)) //说明已经到了首页了
            {

            }
            else   //不是首页
            {
				if(RCNTALL%400==0)
					RereadSD(--RCNTALL);//第399个点
				else
					RereadSD(RCNTALL);//第399个点
                for(i=0; i<PointsPix; i++,RCNTALL--)
                {
                    starttickold=DsdR.Data.starttick;
                    if(RereadSD(RCNTALL-1)==0)
                        break;
                    if(starttickold-DsdR.Data.starttick>=50)   //到龙头了
                    {
                        break;//
                    }
                }
                if(RCNTALL < PointsPix) //又到了首页
                {
                    RCNTALL  = 0;
                    RCNT=0;
                }
                else
                {
					if(RCNTALL%400==0)
						RereadSD(--RCNTALL);//第399个点
				    else
						RereadSD(RCNTALL);//第399个点
                    for(i=0; i<PointsPix; i++,RCNTALL--)
                    {
                        starttickold=DsdR.Data.starttick;
                        if(RereadSD(RCNTALL-1)==0)
                            break;
                        if(starttickold-DsdR.Data.starttick>=50)   //到龙头了
                        {

                            break;//
                        }
                    }
                 //   RCNTALL++;
                    RCNT=0;
                }
            }
            break;
        case KEY_FUNC2://进入data
            RCNTCUR-=20;
            if(RCNTCUR>RCNTALL-1||RCNTCUR<=RCNTALL-(RCNT))
            {
                RCNTCUR=RCNTALL-(RCNT);
                RCNTCURflag = 1; //强型刷新
            }
            RCNTALL -= (RCNT);
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageFile  ;
            break;
        case KEY_FUNC4://
            RCNTCUR+=20;
            if(RCNTCUR>=RCNTALL-1)
            {
                RCNTCUR=RCNTALL-1;
                RCNTCURflag = 1; //强型刷新
            }
            RCNTALL -= (RCNT);
            break;
        case KEY_FUNC5://进入3ch
            if((RCNTALL)*sizeof(DsdR.uD)<=f_size(&fnew))
            {
               // RCNTALL++;
                RCNT=0;
                RCNTALLold=RCNTALL-1;  //从新给一个数
            }

            break;
        case KEY_0:
            if(ScreenFlag==OFF)
                ScreenFlag=ON;
            break;
        case KEY_UP://上
            do
            {
                if(cur->COL>MinCol)
                    cur->COL--;
                else
                    cur->COL=MaxCol;
            }
            while(DDaR[cur->COL-1][0].DP.disp!=2&&cur->COL-1<MaxCol-2);
            if(cur->COL==11)
            {
                cur->CAM = SampleTimeCurR.CAM;
            }
            else if(cur->COL==12)
            {
                cur->CAM = ZoneCurR.CAM;
            }
            else if(cur->COL<=10&&cur->COL>0)    //10个通道 1到10
            {
                CurChanR->COL = cur->COL;

            }
            break;
        case KEY_DOWN:
            do
            {
                if(cur->COL<MaxCol) //最后一行
                    cur->COL++;
                else
                    cur->COL=MinCol;
            }
            while(DDaR[cur->COL-1][0].DP.disp!=2&&cur->COL-1<MaxCol-2);

            if(cur->COL==11)
            {
                cur->CAM = SampleTimeCurR.CAM;
            }
            else if(cur->COL==12)
            {
                cur->CAM = ZoneCurR.CAM;
            }
            else if(cur->COL<=10&&cur->COL>0)
            {
                CurChanR->COL = cur->COL;

            }
            break;
        case KEY_ACC:
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol)
                    cur->COL=MinCol;
            }
            while(DDaR[cur->COL-1][0].DP.disp!=2&&cur->COL-1<MaxCol-2);
            if(cur->COL==11)
            {
                cur->CAM = SampleTimeCurR.CAM;
            }
            else if(cur->COL==12)
            {
                cur->CAM = ZoneCurR.CAM;
            }
            else if(cur->COL<=10&&cur->COL>0)
            {
                CurChanR->COL = cur->COL;
            }
            break;
        case KEY_LEFT://
            RCNTCUR--;
            if(RCNTCUR>RCNTALL-1||RCNTCUR<=RCNTALL-(RCNT))
            {

                RCNTCUR=RCNTALL-(RCNT);
                RCNTCURflag = 1; //强型刷新
            }

            RCNTALL -= (RCNT);
            break;
        case KEY_RIGHT:
            RCNTCUR++;
            if(RCNTCUR>=RCNTALL-1)
            {

                RCNTCUR=RCNTALL-1;
                RCNTCURflag = 1; //强型刷新
            }


            RCNTALL -= (RCNT);
            break;
        case KEY_ENTER:
            if(MaxCam==0)
                break;
            if(cur->CAM<MaxCam-1) //最后一行是功能键
                cur->CAM++;
            else
                cur->CAM=0;
            if(cur->COL==12)
            {
                ZoneCurR.CAM = cur->CAM ;
                RCNTALL -= (RCNT);
            }
            break;
        case KEY_ESC:
            DataSave.Data_type.PageFlag =  FPageFile  ;
            break;
    }
    return keytmp;
}

void JiSuanXYR(struct SLData *p,uint8 i,uint32 starttick,uint32 endtick,uint32 NumMs,uint32 ZoneD)
{


    if(DsdR.Data.CHD[i].SDData.vcind  == VT||DsdR.Data.CHD[i].SDData.vcind == VDC||DsdR.Data.CHD[i].SDData.vcind == VI||DsdR.Data.CHD[i].SDData.vcind == VG||DsdR.Data.CHD[i].SDData.vcind == VR)  //前两行是箭头和ALL
    {
        p->Dxy.X=GIXStartX+(endtick-starttick)/NumMs;
        p->Dxy.Y=GIXStartY-(i%ZoneD)*(PointsPiy/ ZoneD)-((p->DfStr.fval-DDaR[i][0].Low[DDa[i][0].vcind].fval)*(PointsPiy/ ZoneD)/(DDaR[i][0].Up[DDa[i][0].vcind].fval-DDaR[i][0].Low[DDa[i][0].vcind].fval));
    }

}

void UpDataDDaR(struct ChanalData ChanDataR,uint32 i)
{
    // 可能产生电压切换
    DDaR[i][0].Up[DDaR[i][0].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[i].SDData.MaxB);
    DDaR[i][0].Low[DDaR[i][0].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[i].SDData.MinB);
    if((DsdR.Data.DataType&0xf)==(CmdR_DATA&0xf))
        JiSuanXYR(&ChanDataR.chd[i],i,DsdR.Data.starttick,DsdR.Data.CHD[i].SDData.endtick,SampletimeMs[DsdR.Data.SampleTimeCurCAM], ZoneData[ZoneCurR.CAM]);
    else if((DsdR.Data.DataType&0xf)==(CmdR_FDATA&0xf))
        JiSuanXYR(&ChanDataR.chd[i],i,DsdR.Data.starttick,DsdR.Data.CHD[i].SDData.endtick,SampletimeMs[DsdR.Data.SampleTimeCurCAM]/10, ZoneData[ZoneCurR.CAM]);
    DDaR[i][0].DP.Dxy.X=0;
    if(ChanDataR.chd[i].Dxy.X<GIXStartX)
        ChanDataR.chd[i].Dxy.X=GIXStartX-1;
    else if(ChanDataR.chd[i].Dxy.X>GIXStartX+PointsPix)
        ChanDataR.chd[i].Dxy.X=GIXStartX+PointsPix+1;
    DDaR[i][0].DP.Dxy.X=ChanDataR.chd[i].Dxy.X;
    if(ChanDataR.chd[i].Dxy.Y<GIXStartY-PointsPiy)
        ChanDataR.chd[i].Dxy.Y=GIXStartY-PointsPiy;
    else if(ChanDataR.chd[i].Dxy.Y>GIXStartY)
        ChanDataR.chd[i].Dxy.Y=GIXStartY;
    DDaR[i][0].DP.Dxy.Y=ChanDataR.chd[i].Dxy.Y;	
    if(ChanDataR.chd[i].fStr.fval>=DDaR[i][0].Up[DDaR[i][0].vcind].fval)
        ChanDataR.chd[i].fStr=DDaR[i][0].Up[DDaR[i][0].vcind];
    else if(ChanDataR.chd[i].DfStr.fval<=DDaR[i][0].Low[DDaR[i][0].vcind].fval)
        ChanDataR.chd[i].fStr=DDaR[i][0].Low[DDaR[i][0].vcind];
	fStr2DfStr(&ChanDataR.chd[i]);//接收后解析的数据原始数据转换为数据显示
    DDaR[i][0].DP.Num = ChanDataR.chd[i].DfStr;//接收到数据更新
    DDaR[i][0].Uint[DDaR[i][0].vcind].Col = ChanDataR.chd[i].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
    DDaR[i][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新  必须要加 不加不刷新
}


void SerialRemoteHandleR(uint8 *buf,struct ChanalData *pCmd)
{
    uint16 currCharNum;
    uint8 i;
    currCharNum=0;
    for(i=0; i<2; i++)
    {
        memcpy(pCmd->frq[i].byte,&buf[currCharNum],sizeof(pCmd->frq[i].byte));
        pCmd->frq[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
        SwitchChanalData (&pCmd->frq[i].fStr,0);  //小数点后8位调试
		pCmd->frq[i].DfStr = pCmd->frq[i].fStr;
        //fStr2DfStr(&pCmd->frq[i]);//接收后解析的数据原始数据转换为数据显示
        currCharNum+=3;
    }
    for(i=0; i<10; i++)
    {
        memcpy(pCmd->chd[i].byte,&buf[currCharNum],sizeof(pCmd->chd[i].byte));
        pCmd->chd[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
        SwitchChanalData (&pCmd->chd[i].fStr,8);//小数点后8位调试
		pCmd->chd[i].DfStr = pCmd->chd[i].fStr;
      //  fStr2DfStr(&pCmd->chd[i]);//接收后解析的数据原始数据转换为数据显示
        currCharNum+=3;
    }

}
// 0 是到头了1 正常
uint8 RereadSD(uint32 RCNTALL)
{
//    FRESULT res_sd;
    uint8 i = 0;
    UINT fnum=0;
    uint8 rcnt=0;
    static struct ChanalData ChanDataR;
    uint8 buf[36] = {0};
    memset((void *)&ChanDataR,0,sizeof(ChanDataR));
    if((RCNTALL)*sizeof(DsdR.uD)>f_size(&fnew))
        return 0;
    rcnt=0;
    while(f_lseek(&fnew,(RCNTALL)*sizeof(DsdR.uD))!=FR_OK)
    {
        if(rcnt++>100)break;
    };//每次读取之前先移动读写指针达到读写不同数据的目的
    rcnt=0;
    while(f_sync(&fnew)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    rcnt=0;
    while(f_read(&fnew, DsdR.uD, sizeof(DsdR.uD), &fnum)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    stamp2Time( DsdR.Data.Timestamp,&RTC_DateStrStartR,&RTC_TimeStrStartR);
    SampleTimeCurR.CAM=DsdR.Data.SampleTimeCurCAM;
    for(i=0; i<2; i++)
    {
        memcpy(buf+i*3, DsdR.Data.FD[i].SDData.byte,sizeof(DsdR.Data.FD[i].SDData.byte));
		ChanDataR.frq[i].vcind = DsdR.Data.FD[i].SDData.vcind;
    }
    for(i=0; i<CHANNUM; i++)
    {
        memcpy(buf+6+i*3, DsdR.Data.CHD[i].SDData.byte,sizeof(DsdR.Data.CHD[i].SDData.byte));
        DDaR[i][0].vcind = DsdR.Data.CHD[i].SDData.vcind;
		ChanDataR.chd[i].vcind = DsdR.Data.CHD[i].SDData.vcind;
    }
    SerialRemoteHandleR(buf,&ChanDataR);
    for(i=0; i<2; i++)//频率参数
    {
        DFrqR[i].DP.Num = ChanDataR.frq[i].DfStr;//接收到数据更新
        DFrqR[i].Uint[DFrqR[i].vcind].Col = ChanDataR.frq[i].UintOne.Col;  // 显示单位刷新
        DFrqR[i].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新
        DFrqR[i].vcind = DsdR.Data.FD[i].SDData.vcind;
    }
    for(i=0; i<CHANNUM; i++)
    {
        UpDataDDaR(ChanDataR,i);
		
    }
    return 1;
}
void stamp2Time(u32 stamp,RTC_DateTypeDef* d,RTC_TimeTypeDef* t);
void page_Record_cycle(struct CUR *pCUR )
{
    uint16 xR=10;
    uint16 yR=59;
    uint16 HdivR=90;
    uint16 HdivR_Sel=90;
    uint16 i;
    char ptr[10];

    HdivR=20;
    xR=460;
   yR=4+HdivR+24;
    HdivR=26;
    HdivR_Sel=HdivR*2;
    for(i=1; i<13; i++)
    {
        if(i>0&&i<10&&pCUR->COL!=i)
        {
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            if(CompNumStru(&DDaR[i -1][0].DP.Num)==1)//判断数值是否与以前得不同才刷新
                LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i -1][0].DP,xR+1+50,yR+5,ptr,Font16);
        }
        else if(i==10&&pCUR->COL!=i)
        {
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            if(CompNumStru(&DDaR[i -1][0].DP.Num)==1)//判断数值是否与以前得不同才刷新
                LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
        }
        else  if(i>0&&i<10&&pCUR->COL==i)
        {
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            if(CompNumStru(&DDaR[i -1][0].DP.Num)==1)//判断数值是否与以前得不同才刷新
                LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
        }
        else if(i==10&&pCUR->COL==i)
        {
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            if(CompNumStru(&DDaR[i -1][0].DP.Num)==1)//判断数值是否与以前得不同才刷新
                LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
        }
        if( pCUR->COL==i)
        {
            yR+=HdivR_Sel;
        }
        else
        {
            yR+=HdivR;
        }
    }
}


//保护处理
void page_Protect_R(void)
{
    if(SampleTimeCurR.CAM>=SampleMaxCam)
        SampleTimeCurR.CAM = 0;
    if(ZoneCurR.CAM>=ZoneMaxCam)
        ZoneCurR.CAM = 0;
}

void page_Record_F(const struct CUR CURV)
{
    uint16 xR=10;
    uint16 yR=59;
    uint16 HdivR=90;
    uint16 HdivR_Sel=90;
    uint16 i;
    char str[10];
    uint32 BackColor;
    char ptr[10];


    page_CH(CURV);

    xR=460;
    yR=24;
    HdivR=20;
    LCD_DrawRectLJ(xR,  yR, 639-460, HdivR,LCD_COLOR_WHITE);
    LCD_FillRectLJ(xR+1, yR+1, 639-460-2, HdivR-2,LCD_COLOR_BLUE); //标题框填色
    BackColor=LCD_GetBackColor();
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+50,  yR+2, "MONITOR",&Font16);
    LCD_SetBackColor(BackColor);
    xR=460;
    yR=4+HdivR+24;
    HdivR=26;
    HdivR_Sel=HdivR*2;
    BackColor=LCD_GetBackColor();
    for(i=1; i<13; i++)
    {
        if( CURV.COL==i)
        {
            LCD_DrawRect(xR,  yR, 639-460, 20);
            LCD_DrawRect(xR,  yR, 639-460, HdivR_Sel);
        }
        else
        {
            LCD_DrawRect(xR,  yR, 639-460, HdivR);
        }
        if(i>0&&i<10&&CURV.COL!=i)
        {
            sprintf(str,"%d  ",i);
            BackColor=LCD_GetBackColor();
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            page_string(&DDaR[i-1][0],xR+1+50,yR+5,Font16);
        }
        else if(i==10&&CURV.COL!=i)
        {
            sprintf(str,"%d ",i);
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
        }
        else  if(i>0&&i<10&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//标题框填色
            sprintf(str,"%d    CH%d ",i,i);
            BackColor=LCD_GetBackColor();
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            page_string(&DDaR[i-1][0],xR+1+10,yR+5+25,Font20);
        }
        else if(i==10&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//标题框填色
            sprintf(str,"%d   CH%d",i,i);
            BackColor=LCD_GetBackColor();
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            page_string(&DDaR[i-1][0],xR+1+10,yR+5+25,Font20);
        }
        else if(i==11&&CURV.COL!=i) //SAMPLE
        {
            sprintf(str,"%d  ",i);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCurR.CAM]),&Font20);

        }
        else  if(i==11&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //标题框填色
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCurR.CAM]),&Font24);

        }
        else if(i==12&&CURV.COL!=i)//ZONE
        {
            sprintf(str,"%d  ",i);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCurR.CAM]),&Font20);

        }

        else  if(i==12&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //标题框填色
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCurR.CAM]),&Font24);
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

void page_Record_init(const struct CUR CURV)
{
    uint16 xR=10;
    uint16 yR=59;
    uint16 HdivR=90;
    uint16 HdivR_Sel=90;
    uint16 i;
    char str[10];
    uint32 BackColor;
    char ptr[10];
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    page_CH(CURV);
    if(CHI == DataSave.Data_type.LANG )
    {
        COMmenuInit(DataSave.Data_type.LANG,"上一页","  快退 ","返回","快进","下一页",2);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        COMmenuInit(DataSave.Data_type.LANG,"LAST","LEFT","Record","BACK","NEXT",2);
    }
    xR=460;
    yR=4+24;
    HdivR=20;
    LCD_DrawRectLJ(xR,  yR, 639-460, HdivR,LCD_COLOR_WHITE);
    LCD_FillRectLJ(xR+1, yR+1, 639-460-2, HdivR-2,LCD_COLOR_BLUE); //标题框填色
    BackColor=LCD_GetBackColor();
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+50,  yR+2, "MONITOR",&Font16);
    LCD_SetBackColor(BackColor);
    xR=460;
    yR=4+HdivR+24;
    HdivR=26;
    HdivR_Sel=HdivR*2;
    BackColor=LCD_GetBackColor();
    for(i=1; i<13; i++)
    {
        if( CURV.COL==i)
        {
            LCD_DrawRect(xR,  yR, 639-460, 20);
            LCD_DrawRect(xR,  yR, 639-460, HdivR_Sel);
        }
        else
        {
            LCD_DrawRect(xR,  yR, 639-460, HdivR);
        }
        if(i>0&&i<10&&CURV.COL!=i)
        {
            sprintf(str,"%d  ",i);
            BackColor=LCD_GetBackColor();
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            page_string(&DDaR[i-1][0],xR+1+50,yR+5,Font16);
        }
        else if(i==10&&CURV.COL!=i)
        {
            sprintf(str,"%d ",i);
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            memset(ptr,0,sizeof(ptr));
            NumFloat2Char(&DDaR[i-1][0],&DDaR[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaR[i-1][0],ptr,9);
            LCD_DisplayNumLine_EN_CH_LJ_Home(&DDaR[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
        }
        else  if(i>0&&i<10&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//标题框填色
            sprintf(str,"%d    CH%d ",i,i);
            BackColor=LCD_GetBackColor();
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            page_string(&DDaR[i-1][0],xR+1+10,yR+5+25,Font20);
        }
        else if(i==10&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//标题框填色
            sprintf(str,"%d   CH%d",i,i);
            BackColor=LCD_GetBackColor();
            LCD_SetBackColor(colors[i-1]);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
            LCD_SetBackColor(BackColor);
            page_string(&DDaR[i-1][0],xR+1+10,yR+5+25,Font20);
        }
        else if(i==11&&CURV.COL!=i) //SAMPLE
        {
            sprintf(str,"%d  ",i);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCurR.CAM]),&Font20);

        }
        else  if(i==11&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //标题框填色
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCurR.CAM]),&Font24);

        }
        else if(i==12&&CURV.COL!=i)//ZONE
        {
            sprintf(str,"%d  ",i);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCurR.CAM]),&Font20);

        }

        else  if(i==12&&CURV.COL==i)
        {
            LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //标题框填色
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
            LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCurR.CAM]),&Font24);
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



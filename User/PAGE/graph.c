/*
  * @file    Graph.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   ��ͼ����
 */
#include  "include.h"
//������ƿ�
OS_TCB GraphTaskTCB;
//�����ջ
CPU_STK Graph_TASK_STK[Graph_STK_SIZE];
struct CUR CurGraph = {0};//�Ҳ�����ѡ����α�
struct CUR CurGraphOld = {0};//�Ҳ�����ѡ����α��ϴε�λ��
struct CUR CurChan; //��������1 �� 10ͨ��
struct CUR SampleTimeCur= {0};  //SampleTime����ѡ����α�
struct CUR ZoneCur= {0};//����ѡ����α�
const char Sampletime[SampleMaxCam][7] =
{"50ms \0","100ms\0","150ms\0","200ms\0","500ms\0","1s   \0","2s   \0","5s   \0","10s  \0","20s  \0","30s  \0","1min \0","2min \0","5min \0","10min\0","20min\0","30min\0","1h   \0"};

static char SampletimeX[SampleMaxCam][12] =
{
    "X-1s/d    \0","X-2s/d    \0","X-3s/d    \0","X-4s/d    \0",
    "X-10s/d   \0","X-20s/d   \0","X-40s/d   \0","X-100s/d  \0",
    "X-200s/d  \0","X-400s/d  \0","X-600s/d  \0","X-20min/d \0",
    "X-30min/d \0","X-100min/d\0","X-200min/d\0","X-400min/d\0",
    "X-10h/d   \0","X-20h/d   \0"
};

//ÿ��ѡ��ĺ�����
const uint32 SampletimeMs[SampleMaxCam] =
{50,100,150,200,500,1000,2000,5000,10000,200000,300000,600000,1200000,3000000,6000000,12000000,18000000,36000000};
//���ߵ���ʾ 10zone ����ͼy���10������ʮ������
const char  ZoneC[ZoneMaxCam][8] =
{"1zone \0","2zone \0","5zone \0","10zone\0"};
const uint32 ZoneData[ZoneMaxCam] =
{1,2,5,10};
Point Lines[DCOL-1][PointsNum] = {0};//ͨ�����ߵľ���
uint16 Linenum=0;

uint16 LinenumFM=0;
uint8 ShowDateTimeFlag = 0;
uint8 Rightframe = 0; //���ָ����ˢ�±�־ 1ˢ��
RTC_TimeTypeDef RTC_TimeStrStart;//ʱ��
RTC_DateTypeDef RTC_DateStrStart;//����
void Graph_Work(void)
{
//    uint16 y=0;
    static RTC_TimeTypeDef TimeStr;
    //���� ���ұߵĿ�
    if(CurGraph.COL==CurGraphOld.COL&&CurGraph.CAM==CurGraphOld.CAM)
    {
        page_graph_cycle(&CurGraph);//����ˢ��
    }
    else
    {
        page_graph(CurGraph);// ȫˢ��
        Rightframe = 1;
    }
    //�����߲��ڴ˴����������԰ѿػ�����ֱ���ڴ��������ݵ���������
    //���������ʱ��
    if(TimeStr.Seconds!=RTC_TimeStr.Seconds)
    {
        ShowDateTime(RTC_DateStr,RTC_TimeStr,138,0,&Font16);   //��ʾʱ��
        TimeStr=RTC_TimeStr;
    }
}
//��������չʾ����
void Graph_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //��ҳ
    Stu_DisplayData_Init();
    CurGraph.COL=12;
    CurGraph.CAM=0;
    CurChan.COL =12;
    CurGraphOld = CurGraph;
	CurGraph.PAGE = 0;
    page_graph_init(CurGraph);
    DrawGraph_Init();
    Stu_DisplayData_Init();

    SampleTimeCur.CAM =DataSave.Data_type.SampleCAM;
    ZoneCur.CAM  = DataSave.Data_type.ZoneCAM;
    ShowTimeflag=0;
    Rightframe=1;
    while(1)
    {
        //����
        keytmp = KeyTaskCreate(ALL_ReTurn);//����ɨ��15����
		if(CurGraph.PAGE == 0)
		{
			keytmp=ModifyGraph(keytmp,&CurGraph,&CurGraphOld,&CurChan,1,12) ;       //�����ֵ
		}else if(CurGraph.PAGE == 1){
			keytmp=ModifyGraph(keytmp,&CurGraph,&CurGraphOld,&CurChan,1,2) ;       //�����ֵ
		}

        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err); //ɾ���Լ�����
        }
        Graph_Work();
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );
    }
}
//��ʾʱ��
void ShowDateTime(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos,sFONT *fonts)
{
    char str[50];
    //��ʾʱ��
    sprintf(str,"20%0.2d-%0.2d-%0.2d-%0.2d:%0.2d:%0.2d",
            RTC_DateStr.Year,
            RTC_DateStr.Month,
            RTC_DateStr.Date,
            RTC_TimeStr.Hours,
            RTC_TimeStr.Minutes,
            RTC_TimeStr.Seconds);
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos, Ypos,(uint8_t* )str);
}

//��ʾ4 ��ʱ����5s��һ��20��
void ShowTimeLine(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos, uint16 xW,sFONT *fonts,uint32 devtime)
{
    char str[20];
    char strD[20];
    uint8 i;
    uint32 BackColor;
    RTC_TimeTypeDef RTC_TimeStrTemp;
    RTC_DateTypeDef RTC_DateStrTemp;
    uint32 Stamp; //ʱ���
    uint32 TextColor;
    //��ʾʱ��
    Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr);
    sprintf(str,"%0.2d:%0.2d:%0.2d",
            RTC_TimeStr.Hours,
            RTC_TimeStr.Minutes,
            RTC_TimeStr.Seconds);
    sprintf(strD,"S:%0.2d-%0.2d",
            RTC_DateStr.Month,
            RTC_DateStr.Date);
    TextColor=LCD_GetTextColor();
    BackColor=LCD_GetBackColor();
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetBackColor(LCD_COLOR_BLACK );
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos, Ypos+20, (uint8_t* )str);
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*5-20, Ypos+20, (uint8_t* )strD);
    LCD_SetTextColor(TextColor);
    LCD_SetBackColor(BackColor );
    for(i=1; i<4; i++)
    {
        memset(str,0,sizeof(str));
        stamp2Time(i*devtime+Stamp,&RTC_DateStrTemp,&RTC_TimeStrTemp);  //ʱ�����1��ת������
        sprintf(str,"%0.2d:%0.2d:%0.2d",
                RTC_TimeStrTemp.Hours,
                RTC_TimeStrTemp.Minutes,
                RTC_TimeStrTemp.Seconds);
        TextColor=LCD_GetTextColor();
        BackColor=LCD_GetBackColor();
        LCD_SetTextColor(LCD_COLOR_WHITE);
        LCD_SetBackColor(LCD_COLOR_BLACK );
        LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*i, Ypos+20, (uint8_t* )str);
        LCD_SetTextColor(TextColor);
        LCD_SetBackColor(BackColor );
    }
    i=4;
    memset(str,0,sizeof(str));
    stamp2Time(i*devtime+Stamp,&RTC_DateStrTemp,&RTC_TimeStrTemp);  //ʱ�����1��ת������
    sprintf(str,"%0.2d:%0.2d:%0.2d",
            RTC_TimeStrTemp.Hours,
            RTC_TimeStrTemp.Minutes,
            RTC_TimeStrTemp.Seconds);
    sprintf(strD,"E:%0.2d-%0.2d",
            RTC_DateStrTemp.Month,
            RTC_DateStrTemp.Date);
    TextColor=LCD_GetTextColor();
    BackColor=LCD_GetBackColor();
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetBackColor(LCD_COLOR_BLACK );
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*i, Ypos+20, (uint8_t* )str);
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*5+50, Ypos+20, (uint8_t* )strD);
    LCD_SetTextColor(TextColor);
    LCD_SetBackColor(BackColor );
}



//��ʾ4 ��ʱ����0.5s��һ��2��
void ShowTimeLineFM(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr,uint16_t Xpos, uint16_t Ypos, uint16 xW,sFONT *fonts)
{
    char str[20];
    char strD[10];
    uint32 BackColor;
    RTC_TimeTypeDef RTC_TimeStrTemp;
    RTC_DateTypeDef RTC_DateStrTemp;
    uint32 Stamp; //ʱ���
    uint32 TextColor;
    //��ʾʱ��
    Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr);
    sprintf(str,"%0.2d:%0.2d:%0.2d",
            RTC_TimeStr.Hours,
            RTC_TimeStr.Minutes,
            RTC_TimeStr.Seconds);
    sprintf(strD,"S:%0.2d-%0.2d",
            RTC_DateStr.Month,
            RTC_DateStr.Date);
    TextColor=LCD_GetTextColor();
    BackColor=LCD_GetBackColor();
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetBackColor(LCD_COLOR_BLACK );
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos, Ypos+20, (uint8_t* )str);
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*5-20, Ypos+20, (uint8_t* )strD);
    strcat(str,".5");
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*1, Ypos+20, (uint8_t* )str);
    // LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*1+10, Ypos+20, (uint8_t* )strD);
    LCD_SetTextColor(TextColor);
    LCD_SetBackColor(BackColor );

    memset(str,0,sizeof(str));
    stamp2Time(1+Stamp,&RTC_DateStrTemp,&RTC_TimeStrTemp);  //ʱ�����1��ת������
    sprintf(str,"%0.2d:%0.2d:%0.2d",
            RTC_TimeStrTemp.Hours,
            RTC_TimeStrTemp.Minutes,
            RTC_TimeStrTemp.Seconds);
    TextColor=LCD_GetTextColor();
    BackColor=LCD_GetBackColor();
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetBackColor(LCD_COLOR_BLACK );
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*2, Ypos+20, (uint8_t* )str);
    strcat(str,".5");
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*3, Ypos+20, (uint8_t* )str);
    LCD_SetTextColor(TextColor);
    LCD_SetBackColor(BackColor );

    memset(str,0,sizeof(str));
    stamp2Time(2+Stamp,&RTC_DateStrTemp,&RTC_TimeStrTemp);  //ʱ�����1��ת������
    sprintf(str,"%0.2d:%0.2d:%0.2d",
            RTC_TimeStrTemp.Hours,
            RTC_TimeStrTemp.Minutes,
            RTC_TimeStrTemp.Seconds);
    sprintf(strD,"E:%0.2d-%0.2d",
            RTC_DateStrTemp.Month,
            RTC_DateStrTemp.Date);
    TextColor=LCD_GetTextColor();
    BackColor=LCD_GetBackColor();
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetBackColor(LCD_COLOR_BLACK );
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*4, Ypos+20, (uint8_t* )str);
    LCD_DisplayStringLine_EN_CH_LJ_Time(Xpos+xW*5+50, Ypos+20, (uint8_t* )strD);
    LCD_SetTextColor(TextColor);
    LCD_SetBackColor(BackColor );

}
//�Լ���װ����ɫ
void LCD_FillRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor)
{
    uint32 TextColor;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(SetColor);
    LCD_FillRect(Xpos, Ypos, Width,Height);
    LCD_SetTextColor(TextColor);
}
//�Լ���װ������
void LCD_DisplayStringLine_EN_CH_LJ_WithFont(uint16_t x,uint16_t y, uint8_t *ptr,sFONT *fonts)
{
    sFONT * fonttemp = LCD_GetFont();
    LCD_SetFont(fonts);
    LCD_DisplayStringLine_EN_CH_LJ(x,y, ptr);
    LCD_SetFont( fonttemp );
}


//�Լ���װ������
void LCD_DisplayStringLine_EN_CH_LJ_Time(uint16_t x,uint16_t y, uint8_t *ptr)
{
    sFONT * fonttemp = LCD_GetFont();
    DISP_CNL_S(x,y, ptr);
    LCD_SetFont( fonttemp );
}

//�Լ���װ������
void LCD_DisplayStringLine_EN_CH_LJ_WithFC(uint16_t x,uint16_t y, uint8_t *ptr,sFONT *fonts,uint32 SetColor)
{
    sFONT * fonttemp = LCD_GetFont();
    uint32 TextColor;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(SetColor);
    LCD_SetFont(fonts);
    LCD_DisplayStringLine_EN_CH_LJ(x,y, ptr);
    LCD_SetFont( fonttemp );
    LCD_SetTextColor(TextColor);
}
//�Լ���װ����ɫ
void LCD_DrawRectLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,uint32 SetColor)
{
    uint32 TextColor;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(SetColor);
    LCD_DrawRect(Xpos, Ypos, Width,Height);
    LCD_SetTextColor(TextColor);
}
//�Լ���װ����ɫ
void LCD_DrawPolygonLJ(pPoint Points, uint16_t PointCount,uint32 SetColor)
{
    uint32 TextColor;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(SetColor);
    LCD_DrawPolygon(Points,PointCount);
    LCD_SetTextColor(TextColor);
}


//�Լ���װ����ɫ
void LCD_DrawPolygonLJ2(pPoint Points, uint16_t PointCount,uint32 SetColor)
{
    uint32 TextColor;
    pPoint Points2;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(SetColor);
    Points2[0] = Points[PointCount-2];
    Points2[1] = Points[PointCount-1];
    LCD_DrawPolygon(Points2,2);
    LCD_SetTextColor(TextColor);
}
//�Լ���װ����ɫ
void LCD_DrawHLineLJ(uint16_t Xpos, uint16_t Ypos, uint16_t Length,uint32 SetColor)
{
    uint32 TextColor;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(SetColor);
    LCD_DrawHLine(Xpos, Ypos,Length);
    LCD_SetTextColor(TextColor);
}

//������߾�̬���Ƶĳ�ʼ��
void DrawGraph_Init(void)
{
    uint16 x=5;
    uint16 y=20;
    //  uint16 xW=PointsPix;
    LCD_FillRectLJ(x-1, y-1, PointsPix+50,PointsPiy2+1,LCD_COLOR_BLACK);
    lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
//    LCD_DrawRectLJ(x-2+50,y-20,  xW, 1,LCD_COLOR_WHITE);
}

void DrawGraph_FlashZone(const struct CUR CURV,uint16_t PointCount)
{
    uint16 x=5;
    uint16 y=20;

    LCD_FillRectLJ(x-2+50,y-1, PointsPix,PointsPiy2+1,LCD_COLOR_BLACK);
    lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);

}

//�ֲ���� ֻ������߲���
void DrawGraph_Flash(const struct CUR CURV,uint16_t PointCount)
{
    uint16 x=5;
    uint16 y=20;
    uint8 i;
    uint16 Yoff=0;
    uint16 Yoffup=0;
    char ptr[10];
    static uint8 COLold = 0;
    uint32 BackColor;
    if(DataSave.Data_type.PageFlag != FPageGraph)//���ڻ�ͼҳ��ֱ�ӷ���
    {
        return;
    }
    for(i=0; i<DCOL-1; i++)
    {
        LCD_DrawPolygonLJ(Lines[i],PointCount-1,LCD_COLOR_BLACK);
    }
    lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
    if(CURV.COL<11)//ֻ���ڻ���һ֡ʱ�Ÿ���
        COLold =CURV.COL-1;//lj 0610��
    Yoff =  ((COLold)%ZoneData[ ZoneCur.CAM])*(PointsPiy/ ZoneData[ ZoneCur.CAM]);
    Yoffup =  (PointsPiy/ ZoneData[ ZoneCur.CAM]);
    
	if(CURV.PAGE == 0)
	{
		memset(ptr,0,sizeof(ptr));
		if(DDa[COLold][0].vcind==VDC)
			SwitchChanalData(&DDa[COLold][0].Up[DDa[COLold][0].vcind],3);
		else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
			SwitchChanalData(&DDa[COLold][0].Up[DDa[COLold][0].vcind],1);
		NumFloat2Char(&DDa[COLold][0],&DDa[COLold][0].Up[DDa[COLold][0].vcind],ptr);//��ʾ����
		AddUint2CharUnit(DDa[COLold][0],ptr,7);
		BackColor=LCD_GetBackColor();
		LCD_SetBackColor(LCD_COLOR_BLACK);
		if(DataSave.Data_type.PageFlag == FPageGraph)
		{
			if(CurGraph.COL>10)
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )"         ",&Font16,colors[COLold]);
				lcd_Grids(5-2+50,20+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
			}
			else
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[COLold]);

			}
		}
		LCD_SetBackColor(BackColor);
		memset(ptr,0,sizeof(ptr));
		if(DDa[COLold][0].vcind==VDC)
			SwitchChanalData(&DDa[COLold][0].Low[DDa[COLold][0].vcind],3);
		else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
			SwitchChanalData(&DDa[COLold][0].Up[DDa[COLold][0].vcind],1);
		NumFloat2Char(&DDa[COLold][0],&DDa[COLold][0].Low[DDa[COLold][0].vcind],ptr);//��ʾ����
		AddUint2CharUnit(DDa[COLold][0],ptr,7);
		BackColor=LCD_GetBackColor();
		LCD_SetBackColor(LCD_COLOR_BLACK);
		if(DataSave.Data_type.PageFlag == FPageGraph)
		{
			if(CurGraph.COL>10)
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )"         ",&Font16,colors[COLold]);
				lcd_Grids(5-2+50,20+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
			}
			else
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[COLold]);
			}

		}
		LCD_SetBackColor(BackColor);
	}else if(CURV.PAGE == 1){
		memset(ptr,0,sizeof(ptr));
		if(DDa[COLold+11][0].vcind==VDC)
			SwitchChanalData(&DDa[COLold+11][0].Up[DDa[COLold+11][0].vcind],3);
		else if(DDa[i+11-1][0].vcind==VR||DDa[i+11-1][0].vcind==VI||DDa[i+11-1][0].vcind==VG)
			SwitchChanalData(&DDa[COLold+11][0].Up[DDa[COLold+11][0].vcind],1);
		NumFloat2Char(&DDa[COLold+11][0],&DDa[COLold+11][0].Up[DDa[COLold+11][0].vcind],ptr);//��ʾ����
		AddUint2CharUnit(DDa[COLold+11][0],ptr,7);
		BackColor=LCD_GetBackColor();
		LCD_SetBackColor(LCD_COLOR_BLACK);
		if(DataSave.Data_type.PageFlag == FPageGraph)
		{
			if(CurGraph.COL>10)
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )"         ",&Font16,colors[COLold]);
				lcd_Grids(5-2+50,20+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
			}
			else
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[COLold]);

			}
		}
		LCD_SetBackColor(BackColor);
		memset(ptr,0,sizeof(ptr));
		if(DDa[COLold+11][0].vcind==VDC)
			SwitchChanalData(&DDa[COLold+11][0].Low[DDa[COLold+11][0].vcind],3);
		else if(DDa[i+11-1][0].vcind==VR||DDa[i+11-1][0].vcind==VI||DDa[i+11-1][0].vcind==VG)
			SwitchChanalData(&DDa[COLold+11][0].Up[DDa[COLold+11][0].vcind],1);
		NumFloat2Char(&DDa[COLold+11][0],&DDa[COLold+11][0].Low[DDa[COLold+11][0].vcind],ptr);//��ʾ����
		AddUint2CharUnit(DDa[COLold+11][0],ptr,7);
		BackColor=LCD_GetBackColor();
		LCD_SetBackColor(LCD_COLOR_BLACK);
		if(DataSave.Data_type.PageFlag == FPageGraph)
		{
			if(CurGraph.COL>10)
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )"         ",&Font16,colors[COLold]);
				lcd_Grids(5-2+50,20+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
			}
			else
			{
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[COLold]);
			}

		}
		LCD_SetBackColor(BackColor);
	}
}

uint8 DrawGraph(const struct CUR CURV,uint16_t PointCount)
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
        if(DataSave.Data_type.DisLog[i+2][0].index[1] !=OFF)
        {
            lastnum = i;
            break;
        }
    }
	if(CURV.PAGE == 0)
	{
		for(i=0; i<DCOL-1; i++)
		{
			if(DDa[i][0].DP.Dxy.Y>GIXStartY)
				DDa[i][0].DP.Dxy.Y=GIXStartY;
			if(DDa[i][0].DP.Dxy.X<GIXStartX)
				DDa[i][0].DP.Dxy.X=GIXStartX-1;
			else if(DDa[i][0].DP.Dxy.X>GIXStartX+PointsPix)
				DDa[i][0].DP.Dxy.X=GIXStartX+PointsPix+1;

			if(DDa[i][0].DP.Dxy.Y<GIXStartY-PointsPiy)
				DDa[i][0].DP.Dxy.Y=GIXStartY-PointsPiy;
			else if(DDa[i][0].DP.Dxy.Y>GIXStartY)
				DDa[i][0].DP.Dxy.Y=GIXStartY;
			if(PointCount>0&&PointCount<PointsNum)
			{
				if(DDa[i][0].DP.dispold==1)
					continue;
				Lines[i][PointCount-1] =DDa[i][0].DP.Dxy;
				if(DDa[i][0].DP.Dxy.X>GIXStartX+PointsPix)    //����ͼ��
				{
					//DDa[i][0].DP.Dxy.X = GIXStartX+400; //��������
					r =1;
					continue;
				}
				if(DataSave.Data_type.DisLog[i+2][0].index[1] == OFF) //off״̬������
				{
					continue;
				}
				if(PointCount>1&&DataSave.Data_type.PageFlag == FPageGraph)   //ֻ��ҳ���л�����ͼҳ��ʱ�Ż�
				{
					LCD_DrawPolygonLJ2(Lines[i],PointCount,colors[i]);
				}
				if(PointCount==1||Rightframe==1)
				{

					if(i<DCOL-2)
						sprintf(str," %d  ",i+1);
					else
						sprintf(str," %d ",i+1);
					TextColor=LCD_GetTextColor();
					BackColor=LCD_GetBackColor();
					if(CURV.COL-1>10)  //Խ��
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
						if(DataSave.Data_type.PageFlag == FPageGraph)
						{
							LCD_FillRectLJ(x-4-1, y,52,PointsPiy2,LCD_COLOR_BLACK);   // �������ϻ�
						}
						flag = 1;
					}
					if(DataSave.Data_type.PageFlag == FPageGraph)
					{
						LCD_DrawHLineLJ(8+30, Lines[i][0].Y, 12,colors[i]);
						//��ָ֤ʾ��ͷ������
						if((Lines[i][0].Y+8)>GIXStartY)
							LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  Lines[i][0].Y-15, (uint8_t* )str,&Font16);
						else if((Lines[i][0].Y-8)<GIXEndY)
							LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  Lines[i][0].Y, (uint8_t* )str,&Font16);
						else
							LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  Lines[i][0].Y-8, (uint8_t* )str,&Font16);
					}
					LCD_SetColors(TextColor,BackColor);
					if(i== lastnum)
						Rightframe = 0;  //����ѱ�־��0
				}
			}
		}
	}else if(CURV.PAGE == 1){
		for(i=0; i<2; i++)
		{
			if(DDa[i+11][0].DP.Dxy.Y>GIXStartY)
				DDa[i+11][0].DP.Dxy.Y=GIXStartY;
			if(DDa[i+11][0].DP.Dxy.X<GIXStartX)
				DDa[i+11][0].DP.Dxy.X=GIXStartX-1;
			else if(DDa[i+11][0].DP.Dxy.X>GIXStartX+PointsPix)
				DDa[i+11][0].DP.Dxy.X=GIXStartX+PointsPix+1;

			if(DDa[i+11][0].DP.Dxy.Y<GIXStartY-PointsPiy)
				DDa[i+11][0].DP.Dxy.Y=GIXStartY-PointsPiy;
			else if(DDa[i+11][0].DP.Dxy.Y>GIXStartY)
				DDa[i+11][0].DP.Dxy.Y=GIXStartY;
			if(PointCount>0&&PointCount<PointsNum)
			{
				if(DDa[i+11][0].DP.dispold==1)
					continue;
				Lines[i][PointCount-1] =DDa[i+11][0].DP.Dxy;
				if(DDa[i+11][0].DP.Dxy.X>GIXStartX+PointsPix)    //����ͼ��
				{
					//DDa[i][0].DP.Dxy.X = GIXStartX+400; //��������
					r =1;
					continue;
				}
				if(DataSave.Data_type.DisLog[i+2+10][0].index[1] == OFF) //off״̬������
				{
					continue;
				}
				if(PointCount>1&&DataSave.Data_type.PageFlag == FPageGraph)   //ֻ��ҳ���л�����ͼҳ��ʱ�Ż�
				{
					LCD_DrawPolygonLJ2(Lines[i],PointCount,colors[i]);
				}
				if(PointCount==1||Rightframe==1)
				{

					if(i<1)
						sprintf(str," %d ",i+1+10);
					else
						sprintf(str," %d ",i+1+10);
					TextColor=LCD_GetTextColor();
					BackColor=LCD_GetBackColor();
					if(CURV.COL-1>10)  //Խ��
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
						if(DataSave.Data_type.PageFlag == FPageGraph)
						{
							LCD_FillRectLJ(x-4-1, y,52,PointsPiy2,LCD_COLOR_BLACK);   // �������ϻ�
						}
						flag = 1;
					}
					if(DataSave.Data_type.PageFlag == FPageGraph)
					{
						LCD_DrawHLineLJ(8+30, Lines[i][0].Y, 12,colors[i]);
						//��ָ֤ʾ��ͷ������
						if((Lines[i][0].Y+8)>GIXStartY)
							LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  Lines[i][0].Y-15, (uint8_t* )str,&Font16);
						else if((Lines[i][0].Y-8)<GIXEndY)
							LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  Lines[i][0].Y, (uint8_t* )str,&Font16);
						else
							LCD_DisplayStringLine_EN_CH_LJ_WithFont(8,  Lines[i][0].Y-8, (uint8_t* )str,&Font16);
					}
					LCD_SetColors(TextColor,BackColor);
					if(i== lastnum)
						Rightframe = 0;  //����ѱ�־��0
				}
			}
		}
	}
    return r;
}

//��ͼ�ļ�ֵ����
uint8 ModifyGraph(uint8 keytmp,struct CUR* cur,struct CUR* curold,struct CUR* curchan,char MinCol,char MaxCol)                 // ��ֵƥ�亯������while��ѭ���з���ƥ��
{
    OS_ERR err;
    char MaxCam = 0;
    *curold=*cur;
    cur->OPN=keytmp;                        //����ֵ����������־��������ǰ�Ĳ���״̬
    if(cur->COL==11)
    {
        MaxCam = SampleMaxCam;
        cur->CAM = SampleTimeCur.CAM;
    }
    else if(cur->COL==12)
    {
        MaxCam = ZoneMaxCam;
        cur->CAM = ZoneCur.CAM;
    }
    switch(cur->OPN)
    {
        case KEY_FUNC1://����10ch
            DataSave.Data_type.PageFlag =  FPageCh ;
            break;
        case KEY_FUNC2://����data
            DataSave.Data_type.PageFlag =  FPageDisplayData ;
            break;
        case KEY_FUNC4://����3ch
            DataSave.Data_type.PageFlag =  FPageCh3 ;
            break;
        case KEY_FUNC5://�����ļ�
            if(FileRes ==FILEReady)
                DataSave.Data_type.PageFlag =  FPageFile  ;
            break;

        case KEY_MANU://��������
            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
        case KEY_DOT:
            if(FileRes ==FILEReady)//δ�����ļ�̬
                SaveTrig = TRIGStart;
            else if(FileRes ==FILETFOpen)//����tf���ļ�
                SaveTrig = TRIGStopTF;
            else if(FileRes ==FILEUSBOpen)//����usb���ļ�
                SaveTrig = TRIGStopUSB;
            break;
        case KEY_0:
            if(ScreenFlag==OFF)
                ScreenFlag=ON;

            break;
        case KEY_UP://��
            do
            {
                if(cur->COL>MinCol)
                    cur->COL--;
                else
                    cur->COL=MaxCol;
            }
            while(DDa[cur->COL-1][0].DP.disp!=2&&cur->COL-1<MaxCol-2);
            if(cur->COL==11)
            {
                cur->CAM = SampleTimeCur.CAM;
            }
            else if(cur->COL==12)
            {
                cur->CAM = ZoneCur.CAM;
            }
            else if(cur->COL<=10&&cur->COL>0)    //10��ͨ�� 1��10
            {
                curchan->COL = cur->COL;
            }
            break;
        case KEY_DOWN:
            do
            {
                if(cur->COL<MaxCol) //���һ��
                    cur->COL++;
                else
                    cur->COL=MinCol;
            }
            while(DDa[cur->COL-1][0].DP.disp!=2&&cur->COL-1<MaxCol-2);
            if(cur->COL==11)
            {
                cur->CAM = SampleTimeCur.CAM;
            }
            else if(cur->COL==12)
            {
                cur->CAM = ZoneCur.CAM;
            }
            else if(cur->COL<=10&&cur->COL>0)
            {
                curchan->COL = cur->COL;
            }
            break;
        case KEY_ACC:
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol)
                    cur->COL=MinCol;
            }
            while(DDa[cur->COL-1][0].DP.disp!=2&&cur->COL-1<MaxCol-2);
            if(cur->COL==11)
            {
                cur->CAM = SampleTimeCur.CAM;
            }
            else if(cur->COL==12)
            {
                cur->CAM = ZoneCur.CAM;
            }
            else if(cur->COL<=10&&cur->COL>0)
            {
                curchan->COL = cur->COL;
            }
            break;
        case KEY_LEFT://
            if(MaxCam==0)
			{
				if(cur->PAGE == 0)
				{
					cur->PAGE = 1;
					CurChan.PAGE= 1;
				}else{
					cur->PAGE = 0;
					CurChan.PAGE= 0;
				}
				Stu_DisplayData_Init();
				CurGraph.COL=12;
				CurGraph.CAM=0;
				CurChan.COL =12;
				CurGraphOld = CurGraph;
				SampleTimeCur.CAM =DataSave.Data_type.SampleCAM;
				ZoneCur.CAM  = DataSave.Data_type.ZoneCAM;
				ShowTimeflag=0;
				Rightframe=1;
				page_graph_init(CurGraph);
				DrawGraph_Init();
				Stu_DisplayData_Init();
                break;
			}
            if(cur->CAM>0)
                cur->CAM--;
            else
                cur->CAM=MaxCam-1;
            if(cur->COL==11)
            {
                SampleTimeCur.CAM = cur->CAM;
                DataSave.Data_type.SampleCAM = SampleTimeCur.CAM;
                OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
                SetChanSetTime();
                SendSetSYNCTime(ChanSet);
                OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
                SaveAllPara();
                OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
            }
            else if(cur->COL==12)
            {
                ZoneCur.CAM = cur->CAM ;
                DataSave.Data_type.ZoneCAM= cur->CAM ;
                SaveAllPara();
            }
            break;
        case KEY_RIGHT:
            if(MaxCam==0)
			{
				if(cur->PAGE == 0)
				{
					cur->PAGE = 1;
					CurChan.PAGE= 1;
				}else{
					cur->PAGE = 0;
					CurChan.PAGE= 0;
				}
				Stu_DisplayData_Init();
				CurGraph.COL=12;
				CurGraph.CAM=0;
				CurChan.COL =12;
				CurGraphOld = CurGraph;
				SampleTimeCur.CAM =DataSave.Data_type.SampleCAM;
				ZoneCur.CAM  = DataSave.Data_type.ZoneCAM;
				ShowTimeflag=0;
				Rightframe=1;
				page_graph_init(CurGraph);
				DrawGraph_Init();
				Stu_DisplayData_Init();
                break;
			}
            if(cur->CAM<MaxCam-1) //���һ���ǹ��ܼ�
                cur->CAM++;
            else
                cur->CAM=0;
            if(cur->COL==11)
            {
                SampleTimeCur.CAM = cur->CAM;
                DataSave.Data_type.SampleCAM = SampleTimeCur.CAM;
                CompSample();
                OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
                SetChanSetTime();
                SendSetSYNCTime(ChanSet);
                OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
                SaveAllPara();
                OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
            }
            else if(cur->COL==12)
            {
                ZoneCur.CAM = cur->CAM ;
                DataSave.Data_type.ZoneCAM= cur->CAM ;
                SaveAllPara();
            }
            break;
    }
    return keytmp;
}
//�ж�������float �Ƿ����
int CompFloat(float a,float b)
{
    const float EPSINON = 0.000001;
    float x = 0.000f;
    x = a-b;
    if (( x >= -EPSINON ) && ( x <= EPSINON ))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int CompNumStru(struct NumStru *pNum)
{
    return CompFloat(pNum->fvalold,pNum->fval) ;
}


void page_graph_cycle(struct CUR *pCUR )
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

	if(pCUR->PAGE == 0)
	{
		for(i=1; i<13; i++)
		{
			if(i>0&&i<10&&pCUR->COL!=i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
//				if(CompNumStru(&DDa[i -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i -1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else if(i==10&&pCUR->COL!=i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
//				if(CompNumStru(&DDa[i -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else  if(i>0&&i<10&&pCUR->COL==i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
//				if(CompNumStru(&DDa[i -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
			}
			else if(i==10&&pCUR->COL==i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
//				if(CompNumStru(&DDa[i -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
			}
			else if(i==11&&pCUR->COL!=i) //SAMPLE
			{

			}
			else  if(i==11&&pCUR->COL==i)
			{

			}
			else if(i==12&&pCUR->COL!=i)//ZONE
			{


			}
			else  if(i==12&&pCUR->COL==i)
			{

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
	}else if(pCUR->PAGE == 1){
		for(i=1; i<3; i++)
		{
			if(i>0&&i<10&&pCUR->COL!=i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i+11-1][0],&DDa[i+11-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i+11-1][0],ptr,9);
//				if(CompNumStru(&DDa[i+11 -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+11 -1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else if(i==10&&pCUR->COL!=i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
//				if(CompNumStru(&DDa[i -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else  if(i>0&&i<10&&pCUR->COL==i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i+11-1][0],&DDa[i+11-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i+11-1][0],ptr,9);
//				if(CompNumStru(&DDa[i+11 -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+11-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
			}
			else if(i==10&&pCUR->COL==i)
			{
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
//				if(CompNumStru(&DDa[i -1][0].DP.Num)==1)//�ж���ֵ�Ƿ�����ǰ�ò�ͬ��ˢ��
					LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
			}
			else if(i==11&&pCUR->COL!=i) //SAMPLE
			{

			}
			else  if(i==11&&pCUR->COL==i)
			{

			}
			else if(i==12&&pCUR->COL!=i)//ZONE
			{


			}
			else  if(i==12&&pCUR->COL==i)
			{

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
}

void page_CH(const struct CUR CURV)
{
    uint16 x=2;
    uint16 y=0;
    uint32 BackColor,TextColor;
    if(CURV.COL-1<10)
    {
        TextColor=LCD_GetTextColor();
        BackColor=LCD_GetBackColor();
        LCD_SetColors(colors[CURV.COL-1],LCD_COLOR_HUI);
        LCD_DrawRectLJ(x-1,  y+1-1, 40+5, 16+2,LCD_COLOR_WHITE);
        LCD_FillRectLJ(x, y+1,40+2, 16,LCD_COLOR_HUI); //�������ɫ
        LCD_DisplayStringLine_EN_CH_LJ_WithFont(x,  y+1, " CH ",&Font16);
        LCD_SetColors(TextColor,BackColor);
    }
    else
    {
        TextColor=LCD_GetTextColor();
        BackColor=LCD_GetBackColor();
        LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
        LCD_DrawRectLJ(x-1,  y+1-1, 40+5, 16+2,LCD_COLOR_WHITE);
        LCD_FillRectLJ(x, y+1,40+2, 16,LCD_COLOR_HUI); //�������ɫ
        LCD_DisplayStringLine_EN_CH_LJ_WithFont(x,  y+1, " CH ",&Font16);
        LCD_SetColors(TextColor,BackColor);

    }
}


void page_time(struct CUR *pCUR)
{
}


void page_string(struct RDispData  *p,uint16 xR,uint16 yR,sFONT FontType)
{
    char ptr[10];
    memset(ptr,0,sizeof(ptr));
    NumFloat2Char(&(*p),&p->DP.Num,ptr);
    AddUint2Char((*p),ptr,9);
    LCD_DisplayNumLine_EN_CH_LJ_Home(&p->DP,xR,yR,ptr,FontType);
}
//��������
void page_Protect(void)
{
    if(SampleTimeCur.CAM>=SampleMaxCam)
        SampleTimeCur.CAM = 0;
    if(ZoneCur.CAM>=ZoneMaxCam)
        ZoneCur.CAM = 0;
}

void page_graph(const struct CUR CURV)
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
    LCD_FillRectLJ(xR-1, yR-1, 639-460, 26*13,LCD_COLOR_BLACK); //����ұ߿�
    BackColor=LCD_GetBackColor();

    LCD_DisplayStringLine_EN_CH_LJ_WithFont(454, 403, (uint8_t* )SampletimeX[SampleTimeCur.CAM],&Font16);
	if(CURV.PAGE == 0)
	{
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
			if((COLOld!=CURV.COL&&CURV.COL>0&&CURV.COL<=10)||ZoneCAMold!=ZoneCur.CAM)
			{
				memset(ptr,0,sizeof(ptr));
				strncpy(ptr,"         \0",sizeof(ptr)) ;
				Yoff =  ((COLOld-1)%ZoneData[ ZoneCAMold ])*(PointsPiy/ ZoneData[ ZoneCAMold ]);
				Yoffup =  (PointsPiy/ ZoneData[ ZoneCAMold ]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[COLOld-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[COLOld-1]);
				lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
				if(CURV.COL>0&&CURV.COL<=10)
					COLOld = CURV.COL;
				ZoneCAMold = ZoneCur.CAM;
				// if(320-17-Yoff-Yoffup<0)
				//     LCD_DrawRectLJ(x-2+50, y-20, PointsPix, 1,LCD_COLOR_WHITE);
			}

			DDa[i-1][0].DP.dispold = 0;      //���� �� .DP.dispold == 0  ��disp== 1�Ż�ˢ��

	//��ʾ�ұ߿���
			if(i>0&&i<10&&CURV.COL!=i)
			{
				sprintf(str,"%d  ",i);
				BackColor=LCD_GetBackColor();
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i -1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else if(i==10&&CURV.COL!=i)
			{
				sprintf(str,"%d ",i);
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else  if(i>0&&i<10&&CURV.COL==i)
			{
				TextColor=LCD_GetTextColor();
				LCD_SetTextColor(LCD_COLOR_BLACK);

				LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //�������ɫ
				LCD_SetTextColor( TextColor);
				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);

				Yoff =  ((i-1)%ZoneData[ ZoneCur.CAM])*(PointsPiy/ ZoneData[ ZoneCur.CAM]);
				Yoffup =  (PointsPiy/ ZoneData[ ZoneCur.CAM]);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],3);
				else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],1);
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].Up[DDa[i-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i-1][0],ptr,7);

				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[i-1]);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i-1][0].Low[DDa[i-1][0].vcind],3);
				else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],1);
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].Low[DDa[i-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i-1][0],ptr,7);

				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[i-1]);
			}
			else if(i==10&&CURV.COL==i)
			{
				TextColor=LCD_GetTextColor();
				LCD_SetTextColor(LCD_COLOR_BLACK);
				LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //�������ɫ
				LCD_SetTextColor( TextColor);

				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				Yoff =  ((i-1)%ZoneData[ ZoneCur.CAM])*(PointsPiy/ ZoneData[ ZoneCur.CAM]);
				Yoffup =  (PointsPiy/ ZoneData[ ZoneCur.CAM]);
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],3);
				else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],1);

				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].Up[DDa[i-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i-1][0],ptr,7);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[i-1]);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i-1][0].Low[DDa[i-1][0].vcind],3);
				else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
					SwitchChanalData(&DDa[i-1][0].Low[DDa[i-1][0].vcind],1);
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].Low[DDa[i-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i-1][0],ptr,7);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[i-1]);
			}
			else if(i==11&&CURV.COL!=i) //SAMPLE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
				if(MDataF==ON)//���ڿ��״̬
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )("5ms"),&Font20);
				else
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCur.CAM]),&Font20);
			}

			else  if(i==11&&CURV.COL==i)
			{

				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
				if(MDataF==ON)//���ڿ��״̬
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )("5ms"),&Font24);
				else
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCur.CAM]),&Font24);
			}
			else if(i==12&&CURV.COL!=i)//ZONE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font20);
			}
			else  if(i==12&&CURV.COL==i)
			{

				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ
				TextColor=LCD_GetTextColor();
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font24);
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
	}else if(CURV.PAGE == 1){
		for(i=1; i<3; i++)
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
			if((COLOld!=CURV.COL&&CURV.COL>0&&CURV.COL<=10)||ZoneCAMold!=ZoneCur.CAM)
			{
				memset(ptr,0,sizeof(ptr));
				strncpy(ptr,"         \0",sizeof(ptr)) ;
				Yoff =  ((COLOld-1)%ZoneData[ ZoneCAMold ])*(PointsPiy/ ZoneData[ ZoneCAMold ]);
				Yoffup =  (PointsPiy/ ZoneData[ ZoneCAMold ]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[COLOld-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[COLOld-1]);
				lcd_Grids(x-2+50,y+2,20,20,21,21,LCD_COLOR_VERYDARKGRAY,1);
				if(CURV.COL>0&&CURV.COL<=10)
					COLOld = CURV.COL;
				ZoneCAMold = ZoneCur.CAM;
				// if(320-17-Yoff-Yoffup<0)
				//     LCD_DrawRectLJ(x-2+50, y-20, PointsPix, 1,LCD_COLOR_WHITE);
			}

			DDa[i+11-1][0].DP.dispold = 0;      //���� �� .DP.dispold == 0  ��disp== 1�Ż�ˢ��

	//��ʾ�ұ߿���
			if(i>0&&i<10&&CURV.COL!=i)
			{
				sprintf(str,"%d ",i+10);
				BackColor=LCD_GetBackColor();
				if(ChanData.chalarm[i+11-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i+11-1][0],&DDa[i+11-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i+11-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+11 -1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else if(i==10&&CURV.COL!=i)
			{
				sprintf(str,"%d ",i+10);
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else  if(i>0&&i<10&&CURV.COL==i)
			{
				TextColor=LCD_GetTextColor();
				LCD_SetTextColor(LCD_COLOR_BLACK);

				LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //�������ɫ
				LCD_SetTextColor( TextColor);
				sprintf(str,"%d  CH%d",i+10,i+10);
//				sprintf(str,"%d  %6s",i+10,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i+11-1][0],&DDa[i+11-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i+11-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+11-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);

				Yoff =  ((i-1)%ZoneData[ ZoneCur.CAM])*(PointsPiy/ ZoneData[ ZoneCur.CAM]);
				Yoffup =  (PointsPiy/ ZoneData[ ZoneCur.CAM]);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i+11-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i+11-1][0].Up[DDa[i+11-1][0].vcind],3);
				else if(DDa[i+11-1][0].vcind==VR||DDa[i+11-1][0].vcind==VI||DDa[i+11-1][0].vcind==VG)
					SwitchChanalData(&DDa[i+11-1][0].Up[DDa[i+11-1][0].vcind],1);
				NumFloat2Char(&DDa[i+11-1][0],&DDa[i+11-1][0].Up[DDa[i+11-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i+11-1][0],ptr,7);

				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[i-1]);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i+11-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i+11-1][0].Low[DDa[i+11-1][0].vcind],3);
				else if(DDa[i+11-1][0].vcind==VR||DDa[i+11-1][0].vcind==VI||DDa[i+11-1][0].vcind==VG)
					SwitchChanalData(&DDa[i+11-1][0].Up[DDa[i+11-1][0].vcind],1);
				NumFloat2Char(&DDa[i+11-1][0],&DDa[i+11-1][0].Low[DDa[i+11-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i+11-1][0],ptr,7);

				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[i-1]);
			}
			else if(i==10&&CURV.COL==i)
			{
				TextColor=LCD_GetTextColor();
				LCD_SetTextColor(LCD_COLOR_BLACK);
				LCD_FillRect(xR+1, yR+1, 639-460-2, 22); //�������ɫ
				LCD_SetTextColor( TextColor);

				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				if(ChanData.chalarm[i-1]==0)
				{
					LCD_SetBackColor(colors[i-1]);
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
					LCD_SetBackColor(BackColor);
				}

				Yoff =  ((i-1)%ZoneData[ ZoneCur.CAM])*(PointsPiy/ ZoneData[ ZoneCur.CAM]);
				Yoffup =  (PointsPiy/ ZoneData[ ZoneCur.CAM]);
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+10,yR+5+25,ptr,Font20);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],3);
				else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
					SwitchChanalData(&DDa[i-1][0].Up[DDa[i-1][0].vcind],1);

				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].Up[DDa[i-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i-1][0],ptr,7);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff-Yoffup,(uint8_t* )ptr,&Font16,colors[i-1]);
				memset(ptr,0,sizeof(ptr));
				if(DDa[i-1][0].vcind==VDC)
					SwitchChanalData(&DDa[i-1][0].Low[DDa[i-1][0].vcind],3);
				else if(DDa[i-1][0].vcind==VR||DDa[i-1][0].vcind==VI||DDa[i-1][0].vcind==VG)
					SwitchChanalData(&DDa[i-1][0].Low[DDa[i-1][0].vcind],1);
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].Low[DDa[i-1][0].vcind],ptr);//��ʾ����
				AddUint2CharUnit(DDa[i-1][0],ptr,7);
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(32+StrPiX,PointsPiy1+y-17-Yoff,(uint8_t* )ptr,&Font16,colors[i-1]);
			}
			else if(i==11&&CURV.COL!=i) //SAMPLE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
				if(MDataF==ON)//���ڿ��״̬
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )("5ms"),&Font20);
				else
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCur.CAM]),&Font20);
			}

			else  if(i==11&&CURV.COL==i)
			{

				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
				if(MDataF==ON)//���ڿ��״̬
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )("5ms"),&Font24);
				else
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCur.CAM]),&Font24);
			}
			else if(i==12&&CURV.COL!=i)//ZONE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font20);
			}
			else  if(i==12&&CURV.COL==i)
			{

				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ
				TextColor=LCD_GetTextColor();
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font24);
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
}

void page_graph_init(const struct CUR CURV)
{
    static uint16 xR=10;
    static uint16 yR=59;
    static uint16 HdivR=90;
    static uint16 HdivR_Sel=90;
    static uint16 i;
    static char str[10];
    static uint32 BackColor;
    static char ptr[10];
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    page_CH(CURV);
    if(CHI == DataSave.Data_type.LANG )
    {
        COMmenuInit(DataSave.Data_type.LANG,"ȫͨ��","������ʾ","����","��ͨ��","�ļ�",2);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        COMmenuInit(DataSave.Data_type.LANG,"10CH","DATA","GRAPH","SET","3CH",2);
    }
    LCD_DisplayStringLine_EN_CH_LJ_WithFont(454, 403, (uint8_t* )SampletimeX[SampleTimeCur.CAM],&Font16);
    xR=460;
    yR=4+24;
    HdivR=20;
    LCD_DrawRectLJ(xR,  yR, 639-460, HdivR,LCD_COLOR_WHITE);
    LCD_FillRectLJ(xR+1, yR+1, 639-460-2, HdivR-2,LCD_COLOR_BLUE); //�������ɫ
    BackColor=LCD_GetBackColor();
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+50,  yR+2, "MONITOR",&Font16);
    LCD_SetBackColor(BackColor);
    xR=460;
    yR=4+HdivR+24;
    HdivR=26;
    HdivR_Sel=HdivR*2;
    BackColor=LCD_GetBackColor();
	
	if(CURV.PAGE == 0)
	{
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
				page_string(&DDa[i-1][0],xR+1+50,yR+5,Font16);
			}
			else if(i==10&&CURV.COL!=i)
			{
				sprintf(str,"%d ",i);
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else  if(i>0&&i<10&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ
				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				page_string(&DDa[i-1][0],xR+1+10,yR+5+25,Font16);
			}
			else if(i==10&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ

				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				page_string(&DDa[i-1][0],xR+1+10,yR+5+25,Font16);
			}
			else if(i==11&&CURV.COL!=i) //SAMPLE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
				if(MDataF==ON)//���ڿ��״̬
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )("5ms"),&Font20);
				else
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCur.CAM]),&Font20);

			}
			else  if(i==11&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //�������ɫ
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCur.CAM]),&Font24);

			}
			else if(i==12&&CURV.COL!=i)//ZONE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font20);

			}

			else  if(i==12&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //�������ɫ
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font24);
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
	}else if(CURV.PAGE == 1){
		for(i=1; i<3; i++)
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
			if(i>0&&i<3&&CURV.COL!=i)
			{
				sprintf(str,"%d  ",i+10);
				BackColor=LCD_GetBackColor();
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				page_string(&DDa[i+11-1][0],xR+1+50,yR+5,Font16);
			}
			else if(i==10&&CURV.COL!=i)
			{
				sprintf(str,"%d ",i);
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+3,yR+3,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				memset(ptr,0,sizeof(ptr));
				NumFloat2Char(&DDa[i-1][0],&DDa[i-1][0].DP.Num,ptr);
				AddUint2Char(DDa[i-1][0],ptr,9);
				LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i-1][0].DP,xR+1+50,yR+5,ptr,Font16);
			}
			else  if(i>0&&i<10&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ
				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				page_string(&DDa[i-1][0],xR+1+10,yR+5+25,Font16);
			}
			else if(i==10&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK);//�������ɫ

				sprintf(str,"%d  %6s",i,DataSave.Data_type.ChName[i-1]);
				BackColor=LCD_GetBackColor();
				LCD_SetBackColor(colors[i-1]);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )str,&Font20);
				LCD_SetBackColor(BackColor);
				page_string(&DDa[i-1][0],xR+1+10,yR+5+25,Font16);
			}
			else if(i==11&&CURV.COL!=i) //SAMPLE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"SAMPLE",&Font20);
				if(MDataF==ON)//���ڿ��״̬
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )("5ms"),&Font20);
				else
					LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+90,yR+3,(uint8_t* )(&Sampletime[SampleTimeCur.CAM]),&Font20);

			}
			else  if(i==11&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //�������ɫ
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"SAMPLE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&Sampletime[ SampleTimeCur.CAM]),&Font24);

			}
			else if(i==12&&CURV.COL!=i)//ZONE
			{
				sprintf(str,"%d  ",i);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+3,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+70,yR+3,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font20);

			}

			else  if(i==12&&CURV.COL==i)
			{
				LCD_FillRectLJ(xR+1, yR+1, 639-460-2, 22,LCD_COLOR_BLACK); //�������ɫ
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1,yR+1,(uint8_t* )"ZONE",&Font20);
				LCD_DisplayStringLine_EN_CH_LJ_WithFont(xR+1+10,yR+3+25,(uint8_t* )(&ZoneC[ZoneCur.CAM]),&Font24);
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
}

//������
void lcd_Grids(u16 x1,u16 y1,u16 HSpace,u16 VSpace,u8 NumOfRows,u8 NumOfColumns,u32 BorderColor,u8 span)
{
    u16 xsta,ysta,xend,yend;
    u8 i;
    uint32 TextColor;
    TextColor=LCD_GetTextColor();
    LCD_SetTextColor(BorderColor);
    //����ˮƽ��
    for(i=0; i<NumOfColumns; i++)
    {
        xsta=x1;
        ysta=y1+VSpace*i;
        xend=x1+HSpace*(NumOfRows-1);
        yend=y1+VSpace*i;
        if((i%2)!=0)
            lcd_line(xsta,ysta,xend,yend,span,BorderColor);
    }
    //���ƴ�ֱ��
    for(i=0; i<NumOfRows; i++)
    {
        xsta=x1+HSpace*i;
        ysta=y1;
        xend=x1+HSpace*i;
        yend=y1+VSpace*(NumOfColumns-1);
        if((i%2)!=0)
            lcd_line(xsta,ysta,xend,yend,span,BorderColor);
    }

    for(i=0; i<NumOfColumns; i++)
    {
        xsta=x1;
        ysta=y1+VSpace*i;
        xend=x1+HSpace*(NumOfRows-1);
        yend=y1+VSpace*i;
        if((i%2)==0)
            lcd_line(xsta,ysta,xend,yend,span,LCD_COLOR_GRAY1);
    }
    for(i=0; i<NumOfRows; i++)
    {
        xsta=x1+HSpace*i;
        ysta=y1;
        xend=x1+HSpace*i;
        yend=y1+VSpace*(NumOfColumns-1);
        if((i%2)==0)
            lcd_line(xsta,ysta,xend,yend,span,LCD_COLOR_GRAY1);
    }
    LCD_SetTextColor( TextColor);
}

//��ʵ�߻�����
//x1,y1:�������
//x2,y2:�յ�����
//Span:����
void lcd_line(u16 x1, u16 y1, u16 x2, u16 y2,u8 Span,u32 BorderColor)
{
    u16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //������������
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //���õ�������
    else if(delta_x==0)incx=0;//��ֱ��
    else
    {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//ˮƽ��
    else
    {
        incy=-1;
        delta_y=-delta_y;
    }
    if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
    else distance=delta_y;

    for(t=0; t<=distance+1; t++ ) //�������
    {
        if((uRow<=x2)&(uCol<=y2))
        {
            LCD_DrawPixel(uRow,uCol,BorderColor);//����
        }
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=(incx+Span);
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=(incy+Span);
        }
    }
}


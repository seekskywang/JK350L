/*
  * @file    Work.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   主设置界面的业务逻辑
 */
#include "include.h"
//任务控制块
OS_TCB DoWorkTaskTCB;
//任务堆栈
CPU_STK DoWork_TASK_STK[DoWork_STK_SIZE];
struct RDispPara DisInPut[DiaTEMPCOL][DiaTEMPCAM] = {0}; //子对话框温度对话框的控键矩阵
struct RDispPara DisDC[DiaDCCOL][DiaDCCAM] = {0};//子对话框电压对话框的控键矩阵
struct RDispPara DisDI[DiaDICOL][DiaDICAM] = {0};//子对话框电流对话框的控键矩阵
struct RDispPara DisVG[DiaVGCOL][DiaVGCAM] = {0};//子对话框重力对话框的控键矩阵
struct RDispPara DisVR[DiaVRCOL][DiaVRCAM] = {0};//子对话框重力对话框的控键矩阵

static   char disp[HCOL][HCAM];                 //是否显示
static   char dispold[HCOL][HCAM];                 //是否显示

//各通道滤波器选择
char FilerIndex[CHANNUM] = {0};
//滤波器的跨度 0是关闭状态
const int FilterVal[6] = {0,2,5,10,20,40};
uint8 HidFlag = OFF;
uint32 HidCnt = 0;
uint32 HidCntOld = 0;
//绘制U盘连接图标flag==ON 强行刷新
void DrawUdisk(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag)
{
    uint32 BackColor;
    uint32 TextColor;
    OS_ERR err;
    static uint8_t OnOffVal = 3;
    static uint16_t XposOld = 0;
    static uint16_t YposOld = 0;
    if(OnOffVal!=OnOff||XposOld!=Xpos||YposOld!=Ypos||flag==ON)
    {
        OSSchedLock(&err);
        TextColor = LCD_GetTextColor();
        BackColor = LCD_GetBackColor();
        LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
        if(OnOff==ON)
        {

            LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"丙",FONTTYPE24);
        }
        else
        {
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"凶",FONTTYPE24);
        }
        LCD_SetColors(TextColor,BackColor);
        OSSchedUnlock(&err);
        OnOffVal=OnOff;
        XposOld=Xpos;
        YposOld=Ypos;
    }
}

void DrawWIFI(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag)
{
    uint32 BackColor;
    uint32 TextColor;
    OS_ERR err;
    static uint8_t OnOffVal = 3;
    static uint16_t XposOld = 0;
    static uint16_t YposOld = 0;
    if(OnOffVal!=OnOff||XposOld!=Xpos||YposOld!=Ypos||flag==ON)
    {
        TextColor = LCD_GetTextColor();
        BackColor = LCD_GetBackColor();
        OSSchedLock(&err);
        if(OnOff==ON)
        {
            LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"尸",FONTTYPE24);
        }
        else
        {
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"丸",FONTTYPE24);

        }
        LCD_SetColors(TextColor,BackColor);
        OSSchedUnlock(&err);
        OnOffVal=OnOff;
        XposOld=Xpos;
        YposOld=Ypos;
    }
}


void DrawBat(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff1,uint8_t flag)
{
    uint32 BackColor;
    uint32 TextColor;
    OS_ERR err;
    static uint8_t OnOffCnt = 0;
    static uint8_t OnOffVal = 3;
    static uint16_t XposOld = 0;
    static uint16_t YposOld = 0;
    uint8 flagChange = 0;
    uint8_t OnOff = 0x7&OnOff1;
    if((0x40&OnOff1)==0x40)
    {
        flagChange = 1;//在充电
        OnOffCnt++;
    }
    if(flagChange==1)
    {

        OSSchedLock(&err);
        TextColor = LCD_GetTextColor();
        BackColor = LCD_GetBackColor();
        if(OnOffCnt==1)  //没电
        {
            LCD_SetColors(LCD_COLOR_VERYDARKCYAN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"乙",FONTTYPE24);
        }
        else if(OnOffCnt==3)
        {
            LCD_SetColors(LCD_COLOR_VERYDARKCYAN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"帅",FONTTYPE24);

        }
        else if(OnOffCnt==5)
        {
            LCD_SetColors(LCD_COLOR_VERYDARKCYAN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"丁",FONTTYPE24);
        }
        else if(OnOffCnt==7)
        {
            LCD_SetColors(LCD_COLOR_VERYDARKCYAN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"川",FONTTYPE24);
        }
        else if(OnOffCnt==9)
        {
            LCD_SetColors(LCD_COLOR_VERYDARKCYAN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"卜",FONTTYPE24);
        }
        else if(OnOffCnt==11)
        {
            if(OnOff==0)  //没电
            {
                LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"乙",FONTTYPE24);
            }
            else if(OnOff==2||OnOff==1)
            {
                LCD_SetColors(LCD_COLOR_DARKYELLOW,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"帅",FONTTYPE24);

            }
            else if(OnOff==3)
            {
                LCD_SetColors(LCD_COLOR_DARKYELLOW,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"丁",FONTTYPE24);
            }
            else if(OnOff==4)
            {
                LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"川",FONTTYPE24);
            }
            else if(OnOff==5)
            {
                LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"卜",FONTTYPE24);
            }
        }
        else if(OnOffCnt>=19)
        {
            OnOffCnt = 0;
        }
        LCD_SetColors(TextColor,BackColor);
        OSSchedUnlock(&err);
    }
    else
    {
        if(OnOffVal!=OnOff||XposOld!=Xpos||YposOld!=Ypos||flag==ON)
        {

            OSSchedLock(&err);
            TextColor = LCD_GetTextColor();
            BackColor = LCD_GetBackColor();
            if(OnOff==0)  //没电
            {
                LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"乙",FONTTYPE24);
            }
            else if(OnOff==2||OnOff==1)
            {
                LCD_SetColors(LCD_COLOR_DARKYELLOW,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"帅",FONTTYPE24);

            }
            else if(OnOff==3)
            {
                LCD_SetColors(LCD_COLOR_DARKYELLOW,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"丁",FONTTYPE24);
            }
            else if(OnOff==4)
            {
                LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"川",FONTTYPE24);
            }
            else if(OnOff==5)
            {
                LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
                LCD_DisplayStringLineLJ(Xpos,Ypos,"卜",FONTTYPE24);
            }
            LCD_SetColors(TextColor,BackColor);
            OSSchedUnlock(&err);
            OnOffVal=OnOff;
            XposOld=Xpos;
            YposOld=Ypos;
        }
    }
}



void DrawSave(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag)
{
    uint32 BackColor;
    uint32 TextColor;
    OS_ERR err;
    static uint8_t OnOffVal = 3;
    static uint16_t XposOld = 0;
    static uint16_t YposOld = 0;
    static char buf[6]= {0};
    if(OnOffVal!=OnOff||XposOld!=Xpos||YposOld!=Ypos||flag==ON)
    {
        TextColor = LCD_GetTextColor();
        BackColor = LCD_GetBackColor();
        OSSchedLock(&err);
        if(OnOff == FILETFOpen||OnOff == FILEUSBOpen)
        {
            
            memset(buf,0,sizeof(buf));
			if(OnOff == FILETFOpen)
			{
				sprintf((char *)buf,"NB%0.2d \0",FileHao);
				LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
				LCD_DisplayStringLineLJ(Xpos,Ypos,"弓",FONTTYPE24);
			}
			else if(OnOff == FILEUSBOpen)
			{
				strcpy(buf,"UDisk\0");
				LCD_SetColors(LCD_COLOR_VERYDARKCYAN,LCD_COLOR_BLACK);
				LCD_DisplayStringLineLJ(Xpos,Ypos,"弓",FONTTYPE24);
			}
            switch(DataSave.Data_type.PageFlag)
            {
                case FPageCh ://进入小字体数据展示
                    LCD_SetFont(&Font16);
                    LCD_DisplayStringLine_EN_CH_LJ(180,10, (uint8_t *)buf);
                    break;
                case FPageDisplayData://数据展示界面
                    LCD_SetFont(&Font16);
                    LCD_DisplayStringLine_EN_CH_LJ(180,10, (uint8_t *)buf);
                    break;
                case FPageCh3 ://进入大字体数据展示
                    LCD_SetFont(&Font16);
                    LCD_DisplayStringLine_EN_CH_LJ(180,10, (uint8_t *)buf);
                    break;
                case FPageGraph  ://
                    LCD_SetFont(&Font16);
                    LCD_DisplayStringLine_EN_CH_LJ(454+120, 403, (uint8_t *)buf);
                    break;
                case FPageFile  ://
                    LCD_SetFont(&Font16);
                    LCD_DisplayStringLine_EN_CH_LJ(340,10, (uint8_t *)buf);
                    break;


            }

        }
        else if(OnOff==SDERR)
        {
            LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"弓",FONTTYPE24);
        }
        else
        {
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"弓",FONTTYPE24);

        }
        LCD_SetColors(TextColor,BackColor);
        OSSchedUnlock(&err);
        OnOffVal=OnOff;
        XposOld=Xpos;
        YposOld=Ypos;
    }
}


void DrawHid(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag)
{
    uint32 BackColor;
    uint32 TextColor;
    OS_ERR err;
    static uint8_t OnOffVal = 3;
    static uint16_t XposOld = 0;
    static uint16_t YposOld = 0;
    if(OnOffVal!=OnOff||XposOld!=Xpos||YposOld!=Ypos||flag==ON)
    {
        TextColor = LCD_GetTextColor();
        BackColor = LCD_GetBackColor();
        OSSchedLock(&err);
        if(OnOff==ON)
        {
            LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"女",FONTTYPE24);
        }
        else
        {
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(Xpos,Ypos,"女",FONTTYPE24);

        }
        LCD_SetColors(TextColor,BackColor);
        OSSchedUnlock(&err);
        OnOffVal=OnOff;
        XposOld=Xpos;
        YposOld=Ypos;
    }
}

//键值扫描任务
//ALL_ReTurn   非堵塞空键值也会返回键值
//NoNull_ReTurn  会产生堵塞非空才返回键值15ms
uint8 KeyTaskCreate(uint8 key_ReTurn)
{
    OS_ERR err;
    uint8 keytmp=0;
    KEY_ReTurn  = key_ReTurn;
    OSTaskCreate((OS_TCB*     )&KEY0TaskTCB,
                 (CPU_CHAR*   )"KEY0 task",
                 (OS_TASK_PTR )KEY0_task,
                 (void*       )0,
                 (OS_PRIO     )KEY0_TASK_PRIO,
                 (CPU_STK*    )&KEY0_TASK_STK[0],
                 (CPU_STK_SIZE)KEY0_STK_SIZE/10,
                 (CPU_STK_SIZE)KEY0_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void*       )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
    OSSemPend(&Key_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
    keytmp=KEY_value;
    KEY_value = KEY_NULL;//清空键值
    OSTaskDel(&KEY0TaskTCB,&err);
    return keytmp;
}

//设置菜单任务
void DoWork_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //主页
    Stu_home_Init();
    page_home();
    while(1)
    {
        //DoWork任务
//        BluidICOTask();
        keytmp = KeyTaskCreate(NoNull_ReTurn);
//       OSTaskDel(&ICOTaskTCB,&err);
        keytmp=Modify(keytmp,&CurDisp,&CurDispOld);     //处理键值
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
        }
        page_home_cycle(CurDisp);
    }
}
//切换界面
void SWManu(void)
{
    uint8 m;
    uint8 n;
    if(DataSave.Data_type.SW==0)
    {
        for(m=0; m<HCOL+2; m++)
            for(n=0; n<HCAM; n++)
            {
                disp[m][n] = DisLog[m][n].disp;
                dispold[m][n] = DisLog[m][n].dispold;
            }
    }
    DataSave.Data_type.SW^=1;
    for(m=0; m<HCOL+2; m++)
        for(n=0; n<HCAM; n++)
        {

            if(DataSave.Data_type.SW==1)
            {
                if(m>0&&n>0&&n<HCAM-1)
                {
                    DisLog[m][n].disp=0;
                    DisLog[m][n].dispold=0;
                }
            }
            else
            {
                DisLog[m][n].disp=disp[m][n] ;
                DisLog[m][n].dispold=dispold[m][n] ;
            }
        }
    Stu_Mid_Init();
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    page_Mindle();
    if(CHI == DataSave.Data_type.LANG )
    {
        if(DataSave.Data_type.SW==0)
        {
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(2,5, (uint8_t *)"显示设置",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
            LCD_DisplayStringLineLJ(2+96,5, (uint8_t *)"仆报警范围",FONTTYPE24);
        }
        else
        {

            LCD_DisplayStringLineLJ(2,5, (uint8_t *)"显示设置仆",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(2+120,5, (uint8_t *)"报警范围",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
        }
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        if(DataSave.Data_type.SW==0)
        {
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(3,5, (uint8_t *)"DISPLAY",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
            LCD_DisplayStringLineLJ(2+112,5, (uint8_t *)"仆RANG",FONTTYPE24);
        }
        else
        {

            LCD_DisplayStringLineLJ(3,5, (uint8_t *)"DISPLAY仆",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(2+136,5, (uint8_t *)"RANG",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
        }
    }

}


void SaveAll(void)
{
    uint8 m,n;

    for(m=0; m<HCOL+2; m++)
        for(n=0; n<HCAM; n++)
        {
            if(n<HCAM-2&&m<HCOL-1)
            {
                DataSave.Data_type.DisLog[m][n].cind=DisLog[m][n].cind;
                memcpy(DataSave.Data_type.DisLog[m][n].index,DisLog[m][n].index,sizeof(DisLog[m][n].index));
            }else if(n<HCAM-2&&m<HCOL+2&&m>HCOL-1){
				memcpy(DataSave.Data_type.DisLog[m-1][n].index,DisLog[m][n].index,sizeof(DisLog[m][n].index));
				
			}
        }
}


void SaveCOL1(struct CUR* cur)
{
    uint8 m,n;
	if(cur->PAGE == 0)
	{
		if(cur->COL==1)//如果是第一行的
		{
			for(m=0; m<HCOL; m++)
				for(n=0; n<HCAM; n++)
				{
					if(n<HCAM-2&&m<HCOL-2)
					{
						DataSave.Data_type.DisLog[m][n].cind=DisLog[m][n].cind;
						memcpy(DataSave.Data_type.DisLog[m][n].index,DisLog[m][n].index,sizeof(DisLog[m][n].index));
					}
				}
		}
		else
		{
			DataSave.Data_type.DisLog[cur->COL][cur->CAM].cind=DisLog[cur->COL][cur->CAM].cind;
			memcpy(DataSave.Data_type.DisLog[cur->COL][cur->CAM].index,DisLog[cur->COL][cur->CAM].index,sizeof(DisLog[cur->COL][cur->CAM].index));
		}
	}else if(cur->PAGE == 1){
		DataSave.Data_type.DisLog[cur->COL+11][cur->CAM].cind=DisLog[cur->COL+12][cur->CAM].cind;
		memcpy(DataSave.Data_type.DisLog[cur->COL+11][cur->CAM].index,DisLog[cur->COL+12][cur->CAM].index,sizeof(DisLog[cur->COL+12][cur->CAM].index));
	}
    SaveAllPara();
}

void SetChanFilter(void)
{
    uint32 i=0;
    for(i=0; i<12; i++)
    {
        FilerIndex[i] = DisLog[2+i][3].index[4];
    }
}
uint8 Modify(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // 键值匹配函数，在while大循环中反复匹配
{
    OS_ERR err;
    *curold=*cur;
    cur->OPN=keytmp;                        //将键值赋给操作标志，标明当前的操作状态
    switch(cur->OPN)
    {
        case KEY_FUNC1://进入显示设置
            break;
        case KEY_FUNC2://进入data 处理
            DataSave.Data_type.PageFlag =  FPageHDATA ;
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageFile ;
            break;
        case KEY_FUNC4:// 接口
            DataSave.Data_type.PageFlag =  FPageIF ;
            break;
        case KEY_FUNC5://进入设置
            DataSave.Data_type.PageFlag =  FPageOtur  ;
            break;
        case      KEY_ESC:
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
            do
            {
				if(cur->PAGE == 0)
				{
					if(cur->COL>0)
						cur->COL--;
					else
						cur->COL=HCOL-2;
				}else if(cur->PAGE == 1){
					if(cur->COL>1)
					{
						cur->COL--;
					}
					else{
						cur->COL=HCOL-2;
						cur->PAGE=0;
						Stu_Mid_Init();
						page_home();
					}
				}
            }
            while(DisLog[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            break;
        case KEY_DOWN:
            do
            {
				if(cur->PAGE == 0)
				{
					if(cur->COL<HCOL-2) //最后一行是功能键
					{
						cur->COL++;
					}else{
						cur->PAGE=1;
						cur->COL=1;
						Stu_Mid_Init();
						page_home();
					}
				}else if(cur->PAGE == 1){
					if(cur->COL<2) //最后一行是功能键
					{
						cur->COL++;
					}else{
						cur->PAGE=0;
						cur->COL=0;
						Stu_Mid_Init();
						page_home();
					}
				}
            }
            while(DisLog[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            break;
        case KEY_ACC:
            do
            {
                cur->COL+=2;
                if(cur->COL>HCOL-2)
                    cur->COL=1;
            }
            while(DisLog[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            break;
        case KEY_LEFT://
            if(cur->CAM==0&&cur->COL==0)  //切换按钮
            {
                SWManu();
            }
            else
            {
                do
                {
                    if(cur->CAM>0)
                        cur->CAM--;
                    else
                        cur->CAM=HCAM-1;
                }
                while(DisLog[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            }
            break;
        case KEY_RIGHT:
            if(cur->CAM==0&&cur->COL==0)  //切换按钮
            {
                SWManu();
            }
            else
            {
                do
                {
                    if(cur->CAM<HCAM-1) //最后一行是功能键
                        cur->CAM++;
                    else
                        cur->CAM=0;
                }
                while(DisLog[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            }
            break;
        case KEY_ENTER:
			if(cur->PAGE == 0)
			{
				if(cur->CAM==0&&cur->COL>0&&cur->COL<=HCOL-2)  //第一列
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
//						if(cur->PAGE == 0 && cur->COL < 11)
//						{
//							InPutManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
//							OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
//							SetChanSetMod();
//							SendSetMod(ChanSet);
//							OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
//							SaveCOL1(cur);
//							OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
//						}
					}
				}
				else if(cur->CAM==1&&cur->COL>0&&cur->COL<=HCOL-2)
				{
					if(DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VG) //重力 
					{
						VRVGDISensorManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
						SetChanSetMod();
						SendSetMod(ChanSet);
						OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
						SaveCOL1(cur);
						OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
					}
					else if(DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VR) //湿度
					{
						if(cur->PAGE == 0 && cur->COL < 12)
						{
							H3SensorManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
							OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
							SetChanSetMod();
							SendSetHMod(ChanHSet);
							OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
							SaveCOL1(cur);
							OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
						}
					}
					else if(DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VI) //电流
					{
						VRVGDISensorManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
						SetChanSetMod();
						SendSetMod(ChanSet);
						OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
						SaveCOL1(cur);
						OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
					}
					else
					{
						SensorManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
						SetChanSetMod();
						SendSetMod(ChanSet);
						OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
						SaveCOL1(cur);
						OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);

					}

				}
				else if(cur->CAM==2&&cur->COL>0&&cur->COL<=HCOL-2)
				{
					RangeManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
					SetChanSetMod();
					SendSetMod(ChanSet);
					OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
					SaveCOL1(cur);
					OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
				}
				else if(cur->CAM==3&&cur->COL>0&&cur->COL<=HCOL-2)
				{
					//滤波器选择
					FilterManu(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					SetChanFilter();
					SaveCOL1(cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VDC)//DC
				{
					DCDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VT)//TEMP
				{
					TEMPDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VI)//I
				{
					DIDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VG)//G
				{
					VGDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VR)//R
				{
					VRDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VDC)//DC
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						DCRangeDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
					else
					{
						DCLimitDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VT)//TEMP
				{
					if(cur->PAGE == 0)
					{
						if(DataSave.Data_type.SW==0) //非报警模式显示
						{
							TEMPRangeDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						}
						else
						{
							TEMPLimitDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						}
					}else if(cur->PAGE == 1){
						if(DataSave.Data_type.SW==0) //非报警模式显示
						{
							VRRangeDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						}
						else
						{
							VRLimitDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						}
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VI)//I
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						DIRangeDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
					else
					{
						DILimitDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VG)//G
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						VGRangeDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
					else
					{
						VGLimitDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL][0].index[DisLog[cur->COL][0].cind]==VR)//R
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						if(cur->PAGE == 0)
						{
							VRRangeDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
						}else if(cur->PAGE == 1){
							VRRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						}
						
					}
					else
					{
						VRLimitDia(keytmp,&DisLog[cur->COL][cur->CAM],cur);
					}
				}
			}else if(cur->PAGE == 1){
				if(cur->CAM==0&&cur->COL>0&&cur->COL<=HCOL-2)  //第一列
				{
//					if(DataSave.Data_type.SW==0) //非报警模式显示
//					{
//						if(cur->PAGE == 1 && cur->COL < 11)
//						{
//							InPutManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
//							OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
//							SetChanSetMod();
//							SendSetMod(ChanSet);
//							OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
//							SaveCOL1(cur);
//							OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
//						}
//					}
				}
				else if(cur->CAM==1&&cur->COL>0&&cur->COL<=HCOL-2)
				{
					if(DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VG) //重力 
					{
						VRVGDISensorManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
						SetChanSetMod();
						SendSetMod(ChanSet);
						OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
						SaveCOL1(cur);
						OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
					}
					else if(DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VR) //湿度
					{
						if(cur->PAGE == 1 && cur->COL < 11)
						{
							H3SensorManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
							OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
							SetChanSetMod();
							SendSetHMod(ChanHSet);
							OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
							SaveCOL1(cur);
							OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
						}
					}
					else if(DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VI) //电流
					{
						VRVGDISensorManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
						SetChanSetMod();
						SendSetMod(ChanSet);
						OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
						SaveCOL1(cur);
						OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
					}
					else
					{
						SensorManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
						SetChanSetMod();
						SendSetMod(ChanSet);
						OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
						SaveCOL1(cur);
						OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);

					}

				}
				else if(cur->CAM==2&&cur->COL>0&&cur->COL<=HCOL-2)
				{
					RangeManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//挂起任务
					SetChanSetMod();
					SendSetMod(ChanSet);
					OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
					SaveCOL1(cur);
					OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
				}
				else if(cur->CAM==3&&cur->COL>0&&cur->COL<=HCOL-2)
				{
					//滤波器选择
					FilterManu(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					SetChanFilter();
					SaveCOL1(cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VDC)//DC
				{
					DCDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VT)//TEMP
				{
					TEMPDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VI)//I
				{
					DIDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VG)//G
				{
					VGDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
				}
				else if(cur->CAM==4&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VR)//R
				{
					VRDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VDC)//DC
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						DCRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
					else
					{
						DCLimitDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VT)//TEMP
				{
					if(cur->PAGE == 0)
					{
						if(DataSave.Data_type.SW==0) //非报警模式显示
						{
							TEMPRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						}
						else
						{
							TEMPLimitDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						}
					}else if(cur->PAGE == 1){
						if(DataSave.Data_type.SW==0) //非报警模式显示
						{
							VRRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						}
						else
						{
							VRLimitDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						}
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VI)//I
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						DIRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
					else
					{
						DILimitDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VG)//G
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						VGRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
					else
					{
						VGLimitDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
				}
				else if(cur->CAM==5&&cur->COL>=1&&cur->COL<=HCOL-2&&DisLog[cur->COL+12][0].index[DisLog[cur->COL+12][0].cind]==VR)//R
				{
					if(DataSave.Data_type.SW==0) //非报警模式显示
					{
						VRRangeDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
						
					}
					else
					{
						VRLimitDia(keytmp,&DisLog[cur->COL+12][cur->CAM],cur);
					}
				}
			}
            break;
    }
    Stu_home_cycle(cur,curold);
    return keytmp;
}
//传感器类型的菜单类型
void SensorManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0;
    uint8 m,n;
    struct RDispPara DisInPut[SensorCOL][SensorCAM] = {0};
    static struct RDispPara DisLogOld;
    Ceng =Ceng;
    for(m=0; m<SensorCOL; m++)
        for(n=0; n<SensorCAM; n++)
        {
            if(m==SensorCOL-1&&n==SensorCAM-1)//TC-W 不要
                continue;
            DisInPut[m][n].flag = 0;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang = DataSave.Data_type.LANG;
            DisInPut[m][n].cind=2;
            DisInPut[m][n].index[ DisInPut[m][n].cind]=n*SensorCOL+m;
        }
    DisLogOld = *RD;
    do
    {
        if(cur->COL<7)
        {
            box=RD->Box;
            Ceng = RD->Ceng;
            Disp_Box_Zhi( box.x1,box.y2,box.x2+(box.w*(SensorCAM-1)),box.y2+(box.h+8)*SensorCOL,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            for(m=0; m<SensorCOL; m++)
                for(n=0; n<SensorCAM; n++)
                {
                    if(m==SensorCOL-1&&n==SensorCAM-1)//TC-W 不要
                        continue;
                    DisInPut[m][n].flag = 1;
                    DisInPut[m][n].disp=1;
                    DisInPut[m][n].dispold=0;
                    DisInPut[m][n].sel = 0;
                    DisInPut[m][n].selold=1;
                    DisInPut[m][n].Box.x1 = RD->Box.x1+(box.w*(n));
                    DisInPut[m][n].Box.y1 = RD->Box.y1+(box.h+8)*(m+1);
                    DisInPut[m][n].Box.x2 = RD->Box.x2+(box.w*(n)) ;
                    DisInPut[m][n].Box.y2 = RD->Box.y2+(box.h+8)*(m+1);
                    DisInPut[m][n].FontCH.x1 = RD->FontCH.x1+(box.w*(n));
                    DisInPut[m][n].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
                    DisInPut[m][n].FontEN.x1 = RD->FontEN.x1+(box.w*(n));
                    DisInPut[m][n].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
                    if(RD->index[RD->cind]==DisInPut[m][n].index[DisInPut[m][n].cind])
                    {
                        DisInPut[m][n].sel = 1;
                        DisInPut[m][n].selold=0;
                        DisInPut[m][n].flag = 0;
                    }
                    //  Disp_Box_Zi(&DisInPut[m][n],DisInPut[m][n].Box.x1,DisInPut[m][n].Box.y1,DisInPut[m][n].Box.x2,DisInPut[m][n].Box.y2);
                    LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m][n],DisInPut[m][n].FontEN.x1,DisInPut[m][n].FontEN.y1);
                }
            //  Disp_Box_Zhi2(  box.x1,box.y2,box.x2+(box.w*(SensorCAM-1)),box.y2+(box.h+8)*SensorCOL,LCD_COLOR_WHITE);
        }
        else  if(cur->COL<HCOL-1)
        {
            box=RD->Box;
            Ceng = RD->Ceng;
            Disp_Box_Zhi( box.x1,box.y1-(box.h+8)*SensorCOL,box.x2+(box.w*(SensorCAM-1)),box.y1,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            for(m=0; m<SensorCOL; m++)
                for(n=0; n<SensorCAM; n++)
                {
                    if(m==SensorCOL-1&&n==SensorCAM-1)//TC-W 不要
                        continue;
                    DisInPut[m][n].flag = 1;
                    DisInPut[m][n].disp=1;
                    DisInPut[m][n].dispold=0;
                    DisInPut[m][n].sel = 0;
                    DisInPut[m][n].selold=1;
                    DisInPut[m][n].Box.x1 = RD->Box.x1+(box.w*(n));
                    DisInPut[m][n].Box.y1 = RD->Box.y1+(box.h+8)*(m)-(box.h+8)*SensorCOL;
                    DisInPut[m][n].Box.x2 = RD->Box.x2 +(box.w*(n));
                    DisInPut[m][n].Box.y2 = RD->Box.y2+(box.h+8)*(m)-(box.h+8)*SensorCOL;
                    DisInPut[m][n].FontCH.x1 = RD->FontCH.x1+(box.w*(n));
                    DisInPut[m][n].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m)-(box.h+8)*SensorCOL;
                    DisInPut[m][n].FontEN.x1 = RD->FontEN.x1+(box.w*(n));
                    DisInPut[m][n].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m)-(box.h+8)*SensorCOL;
                    if(RD->index[RD->cind]==DisInPut[m][n].index[DisInPut[m][n].cind])
                    {
                        DisInPut[m][n].sel = 1;
                        DisInPut[m][n].selold=0;
                        DisInPut[m][n].flag = 0;
                    }
                    //  Disp_Box_Zi(&DisInPut[m][n],DisInPut[m][n].Box.x1,DisInPut[m][n].Box.y1,DisInPut[m][n].Box.x2,DisInPut[m][n].Box.y2);
                    LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m][n],DisInPut[m][n].FontEN.x1,DisInPut[m][n].FontEN.y1);
                }
            // Disp_Box_Zhi2(  box.x1,box.y1-(box.h+8)*SensorCOL,box.x2+(box.w*(SensorCAM-1)),box.y1,LCD_COLOR_WHITE);
        }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyCAM1(keytmp,RD,&DisLogOld,&CurDisp,14,SensorCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyCAM1(keytmp,RD,&DisLogOld,&CurDisp,14,SensorCOL);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    page_Mindle();
}


//重力湿度电流选择的下拉框的菜单
void VRVGDISensorManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[VRVGSensorCOL]= {0};
    static struct RDispPara DisLogOld;
    Ceng =Ceng ;
    for(m=0; m<VRVGSensorCOL; m++)
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
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*VRVGSensorCOL,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        for(m=0; m<VRVGSensorCOL; m++)
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
            if(DisInPut[m].FontCH.x1==0)
            {
                DisInPut[m].FontCH.x1 = RD->FontEN.x1;
                DisInPut[m].FontCH.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
            }
            else if(DisInPut[m].FontEN.x1==0)
            {
                DisInPut[m].FontEN.x1 = RD->FontCH.x1;
                DisInPut[m].FontEN.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
            }
            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
            {
                DisInPut[m].sel = 1;
                DisInPut[m].selold=0;
                DisInPut[m].flag = 0;
            }
            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
            if(CHI == DataSave.Data_type.LANG)
            {
                if(m==SenI)
                    LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1);
            }
            else
            {
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
            }
        }
        Disp_Box_Zhi2(  box.x1,box.y2,box.x2,box.y2+(box.h+8)*VRVGSensorCOL,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyCAM1(keytmp,RD,&DisLogOld,&CurDisp,VRVGSensorCOL,1);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyCAM1(keytmp,RD,&DisLogOld,&CurDisp,VRVGSensorCOL,1);
    page_home();
}


//湿度传感器下拉菜单
void H3SensorManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[VRVGSensorCOL]= {0};
    static struct RDispPara DisLogOld;
    Ceng =Ceng ;
    for(m=0; m<VRVGSensorCOL; m++)
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
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*VRVGSensorCOL,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        for(m=0; m<VRVGSensorCOL; m++)
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
            if(DisInPut[m].FontCH.x1==0)
            {
                DisInPut[m].FontCH.x1 = RD->FontEN.x1;
                DisInPut[m].FontCH.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
            }
            else if(DisInPut[m].FontEN.x1==0)
            {
                DisInPut[m].FontEN.x1 = RD->FontCH.x1;
                DisInPut[m].FontEN.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
            }
            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
            {
                DisInPut[m].sel = 1;
                DisInPut[m].selold=0;
                DisInPut[m].flag = 0;
            }
            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
//            if(CHI == DataSave.Data_type.LANG)
//            {
//                if(m==SenI)
                    LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
//                else
//                    LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1);
//            }
//            else
//            {
//                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
//            }
        }
        Disp_Box_Zhi2(  box.x1,box.y2,box.x2,box.y2+(box.h+8)*VRVGSensorCOL,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyCAM1(keytmp,RD,&DisLogOld,&CurDisp,VRVGSensorCOL,1);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyCAM1(keytmp,RD,&DisLogOld,&CurDisp,VRVGSensorCOL,1);
    page_home();
}

//值域选择的下拉框的菜单
void RangeManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[RangeCOL]= {0};
    static struct RDispPara DisLogOld;
    Ceng =Ceng ;
    for(m=0; m<RangeCOL; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=3;
        DisInPut[m].index[DisInPut[m].cind]=m;
    }
    DisLogOld = *RD;
    do
    {
        box=RD->Box;
        Ceng = RD->Ceng;
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*RangeCOL,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        for(m=0; m<RangeCOL; m++)
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
            LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2(  box.x1,box.y2,box.x2,box.y2+(box.h+8)*RangeCOL,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyCAM2(keytmp,RD,&DisLogOld,&CurDisp,RangeCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyCAM2(keytmp,RD,&DisLogOld,&CurDisp,RangeCOL);
    page_home();
}
// 电压上下限范围对话框初始化
void DCLimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=18;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 4;

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);

        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "报警：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Limit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Alarm:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}

// 重力上下限范围对话框初始化
void VGLimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=18;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 4;

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);

        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "报警：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Limit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Alarm:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}
// 湿度上下限范围对话框初始化
void VRLimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=18;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 4;

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);

        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "报警：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Limit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Alarm:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}
// 电流上下限范围对话框初始化
void DILimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=18;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 4;

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);

        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "报警：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Limit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Alarm:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}


//void DCOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
//{
//    struct BoxFram  box;
//    uint8 Ceng =0 ;
//    uint8 m;
//    struct RDispPara DisInPut[DCOnOffCOL] = {0};
//    static struct RDispPara DisLogOld;
//    for(m=0; m<DCOnOffCOL; m++)
//    {
//        DisInPut[m].flag = 0;
//        DisInPut[m].disp=0;
//        DisInPut[m].dispold=0;
//        DisInPut[m].lang = DataSave.Data_type.LANG;
//        DisInPut[m].cind=RD->cind;
//        DisInPut[m].index[DisInPut[m].cind]=m;
//    }
//    DisLogOld = *RD;
//    do
//    {
//        box=RD->Box;
//        Ceng = RD->Ceng;
//        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
//        for(m=0; m<DCOnOffCOL; m++)
//        {
//            DisInPut[m].flag = 1;
//            DisInPut[m].disp=1;
//            DisInPut[m].dispold=0;
//            DisInPut[m].sel = 0;
//            DisInPut[m].selold=1;
//            DisInPut[m].Box.x1 = RD->Box.x1;
//            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1);
//            DisInPut[m].Box.x2 = RD->Box.x2 ;
//            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1);
//            DisInPut[m].FontCH.x1 = RD->FontCH.x1;
//            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
//            DisInPut[m].FontEN.x1 = RD->FontEN.x1;
//            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
//            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
//            {
//                DisInPut[m].sel = 1;
//                DisInPut[m].selold=0;
//                DisInPut[m].flag = 0;
//            }
//            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
//            if(CHI == DataSave.Data_type.LANG )
//                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
//            else
//                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
//        }
//        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
//        keytmp = KeyTaskCreate(NoNull_ReTurn);
//        ModifyOnOff(keytmp,RD,&DisLogOld,cur,DCOnOffCOL);
//    }while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
//    DCDiaInit(keytmp,RD, &CurDisp) ;
//}



// 电压范围对话框初始化
void DCRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=15;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 2;

    if(cur->COL!=1)
    {
        DisInPut[0][1].cind=17;
        DisInPut[0][1].disp=1;
    }

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        if(cur->COL!=1)
            LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "通道名：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "跟踪：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Range Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        if(cur->COL!=1)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "Name:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Tra:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}




// 电流范围对话框初始化
void DIRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=15;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 2;

    if(cur->COL!=1)
    {
        DisInPut[0][1].cind=17;
        DisInPut[0][1].disp=1;
    }

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        if(cur->COL!=1)
            LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "通道名：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "跟踪：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Range Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        if(cur->COL!=1)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "Name:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Tra:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}

// 重力范围对话框初始化
void VGRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=15;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 2;

    if(cur->COL!=1)
    {
        DisInPut[0][1].cind=17;
        DisInPut[0][1].disp=1;
    }

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        if(cur->COL!=1)
            LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "通道名：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "跟踪：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Range Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        if(cur->COL!=1)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "Name:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Tra:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}



// 湿度范围对话框初始化
void VRRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=15;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 2;

    if(cur->COL!=1 || cur->PAGE  == 1)
    {
        DisInPut[0][1].cind=17;
        DisInPut[0][1].disp=1;
    }

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        if(cur->COL!=1 || cur->PAGE == 1)
            LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "通道名：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "跟踪：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Range Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        if(cur->COL!=1)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "Name:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Tra:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}

// 电压对话框初始化
void DCDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaDCCOL; m++)
        for(n=0; n<DiaDCCAM; n++)
        {
            DisDC[m][n].flag = 1;
            DisDC[m][n].disp=0;
            DisDC[m][n].dispold=0;
            DisDC[m][n].lang =DataSave.Data_type.LANG;
        }
    DisDC[0][0].cind=15;
    DisDC[0][0].index[DisDC[0][0].cind] = RD->index[RD->cind];
    DisDC[0][0].disp=1;
    DisDC[0][0].sel=1;
    DisDC[0][0].selold=0;
    DisDC[0][0].flag = 0;
    DisDC[0][0].Ceng = 2;
    DisDC[1][0].cind=17;
    DisDC[1][0].disp=1;
    DisDC[1][1].cind=17;
    DisDC[1][1].disp=1;
    DisDC[2][0].cind=17;
    DisDC[2][0].disp=1;
    DisDC[2][1].cind=17;
    DisDC[2][1].disp=1;
    DisDC[3][0].cind=16;
    DisDC[3][0].index[DisDC[3][0].cind] = 0;
    DisDC[3][0].disp=1;
    DisDC[3][1].cind=16;
    DisDC[3][1].index[DisDC[3][1].cind] = 1;
    DisDC[3][1].disp=1;
    if(cur->COL==1)
    {
        //    DisDC[0][0].index[DisDC[0][0].cind]=DataSave.Data_type.ComputeDC[CHANNUM] ;
        DisDC[1][0].Num = DataSave.Data_type.DCUpMeas[CHANNUM];
        DisDC[1][1].Num=DataSave.Data_type.DCUpEu[CHANNUM] ;
        DisDC[2][0].Num =DataSave.Data_type.DCLowMeas[CHANNUM] ;
        DisDC[2][1].Num= DataSave.Data_type.DCLowEu[CHANNUM];
    }
    else
    {
        //      DisDC[0][0].index[DisDC[0][0].cind]=DataSave.Data_type.ComputeDC[cur->COL-2] ;
        DisDC[1][0].Num = DataSave.Data_type.DCUpMeas[cur->COL-2];
        DisDC[1][1].Num = DataSave.Data_type.DCUpEu[cur->COL-2];
        DisDC[2][0].Num = DataSave.Data_type.DCLowMeas[cur->COL-2];
        DisDC[2][1].Num = DataSave.Data_type.DCLowEu[cur->COL-2];
    }


    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "单位转换",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "输出值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "显示值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "功能",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "下限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Unit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Eu");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Dis");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "EU:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Upper");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Lower");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "Unit:");
    }
}

//温度对话框初始化
void TEMPDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPCOL; m++)
        for(n=0; n<DiaTEMPCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=15;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 2;

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;
    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "单位转换",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "输出值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "显示值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "功能",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "调整值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Unit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Eu");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Dis");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "EU:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Adj:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}


//重力对话框初始化
void VGDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaVGCOL; m++)
        for(n=0; n<DiaVGCAM; n++)
        {
            DisVG[m][n].flag = 1;
            DisVG[m][n].disp=0;
            DisVG[m][n].dispold=0;
            DisVG[m][n].lang =DataSave.Data_type.LANG;
        }
    DisVG[0][0].cind=15;
    DisVG[0][0].index[DisVG[0][0].cind] = RD->index[RD->cind];
    DisVG[0][0].disp=1;
    DisVG[0][0].sel=1;
    DisVG[0][0].selold=0;
    DisVG[0][0].flag = 0;
    DisVG[0][0].Ceng = 2;

    DisVG[1][0].cind=17;
    DisVG[1][0].disp=1;
    DisVG[1][1].cind=17;
    DisVG[1][1].disp=1;

    DisVG[2][0].cind=17;
    DisVG[2][0].disp=1;
    DisVG[2][1].cind=17;
    DisVG[2][1].disp=1;

    DisVG[3][0].cind=16;
    DisVG[3][0].index[DisVG[3][0].cind] = 0;
    DisVG[3][0].disp=1;
    DisVG[3][1].cind=16;
    DisVG[3][1].index[DisVG[3][1].cind] = 1;
    DisVG[3][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "单位转换",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "输出值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "显示值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "功能",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "下限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Unit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Eu");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Dis");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "EU:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Upper");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Lower");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "Unit:");
    }
}


//湿度对话框初始化
void VRDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaVRCOL; m++)
        for(n=0; n<DiaVRCAM; n++)
        {
            DisVR[m][n].flag = 1;
            DisVR[m][n].disp=0;
            DisVR[m][n].dispold=0;
            DisVR[m][n].lang =DataSave.Data_type.LANG;
        }
    DisVR[0][0].cind=15;
    DisVR[0][0].index[DisVR[0][0].cind] = RD->index[RD->cind];
    DisVR[0][0].disp=1;
    DisVR[0][0].sel=1;
    DisVR[0][0].selold=0;
    DisVR[0][0].flag = 0;
    DisVR[0][0].Ceng = 2;

    DisVR[1][0].cind=17;
    DisVR[1][0].disp=1;
    DisVR[1][1].cind=17;
    DisVR[1][1].disp=1;

    DisVR[2][0].cind=17;
    DisVR[2][0].disp=1;
    DisVR[2][1].cind=17;
    DisVR[2][1].disp=1;

    DisVR[3][0].cind=16;
    DisVR[3][0].index[DisVR[3][0].cind] = 0;
    DisVR[3][0].disp=1;
    DisVR[3][1].cind=16;
    DisVR[3][1].index[DisVR[3][1].cind] = 1;
    DisVR[3][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "单位转换",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "输出值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "显示值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "功能",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "下限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Unit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Eu");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Dis");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "EU:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Upper");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Lower");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "Unit:");
    }
}


//电流对话框初始化
void DIDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaDICOL; m++)
        for(n=0; n<DiaDICAM; n++)
        {
            DisDI[m][n].flag = 1;
            DisDI[m][n].disp=0;
            DisDI[m][n].dispold=0;
            DisDI[m][n].lang =DataSave.Data_type.LANG;
        }
    DisDI[0][0].cind=15;
    DisDI[0][0].index[DisDI[0][0].cind] = RD->index[RD->cind];
    DisDI[0][0].disp=1;
    DisDI[0][0].sel=1;
    DisDI[0][0].selold=0;
    DisDI[0][0].flag = 0;
    DisDI[0][0].Ceng = 2;

    DisDI[1][0].cind=17;
    DisDI[1][0].disp=1;
    DisDI[1][1].cind=17;
    DisDI[1][1].disp=1;

    DisDI[2][0].cind=17;
    DisDI[2][0].disp=1;
    DisDI[2][1].cind=17;
    DisDI[2][1].disp=1;

    DisDI[3][0].cind=16;
    DisDI[3][0].index[DisDI[3][0].cind] = 0;
    DisDI[3][0].disp=1;
    DisDI[3][1].cind=16;
    DisDI[3][1].index[DisDI[3][1].cind] = 1;
    DisDI[3][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "单位转换",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "输出值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "显示值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "功能",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "下限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Unit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Eu");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Dis");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "EU:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Upper");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Lower");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*5, "Unit:");
    }
}
//温度范围对话框初始化
void TEMPRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    DisInPut[0][0].cind=15;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 2;
    if(cur->COL!=1)
    {
        DisInPut[0][1].cind=17;
        DisInPut[0][1].disp=1;
    }


    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);

        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "跟踪：",FONTTYPE16);
        if(cur->COL!=1)
            LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "通道名：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Range Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        if(cur->COL!=1)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*2, x1y1->Box.y1+Ydiv, "Name:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Tra:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}

//温度上下限对话框初始化
void TEMPLimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    for(m=0; m<DiaTEMPRangeCOL; m++)
    {
        for(n=0; n<DiaTEMPRangeCAM; n++)
        {
            DisInPut[m][n].flag = 1;
            DisInPut[m][n].disp=0;
            DisInPut[m][n].dispold=0;
            DisInPut[m][n].lang =DataSave.Data_type.LANG;
        }
    }
    DisInPut[0][0].cind=18;
    DisInPut[0][0].index[DisInPut[0][0].cind] = RD->index[RD->cind];
    DisInPut[0][0].disp=1;
    DisInPut[0][0].sel=1;
    DisInPut[0][0].selold=0;
    DisInPut[0][0].flag = 0;
    DisInPut[0][0].Ceng = 4;

    DisInPut[1][0].cind=17;
    DisInPut[1][0].disp=1;
    DisInPut[1][1].cind=17;
    DisInPut[1][1].disp=1;

    DisInPut[2][0].cind=16;
    DisInPut[2][0].index[DisInPut[2][0].cind] = 0;
    DisInPut[2][0].disp=1;
    DisInPut[2][1].cind=16;
    DisInPut[2][1].index[DisInPut[2][1].cind] = 1;
    DisInPut[2][1].disp=1;

    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];

    Xdiv=80;
    Disp_Box_Zhi(x1y1->Box.x1, x1y1->Box.y1,x2y2->Box.x2, x2y2->Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1->Box.x1, x1y1->Box.y1+22, x2y2->Box.x2-x1y1->Box.x1);//标题
    LCD_DrawRect(x1y1->Box.x1+2,x1y1->Box.y1+Ydiv*2,x2y2->Box.x2-x1y1->Box.x1-4, 24); //中框
    LCD_DrawHLine(x1y1->Box.x1, x2y2->Box.y2-Ydiv, x2y2->Box.x2-x1y1->Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+1, x1y1->Box.y1+1, x2y2->Box.x2-x1y1->Box.x1-2, 21); //标题框填色
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_FillRect(x1y1->Box.x1+2+1,x1y1->Box.y1+Ydiv*2+1,x2y2->Box.x2-x1y1->Box.x1-4-2, 24-2);//中框框填色
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "通道设置",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "上限值",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "下限值",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "报警：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "设置值：",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "单位：",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2, "Limit Setting");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*2, "Max");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*2, "Min");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+Ydiv, "Alarm:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*3, "Val:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2, x1y1->Box.y1+2+Ydiv*4, "Unit:");
    }
}

//电压上下限范围对话框此对话框和温度对话框一样调用的内容也一样
void DCLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    DILimitDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[CHANNUM];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DCLMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DCLMin[CHANNUM];
    }
    else
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DCLMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DCLMin[cur->COL-2];
    }
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VDC].Ch[2]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.Alarm[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
                DataSave.Data_type.DCLMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.DCLMin[m] = DisInPut[1][1].Num ;
            }
        }
        else
        {
            DataSave.Data_type.Alarm[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            DataSave.Data_type.DCLMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.DCLMin[cur->COL-2] = DisInPut[1][1].Num ;
        }
    }

    page_Mindle();
    SaveAllPara();
}

//重力上下限范围对话框此对话框和温度对话框一样调用的内容也一样
void VGLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    VGLimitDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[CHANNUM];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.VGLMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.VGLMin[CHANNUM];

    }
    else
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.VGLMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.VGLMin[cur->COL-2];
    }
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VG].Ch[0]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.Alarm[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
                DataSave.Data_type.VGLMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.VGLMin[m] = DisInPut[1][1].Num ;
            }
        }
        else
        {
            DataSave.Data_type.Alarm[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            DataSave.Data_type.VGLMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.VGLMin[cur->COL-2] = DisInPut[1][1].Num ;
        }
    }
    page_Mindle();
    SaveAllPara();
}


//湿度上下限范围对话框此对话框和温度对话框一样调用的内容也一样
void VRLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    VRLimitDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[CHANNUM];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.VRLMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.VRLMin[CHANNUM];

    }
    else
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.VRLMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.VRLMin[cur->COL-2];
    }

    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VR].Ch[0]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.Alarm[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
                DataSave.Data_type.VRLMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.VRLMin[m] = DisInPut[1][1].Num ;
            }
        }
        else
        {
            DataSave.Data_type.Alarm[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            DataSave.Data_type.VRLMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.VRLMin[cur->COL-2] = DisInPut[1][1].Num ;
        }
    }
    page_Mindle();
    SaveAllPara();
}

//重力上下限范围对话框此对话框和温度对话框一样调用的内容也一样
void DILimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    DCLimitDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {

        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[CHANNUM];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DILMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DILMin[CHANNUM];

    }
    else
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DILMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DILMin[cur->COL-2];
    }
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VI].Ch[0]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.Alarm[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
                DataSave.Data_type.DILMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.DILMin[m] = DisInPut[1][1].Num ;
            }
        }
        else
        {
            DataSave.Data_type.Alarm[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            DataSave.Data_type.DILMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.DILMin[cur->COL-2] = DisInPut[1][1].Num ;
        }
    }
    page_Mindle();
    SaveAllPara();
}


//电压范围对话框此对话框和温度对话框一样调用的内容也一样
void DCRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    DCRangeDiaInit(keytmp,RD, cur) ;

    if(cur->COL==1)
    {
        //strncpy(DisInPut[0][1].Name," ",sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[CHANNUM];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DCMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DCMin[CHANNUM];

    }
    else
    {
        strncpy(DisInPut[0][1].Name,DataSave.Data_type.ChName[cur->COL-2],sizeof(DataSave.Data_type.ChName[cur->COL-2]) );

        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DCMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DCMin[cur->COL-2];
    }

    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[0][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VDC].Ch[2]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==0&&n==1)
                {
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,DisInPut[m][n].Name,Font20);
                }
                else if(m==1)
                {
                    SwitchChanalData(&DisInPut[m][n].Num,3);
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
        if(keytmp==KEY_ENTER&&Cur.CAM==1&&Cur.COL==0)
        {
            DCRangeDiaInit(keytmp,RD, cur) ;
            DisInPut[0][0].sel=0;
            DisInPut[0][0].selold=1;
        }


    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }

        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.TraceF[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
                DataSave.Data_type.DCMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.DCMin[m] = DisInPut[1][1].Num ;
                //如果当前是温度操作就更新
                if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind]&&m<CHANNUM)
                {
                    DDa[m][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DCMax[m] ;
                    DDa[m][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DCMin[m];

                }

            }
        }
        else
        {
            DataSave.Data_type.TraceF[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            strncpy(DataSave.Data_type.ChName[cur->COL-2],DisInPut[0][1].Name,sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
            DataSave.Data_type.DCMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.DCMin[cur->COL-2] = DisInPut[1][1].Num ;
            //如果当前是电压操作就更新

            DDa[cur->COL-2][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DCMax[cur->COL-2] ;
            DDa[cur->COL-2][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DCMin[cur->COL-2];


        }
    }
    page_Mindle();
    SaveAllPara();
}

//电流范围对话框此对话框和温度对话框一样调用的内容也一样
void DIRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    DIRangeDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {
        // strncpy(DisInPut[0][1].Name," ",sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[CHANNUM];   //跟踪的掉电保持数据

        DisInPut[1][0].Num = DataSave.Data_type.DIMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DIMin[CHANNUM];

    }
    else
    {
        strncpy(DisInPut[0][1].Name,DataSave.Data_type.ChName[cur->COL-2],sizeof(DataSave.Data_type.ChName[cur->COL-2]) );

        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.DIMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.DIMin[cur->COL-2];
    }

    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[0][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VI].Ch[0]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==0&&n==1)
                {
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,DisInPut[m][n].Name,Font20);
                }
                else if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
        if(keytmp==KEY_ENTER&&Cur.CAM==1&&Cur.COL==0)
        {
            DIRangeDiaInit(keytmp,RD, cur) ;
            DisInPut[0][0].sel=0;
            DisInPut[0][0].selold=1;
        }
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }



        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.TraceF[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据

                DataSave.Data_type.DIMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.DIMin[m] = DisInPut[1][1].Num ;
                //如果当前是温度操作就更新
                if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind]&&m<CHANNUM)
                {
                    DDa[m][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DIMax[m] ;
                    DDa[m][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DIMin[m];
                }

            }
        }
        else
        {
            DataSave.Data_type.TraceF[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            strncpy(DataSave.Data_type.ChName[cur->COL-2],DisInPut[0][1].Name,sizeof(DataSave.Data_type.ChName[cur->COL-2]) );

            DataSave.Data_type.DIMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.DIMin[cur->COL-2] = DisInPut[1][1].Num ;
            //如果当前是电压操作就更新
            DDa[cur->COL-2][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DIMax[cur->COL-2] ;
            DDa[cur->COL-2][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.DIMin[cur->COL-2];

        }
    }
    page_Mindle();
    SaveAllPara();
}


//重力范围对话框此对话框和温度对话框一样调用的内容也一样
void VGRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    VGRangeDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {
        //strncpy(DisInPut[0][1].Name," ",sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[CHANNUM];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.VGMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.VGMin[CHANNUM];

    }
    else
    {
        strncpy(DisInPut[0][1].Name,DataSave.Data_type.ChName[cur->COL-2],sizeof(DataSave.Data_type.ChName[cur->COL-2]) );

        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[cur->COL-2];   //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.VGMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.VGMin[cur->COL-2];
    }
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[0][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VG].Ch[0]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==0&&n==1)
                {
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,DisInPut[m][n].Name,Font20);
                }
                else if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
        if(keytmp==KEY_ENTER&&Cur.CAM==1&&Cur.COL==0)
        {
            VGRangeDiaInit(keytmp,RD, cur) ;
            DisInPut[0][0].sel=0;
            DisInPut[0][0].selold=1;
        }
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
        {
            DisInPut[1][0].Num.fval =1;
            DisInPut[1][1].Num.fval =-1;
        }

        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.TraceF[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
                DataSave.Data_type.VGMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
                DataSave.Data_type.VGMin[m] = DisInPut[1][1].Num ;
                //如果当前是温度操作就更新
                if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind]&&m<CHANNUM)
                {
                    DDa[m][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VGMax[m] ;
                    DDa[m][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VGMin[m];
                }

            }
        }
        else
        {
            DataSave.Data_type.TraceF[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
            strncpy(DataSave.Data_type.ChName[cur->COL-2],DisInPut[0][1].Name,sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
            DataSave.Data_type.VGMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
            DataSave.Data_type.VGMin[cur->COL-2] = DisInPut[1][1].Num ;
            //如果当前是电压操作就更新

            DDa[cur->COL-2][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VGMax[cur->COL-2] ;
            DDa[cur->COL-2][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VGMin[cur->COL-2];

        }
    }
    page_Mindle();
    SaveAllPara();
}





//湿度范围对话框此对话框和温度对话框一样调用的内容也一样
void VRRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    VRRangeDiaInit(keytmp,RD, cur) ;
	if(cur->PAGE == 0)
	{
		if(cur->COL==1)
		{

	//  strncpy(DisInPut[0][1].Name," ",sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
			DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[CHANNUM];   //跟踪的掉电保持数据
			DisInPut[1][0].Num = DataSave.Data_type.VRMax[CHANNUM];//掉电保持数据赋值给数据显示
			DisInPut[1][1].Num = DataSave.Data_type.VRMin[CHANNUM];

		}
		else
		{
			strncpy(DisInPut[0][1].Name,DataSave.Data_type.ChName[cur->COL-2],sizeof(DataSave.Data_type.ChName[cur->COL-2]) );

			DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[cur->COL-2];   //跟踪的掉电保持数据
			DisInPut[1][0].Num = DataSave.Data_type.VRMax[cur->COL-2];//掉电保持数据赋值给数据显示
			DisInPut[1][1].Num = DataSave.Data_type.VRMin[cur->COL-2];
		}
		Xdiv=80;
		do
		{
			Ydiv=24;
			Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[0][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Ydiv=32;
			LCD_SetFont(&Font20);
			LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VR].Ch[0]);
			for(m=0; m<DiaTEMPCOL; m++)
				for(n=0; n<DiaTEMPCAM; n++)
				{
					if(m==0&&n==1)
					{
						LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,DisInPut[m][n].Name,Font20);
					}
					else if(m==1)
					{
						NumFloat2CharS(&DisInPut[m][n].Num,ptr);
						LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
					}
					else
					{
						if(CHI == DataSave.Data_type.LANG )
							LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
						else
							LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
					}
				}
			keytmp = KeyTaskCreate(NoNull_ReTurn);
			//温度范围和温度的键值处理一样
			ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
			if(keytmp==KEY_ENTER&&Cur.CAM==1&&Cur.COL==0)
			{
				VRRangeDiaInit(keytmp,RD, cur) ;
				DisInPut[0][0].sel=0;
				DisInPut[0][0].selold=1;
			}
		}
		while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
		Stu_Mid_Init();
		//温度范围和温度的键值处理一样
		ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
		LCD_SetTextColor(LCD_COLOR_WHITE);

		if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
		{
			if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
			{
				DisInPut[1][0].Num.fval =1;
				DisInPut[1][1].Num.fval =-1;
			}


			if(cur->COL==1)
			{
				for(m=0; m<CHANNUM+1; m++)
				{
					DataSave.Data_type.TraceF[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
					DataSave.Data_type.VRMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
					DataSave.Data_type.VRMin[m] = DisInPut[1][1].Num ;
					//如果当前是温度操作就更新

					if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind]&&m<CHANNUM)
					{
						DDa[m][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VRMax[m] ;
						DDa[m][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VRMin[m];
					}


				}
			}
			else
			{
				DataSave.Data_type.TraceF[cur->COL-2]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
				strncpy(DataSave.Data_type.ChName[cur->COL-2],DisInPut[0][1].Name,sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
				DataSave.Data_type.VRMax[cur->COL-2] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
				DataSave.Data_type.VRMin[cur->COL-2] = DisInPut[1][1].Num ;
				//如果当前是电压操作就更新

				DDa[cur->COL-2][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VRMax[cur->COL-2] ;
				DDa[cur->COL-2][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VRMin[cur->COL-2];

			}
		}
	}else if(cur->PAGE == 1){
//		if(cur->COL==1)
//		{

//	//  strncpy(DisInPut[0][1].Name," ",sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
//			DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[CHANNUM];   //跟踪的掉电保持数据
//			DisInPut[1][0].Num = DataSave.Data_type.VRMax[CHANNUM];//掉电保持数据赋值给数据显示
//			DisInPut[1][1].Num = DataSave.Data_type.VRMin[CHANNUM];

//		}
//		else
//		{
			strncpy(DisInPut[0][1].Name,DataSave.Data_type.ChName[cur->COL+9],sizeof(DataSave.Data_type.ChName[cur->COL+9]) );

			DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[cur->COL+10];   //跟踪的掉电保持数据
			DisInPut[1][0].Num = DataSave.Data_type.VRMax[cur->COL+10];//掉电保持数据赋值给数据显示
			DisInPut[1][1].Num = DataSave.Data_type.VRMin[cur->COL+10];
//		}
		
		Xdiv=80;
		do
		{
			Ydiv=24;
			Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[0][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Ydiv=32;
			LCD_SetFont(&Font20);
			LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VR].Ch[0]);
			for(m=0; m<DiaTEMPCOL; m++)
				for(n=0; n<DiaTEMPCAM; n++)
				{
					if(m==0&&n==1)
					{
						LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,DisInPut[m][n].Name,Font20);
					}
					else if(m==1)
					{
						NumFloat2CharS(&DisInPut[m][n].Num,ptr);
						LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
					}
					else
					{
						if(CHI == DataSave.Data_type.LANG )
							LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
						else
							LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
					}
				}
			keytmp = KeyTaskCreate(NoNull_ReTurn);
			//温度范围和温度的键值处理一样
			ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
			if(keytmp==KEY_ENTER&&Cur.CAM==1&&Cur.COL==0)
			{
				VRRangeDiaInit(keytmp,RD, cur) ;
				DisInPut[0][0].sel=0;
				DisInPut[0][0].selold=1;
			}
		}
		while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
		Stu_Mid_Init();
		//温度范围和温度的键值处理一样
		ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
		LCD_SetTextColor(LCD_COLOR_WHITE);

		if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
		{
			if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<(float)0.1)   //最大温度和最小只差0.1 错误
			{
				DisInPut[1][0].Num.fval =1;
				DisInPut[1][1].Num.fval =-1;
			}


//			if(cur->COL==1)
//			{
//				for(m=0; m<CHANNUM+1+2; m++)
//				{
//					DataSave.Data_type.TraceF[m]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
//					DataSave.Data_type.VRMax[m] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
//					DataSave.Data_type.VRMin[m] = DisInPut[1][1].Num ;
//					//如果当前是温度操作就更新

//					if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind]&&m<CHANNUM)
//					{
//						DDa[m][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VRMax[m] ;
//						DDa[m][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.VRMin[m];
//					}


//				}
//			}
//			else
//			{
				DataSave.Data_type.TraceF[cur->COL+10]=DisInPut[0][0].index[DisInPut[0][0].cind] ; //跟踪的掉电保持数据
				strncpy(DataSave.Data_type.ChName[cur->COL+9],DisInPut[0][1].Name,sizeof(DataSave.Data_type.ChName[cur->COL+9]) );
				DataSave.Data_type.VRMax[cur->COL+10] = DisInPut[1][0].Num;//掉电保持数据赋值给数据显示
				DataSave.Data_type.VRMin[cur->COL+10] = DisInPut[1][1].Num ;
				//如果当前是电压操作就更新

				DDa[cur->COL+10][0].Up[DDa[cur->COL+10][0].vcind]=DataSave.Data_type.VRMax[cur->COL+10] ;
				DDa[cur->COL+10][0].Low[DDa[cur->COL+10][0].vcind]=DataSave.Data_type.VRMin[cur->COL+10];

//			}
		}
	}
    page_Mindle();
    SaveAllPara();
}


//温度上下限范围对话框
void TEMPLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    TEMPLimitDiaInit(keytmp,RD, cur) ;
    if(cur->COL==1)
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[CHANNUM];  //报警类型的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.TempLMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.TempLMin[CHANNUM];
    }
    else
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.Alarm[cur->COL-2];  //报警类型的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.TempLMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.TempLMin[cur->COL-2];
    }
    if(DataSave.Data_type.TUnit==1)  //华氏
    {
        DisInPut[1][0].Num.fval = (float)(DisInPut[1][0].Num.fval*(float)1.8+(float)32);
        DisInPut[1][1].Num.fval = (float)(DisInPut[1][1].Num.fval*(float)1.8+(float)32);
    }
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VT].Ch[DataSave.Data_type.TUnit]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyLimitTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DataSave.Data_type.TUnit==1)  //华氏
        {
            DisInPut[1][0].Num.fval = (float)((DisInPut[1][0].Num.fval-(float)32)/(float)1.8);
            DisInPut[1][1].Num.fval = (float)((DisInPut[1][1].Num.fval-(float)32)/(float)1.8);
        }
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<1)   //最大温度和最小温度只差1 错误
        {
            DisInPut[1][0].Num.fval =TEMPMAXVAL;
            DisInPut[1][1].Num.fval =TEMPMINVAL;
        }

        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.Alarm[m] = DisInPut[0][0].index[DisInPut[0][0].cind];  //跟踪的掉电保持数据
                DataSave.Data_type.TempLMax[m] =  DisInPut[1][0].Num ;//更新数据
                DataSave.Data_type.TempLMin[m] = DisInPut[1][1].Num ;
            }
        }
        else
        {
            DataSave.Data_type.Alarm[cur->COL-2] = DisInPut[0][0].index[DisInPut[0][0].cind];  //跟踪的掉电保持数据
            DataSave.Data_type.TempLMax[cur->COL-2] =  DisInPut[1][0].Num ;//更新数据
            DataSave.Data_type.TempLMin[cur->COL-2] = DisInPut[1][1].Num ;
        }
    }
    page_Mindle();
    SaveAllPara();
}

//温度范围对话框
void TEMPRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点定对话框的大小
    x2y2 =  &DisLog[10][4];
    TEMPRangeDiaInit(keytmp,RD, cur) ;

    if(cur->COL==1)
    {
        // strncpy(DisInPut[0][1].Name," ",sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[CHANNUM];  //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.TempMax[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.TempMin[CHANNUM];
    }
    else
    {
        strncpy(DisInPut[0][1].Name,DataSave.Data_type.ChName[cur->COL-2],sizeof(DataSave.Data_type.ChName[cur->COL-2]) );
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.TraceF[cur->COL-2];  //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.TempMax[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.TempMin[cur->COL-2];
    }
    if(DataSave.Data_type.TUnit==1)  //华氏
    {
        DisInPut[1][0].Num.fval = DisInPut[1][0].Num.fval*(float)1.8+(float)32;
        DisInPut[1][1].Num.fval = DisInPut[1][1].Num.fval*(float)1.8+(float)32;
    }
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[0][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);

        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VT].Ch[DataSave.Data_type.TUnit]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==0&&n==1)
                {
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,DisInPut[m][n].Name,Font20);
                }
                else if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //温度范围和温度的键值处理一样
        ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
        if(keytmp==KEY_ENTER&&Cur.CAM==1&&Cur.COL==0)
        {
            TEMPRangeDiaInit(keytmp,RD, cur) ;
            DisInPut[0][0].sel=0;
            DisInPut[0][0].selold=1;
        }
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    //温度范围和温度的键值处理一样
    ModifyRangeTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPRangeCOL,DiaTEMPRangeCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DataSave.Data_type.TUnit==1)  //华氏
        {
            DisInPut[1][0].Num.fval = (float)((DisInPut[1][0].Num.fval-(float)32)/(float)1.8);
            DisInPut[1][1].Num.fval = (float)((DisInPut[1][1].Num.fval-(float)32)/(float)1.8);
        }
        if(DisInPut[1][0].Num.fval-DisInPut[1][1].Num.fval<1)   //最大温度和最小温度只差1 错误
        {
            DisInPut[1][0].Num.fval =TEMPMAXVAL;
            DisInPut[1][1].Num.fval =TEMPMINVAL;
        }

        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.TraceF[m] = DisInPut[0][0].index[DisInPut[0][0].cind];  //跟踪的掉电保持数据
                DataSave.Data_type.TempMax[m] =  DisInPut[1][0].Num ;//更新数据
                DataSave.Data_type.TempMin[m] = DisInPut[1][1].Num ;
                //如果当前是温度操作就更新
                if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind]&&m<CHANNUM)
                {
                    DDa[m][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.TempMax[m] ;
                    DDa[m][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.TempMin[m];
                }


            }
        }
        else
        {
            strncpy(DataSave.Data_type.ChName[cur->COL-2],DisInPut[0][1].Name,sizeof(DataSave.Data_type.ChName[cur->COL-2]) );

            DataSave.Data_type.TraceF[cur->COL-2] = DisInPut[0][0].index[DisInPut[0][0].cind];  //跟踪的掉电保持数据
            DataSave.Data_type.TempMax[cur->COL-2] =  DisInPut[1][0].Num ;//更新数据
            DataSave.Data_type.TempMin[cur->COL-2] = DisInPut[1][1].Num ;
//如果当前是温度操作就更新
            if(DDa[cur->COL-2][0].vcind==VT)
            {
                DDa[cur->COL-2][0].Up[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.TempMax[cur->COL-2] ;
                DDa[cur->COL-2][0].Low[DDa[cur->COL-2][0].vcind]=DataSave.Data_type.TempMin[cur->COL-2];
            }

        }
    }
    page_Mindle();


    SaveAllPara();
}

//温度对话框
void TEMPDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    TEMPDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    if(cur->COL==1)
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.ComputeT[CHANNUM];  //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.TempAdjMeas[CHANNUM];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.TempAdjEu[CHANNUM];
    }
    else
    {
        DisInPut[0][0].index[DisInPut[0][0].cind] = DataSave.Data_type.ComputeT[cur->COL-2];  //跟踪的掉电保持数据
        DisInPut[1][0].Num = DataSave.Data_type.TempAdjMeas[cur->COL-2];//掉电保持数据赋值给数据显示
        DisInPut[1][1].Num = DataSave.Data_type.TempAdjEu[cur->COL-2];
    }

    if(DataSave.Data_type.TUnit==1)  //华氏
    {
        DisInPut[1][0].Num.fval = (float)(DisInPut[1][0].Num.fval*(float)1.8+(float)32);
        DisInPut[1][1].Num.fval = (float)(DisInPut[1][1].Num.fval*(float)1.8+(float)32);
    }
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisInPut[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisInPut[2][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*4, (uint8_t*)ConstUint[VT].Ch[DataSave.Data_type.TUnit]);
        for(m=0; m<DiaTEMPCOL; m++)
            for(n=0; n<DiaTEMPCAM; n++)
            {
                if(m==1)
                {
                    NumFloat2CharS(&DisInPut[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+2,DisInPut[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisInPut[m][n],DisInPut[m][n].Box.x1+15,DisInPut[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPCOL,DiaTEMPCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==2)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyTEMP(keytmp,RD,&DisLogOld,&Cur,DiaTEMPCOL,DiaTEMPCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    if(keytmp==KEY_ENTER&&Cur.COL==2&&Cur.CAM==0)
    {
        if(DataSave.Data_type.TUnit==1)  //华氏
        {
            DisInPut[1][0].Num.fval = (float)((DisInPut[1][0].Num.fval-(float)32)/(float)1.8);
            DisInPut[1][1].Num.fval = (float)((DisInPut[1][1].Num.fval-(float)32)/(float)1.8);
        }

        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.ComputeT[m] = DisInPut[0][0].index[DisInPut[0][0].cind];
                DataSave.Data_type.TempAdjMeas[m]  = DisInPut[1][0].Num ;//更新掉电保持数据
                DataSave.Data_type.TempAdjEu[m] = DisInPut[1][1].Num;
                DisLog[m+1][4].index[DisLog[m+1][4].cind]=DataSave.Data_type.ComputeT[m];
            }
        }
        else
        {
            DataSave.Data_type.ComputeT[cur->COL-2] = DisInPut[0][0].index[DisInPut[0][0].cind];
            DataSave.Data_type.TempAdjMeas[cur->COL-2]  = DisInPut[1][0].Num ;//更新掉电保持数据
            DataSave.Data_type.TempAdjEu[cur->COL-2] = DisInPut[1][1].Num;
        }
    }
    page_Mindle();
    SaveAllPara();
}

//电流对话框
void DIDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    DIDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    if(cur->COL==1)
    {
        if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
        {
            //此处需要减2
            DataSave.Data_type.ComputeDI[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM]=ON;
        }
        DisDI[0][0].index[DisDI[0][0].cind] = DataSave.Data_type.ComputeDI[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];  //跟踪的掉电保持数据
        DisDI[1][0].Num = DataSave.Data_type.DIUpMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];//掉电保持数据赋值给数据显示
        DisDI[1][1].Num = DataSave.Data_type.DIUpEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];
        DisDI[2][0].Num = DataSave.Data_type.DILowMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];//掉电保持数据赋值给数据显示
        DisDI[2][1].Num = DataSave.Data_type.DILowEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];


    }
    else
    {
        if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
        {
            //此处需要减2
            DataSave.Data_type.ComputeDI[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]=ON;
        }
        DisDI[0][0].index[DisDI[0][0].cind] = DataSave.Data_type.ComputeDI[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];  //跟踪的掉电保持数据
        DisDI[1][0].Num = DataSave.Data_type.DIUpMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];//掉电保持数据赋值给数据显示
        DisDI[1][1].Num = DataSave.Data_type.DIUpEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];
        DisDI[2][0].Num = DataSave.Data_type.DILowMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];//掉电保持数据赋值给数据显示
        DisDI[2][1].Num = DataSave.Data_type.DILowEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];

    }

    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisDI[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDI[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDI[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDI[2][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDI[2][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDI[3][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDI[3][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VI].Ch[0]);
        }
        else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VI].Ch[0]);
        }
        else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VI].Ch[1]);
        }
        for(m=0; m<DiaDICOL; m++)
            for(n=0; n<DiaDICAM; n++)
            {
                if(m==1||m==2)
                {
                    NumFloat2CharS(&DisDI[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisDI[m][n],DisDI[m][n].Box.x1+2,DisDI[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisDI[m][n],DisDI[m][n].Box.x1+15,DisDI[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisDI[m][n],DisDI[m][n].Box.x1+15,DisDI[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyDI(keytmp,RD,&DisLogOld,&Cur,DiaDICOL,DiaDICAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyDI(keytmp,RD,&DisLogOld,&Cur,DiaDICOL,DiaDICAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    if(keytmp==KEY_ENTER&&Cur.COL==3&&Cur.CAM==0)
    {
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI)
                {
                    DataSave.Data_type.ComputeDI[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisDI[0][0].index[DisDI[0][0].cind];
                    DisLog[m+1][4].index[DisLog[m+1][4].cind]=DataSave.Data_type.ComputeDI[SenI][m];
                }

                DataSave.Data_type.DIUpMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m]  = DisDI[1][0].Num ;//更新掉电保持数据
                DataSave.Data_type.DIUpEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisDI[1][1].Num;
                DataSave.Data_type.DILowMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m]  = DisDI[2][0].Num ;//更新掉电保持数据
                DataSave.Data_type.DILowEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisDI[2][1].Num;
            }
        }
        else
        {
            if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI)
            {
                DataSave.Data_type.ComputeDI[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisDI[0][0].index[DisDI[0][0].cind];
            }

            DataSave.Data_type.DIUpMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]  = DisDI[1][0].Num ;//更新掉电保持数据
            DataSave.Data_type.DIUpEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisDI[1][1].Num;
            DataSave.Data_type.DILowMeas[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]  = DisDI[2][0].Num ;//更新掉电保持数据
            DataSave.Data_type.DILowEu[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisDI[2][1].Num;
        }
    }
    page_Mindle();

    SaveAllPara();
}


//重力对话框
void VGDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    VGDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    if(cur->COL==1)
    {
        if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
        {
            DataSave.Data_type.ComputeVG[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM]=ON;
        }
        DisVG[0][0].index[DisVG[0][0].cind] = DataSave.Data_type.ComputeVG[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];  //跟踪的掉电保持数据
        DisVG[1][0].Num = DataSave.Data_type.VGUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];//掉电保持数据赋值给数据显示
        DisVG[1][1].Num = DataSave.Data_type.VGUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];
        DisVG[2][0].Num = DataSave.Data_type.VGLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];//掉电保持数据赋值给数据显示
        DisVG[2][1].Num = DataSave.Data_type.VGLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];

    }
    else
    {
        if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
        {
            DataSave.Data_type.ComputeVG[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]=ON;
        }
        DisVG[0][0].index[DisVG[0][0].cind] = DataSave.Data_type.ComputeVG[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];  //跟踪的掉电保持数据
        DisVG[1][0].Num = DataSave.Data_type.VGUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];//掉电保持数据赋值给数据显示
        DisVG[1][1].Num = DataSave.Data_type.VGUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];
        DisVG[2][0].Num = DataSave.Data_type.VGLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];//掉电保持数据赋值给数据显示
        DisVG[2][1].Num = DataSave.Data_type.VGLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];

    }

    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisVG[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisVG[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisVG[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisVG[2][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisVG[2][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);

        Disp_Box_Home(&DisVG[3][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisVG[3][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VI].Ch[0]);
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VG].Ch[1]);
        }
        else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VG].Ch[0]);
        }
        else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
            LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VG].Ch[1]);
        }
        for(m=0; m<DiaVGCOL; m++)
            for(n=0; n<DiaVGCAM; n++)
            {
                if(m==1||m==2)
                {
                    NumFloat2CharS(&DisVG[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisVG[m][n],DisVG[m][n].Box.x1+2,DisVG[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisVG[m][n],DisVG[m][n].Box.x1+15,DisVG[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisVG[m][n],DisVG[m][n].Box.x1+15,DisVG[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyVG(keytmp,RD,&DisLogOld,&Cur,DiaVGCOL,DiaVGCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyVG(keytmp,RD,&DisLogOld,&Cur,DiaVGCOL,DiaVGCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    if(keytmp==KEY_ENTER&&Cur.COL==3&&Cur.CAM==0)
    {
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.VGUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m]  = DisVG[1][0].Num ;//更新掉电保持数据
                DataSave.Data_type.VGUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisVG[1][1].Num;
                DataSave.Data_type.VGLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m]  = DisVG[2][0].Num ;//更新掉电保持数据
                DataSave.Data_type.VGLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisVG[2][1].Num;

            }
        }
        else
        {
            DataSave.Data_type.VGUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]  = DisVG[1][0].Num ;//更新掉电保持数据
            DataSave.Data_type.VGUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisVG[1][1].Num;
            DataSave.Data_type.VGLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]  = DisVG[2][0].Num ;//更新掉电保持数据
            DataSave.Data_type.VGLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisVG[2][1].Num;

        }
    }
    page_Mindle();

    //  DataSave.Data_type.ComputeVG[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisVG[0][0].index[DisVG[0][0].cind];
    SaveAllPara();
}


//湿度对话框
void VRDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    VRDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
	if(cur->PAGE == 0)
	{
		if(cur->COL==1)
		{
			if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
			{
				DataSave.Data_type.ComputeVR[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM]=ON;
			}
			DisVR[0][0].index[ DisVR[0][0].cind] = DataSave.Data_type.ComputeVR[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];  //跟踪的掉电保持数据
			DisVR[1][0].Num = DataSave.Data_type.VRUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];//掉电保持数据赋值给数据显示
			DisVR[1][1].Num = DataSave.Data_type.VRUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];
			DisVR[2][0].Num = DataSave.Data_type.VRLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];//掉电保持数据赋值给数据显示
			DisVR[2][1].Num = DataSave.Data_type.VRLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][CHANNUM];

		}
		else
		{
			if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV||DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
			{
				DataSave.Data_type.ComputeVR[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]=ON;
			}
			DisVR[0][0].index[ DisVR[0][0].cind] = DataSave.Data_type.ComputeVR[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];  //跟踪的掉电保持数据
			DisVR[1][0].Num = DataSave.Data_type.VRUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];//掉电保持数据赋值给数据显示
			DisVR[1][1].Num = DataSave.Data_type.VRUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];
			DisVR[2][0].Num = DataSave.Data_type.VRLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];//掉电保持数据赋值给数据显示
			DisVR[2][1].Num = DataSave.Data_type.VRLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2];

		}
		do
		{
			Ydiv=24;
			Disp_Box_Home(&DisVR[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[2][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[2][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);

			Disp_Box_Home(&DisVR[3][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[3][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Ydiv=32;
			LCD_SetFont(&Font20);
			if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI)
			{
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VI].Ch[0]);
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VR].Ch[0]);
			}
			else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV)
			{
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VR].Ch[0]);
			}
			else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
			{
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VR].Ch[0]);
			}
			for(m=0; m<DiaVRCOL; m++)
				for(n=0; n<DiaVRCAM; n++)
				{
					if(m==1||m==2)
					{
						NumFloat2CharS(&DisVR[m][n].Num,ptr);
						LCD_DisplayNumLine_EN_CH_LJ_Home(&DisVR[m][n],DisVR[m][n].Box.x1+2,DisVR[m][n].Box.y1+2,ptr,Font20);
					}
					else
					{
						if(CHI == DataSave.Data_type.LANG )
							LCD_DisplayStringLineLJ_Home(&DisVR[m][n],DisVR[m][n].Box.x1+15,DisVR[m][n].Box.y1+2,FONTTYPE16);
						else
							LCD_DisplayStringLine_EN_CH_LJ_Home(&DisVR[m][n],DisVR[m][n].Box.x1+15,DisVR[m][n].Box.y1+2);
					}
				}
			keytmp = KeyTaskCreate(NoNull_ReTurn);
			ModifyVR(keytmp,RD,&DisLogOld,&Cur,DiaVRCOL,DiaVRCAM);
		}
		while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC);
		Stu_Mid_Init();
		ModifyVR(keytmp,RD,&DisLogOld,&Cur,DiaVRCOL,DiaVRCAM);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		if(keytmp==KEY_ENTER&&Cur.COL==3&&Cur.CAM==0)
		{
			if(cur->COL==1)
			{
				for(m=0; m<CHANNUM+1; m++)
				{
					DataSave.Data_type.VRUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m]  = DisVR[1][0].Num ;//更新掉电保持数据
					DataSave.Data_type.VRUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisVR[1][1].Num;
					DataSave.Data_type.VRLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m]  = DisVR[2][0].Num ;//更新掉电保持数据
					DataSave.Data_type.VRLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][m] = DisVR[2][1].Num;

				}
			}
			else
			{
				DataSave.Data_type.VRUpA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]  = DisVR[1][0].Num ;//更新掉电保持数据
				DataSave.Data_type.VRUpB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisVR[1][1].Num;
				DataSave.Data_type.VRLowA[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2]  = DisVR[2][0].Num ;//更新掉电保持数据
				DataSave.Data_type.VRLowB[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisVR[2][1].Num;

			}
		}
	}else if(cur->PAGE == 1){

			if(DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]==SenI||DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]==SenSV||DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]==SenBV)
			{
				DataSave.Data_type.ComputeVR[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10]=ON;
			}
			DisVR[0][0].index[ DisVR[0][0].cind] = DataSave.Data_type.ComputeVR[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10];  //跟踪的掉电保持数据
			DisVR[1][0].Num = DataSave.Data_type.VRUpA[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10];//掉电保持数据赋值给数据显示
			DisVR[1][1].Num = DataSave.Data_type.VRUpB[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10];
			DisVR[2][0].Num = DataSave.Data_type.VRLowA[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10];//掉电保持数据赋值给数据显示
			DisVR[2][1].Num = DataSave.Data_type.VRLowB[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10];

		do
		{
			Ydiv=24;
			Disp_Box_Home(&DisVR[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[2][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[2][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);

			Disp_Box_Home(&DisVR[3][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisVR[3][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Ydiv=32;
			LCD_SetFont(&Font20);
			if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenI)
			{
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VI].Ch[0]);
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VR].Ch[0]);
			}
			else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenSV)
			{
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VR].Ch[0]);
			}
			else if(DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]==SenBV)
			{
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
				LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv*3+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VR].Ch[0]);
			}
			for(m=0; m<DiaVRCOL; m++)
				for(n=0; n<DiaVRCAM; n++)
				{
					if(m==1||m==2)
					{
						NumFloat2CharS(&DisVR[m][n].Num,ptr);
						LCD_DisplayNumLine_EN_CH_LJ_Home(&DisVR[m][n],DisVR[m][n].Box.x1+2,DisVR[m][n].Box.y1+2,ptr,Font20);
					}
					else
					{
						if(CHI == DataSave.Data_type.LANG )
							LCD_DisplayStringLineLJ_Home(&DisVR[m][n],DisVR[m][n].Box.x1+15,DisVR[m][n].Box.y1+2,FONTTYPE16);
						else
							LCD_DisplayStringLine_EN_CH_LJ_Home(&DisVR[m][n],DisVR[m][n].Box.x1+15,DisVR[m][n].Box.y1+2);
					}
				}
			keytmp = KeyTaskCreate(NoNull_ReTurn);
			ModifyVR(keytmp,RD,&DisLogOld,&Cur,DiaVRCOL,DiaVRCAM);
		}
		while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC);
		Stu_Mid_Init();
		ModifyVR(keytmp,RD,&DisLogOld,&Cur,DiaVRCOL,DiaVRCAM);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		if(keytmp==KEY_ENTER&&Cur.COL==3&&Cur.CAM==0)
		{
				DataSave.Data_type.VRUpA[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10]  = DisVR[1][0].Num ;//更新掉电保持数据
				DataSave.Data_type.VRUpB[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10] = DisVR[1][1].Num;
				DataSave.Data_type.VRLowA[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10]  = DisVR[2][0].Num ;//更新掉电保持数据
				DataSave.Data_type.VRLowB[DisLog[cur->COL+12][1].index[DisLog[cur->COL+12][1].cind]][cur->COL+10] = DisVR[2][1].Num;

		}
	}
    page_Mindle();

    //  DataSave.Data_type.ComputeVR[DisLog[cur->COL][1].index[DisLog[cur->COL][1].cind]][cur->COL-2] = DisVR[0][0].index[DisVR[0][0].cind];
    SaveAllPara();
}


//电压对话框
void DCDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara* x1y1;
    struct RDispPara* x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.COL=Cur.CAM=0;
    DisLogOld = *RD;
    x1y1 =  &DisLog[3][1];//取点
    x2y2 =  &DisLog[10][4];
    DCDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    if(cur->COL==1)
    {
        DisDC[0][0].index[DisDC[0][0].cind]=DataSave.Data_type.ComputeDC[CHANNUM] ;
        DisDC[1][0].Num = DataSave.Data_type.DCUpMeas[CHANNUM];
        DisDC[1][1].Num=DataSave.Data_type.DCUpEu[CHANNUM] ;
        DisDC[2][0].Num =DataSave.Data_type.DCLowMeas[CHANNUM] ;
        DisDC[2][1].Num= DataSave.Data_type.DCLowEu[CHANNUM];
    }
    else
    {
        DisDC[0][0].index[DisDC[0][0].cind]=DataSave.Data_type.ComputeDC[cur->COL-2] ;
        DisDC[1][0].Num = DataSave.Data_type.DCUpMeas[cur->COL-2];
        DisDC[1][1].Num=DataSave.Data_type.DCUpEu[cur->COL-2] ;
        DisDC[2][0].Num =DataSave.Data_type.DCLowMeas[cur->COL-2] ;
        DisDC[2][1].Num= DataSave.Data_type.DCLowEu[cur->COL-2];
    }

    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisDC[0][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv,x1y1->Box.x1+Xdiv+80,x1y1->Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDC[1][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDC[1][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*4,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDC[2][0],x1y1->Box.x1+Xdiv,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv+130,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDC[2][1],x1y1->Box.x1+Xdiv*3-20,x1y1->Box.y1+2+Ydiv*5,x1y1->Box.x1+Xdiv*3+110,x1y1->Box.y1+2+Ydiv*6,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDC[3][0],x1y1->Box.x1+Xdiv,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv+60,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDC[3][1],x1y1->Box.x1+Xdiv*3,x2y2->Box.y1-4,x1y1->Box.x1+Xdiv*3+120,x2y2->Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1->Box.x1+2+Xdiv+15, x1y1->Box.y1+2+Ydiv*5, (uint8_t*)ConstUint[VDC].Ch[2]);
        for(m=0; m<DiaDCCOL; m++)
            for(n=0; n<DiaDCCAM; n++)
            {
                if(m==1||m==2)
                {
                    NumFloat2CharS(&DisDC[m][n].Num,ptr);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisDC[m][n],DisDC[m][n].Box.x1+2,DisDC[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisDC[m][n],DisDC[m][n].Box.x1+15,DisDC[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisDC[m][n],DisDC[m][n].Box.x1+15,DisDC[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyDC(keytmp,RD,&DisLogOld,&Cur,DiaDCCOL,DiaDCCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC); //DC的对话框第4行是确定和取消按钮
    Stu_Mid_Init();
    ModifyDC(keytmp,RD,&DisLogOld,&Cur,DiaDCCOL,DiaDCCAM);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    if(keytmp==KEY_ENTER&&Cur.COL==3&&Cur.CAM==0)
    {
        if(cur->COL==1)
        {
            for(m=0; m<CHANNUM+1; m++)
            {
                DataSave.Data_type.ComputeDC[m] = DisDC[0][0].index[DisDC[0][0].cind];
                DataSave.Data_type.DCUpMeas[m] = DisDC[1][0].Num ;
                DataSave.Data_type.DCUpEu[m] = DisDC[1][1].Num;
                DataSave.Data_type.DCLowMeas[m]  = DisDC[2][0].Num ;
                DataSave.Data_type.DCLowEu[m] = DisDC[2][1].Num;
                DisLog[m+1][4].index[DisLog[m+1][4].cind]=DataSave.Data_type.ComputeDC[m];
            }
        }
        else
        {
            DataSave.Data_type.ComputeDC[cur->COL-2] = DisDC[0][0].index[DisDC[0][0].cind];
            DataSave.Data_type.DCUpMeas[cur->COL-2] = DisDC[1][0].Num ;
            DataSave.Data_type.DCUpEu[cur->COL-2] = DisDC[1][1].Num;
            DataSave.Data_type.DCLowMeas[cur->COL-2]  = DisDC[2][0].Num ;
            DataSave.Data_type.DCLowEu[cur->COL-2] = DisDC[2][1].Num;
        }
    }
    page_Mindle();



    SaveAllPara();
}

void FilterManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[FilterCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<FilterCOL; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=4;
        DisInPut[m].index[DisInPut[m].cind]=m;
    }
    DisLogOld = *RD;
	if(cur->PAGE == 0)
	{
		do
		{
			if(cur->COL<7)
			{
				box=RD->Box;
				Ceng = RD->Ceng;
				Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
				for(m=0; m<FilterCOL; m++)
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
					LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
				}
				Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
			}
			else  if(cur->COL<HCOL-1)
			{
				box=RD->Box;
				Ceng = RD->Ceng;
				Disp_Box_Zhi( box.x1,box.y1-(box.h+8)*Ceng,box.x2,box.y1,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
				for(m=0; m<FilterCOL; m++)
				{
					DisInPut[m].flag = 1;
					DisInPut[m].disp=1;
					DisInPut[m].dispold=0;

					DisInPut[m].sel = 0;
					DisInPut[m].selold=1;
					DisInPut[m].Box.x1 = RD->Box.x1;
					DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
					DisInPut[m].Box.x2 = RD->Box.x2 ;
					DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m)-(box.h+8)*Ceng;
					DisInPut[m].FontCH.x1 = RD->FontCH.x1;
					DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
					DisInPut[m].FontEN.x1 = RD->FontEN.x1;
					DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
					if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
					{
						DisInPut[m].sel = 1;
						DisInPut[m].selold=0;
						DisInPut[m].flag = 0;
					}
					Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
					LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
				}
				Disp_Box_Zhi2( box.x1,box.y1-(box.h+8)*Ceng,box.x2,box.y1,LCD_COLOR_WHITE);
			}
			keytmp = KeyTaskCreate(NoNull_ReTurn);
			ModifyCAM3(keytmp,RD,&DisLogOld,&CurDisp,FilterCOL);
		}
		while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
		Stu_Mid_Init();
		ModifyCAM3(keytmp,RD,&DisLogOld,&CurDisp,FilterCOL);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
		LCD_SetTextColor(LCD_COLOR_WHITE);
	}else if(cur->PAGE == 1){
		do
		{
			if(cur->COL<7)
			{
				box=RD->Box;
				Ceng = RD->Ceng;
				Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
				for(m=0; m<FilterCOL; m++)
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
					LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
				}
				Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
			}
			else  if(cur->COL<HCOL-1)
			{
				box=RD->Box;
				Ceng = RD->Ceng;
				Disp_Box_Zhi( box.x1,box.y1-(box.h+8)*Ceng,box.x2,box.y1,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
				for(m=0; m<FilterCOL; m++)
				{
					DisInPut[m].flag = 1;
					DisInPut[m].disp=1;
					DisInPut[m].dispold=0;

					DisInPut[m].sel = 0;
					DisInPut[m].selold=1;
					DisInPut[m].Box.x1 = RD->Box.x1;
					DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
					DisInPut[m].Box.x2 = RD->Box.x2 ;
					DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m)-(box.h+8)*Ceng;
					DisInPut[m].FontCH.x1 = RD->FontCH.x1;
					DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
					DisInPut[m].FontEN.x1 = RD->FontEN.x1;
					DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
					if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
					{
						DisInPut[m].sel = 1;
						DisInPut[m].selold=0;
						DisInPut[m].flag = 0;
					}
					Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
					LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
				}
				Disp_Box_Zhi2( box.x1,box.y1-(box.h+8)*Ceng,box.x2,box.y1,LCD_COLOR_WHITE);
			}
			keytmp = KeyTaskCreate(NoNull_ReTurn);
			ModifyCAM3(keytmp,RD,&DisLogOld,&CurDisp,FilterCOL);
		}
		while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
		Stu_Mid_Init();
		ModifyCAM3(keytmp,RD,&DisLogOld,&CurDisp,FilterCOL);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_FillRect(2, DisLog[1][0].Box.y1-16, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+70);
		LCD_SetTextColor(LCD_COLOR_WHITE);
	}
    page_Mindle();
}

//浮点型数据转换为字符串
void float2char(float slope,char*buffer,int n)  //
{
    int temp,i,j;
    float slopeold;
    slopeold =slope;
    if(slope>=0)//
        buffer[0] = '+';
    else
    {
        buffer[0] = '-';
        slope = -slope;
    }
    temp = (int)slope;//
    for(i=0; temp!=0; i++) //
        temp /=10;
    temp =(int)slope;
    for(j=i; j>0; j--) //
    {
        buffer[j] = temp%10+'0';
        temp /=10;
    }
    buffer[i+1] = '.';
    slope -=(int)slope;
    for(i=i+2; i<n-1; i++) //
    {
        slope*=10;
        buffer[i]=(int)slope+'0';
        slope-=(int)slope;
    }
    buffer[n-1] = '\0';
    if(slopeold<1&&slopeold>-1)
    {
        for(i=n-1; i>1; i--)
        {
            buffer[i] = buffer[i-1];
        }
        buffer[1] = '0';
    }
}

void LCD_DisplayNumLine_EN_CH_LJ(struct RDispPara* RD, char *ptr,uint32 SetTextColor,uint32 SetBackColor,sFONT FontType)
{
    uint32 BackColor;
    uint32 TextColor;
    sFONT *FType = NULL;
    FType=LCD_GetFont();
    LCD_SetFont(&FontType);
    TextColor=LCD_GetTextColor();
    BackColor=LCD_GetBackColor();
    LCD_SetColors(SetTextColor, SetBackColor);
    LCD_DisplayStringLine_EN_CH_LJ(RD->FontEN.x1,RD->FontEN.y1,(uint8_t* )ptr);
    LCD_SetFont(FType);
    LCD_SetColors(TextColor, BackColor);
}


//不加华师
void NumFloat2CharSForNumFrame(struct NumStru *Num,char* ptr)
{
    uint8 i;
    int len;
    uint8 BuChangbit;
    char bitnum = 0;

    bitnum = Num->bit+2;
    if(Num->Decimalbit>0)
        bitnum+=(Num->Decimalbit+2);
    float2char(Num->fval, ptr,(int)(bitnum));
    //浮点型修补
    if(Num->Decimalbit>0&&ptr[bitnum-2]-'0'>=5)
    {
        BuChangbit=bitnum-3;
        while(++ptr[BuChangbit]-'0'==10)
        {
            ptr[BuChangbit] = '0';
            BuChangbit--;
            if(ptr[BuChangbit]=='.')
                BuChangbit--;
            if(ptr[BuChangbit]=='+')
            {
                for(i=bitnum-2; i>BuChangbit; i--)
                {
                    ptr[i] = ptr[i-1];
                }
                Num->Decimalbit++;
                break;
            }
        }
    }
    bitnum = 0;
    if(Num->Decimalbit==0&&Num->dot==0)
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        ptr[bitnum] = '\0' ;
    }
    else if(Num->dot>0&&Num->Decimalbit==0)
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        if(Num->fval<1&&Num->fval>-1)
        {
            ptr[1]='0';
        }
        ptr[bitnum] = '.' ;
        ptr[bitnum+1] = '\0' ;
    }
    else
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        if(Num->fval<1&&Num->fval>-1&&ptr[bitnum]=='0')
            bitnum++;
        if(ptr[bitnum]=='.')
            bitnum++;
        bitnum += Num->Decimalbit;
        ptr[bitnum] = '\0' ;
    }


}


//数字框
void NumFrame(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    char  ptr[10] = "         ";
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
        NumFloat2CharSForNumFrame(&RD->Num,ptr);
        LCD_DisplayNumLine_EN_CH_LJ(RD,ptr,LCD_COLOR_WHITE,LCD_COLOR_HUI,Font20);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    RD->sel = 1;
    RD->selold = 0;
    LCD_DisplayNumLine_EN_CH_LJ_Home(RD,RD->Box.x1+2,RD->Box.y1+2,ptr,Font20);
}

//0n Off切换下拉框
void VROnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{

    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[DCOnOffCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<DCOnOffCOL; m++)
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
        for(m=0; m<DCOnOffCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,DCOnOffCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    VRDiaInit(keytmp,RD, &CurDisp) ;
}


//0n Off切换下拉框
void VGOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
//  uint8 err;
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[DCOnOffCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<DCOnOffCOL; m++)
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
        for(m=0; m<DCOnOffCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,DCOnOffCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    VGDiaInit(keytmp,RD, &CurDisp) ;
}


//0n Off切换下拉框
void DIOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
//  uint8 err;
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[DCOnOffCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<DCOnOffCOL; m++)
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
        for(m=0; m<DCOnOffCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,DCOnOffCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    DIDiaInit(keytmp,RD, &CurDisp) ;
}
//0n Off切换下拉框
void DCOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
//  uint8 err;
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[DCOnOffCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<DCOnOffCOL; m++)
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
        for(m=0; m<DCOnOffCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,DCOnOffCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    DCDiaInit(keytmp,RD, &CurDisp) ;
}
//Limit切换下拉框
void TEMPLimitOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[LimitCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<LimitCOL; m++)
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
        for(m=0; m<LimitCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,LimitCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    //此处调用温度范围对话框初始化
    TEMPLimitDiaInit(keytmp,RD, &CurDisp) ;
}
//0n Off切换下拉框
void TEMPRangeOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[TEMPOnOffCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<TEMPOnOffCOL; m++)
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
        for(m=0; m<TEMPOnOffCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,TEMPOnOffCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    //此处调用温度范围对话框初始化
    TEMPRangeDiaInit(keytmp,RD, &CurDisp) ;
}

//0n Off切换下拉框
void TEMPOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[TEMPOnOffCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<TEMPOnOffCOL; m++)
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
        for(m=0; m<TEMPOnOffCOL; m++)
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
                LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyOnOff(keytmp,RD,&DisLogOld,cur,TEMPOnOffCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    //此处调用温度对话框初始化
    TEMPDiaInit(keytmp,RD, &CurDisp) ;
}

//菜单
void InPutManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[InPutCOL] = {0};
    static struct RDispPara DisLogOld;
    for(m=0; m<InPutCOL; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=1;
        DisInPut[m].index[DisInPut[m].cind]=m;
    }
    DisLogOld = *RD;
    do
    {
        if(cur->COL<7)
        {
            box=RD->Box;
            Ceng = RD->Ceng;
            Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            for(m=0; m<InPutCOL; m++)
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
                    LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
            }
            Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        }
        else  if(cur->COL<HCOL-1)
        {
            box=RD->Box;
            Ceng = RD->Ceng;
            Disp_Box_Zhi( box.x1,box.y1-(box.h+8)*Ceng,box.x2,box.y1,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            for(m=0; m<InPutCOL; m++)
            {
                DisInPut[m].flag = 1;
                DisInPut[m].disp=1;
                DisInPut[m].dispold=0;
                DisInPut[m].sel = 0;
                DisInPut[m].selold=1;
                DisInPut[m].Box.x1 = RD->Box.x1;
                DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
                DisInPut[m].Box.x2 = RD->Box.x2 ;
                DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m)-(box.h+8)*Ceng;
                DisInPut[m].FontCH.x1 = RD->FontCH.x1;
                DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
                DisInPut[m].FontEN.x1 = RD->FontEN.x1;
                DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m)-(box.h+8)*Ceng;
                if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
                {
                    DisInPut[m].sel = 1;
                    DisInPut[m].selold=0;
                    DisInPut[m].flag = 0;
                }
                Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);
                if(CHI ==DataSave.Data_type.LANG)
                    LCD_DisplayStringLineLJ_Zi(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_Zi(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1);
            }
            Disp_Box_Zhi2( box.x1,box.y1-(box.h+8)*Ceng,box.x2,box.y1,LCD_COLOR_WHITE);
        }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyCAM0(keytmp,RD,&DisLogOld,&CurDisp,InPutCOL);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_Mid_Init();
    ModifyCAM0(keytmp,RD,&DisLogOld,&CurDisp,InPutCOL);

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_FillRect(2, DisLog[1][0].Box.y1-20, 637, DisLog[HCOL-2][HCAM-1].Box.y2-DisLog[1][0].Box.y1+53);//清除中间区域
    LCD_SetTextColor(LCD_COLOR_WHITE);
    page_Mindle();
}
//第一列下拉框的键值处理函数
void ModifyCAM1(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol,char hCol)
{
    uint8 m;
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
            if(sel->COL==1)
            {
                for(m=1; m<HCOL-1; m++)
                {
                    if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind])
                    {
                        DisLog[m][1].index[DisLog[m][1].cind] = RD->index[RD->cind];
                        DisLog[m][1].disp=1;
                        DisLog[m][1].dispold=0;
                    }
                }
            }
            else if(sel->COL>1&&sel->COL<HCOL-1)
            {
                DisLog[sel->COL][1].disp=1;
                DisLog[sel->COL][1].dispold=0;
                DisLog[sel->COL][1].index[DisLog[sel->COL][1].cind] = RD->index[RD->cind];
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
        case KEY_RIGHT:
            (RD->index[RD->cind])+=hCol;
            if((RD->index[RD->cind])>MaxCol-1)
                (RD->index[RD->cind])-=hCol;
            break;
        case KEY_LEFT:
            (RD->index[RD->cind])-=hCol;
            if((RD->index[RD->cind])>MaxCol-1)
                (RD->index[RD->cind])+=hCol;
            break;
    }
}
//第二列下拉框的键值处理函数
void ModifyCAM2(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)
{
    uint8 m;
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
            if(sel->COL==1)
            {
                for(m=1; m<HCOL-1; m++)
                {
                    if(DisLog[1][0].index[DisLog[1][0].cind] ==DisLog[m][0].index[DisLog[m][0].cind])
                    {
                        DisLog[m][2].index[DisLog[m][2].cind] = RD->index[RD->cind];
                        DisLog[m][2].disp=1;
                        DisLog[m][2].dispold=0;
                    }
                }
            }
            else if(sel->COL>1&&sel->COL<HCOL-1)
            {
                DisLog[sel->COL][2].disp=1;
                DisLog[sel->COL][2].dispold=0;
                DisLog[sel->COL][2].index[DisLog[sel->COL][1].cind] = RD->index[RD->cind];
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }
}
//电压下拉框的键值处理函数
void ModifyDC(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
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
            DisDC[cur->COL][cur->CAM].selold=1;
            DisDC[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisDC[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDC[cur->COL][cur->CAM].selold=0;
            DisDC[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisDC[cur->COL][cur->CAM].selold=1;
            DisDC[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDC[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDC[cur->COL][cur->CAM].selold=0;
            DisDC[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisDC[cur->COL][cur->CAM].selold=1;
            DisDC[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisDC[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDC[cur->COL][cur->CAM].selold=0;
            DisDC[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisDC[cur->COL][cur->CAM].selold=1;
            DisDC[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisDC[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDC[cur->COL][cur->CAM].selold=0;
            DisDC[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisDC[cur->COL][cur->CAM].selold=1;
            DisDC[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDC[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDC[cur->COL][cur->CAM].selold=0;
            DisDC[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->CAM==0&&cur->COL==0)  //
            {
                DCOnOff(keytmp,&DisDC[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==1)  //meas值
            {

                NumFrame(keytmp,&DisDC[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //Eu值
            {

                NumFrame(keytmp,&DisDC[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==2)  //meas值
            {

                NumFrame(keytmp,&DisDC[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==2)  //Eu值
            {

                NumFrame(keytmp,&DisDC[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisDC[0][0].index[DisDC[0][0].cind];
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
//温度下拉框的键值处理函数
void ModifyTEMP(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    switch(keytmp)
    {
        case KEY_UP://上
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->CAM==0&&cur->COL==0)  //
            {
                TEMPOnOff(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==1)  //meas值
            {
                NumFrame(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //Eu值
            {
                NumFrame(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==2)
            {
                RD->index[RD->cind] = DisInPut[0][0].index[DisInPut[0][0].cind];
            }
            else if(cur->CAM==1&&cur->COL==2)
            {
                RD->index[RD->cind] = RDOld->index[RDOld->cind];
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }
}


//温度下拉框的键值处理函数
void ModifyDI(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    switch(keytmp)
    {
        case KEY_UP://上
            DisDI[cur->COL][cur->CAM].selold=1;
            DisDI[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisDI[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDI[cur->COL][cur->CAM].selold=0;
            DisDI[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisDI[cur->COL][cur->CAM].selold=1;
            DisDI[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDI[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDI[cur->COL][cur->CAM].selold=0;
            DisDI[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisDI[cur->COL][cur->CAM].selold=1;
            DisDI[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisDI[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDI[cur->COL][cur->CAM].selold=0;
            DisDI[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisDI[cur->COL][cur->CAM].selold=1;
            DisDI[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisDI[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDI[cur->COL][cur->CAM].selold=0;
            DisDI[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisDI[cur->COL][cur->CAM].selold=1;
            DisDI[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDI[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisDI[cur->COL][cur->CAM].selold=0;
            DisDI[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->CAM==0&&cur->COL==0)  //
            {
                DIOnOff(keytmp,&DisDI[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==1)  //meas值
            {

                NumFrame(keytmp,&DisDI[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //Eu值
            {

                NumFrame(keytmp,&DisDI[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==2)  //meas值
            {

                NumFrame(keytmp,&DisDI[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==2)  //Eu值
            {

                NumFrame(keytmp,&DisDI[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisDI[0][0].index[DisDI[0][0].cind];
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

//湿度下拉框的键值处理函数
void ModifyVR(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    switch(keytmp)
    {
        case KEY_UP://上
            DisVR[cur->COL][cur->CAM].selold=1;
            DisVR[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisVR[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVR[cur->COL][cur->CAM].selold=0;
            DisVR[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisVR[cur->COL][cur->CAM].selold=1;
            DisVR[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisVR[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVR[cur->COL][cur->CAM].selold=0;
            DisVR[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisVR[cur->COL][cur->CAM].selold=1;
            DisVR[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisVR[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVR[cur->COL][cur->CAM].selold=0;
            DisVR[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisVR[cur->COL][cur->CAM].selold=1;
            DisVR[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisVR[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVR[cur->COL][cur->CAM].selold=0;
            DisVR[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisVR[cur->COL][cur->CAM].selold=1;
            DisVR[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisVR[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVR[cur->COL][cur->CAM].selold=0;
            DisVR[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->CAM==0&&cur->COL==0)  //
            {
                // VROnOff(keytmp,&DisVR[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==1)  //meas值
            {

                NumFrame(keytmp,&DisVR[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //Eu值
            {

                NumFrame(keytmp,&DisVR[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==2)  //meas值
            {

                NumFrame(keytmp,&DisVR[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==2)  //Eu值
            {

                NumFrame(keytmp,&DisVR[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisVR[0][0].index[DisVR[0][0].cind];
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

//重力下拉框的键值处理函数
void ModifyVG(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    switch(keytmp)
    {
        case KEY_UP://上
            DisVG[cur->COL][cur->CAM].selold=1;
            DisVG[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisVG[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVG[cur->COL][cur->CAM].selold=0;
            DisVG[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisVG[cur->COL][cur->CAM].selold=1;
            DisVG[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisVG[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVG[cur->COL][cur->CAM].selold=0;
            DisVG[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisVG[cur->COL][cur->CAM].selold=1;
            DisVG[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisVG[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVG[cur->COL][cur->CAM].selold=0;
            DisVG[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisVG[cur->COL][cur->CAM].selold=1;
            DisVG[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisVG[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVG[cur->COL][cur->CAM].selold=0;
            DisVG[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisVG[cur->COL][cur->CAM].selold=1;
            DisVG[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisVG[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisVG[cur->COL][cur->CAM].selold=0;
            DisVG[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->CAM==0&&cur->COL==0)  //
            {
                // VGOnOff(keytmp,&DisVG[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==1)  //meas值
            {

                NumFrame(keytmp,&DisVG[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //Eu值
            {

                NumFrame(keytmp,&DisVG[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==2)  //meas值
            {

                NumFrame(keytmp,&DisVG[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==2)  //Eu值
            {

                NumFrame(keytmp,&DisVG[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisVG[0][0].index[DisVG[0][0].cind];
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


//温度范围下拉框的键值处理函数
void ModifyLimitTEMP(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
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
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->CAM==0&&cur->COL==0)  //
            {
                TEMPLimitOnOff(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==1)  //上限值
            {

                NumFrame(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //下限值
            {

                NumFrame(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }
}
void SoftKeyDiaInitHome(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
{
    uint8 m,n;
    //  uint16 Xdiv=60;
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
    // Xdiv=80;
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
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"通道名",FONTTYPE16);

    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Soft Key");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv,(uint8_t *)"Name");

    }
}
//软件键盘  对话框
static void SoftKeyDiaHome(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
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
    SoftKeyDiaInitHome(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home_File(&DisSoftKey[0][0],x1y1.Box.x1+Xdiv+10,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+290,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        for(n=0; n<DiaSoftKeyCAM; n++)
        {
            Disp_Box_Home_File(&DisSoftKey[1][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*3,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*3+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[2][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*5,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*5+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[3][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*7+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[4][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*9,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*9+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[5][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*11,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*11+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        }
        Disp_Box_Home_File(&DisSoftKey[6][0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_File(&DisSoftKey[6][1],x1y1.Box.x1+Xdiv*4+16,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*4+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);


        for(n=0; n<DiaSoftKeyCAM; n++)
            for(m=0; m<DiaSoftKeyCOL; m++)
            {
                if(m==0&&n==0)
                {
                    LCD_DisplayNumLine_EN_CH_LJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+2,DisSoftKey[m][n].Box.y1+2,DisSoftKey[m][n].Str,Font20);
                    DisSoftKey[m][n].sel=1;
                    DisSoftKey[m][n].selold=0;
                }
                else if(m==6)
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+15,DisSoftKey[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+15,DisSoftKey[m][n].Box.y1+2);
                }
                else if(m>=1&&m<=5)
                {
                    LCD_DisplayNumLine_EN_CH_LJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+6,DisSoftKey[m][n].Box.y1+8,DisSoftKey[m][n].Str,Font24);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==DiaSoftKeyCOL-1)&&keytmp!=KEY_ESC); //DC的对话框第4行是确定和取消按钮
    OSSchedLock(&err);
    Stu_Mid_Init();
    ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    page_home();
    OSSchedUnlock(&err);
}

//温度范围下拉框的键值处理函数
void ModifyRangeTEMP(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    static struct RDispStr RDtemp;
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
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://上
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisInPut[cur->COL][cur->CAM].selold=1;
            DisInPut[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisInPut[cur->COL][cur->CAM].disp!=1);  //不显示跳过
            DisInPut[cur->COL][cur->CAM].selold=0;
            DisInPut[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:


            if(cur->CAM==0&&cur->COL==0)  //
            {
                TEMPRangeOnOff(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==0)  //
            {
                memset(RDtemp.Str,0,sizeof(RDtemp.Str));
                strncpy(RDtemp.Str,DisInPut[cur->COL][cur->CAM].Name,sizeof(DisInPut[cur->COL][cur->CAM].Name));
                SoftKeyDiaHome(keytmp,&RDtemp,cur);
                strncpy(DisInPut[cur->COL][cur->CAM].Name,RDtemp.Str,sizeof(DisInPut[cur->COL][cur->CAM].Name));

            }
            else if(cur->CAM==0&&cur->COL==1)  //上限值
            {

                NumFrame(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==1&&cur->COL==1)  //下限值
            {

                NumFrame(keytmp,&DisInPut[cur->COL][cur->CAM],cur);
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }
}


//第三列下拉框的键值处理函数
void ModifyCAM3(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)
{
    uint8 m; 
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
			if(sel->PAGE == 0)
			{
				if(sel->COL==1)
				{
					for(m=1; m<HCOL-1; m++)
					{
						DisLog[m][3].index[DisLog[m][3].cind] = RD->index[RD->cind];
						DisLog[m][3].disp=1;
						DisLog[m][3].dispold=0;
					}
				}
				else if(sel->COL>1&&sel->COL<HCOL-1)
				{
					DisLog[sel->COL][3].disp=1;
					DisLog[sel->COL][3].dispold=0;
					DisLog[sel->COL][3].index[DisLog[sel->COL][3].cind] = RD->index[RD->cind];
				}
			}else if(sel->PAGE == 1){
				DisLog[sel->COL+12][3].disp=1;
				DisLog[sel->COL+12][3].dispold=0;
				DisLog[sel->COL+12][3].index[DisLog[sel->COL+12][3].cind] = RD->index[RD->cind];
			}
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }
}

//浮点型转换为字符串 里面有浮点型误差修补
void NumFloat2Char(struct RDispData* RD,struct NumStru *Num,char* ptr)
{
    uint8 i;
    int len;
    uint8 BuChangbit;
    char bitnum = 0;
    float fold =0 ;
    u8 bitold =0 ;
    if(RD->vcind==VT&&RD->Uint[VT].Col==1)
    {
        fold = Num->fval;
        bitold =Num->bit;
        Num->fval= (float)1.8*Num->fval+(float)32;
        Num->bit=4;
    }
    if(Num->bit==0&&Num->Decimalbit>0)  //个位没有，小数部分有数
    {
        bitnum = 3;
    }
    else
        bitnum = Num->bit+2;//整数部分位数
    if(Num->Decimalbit>0)//有小数部分
        bitnum+=(Num->Decimalbit+2);
    float2char(Num->fval, ptr,(int)(bitnum));
    //浮点型修补
    if(Num->Decimalbit>0&&ptr[bitnum-2]-'0'>=5)//如果是温度 小数点后第3位
    {
        BuChangbit=bitnum-3;//前面一位
        while(++ptr[BuChangbit]-'0'==10)   //此处要前++
        {
            ptr[BuChangbit] = '0';//9进位后显示0
            BuChangbit--;
            if(ptr[BuChangbit]=='.')
                BuChangbit--;
            if(ptr[BuChangbit]=='+')//已经顶到头了
            {
                for(i=bitnum-2; i>BuChangbit+1; i--)
                {
                    ptr[i] = ptr[i-1];
                }
				ptr[i] = '1';//第二位
				Num->bit++;//整数部分加1
                break;
            }
        }
    }
    bitnum = 0;
    if(Num->Decimalbit==0&&Num->dot==0)
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        ptr[bitnum] = '\0' ;
    }
    else if(Num->dot>0&&Num->Decimalbit==0)
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        if(Num->fval<1&&Num->fval>-1)
        {
            ptr[1]='0';
        }
        ptr[bitnum] = '.' ;
        ptr[bitnum+1] = '\0' ;
    }
    else
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        if(Num->fval<1&&Num->fval>-1&&ptr[bitnum]=='0')
            bitnum++;
        if(ptr[bitnum]=='.')
            bitnum++;
        bitnum += Num->Decimalbit;
        ptr[bitnum] = '\0' ;
    }
//lj 20200730   最后一位是点结束时
    len = strlen(ptr);
    if(ptr[len-1]<'0'||ptr[len-1]>'9' )
        ptr[len-1]='\0' ;
    //恢复
    if(RD->vcind==VT&&RD->Uint[VT].Col==1)
    {
        Num->fval=fold;
        Num->bit=bitold ;
    }
}



//不加华师
void NumFloat2CharS(struct NumStru *Num,char* ptr)
{
    uint8 i;
    int len;
    uint8 BuChangbit;
    char bitnum = 0;

    bitnum = Num->bit+2;
    if(Num->Decimalbit>0)
        bitnum+=(Num->Decimalbit+2);
    float2char(Num->fval, ptr,(int)(bitnum));
    //浮点型修补
    if(Num->Decimalbit>0&&ptr[bitnum-2]-'0'>=5)
    {
        BuChangbit=bitnum-3;
        while(++ptr[BuChangbit]-'0'==10)
        {
            ptr[BuChangbit] = '0';
            BuChangbit--;
            if(ptr[BuChangbit]=='.')
                BuChangbit--;
            if(ptr[BuChangbit]=='+')
            {
                for(i=bitnum-2; i>BuChangbit; i--)
                {
                    ptr[i] = ptr[i-1];
                }
                Num->Decimalbit++;
                break;
            }
        }
    }
    bitnum = 0;
    if(Num->Decimalbit==0&&Num->dot==0)
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        ptr[bitnum] = '\0' ;
    }
    else if(Num->dot>0&&Num->Decimalbit==0)
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        if(Num->fval<1&&Num->fval>-1)
        {
            ptr[1]='0';
        }
        ptr[bitnum] = '.' ;
        ptr[bitnum+1] = '\0' ;
    }
    else
    {
        bitnum = Num->bit;
        if(ptr[0]=='+'||ptr[0]=='-')
            bitnum++;
        if(Num->fval<1&&Num->fval>-1&&ptr[bitnum]=='0')
            bitnum++;
        if(ptr[bitnum]=='.')
            bitnum++;
        bitnum += Num->Decimalbit;
        ptr[bitnum] = '\0' ;
    }
//lj 20200730   最后一位是点结束时
    len = strlen(ptr);
    if(ptr[len-1]=='.' )
        ptr[len-1]='\0' ;
    //恢复

}
float decimal(uint8 Decimalbit)
{
    float r;
    switch(Decimalbit)
    {
        /* Non periodic transfer */
        case 1:
            r=0.1;
            break;
        case 2:
            r=0.01;
            break;
        case 3:
            r=0.001;
            break;
        case 4:
            r=0.0001;
            break;
        case 5:
            r=0.00001;
            break;
        case 6:
            r=0.000001;
            break;
        case 7:
            r=0.0000001;
            break;
        case 8:
            r=0.00000001;
            break;
    }
    return r;
}

float decimalRound(float sp,uint8 Decimalbit)
{
    int r;
    float x;
    switch(Decimalbit)
    {
        /* Non periodic transfer */
        case 1:
            r=10;
            break;
        case 2:
            r=100;
            break;
        case 3:
            r=1000;
            break;
        case 4:
            r=10000;
            break;
        case 5:
            r=100000;
            break;
        case 6:
            r=1000000;
            break;
        case 7:
            r=10000000;
            break;
        case 8:
            r=100000000;
            break;
    }
    switch(Decimalbit)
    {
        /* Non periodic transfer */
        case 1:
            x=0.05;
            break;
        case 2:
            x=0.005;
            break;
        case 3:
            x=0.0005;
            break;
        case 4:
            x=0.00005;
            break;
        case 5:
            x=0.000005;
            break;
        case 6:
            x=0.0000005;
            break;
        case 7:
            x=0.00000005;
            break;
        case 8:
            x=0.000000005;
            break;
    }

    sp=( (float)( (int)( (sp+x)*r ) ) )/r;
    return sp;
}

void ChangeNum(struct RDispPara* RD,float fkey)
{
    float decimalbit = 1;
    if(RD->Num.dot>0)
    {
        RD->Num.Decimalbit++;
        decimalbit = decimal(RD->Num.Decimalbit);
        if(RD->Num.Symbol==1)
            RD->Num.fval+= (-decimalbit*fkey) ;
        else
            RD->Num.fval+= decimalbit*fkey ;
        decimalRound(RD->Num.fval,RD->Num.Decimalbit);
    }
    else
    {
        if(RD->Num.Symbol==1)
            RD->Num.fval=RD->Num.fval *10 - fkey;
        else
            RD->Num.fval=RD->Num.fval *10 + fkey;
        RD->Num.bit++;
    }
    RD->Num.Changflag = 1;
}
//温度数字键值处理函数
void ModifyTEMPNum(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char* ptr)                  // 键值匹配函数，在while大循环中反复匹配
{
    switch(keytmp)
    {
        case  KEY_1:
            ChangeNum( RD,1);
            break;
        case  KEY_2:
            ChangeNum( RD,2);
            break;
        case  KEY_3:
            ChangeNum( RD,3);
            break;
        case  KEY_4:
            ChangeNum( RD,4);
            break;
        case  KEY_5:
            ChangeNum( RD,5);
            break;
        case  KEY_6:
            ChangeNum( RD,6);
            break;
        case  KEY_7:
            ChangeNum( RD,7);
            break;
        case  KEY_8:
            ChangeNum( RD,8);
            break;
        case  KEY_9:
            ChangeNum( RD,9);
            break;
        case  KEY_0:
            if(RD->Num.dot>0) //有点
            {
                RD->Num.Decimalbit++;
            }
            else
            {
                if(RD->Num.bit>0)
                {
                    RD->Num.fval=RD->Num.fval *10;
                    RD->Num.bit++;
                }
                else
                {
                    RD->Num.bit++;
                }
            }
            RD->Num.Changflag = 1;
            break;
        case  KEY_DOT  :
            if(RD->Num.bit>0)
            {
                RD->Num.dot = RD->Num.bit;
                RD->Num.Decimalbit=0;
            }
            else
            {
                RD->Num.bit++;
                RD->Num.dot = RD->Num.bit;
                RD->Num.Decimalbit=0;
            }
            break;
        case  KEY_BACK:
            if(RD->Num.Symbol==0)
            {
                RD->Num.Symbol=1;
            }
            else if(RD->Num.Symbol==1)
            {
                RD->Num.Symbol=0;
            }
            RD->Num.fval*=(-1);
            break;
        case KEY_ESC:
            *RD = *RDOld;
            break;
        case KEY_ENTER:
            if(RD->Num.Changflag == 0)
            {
                *RD = *RDOld;
            }
    }
}

//电压数字键值处理函数
void ModifyDCNum(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char* ptr)                  // 键值匹配函数，在while大循环中反复匹配
{
    switch(keytmp)
    {
        case  KEY_1:
            ChangeNum( RD,1);
            break;
        case  KEY_2:
            ChangeNum( RD,2);
            break;
        case  KEY_3:
            ChangeNum( RD,3);
            break;
        case  KEY_4:
            ChangeNum( RD,4);
            break;
        case  KEY_5:
            ChangeNum( RD,5);
            break;
        case  KEY_6:
            ChangeNum( RD,6);
            break;
        case  KEY_7:
            ChangeNum( RD,7);
            break;
        case  KEY_8:
            ChangeNum( RD,8);
            break;
        case  KEY_9:
            ChangeNum( RD,9);
            break;
        case  KEY_0:
            if(RD->Num.dot>0) //有点
            {
                RD->Num.Decimalbit++;
            }
            else
            {
                if(RD->Num.bit>0)
                {
                    RD->Num.fval=RD->Num.fval *10;
                    RD->Num.bit++;
                }
                else
                {
                    RD->Num.bit++;
                }
            }
            RD->Num.Changflag = 1;
            break;
        case  KEY_DOT  :
            if(RD->Num.bit>0)
            {
                RD->Num.dot = RD->Num.bit;
                RD->Num.Decimalbit=0;
            }
            else
            {
                RD->Num.bit++;
                RD->Num.dot = RD->Num.bit;
                RD->Num.Decimalbit=0;
            }
            break;
        case  KEY_BACK:
            if(RD->Num.Symbol==0)
            {
                RD->Num.Symbol=1;
            }
            else if(RD->Num.Symbol==1)
            {
                RD->Num.Symbol=0;
            }
            RD->Num.fval*=(-1);
            break;
        case KEY_ESC:
            *RD = *RDOld;
            break;
        case KEY_ENTER:
            if(RD->Num.Changflag == 0)
            {
                *RD = *RDOld;
            }
    }
}
//on off键值处理函数
void ModifyOnOff(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)                  // 键值匹配函数，在while大循环中反复匹配
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
    }
}
//第0列键值处理函数
void ModifyCAM0(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)                  // 键值匹配函数，在while大循环中反复匹配
{
    OS_ERR err;
    uint8 m,n;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时200ms
            }
            break;
        case KEY_UP://上m
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
            if(sel->COL==1)
            {
                for(m=1; m<HCOL-2; m++)
                {
                    DisLog[m][0].index[DisLog[m][0].cind] = RD->index[RD->cind];
                    if(RD->index[RD->cind]==OFF)   //OFF
                    {
                        for(n=1; n<HCAM; n++)
                        {
                            DisLog[m][n].disp=0;
                            DisLog[m][n].dispold=1;
                        }
                    }
                    else if(RD->index[RD->cind]==VDC) //DC
                    {
                        for(n=1; n<HCAM; n++)
                        {
                            DisLog[m][n].disp=1;
                            DisLog[m][n].dispold=0;
                        }
                        DisLog[m][1].disp=0;
                        DisLog[m][1].dispold=1;
                        DisLog[m][2].cind =3;
                        DisLog[m][4].cind =5;
                        //  DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDC[m-2];
                        if(m==1)
                            DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDC[CHANNUM];
                        else
                            DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDC[m-2];

                    }
                    else if(RD->index[RD->cind]==VT)//TEMP
                    {
                        for(n=1; n<HCAM; n++)
                        {
                            DisLog[m][n].disp=1;
                            DisLog[m][n].dispold=0;
                        }
                        DisLog[m][1].cind =2;
                        DisLog[m][2].disp=2;
                        DisLog[m][2].dispold=0;
                        DisLog[m][2].cind =12;
                        DisLog[m][2].index[DisLog[m][2].cind]=0;
                        DisLog[m][4].cind =5;
                        //DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeT[m-2];
                        if(m==1)
                            DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeT[CHANNUM];
                        else
                            DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeT[m-2];

                    }
                    else if(RD->index[RD->cind]==VR)//RH
                    {
                        for(n=1; n<HCAM; n++)
                        {
                            DisLog[m][n].disp=1;
                            DisLog[m][n].dispold=0;
                        }
                        // DisLog[m][1].disp=0;
                        //  DisLog[m][1].dispold=1;
                        DisLog[m][1].cind =21;
                        DisLog[m][2].disp=2;
                        DisLog[m][2].dispold=0;
                        DisLog[m][2].cind =13;
                        DisLog[m][2].index[DisLog[m][2].cind]=0;
                        DisLog[m][4].cind =5;
                        DisLog[m][4].index[DisLog[m][4].cind]=ON;

                    }
                    else if(RD->index[RD->cind]==VI)//电流
                    {
                        for(n=1; n<HCAM; n++)
                        {
                            DisLog[m][n].disp=1;
                            DisLog[m][n].dispold=0;
                        }
                        //DisLog[m][1].disp=0;
                        // DisLog[m][1].dispold=1;
                        DisLog[m][1].cind =22;
                        DisLog[m][2].disp=2;
                        DisLog[m][2].dispold=0;
                        DisLog[m][2].cind =19;
                        DisLog[m][4].cind =5;//OFF
                        if(m==1)
                            DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDI[DisLog[m][1].index[DisLog[m][1].cind]][CHANNUM];
                        else
                        {
                            if(DisLog[m][1].index[DisLog[m][1].cind]==SenI)
                                DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDI[DisLog[m][1].index[DisLog[m][1].cind]][m-2];
                            else
                                DisLog[m][4].index[DisLog[m][4].cind]=ON;
                        }

                    }
                    else if(RD->index[RD->cind]==VG)//重力
                    {
                        for(n=1; n<HCAM; n++)
                        {
                            DisLog[m][n].disp=1;
                            DisLog[m][n].dispold=0;
                        }
                        // DisLog[m][1].disp=0;
                        // DisLog[m][1].dispold=1;
                        DisLog[m][1].cind =23;
                        DisLog[m][2].disp=2;
                        DisLog[m][2].dispold=0;
                        DisLog[m][2].cind =20;
                        DisLog[m][4].cind =5;//OFF
                        DisLog[m][4].index[DisLog[m][4].cind]=ON;
                    }
                }

            }
            else if(sel->COL>1&&sel->COL<HCOL-1)
            {
                if(RD->index[RD->cind]==OFF)
                {
                    for(n=1; n<HCAM; n++)
                    {
                        DisLog[sel->COL][n].disp=0;
                        DisLog[sel->COL][n].dispold=1;
                    }
                }
                else if(RD->index[RD->cind]==VDC)
                {
                    for(n=1; n<HCAM; n++)
                    {
                        DisLog[sel->COL][n].disp=1;
                        DisLog[sel->COL][n].dispold=0;
                    }
                    DisLog[sel->COL][1].disp=0;
                    DisLog[sel->COL][1].dispold=1;
                    DisLog[sel->COL][2].cind =3;
                    DisLog[sel->COL][4].cind =5;
                    DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=DataSave.Data_type.ComputeDC[sel->COL-2];
                }
                else if(RD->index[RD->cind]==VT)
                {
                    for(n=1; n<HCAM; n++)
                    {
                        DisLog[sel->COL][n].disp=1;
                        DisLog[sel->COL][n].dispold=0;
                    }
                    DisLog[sel->COL][1].cind =2;
                    DisLog[sel->COL][2].disp=2;
                    DisLog[sel->COL][2].dispold=0;
                    DisLog[sel->COL][2].cind =12;
                    DisLog[sel->COL][2].index[DisLog[sel->COL][2].cind]=0;
                    DisLog[sel->COL][4].cind =5;
                    DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=DataSave.Data_type.ComputeT[sel->COL-2];

                }
                else if(RD->index[RD->cind]==VR)
                {
                    for(n=1; n<HCAM; n++)
                    {
                        DisLog[sel->COL][n].disp=1;
                        DisLog[sel->COL][n].dispold=0;
                    }
                    // DisLog[sel->COL][1].disp=0;
                    //  DisLog[sel->COL][1].dispold=1;
                    DisLog[sel->COL][1].cind =21;
                    DisLog[sel->COL][2].disp=2;
                    DisLog[sel->COL][2].dispold=0;
                    DisLog[sel->COL][2].cind =13;
                    DisLog[sel->COL][2].index[DisLog[sel->COL][2].cind]=0;
                    DisLog[sel->COL][4].cind =5;
                    DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=ON;
                    // DisLog[sel->COL][4].disp=2;
                    //  DisLog[sel->COL][4].dispold=0;
                    //  DisLog[sel->COL][4].cind =14;
                    // DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=0;
                }
                else if(RD->index[RD->cind]==VI)
                {
                    for(n=1; n<HCAM; n++)
                    {
                        DisLog[sel->COL][n].disp=1;
                        DisLog[sel->COL][n].dispold=0;
                    }
                    //     DisLog[sel->COL][1].disp=0;
                    //       DisLog[sel->COL][1].dispold=1;
                    DisLog[sel->COL][1].cind =22;
                    DisLog[sel->COL][2].disp=2;
                    DisLog[sel->COL][2].dispold=0;
                    DisLog[sel->COL][2].cind =19;
                    DisLog[sel->COL][4].cind =5;
                    if(sel->COL>1)
                        DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=DataSave.Data_type.ComputeDI[DisLog[sel->COL][1].index[DisLog[sel->COL][1].cind]][sel->COL-2];
                    else
                    {
                        if(DisLog[sel->COL][1].index[DisLog[sel->COL][1].cind]==SenI)
                            DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=OFF;
                        else
                            DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=ON;

                    }
                }
                else if(RD->index[RD->cind]==VG)//重力
                {
                    for(n=1; n<HCAM; n++)
                    {
                        DisLog[sel->COL][n].disp=1;
                        DisLog[sel->COL][n].dispold=0;
                    }
                    //    DisLog[sel->COL][1].disp=0;
                    //     DisLog[sel->COL][1].dispold=1;
                    DisLog[sel->COL][1].cind =23;
                    DisLog[sel->COL][2].disp=2;
                    DisLog[sel->COL][2].dispold=0;
                    DisLog[sel->COL][2].cind =20;
                    DisLog[sel->COL][4].cind =5;
                    DisLog[sel->COL][4].index[DisLog[sel->COL][4].cind]=ON;
                }

            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }
}


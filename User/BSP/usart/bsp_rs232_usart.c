/*
  * @file    Bsp_rs232_usart.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   使用串口与前端采集的教诲，重定向c库printf函数到usart端口，中断接收模式
 */
#include "include.h"
extern FATFS flash_fs;
extern Diskio_drvTypeDef  SD_Driver;
OS_TCB DoUartTaskTCB;
//任务堆栈
CPU_STK DoUart_TASK_STK[DoUart_STK_SIZE];
OS_SEM  Uart_CMD;       //定义一个信号量，用于任务同步
UART_HandleTypeDef UartHandle;
volatile uint8_t Rxflag=0;    //清数据标志 新的指令发出后清串口接收数据
uint8_t ucTemp;
struct ChanalData ChanData;
struct ChanalMData ChanMData;  // 快速模式
struct ChanalSet ChanSet;
struct ChanalHSet ChanHSet;
struct SLData chTemp;//端子内部温度
volatile uint8_t  UartCmd;
union SD_U Dsd;//sd卡存储数据
union SD_U DsdR;//sd卡读出数据
union SD_U DsdFM[10];//sd卡存储数据快速
uint64_t BlockAddr;
uint8_t ScreenFlag = OFF;   //截屏标识
uint8_t MDataF=OFF;//快采标志
uint8 ShowTimeflag = 0;
uint32 FileSaveNum = 0;//
uint32 sizewatch;
//static float smoothArray[FilerMaxNum] = {0}; //移动平均  相当于低通滤波
/*
        接收DEBUG_USART口的数据，分析并处理
        可以将此段代码封装为一个函数，
        在主程序其它流程调用
 */

static uint8 chrStrT[256];
static uint32 LenT = 0;
uint8 chrStr[256];
uint8 cpyStr[256];

//获得存储的时间 和显示的时间是倍数关系
static RTC_TimeTypeDef RTC_TimeStrStartsamp;//时间
static RTC_DateTypeDef RTC_DateStrStartsamp;//日期
uint16 LinenumSample=0;
void DoRx(struct ChanalData *pCmd,struct ChanalMData *pMD)
{
    static uint8 uart3startbit;           //串口3接收到开始符0xAC标志为，1为收到，0为没收到
    static uint8 uart3length;             //串口3接收到数据的长度
    static uint8 rcvLen3;                  //串口3接收到数据的长度
    OS_ERR err;
    uint8_t a;
    if(Rxflag==1)   //从新计数
    {
        uart3startbit=0;
        uart3length=0;
        memset(chrStr,0,sizeof(chrStr));
        Rxflag=0;
        memset(chrStrT,0,sizeof(chrStrT));
        LenT=0;
    }
    a= ucTemp;
    chrStrT[LenT++] = ucTemp;
    if(uart3startbit==0)
    {
        if(a==ChrStart)
        {
            chrStr[0]=a;
            uart3startbit=1;
            uart3length=1;
        }
    }
    else
    {
        chrStr[uart3length++] = a;
        if(uart3length==3) //第三个字节
        {
            rcvLen3 = chrStr[uart3length-1] +3;
        }
        else if(uart3length>3&&uart3length==rcvLen3) //
        {
            uart3startbit=0;
            uart3length=0;
            UartCmd = SerialRemoteHandle(rcvLen3,chrStr,pCmd,pMD);     //解析
            OSSemPost(&Uart_CMD,OS_OPT_POST_1,&err);//发送信号量
            rcvLen3=0;
        }
    }
}

void FlashTimeVal(struct ChanalSet *p)
{
    uint8 i=0;
    for(i=0; i<10; i++)
    {
        switch(p->Time[i])
        {
            case     1:         //
                p->TimeVal[i] = 10;
                break;
            case        2:         //
                p->TimeVal[i] = 20;
                break;
            case        3:         //
                p->TimeVal[i] = 50;
                break;
            case        4:         //
                p->TimeVal[i] =100;
                break;
            case        5:         //
                p->TimeVal[i] = 200;
                break;
            case        6:         //
                p->TimeVal[i] = 500;
                break;
            case        7:         //
                p->TimeVal[i] = 1000;
                break;
        }

    }
}
uint32 GetTotalTime(struct ChanalSet *p)
{
    uint32 time=0;
    uint8 i=0;
    for(i=0; i<10; i++)
    {
        time+= p->TimeVal[i];
    }
    return time;

}
void FloattoBYTE3(float fval,uint8 *buf)
{
    uint16 u16val=0;
    if(fval>100||fval<-100)    //
    {
        buf[0] =8<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*10);
        }
        else
        {
            u16val = (uint16)(fval*10);
        }

    }
    else if(fval>10||fval<-10)
    {
        buf[0] = 7<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*100);
        }
        else
        {
            u16val = (uint16)(fval*100);
        }
    }
    else if(fval>1||fval<-1)
    {
        buf[0] = 6<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*1000);
        }
        else
        {
            u16val = (uint16)(fval*1000);
        }
    }
    else if(fval>0.1F||fval<-0.1F)
    {
        buf[0] = 5<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*10000);
        }
        else
        {
            u16val = (uint16)(fval*10000);
        }
    }
    else if(fval>0.01F||fval<-0.01F)
    {
        buf[0] = 4<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*100000);
        }
        else
        {
            u16val = (uint16)(fval*100000);
        }
    }
    else if(fval>0.001F||fval<-0.001F)
    {
        buf[0] = 3<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*1000000);
        }
        else
        {
            u16val = (uint16)(fval*1000000);
        }
    }
    else if(fval>0.0001F||fval<-0.0001F)
    {
        buf[0] = 2<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*10000000);
        }
        else
        {
            u16val = (uint16)(fval*10000000);
        }
    }
    else if(fval>0.00001F||fval<-0.00001F)
    {
        buf[0] = 1<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*100000000);
        }
        else
        {
            u16val = (uint16)(fval*100000000);
        }
    }
    else if(fval>0.000001F||fval<-0.000001F)
    {
        buf[0] = 0<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*1000000000);
        }
        else
        {
            u16val = (uint16)(fval*1000000000);
        }
    }
    buf[0]|=(uint8)((u16val>>14)&0x3);
    buf[1]|=(uint8)((u16val>>7)&0x7F);
    buf[2]|=(uint8)((u16val)&0x7F);
}

void GetSDSaveData(uint32 starttick,uint32 endtick,const struct SLData SL,uint8 i,uint8 DataType)
{
    //  uint8 r=0;
    Dsd.Data.DataType = DataType;   //数据类
    //Dsd.Data.SampleTimeCurCAM =SampleTimeCur.CAM;
    if((Dsd.Data.DataType&0xf)==(CmdR_DATA&0xf))
        Dsd.Data.SampleTimeCurCAM =DataSave.Data_type.SaveSample;
    else if((Dsd.Data.DataType&0xf)==(CmdR_FDATA&0xf))
        Dsd.Data.SampleTimeCurCAM =0;//50ms
    Dsd.Data.CHD[i].SDData.vcind =SL.vcind;
    Dsd.Data.CHD[i].SDData.endtick=endtick;
    Dsd.Data.starttick = starttick;
    if(Dsd.Data.CHD[i].SDData.vcind == VDC)  //电压模式
    {
        FloattoBYTE3(DataSave.Data_type.DCMax[i].fval,Dsd.Data.CHD[i].SDData.MaxB);
        FloattoBYTE3(DataSave.Data_type.DCMin[i].fval,Dsd.Data.CHD[i].SDData.MinB);
    }
    else if(Dsd.Data.CHD[i].SDData.vcind == VI)  //电流
    {
        FloattoBYTE3(DataSave.Data_type.DIMax[i].fval,Dsd.Data.CHD[i].SDData.MaxB);
        FloattoBYTE3(DataSave.Data_type.DIMin[i].fval,Dsd.Data.CHD[i].SDData.MinB);
    }
    else if(Dsd.Data.CHD[i].SDData.vcind == VG)  //重力
    {
        FloattoBYTE3(DataSave.Data_type.VGMax[i].fval,Dsd.Data.CHD[i].SDData.MaxB);
        FloattoBYTE3(DataSave.Data_type.VGMin[i].fval,Dsd.Data.CHD[i].SDData.MinB);
    }
    else if(Dsd.Data.CHD[i].SDData.vcind == VR)  //湿度
    {
        FloattoBYTE3(DataSave.Data_type.VRMax[i].fval,Dsd.Data.CHD[i].SDData.MaxB);
        FloattoBYTE3(DataSave.Data_type.VRMin[i].fval,Dsd.Data.CHD[i].SDData.MinB);
    }
    else //温度模式
    {
        FloattoBYTE3(DataSave.Data_type.TempMax[i].fval,Dsd.Data.CHD[i].SDData.MaxB);
        FloattoBYTE3(DataSave.Data_type.TempMin[i].fval,Dsd.Data.CHD[i].SDData.MinB);
    }
    memcpy(Dsd.Data.CHD[i].SDData.byte,SL.byte,sizeof(SL.byte));
//    if(SL.Dxy.X>GIXStartX+PointsPix)
//        r=1;//存储回头
//  return r;
}

void GetSDSaveTime(const RTC_DateTypeDef RTC_DateStr,const RTC_TimeTypeDef RTC_TimeStr)
{
    Dsd.Data.Timestamp=time2Stamp(RTC_DateStr, RTC_TimeStr) ;
}


// 上下限判断
void UpLowComp(uint32 i)
{
	if(i<9)
	{
		if(ChanData.chd[i].vcind==VDC||ChanData.chd[i].vcind==VT||ChanData.chd[i].vcind==VI||ChanData.chd[i].vcind==VG||ChanData.chd[i].vcind==VR)
		{
			if(ChanData.chd[i].fStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
		 
				ChanData.chd[i].fStr = DDa[i][0].Up[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.chd[i].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.chd[i]);//接收后解析的数据原始数据转换为数据显示
			}
			else if(ChanData.chd[i].fStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			 {
				ChanData.chd[i].fStr = DDa[i][0].Low[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.chd[i].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.chd[i]);//接收后解析的数据原始数据转换为数据显示
			}
		}
		else
		{
			if(ChanData.chd[i].DfStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
				ChanData.chd[i].DfStr = DDa[i][0].Up[DDa[i][0].vcind];
			}
			else if(ChanData.chd[i].DfStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			{
				ChanData.chd[i].DfStr = DDa[i][0].Low[DDa[i][0].vcind];
			}
		}
	}else if(i==9){
		if(ChanData.hmd[1].vcind==VDC||ChanData.hmd[1].vcind==VT||ChanData.hmd[1].vcind==VI||ChanData.hmd[1].vcind==VG||ChanData.hmd[1].vcind==VR)
		{
			if(ChanData.hmd[1].fStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
		 
				ChanData.hmd[1].fStr = DDa[i][0].Up[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.hmd[1].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.hmd[1]);//接收后解析的数据原始数据转换为数据显示
			}
			else if(ChanData.hmd[1].fStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			 {
				ChanData.hmd[1].fStr = DDa[i][0].Low[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.hmd[1].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.hmd[1]);//接收后解析的数据原始数据转换为数据显示
			}
		}
		else
		{
			if(ChanData.hmd[i].DfStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
				ChanData.hmd[i].DfStr = DDa[i][0].Up[DDa[i][0].vcind];
			}
			else if(ChanData.hmd[i].DfStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			{
				ChanData.hmd[i].DfStr = DDa[i][0].Low[DDa[i][0].vcind];
			}
		}
	}if(i == 11){
		if(ChanData.hmd[3].vcind==VDC||ChanData.hmd[3].vcind==VT||ChanData.hmd[3].vcind==VI||ChanData.hmd[3].vcind==VG||ChanData.hmd[3].vcind==VR)
		{
			if(ChanData.hmd[3].fStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
		 
				ChanData.hmd[3].fStr = DDa[i][0].Up[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.hmd[3].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.hmd[3]);//接收后解析的数据原始数据转换为数据显示
			}
			else if(ChanData.hmd[3].fStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			 {
				ChanData.hmd[3].fStr = DDa[i][0].Low[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.hmd[3].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.hmd[3]);//接收后解析的数据原始数据转换为数据显示
			}
		}
		else
		{
			if(ChanData.hmd[3].DfStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
				ChanData.hmd[3].DfStr = DDa[i][0].Up[DDa[i][0].vcind];
			}
			else if(ChanData.hmd[3].DfStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			{
				ChanData.hmd[3].DfStr = DDa[i][0].Low[DDa[i][0].vcind];
			}
		}
	}if(i == 12){
		if(ChanData.hmd[5].vcind==VDC||ChanData.hmd[5].vcind==VT||ChanData.hmd[5].vcind==VI||ChanData.hmd[5].vcind==VG||ChanData.hmd[5].vcind==VR)
		{
			if(ChanData.hmd[5].fStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
		 
				ChanData.hmd[5].fStr = DDa[i][0].Up[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.hmd[5].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.hmd[5]);//接收后解析的数据原始数据转换为数据显示
			}
			else if(ChanData.hmd[5].fStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			 {
				ChanData.hmd[5].fStr = DDa[i][0].Low[DDa[i][0].vcind];
				SwitchChanalData (&ChanData.hmd[5].fStr,8);//小数点后8位调试
				fStr2DfStr(&ChanData.hmd[5]);//接收后解析的数据原始数据转换为数据显示
			}
		}
		else
		{
			if(ChanData.hmd[5].DfStr.fval>DDa[i][0].Up[DDa[i][0].vcind].fval)//大于800 显示最大
			{
				ChanData.hmd[5].DfStr = DDa[i][0].Up[DDa[i][0].vcind];
			}
			else if(ChanData.hmd[5].DfStr.fval<DDa[i][0].Low[DDa[i][0].vcind].fval)//小于200 显示最小值
			{
				ChanData.hmd[5].DfStr = DDa[i][0].Low[DDa[i][0].vcind];
			}
		}
	}
	
	
}

void UpLowCompFM(uint32 i,uint8 ch)
{
    if(ChanMData.chmd[i].vcind==VDC||ChanMData.chmd[i].vcind==VT||ChanMData.chmd[i].vcind==VI||ChanMData.chmd[i].vcind==VG||ChanMData.chmd[i].vcind==VR)
    {
        if(ChanMData.chmd[i].fStr.fval>DDa[ch][0].Up[DDa[ch][0].vcind].fval)//大于800 显示最大
        {
            ChanMData.chmd[i].fStr = DDa[ch][0].Up[DDa[ch][0].vcind];
            SwitchChanalData (&ChanMData.chmd[i].fStr,8);//小数点后8位调试
            fStr2DfStr(&ChanMData.chmd[i]);//接收后解析的数据原始数据转换为数据显示
        }
        else if(ChanMData.chmd[i].fStr.fval<DDa[ch][0].Low[DDa[ch][0].vcind].fval)//小于200 显示最小值
        {
            ChanMData.chmd[i].fStr = DDa[ch][0].Low[DDa[ch][0].vcind];
            SwitchChanalData (&ChanMData.chmd[i].fStr,8);//小数点后8位调试
            fStr2DfStr(&ChanMData.chmd[i]);//接收后解析的数据原始数据转换为数据显示
        }
    }
    else
    {
        if(ChanMData.chmd[i].DfStr.fval>DDa[ch][0].Up[DDa[ch][0].vcind].fval)//大于800 显示最大
        {
            ChanMData.chmd[i].DfStr = DDa[ch][0].Up[DDa[ch][0].vcind];
        }
        else if(ChanMData.chmd[i].DfStr.fval<DDa[ch][0].Low[DDa[ch][0].vcind].fval)//小于200 显示最小值
        {
            ChanMData.chmd[i].DfStr = DDa[ch][0].Low[DDa[ch][0].vcind];
        }
    }
}
// 上下报警判断
void MaxMinComp(uint32 i)
{
    ChanData.chalarm[i] = 0;
    if(DDa[i][0].vcind==VDC)
    {

        if(DataSave.Data_type.Alarm[i]==0)
        {
            ChanData.chalarm[i] = 0;
        }
        else if(DataSave.Data_type.Alarm[i]==1&&ChanData.chd[i].DfStr.fval>DataSave.Data_type.DCLMax[i].fval)//上超
        {
            ChanData.chalarm[i] = 1;
        }
        else if(DataSave.Data_type.Alarm[i]==2&&ChanData.chd[i].DfStr.fval<DataSave.Data_type.DCLMin[i].fval)//下超
        {
            ChanData.chalarm[i] = 2;
        }
        else if(DataSave.Data_type.Alarm[i]==3&&(ChanData.chd[i].DfStr.fval>DataSave.Data_type.DCLMax[i].fval ||ChanData.chd[i].DfStr.fval<DataSave.Data_type.DCLMin[i].fval))
        {
            ChanData.chalarm[i] = 3;//超出
        }
    }
    else if(DDa[i][0].vcind==VI)
    {

        if(DataSave.Data_type.Alarm[i]==0)
        {
            ChanData.chalarm[i] = 0;
        }
        else if(DataSave.Data_type.Alarm[i]==1&&ChanData.chd[i].DfStr.fval>DataSave.Data_type.DILMax[i].fval)//上超
        {
            ChanData.chalarm[i] = 1;
        }
        else if(DataSave.Data_type.Alarm[i]==2&&ChanData.chd[i].DfStr.fval<DataSave.Data_type.DILMin[i].fval)//下超
        {
            ChanData.chalarm[i] = 2;
        }
        else if(DataSave.Data_type.Alarm[i]==3&&(ChanData.chd[i].DfStr.fval>DataSave.Data_type.DILMax[i].fval ||ChanData.chd[i].DfStr.fval<DataSave.Data_type.DILMin[i].fval))
        {
            ChanData.chalarm[i] = 3;//超出
        }
    }
    else if (DDa[i][0].vcind==VT)
    {

        if(DataSave.Data_type.Alarm[i]==0)
        {
            ChanData.chalarm[i] = 0;
        }
        else if(DataSave.Data_type.Alarm[i]==1&&ChanData.chd[i].DfStr.fval>DataSave.Data_type.TempLMax[i].fval )//上超
        {
            ChanData.chalarm[i] = 1;
        }
        else if(DataSave.Data_type.Alarm[i]==2&&ChanData.chd[i].DfStr.fval<DataSave.Data_type.TempLMin[i].fval)//下超
        {
            ChanData.chalarm[i] = 2;
        }
        else if(DataSave.Data_type.Alarm[i]==3&&(ChanData.chd[i].DfStr.fval>DataSave.Data_type.TempLMax[i].fval ||ChanData.chd[i].DfStr.fval<DataSave.Data_type.TempLMin[i].fval))
        {
            ChanData.chalarm[i] = 3;//超出
        }
    }
    else if(DDa[i][0].vcind==VG)
    {

        if(DataSave.Data_type.Alarm[i]==0)
        {
            ChanData.chalarm[i] = 0;
        }
        else if(DataSave.Data_type.Alarm[i]==1&&ChanData.chd[i].DfStr.fval>DataSave.Data_type.VGLMax[i].fval)//上超
        {
            ChanData.chalarm[i] = 1;
        }
        else if(DataSave.Data_type.Alarm[i]==2&&ChanData.chd[i].DfStr.fval<DataSave.Data_type.VGLMin[i].fval)//下超
        {
            ChanData.chalarm[i] = 2;
        }
        else if(DataSave.Data_type.Alarm[i]==3&&(ChanData.chd[i].DfStr.fval>DataSave.Data_type.VGLMax[i].fval ||ChanData.chd[i].DfStr.fval<DataSave.Data_type.VGLMin[i].fval))
        {
            ChanData.chalarm[i] = 3;//超出
        }
    }
    else if(DDa[i][0].vcind==VR)
    {

        if(DataSave.Data_type.Alarm[i]==0)
        {
            ChanData.chalarm[i] = 0;
        }
        else if(DataSave.Data_type.Alarm[i]==1&&ChanData.chd[i].DfStr.fval>DataSave.Data_type.VRLMax[i].fval)//上超
        {
            ChanData.chalarm[i] = 1;
        }
        else if(DataSave.Data_type.Alarm[i]==2&&ChanData.chd[i].DfStr.fval<DataSave.Data_type.VRLMin[i].fval)//下超
        {
            ChanData.chalarm[i] = 2;
        }
        else if(DataSave.Data_type.Alarm[i]==3&&(ChanData.chd[i].DfStr.fval>DataSave.Data_type.VRLMax[i].fval ||ChanData.chd[i].DfStr.fval<DataSave.Data_type.VRLMin[i].fval))
        {
            ChanData.chalarm[i] = 3;//超出
        }
    }
}

void GetStartDateTime(void)
{
    HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStrStart, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStrStart, RTC_FORMAT_BIN);
}

void GetStartDateTimesamp(void)
{
    HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStrStartsamp, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStrStartsamp, RTC_FORMAT_BIN);

}

void UpDataAllDFrq(void)
{
    uint32 i=0;
    for(i=0; i<2; i++)//频率参数
    {
        DFrq[i].DP.Num = ChanData.frq[i].DfStr;//接收到数据更新
        DFrq[i].DP.Dxy = ChanData.frq[i].Dxy;
        DFrq[i].Uint[DFrq[i].vcind].Col = ChanData.frq[i].UintOne.Col;  // 显示单位刷新
        DFrq[i].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新
    }
}


void UpDataAllDHmd(void)
{
    uint32 i=0;
    for(i=0; i<3; i++)//频率参数
    {
        DHmd[i].DP.Num = ChanData.hmd[i].DfStr;//接收到数据更新
        DHmd[i].DP.Dxy = ChanData.hmd[i].Dxy;
        DHmd[i].Uint[DHmd[i].vcind].Col = ChanData.hmd[i].UintOne.Col;  // 显示单位刷新
        DHmd[i].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新
    }
}

uint32 UpDataDDa(uint32 starttick,uint32 i)
{
    uint32 endtick=0;
    
	if(i<9)
	{
		endtick=JiSuanXY(&ChanData.chd[i],i,starttick,SampletimeMs[SampleTimeCur.CAM], ZoneData[ ZoneCur.CAM]);//坐标确定
		DDa[i][0].DP.Num = ChanData.chd[i].DfStr;//接收到数据更新
		DDa[i][0].DP.Dxy = ChanData.chd[i].Dxy;
		DDa[i][0].Uint[DDa[i][0].vcind].Col = ChanData.chd[i].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
		DDa[i][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新  必须要加 不加不刷新
	}else{
		if(i == 9)
		{
			endtick=JiSuanXY(&ChanData.hmd[1],9,starttick,SampletimeMs[SampleTimeCur.CAM], ZoneData[ ZoneCur.CAM]);//坐标确定
			DDa[i][0].DP.Num = ChanData.hmd[2*(i-9)+1].DfStr;//接收到数据更新
			DDa[i][0].DP.Dxy = ChanData.hmd[2*(i-9)+1].Dxy;
			DDa[i][0].Uint[DDa[i][0].vcind].Col = ChanData.hmd[2*(i-9)+1].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
			DDa[i][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新  必须要加 不加不刷新
		}else if(i > 10){
			endtick=JiSuanXY2(&ChanData.hmd[2*(i-10)+1],i-11,starttick,SampletimeMs[SampleTimeCur.CAM], ZoneData[ ZoneCur.CAM]);//坐标确定
			DDa[i][0].DP.Num = ChanData.hmd[2*(i-10)+1].DfStr;//接收到数据更新
			DDa[i][0].DP.Dxy = ChanData.hmd[2*(i-10)+1].Dxy;
			DDa[i][0].Uint[DDa[i][0].vcind].Col = ChanData.hmd[2*(i-10)+1].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
			DDa[i][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新  必须要加 不加不刷新
		}

//		DDa[i][0].DP.Num = ChanData.hmd[i-10].DfStr;//接收到数据更新
//		DDa[i][0].DP.Dxy = ChanData.hmd[i-10].Dxy;
//		DDa[i][0].Uint[DDa[i][0].vcind].Col = ChanData.hmd[i-10].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
//		DDa[i][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新  必须要加 不加不刷新
	}
    return  endtick;
}
void UpDataTemp(void)
{
    DTemp.DP.Num = chTemp.DfStr;//接收到数据更新
    DTemp.Uint[DTemp.vcind].Col = chTemp.UintOne.Col;  // 显示单位刷新
    DTemp.DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新
}
void SetChanSetMod(void)
{
    uint32 i=0;
    for(i=0; i<10; i++)
    {
        if(DisLog[2+i][0].index[1] == OFF)  //
        {
            ChanSet.Mod[i] = 1; //关闭
            ModBusMod[i] = ChanSet.Mod[i];
        }
        else if(DisLog[2+i][0].index[1] == VDC)  //电压模式
        {
            ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//16 17是电压模式
            ModBusMod[i] = ChanSet.Mod[i];
        }
        else if(DisLog[2+i][0].index[1] == VT)  //温度模式
        {
            ChanSet.Mod[i] = 2+DisLog[2+i][1].index[2];
            ModBusMod[i] = ChanSet.Mod[i];
        }
        else if(DisLog[2+i][0].index[1] == VR)  //湿度模式 默认是电压可能是电流
        {
            if(DisLog[2+i][1].index[21]==0)  //AM2305
            {
				if(i < 11)
				{
					ChanHSet.Mod[i-9] = 2;//AM2305
				}else{
					ChanHSet.Mod[i-10] = 2;//AM2305
				}
                ModBusMod[i] = 21;
            }
            else if(DisLog[2+i][1].index[21]==1)    //大电流  由小电压转换而来
            {
                DisLog[2+i][2].index[3] = 0;
               if(i < 11)
				{
					ChanHSet.Mod[i-9] = 3;//HT800
				}else{
					ChanHSet.Mod[i-10] = 3;//HT800
				}
                ModBusMod[i] = 22;
            }
            else if(DisLog[2+i][1].index[21]==2)
            {
                DisLog[2+i][2].index[3] = 1;
                if(i < 11)
				{
					ChanHSet.Mod[i-9] = 4;//AC3000
				}else{
					ChanHSet.Mod[i-10] = 4;//AC3000
				}
                ModBusMod[i] = 23;
            }
        }
        else if(DisLog[2+i][0].index[1] == VI)  //电流
        {
            if(DisLog[2+i][1].index[22]==0)  //小电流
            {
                ChanSet.Mod[i] = 18;//电流模式
                ModBusMod[i] = ChanSet.Mod[i];
            }
            else if(DisLog[2+i][1].index[22]==1)    //大电流  由小电压转换而来
            {
                DisLog[2+i][2].index[3] = 0;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//16 是电压模式
                ModBusMod[i] = 19;
            }
            else if(DisLog[2+i][1].index[22]==2)
            {
                DisLog[2+i][2].index[3] = 1;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//17是电压模式
                ModBusMod[i] = 20;
            }
        }
        else if(DisLog[2+i][0].index[1] == VG)  //重力模式
        {
            if(DisLog[2+i][1].index[23]==0)  //小电流
            {
                ChanSet.Mod[i] = 18;//电流模式
                ModBusMod[i] = 24;
            }
            else if(DisLog[2+i][1].index[23]==1)    //大电流  由小电压转换而来
            {
                DisLog[2+i][2].index[3] = 0;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//16 是电压模式
                ModBusMod[i] = 25;
            }
            else if(DisLog[2+i][1].index[23]==2)
            {
                DisLog[2+i][2].index[3] = 1;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//17是电压模式
                ModBusMod[i] = 26;
            }
        }
        else
        {
            ChanSet.Mod[i] = 0;   //保持不变
            ModBusMod[i] = 0;
        }
    }
	for(i=0;i<2;i++)
	{
		if(DisLog[13+i][0].index[1] == VR)  //湿度模式 默认是电压可能是电流
        {
            if(DisLog[13+i][1].index[21]==0)  //AM2305
            {
				ChanHSet.Mod[i+1] = 2;//AM2305
                ModBusMod[i+10] = 21;
            }
            else if(DisLog[13+i][1].index[21]==1)    //大电流  由小电压转换而来
            {
                DisLog[13+i][2].index[3] = 0;
				ChanHSet.Mod[i+1] = 3;//HT800
                ModBusMod[i+10] = 22;
            }
            else if(DisLog[13+i][1].index[21]==2)
            {
				ChanHSet.Mod[i+1] = 4;//AC3000
                ModBusMod[i+10] = 23;
            }
        }else{
            ChanHSet.Mod[i+1] = 0;   //保持不变
            ModBusMod[i+10] = 0;
        }
	}
}

void SetChanSetMod2(void)
{
    uint32 i=0;
    for(i=0; i<12; i++)
    {
        if(DisLog[2+i][0].index[1] == OFF)  //
        {
            ChanSet.Mod[i] = 1; //关闭
            ModBusMod[i] = ChanSet.Mod[i];
        }
        else if(DisLog[2+i][0].index[1] == VDC)  //电压模式
        {
            ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//16 17是电压模式
            ModBusMod[i] = ChanSet.Mod[i];
        }
        else if(DisLog[2+i][0].index[1] == VT)  //温度模式
        {
            ChanSet.Mod[i] = 2+DisLog[2+i][1].index[2];
            ModBusMod[i] = ChanSet.Mod[i];
        }
        else if(DisLog[2+i][0].index[1] == VR)  //湿度模式 默认是电压可能是电流
        {
            if(DisLog[2+i][1].index[21]==0)  //AM2305
            {
                ChanHSet.Mod[i-8] = 2;//AM2305
                ModBusMod[i] = 21;
            }
            else if(DisLog[2+i][1].index[21]==1)    //大电流  由小电压转换而来
            {
                DisLog[2+i][2].index[3] = 0;
                ChanHSet.Mod[i-8] = 3;//HT800
                ModBusMod[i] = 22;
            }
            else if(DisLog[2+i][1].index[21]==2)
            {
                DisLog[2+i][2].index[3] = 1;
                ChanHSet.Mod[i-8] = 4;//AC3000
                ModBusMod[i] = 23;
            }
        }
        else if(DisLog[2+i][0].index[1] == VI)  //电流
        {
            if(DisLog[2+i][1].index[22]==0)  //小电流
            {
                ChanSet.Mod[i] = 18;//电流模式
                ModBusMod[i] = ChanSet.Mod[i];
            }
            else if(DisLog[2+i][1].index[22]==1)    //大电流  由小电压转换而来
            {
                DisLog[2+i][2].index[3] = 0;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//16 是电压模式
                ModBusMod[i] = 19;
            }
            else if(DisLog[2+i][1].index[22]==2)
            {
                DisLog[2+i][2].index[3] = 1;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//17是电压模式
                ModBusMod[i] = 20;
            }
        }
        else if(DisLog[2+i][0].index[1] == VG)  //重力模式
        {
            if(DisLog[2+i][1].index[23]==0)  //小电流
            {
                ChanSet.Mod[i] = 18;//电流模式
                ModBusMod[i] = 24;
            }
            else if(DisLog[2+i][1].index[23]==1)    //大电流  由小电压转换而来
            {
                DisLog[2+i][2].index[3] = 0;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//16 是电压模式
                ModBusMod[i] = 25;
            }
            else if(DisLog[2+i][1].index[23]==2)
            {
                DisLog[2+i][2].index[3] = 1;
                ChanSet.Mod[i] = 16+DisLog[2+i][2].index[3];//17是电压模式
                ModBusMod[i] = 26;
            }
        }
        else
        {
            ChanSet.Mod[i] = 0;   //保持不变
            ModBusMod[i] = 0;
        }
    }
}

void SetChanSetTime(void)
{
    uint32 i=0;
    for(i=0; i<10; i++)
    {
        if(DataSave.Data_type.SampleCAM==0)
            ChanSet.Time[i] = 2;
        else if(DataSave.Data_type.SampleCAM==1)
            ChanSet.Time[i] = 3;
        else if(DataSave.Data_type.SampleCAM==2)
            ChanSet.Time[i] = 3;
        else if(DataSave.Data_type.SampleCAM==3)
            ChanSet.Time[i] = 4;
        else if(DataSave.Data_type.SampleCAM==4)
            ChanSet.Time[i] = 5;
        else if(DataSave.Data_type.SampleCAM==5)
            ChanSet.Time[i] = 6;
        else if(DataSave.Data_type.SampleCAM==6)
            ChanSet.Time[i] = 7;
        else if(DataSave.Data_type.SampleCAM==7)
            ChanSet.Time[i] = 8;
        else if(DataSave.Data_type.SampleCAM==8)
            ChanSet.Time[i] = 9;
        else if(DataSave.Data_type.SampleCAM==9)
            ChanSet.Time[i] = 10;
        else if(DataSave.Data_type.SampleCAM==10)
            ChanSet.Time[i] = 10;
        else if(DataSave.Data_type.SampleCAM==11)
            ChanSet.Time[i] = 11;
        else
            ChanSet.Time[i] = 11;
    }
    ChanSet.SYNC = DataSave.Data_type.SYNF+1;
}
void ChanSetInit(void)
{
    SetChanSetTime();
    SetChanSetMod();
    FlashTimeVal(&ChanSet);
    ChanSet.alarm = 1;
}

void DoScreen(void)
{
    if(ScreenFlag==ON)
    {
        if(DataSave.Data_type.ShotF==ON)
            Screen_shot_srand("NP",".bmp");
        ScreenFlag=OFF;
    }
}
void DoMData(void)
{
    uint8 cnt,i,ch;
    cnt=0;
    for(i=0; i<10; i++)
    {
        if(ChanSet.Mod[i] != 1)  //关闭状态
        {
            cnt++;
            ch=i;
        }
    }
    if(cnt==1&&SampleTimeCur.CAM==0)
    {
        ChanMData.ch = ch+1;
        for(i=0; i<10; i++)
            ChanMData.chmd[i].vcind  =  ChanData.chd[ch].vcind ;
        MDataF=ON;
    }
    else
    {
        MDataF=OFF;

    }
}
static void DoUart_Init(void)
{
    OS_ERR err;
    OSSemCreate ((OS_SEM*   )&Uart_CMD,
                 (CPU_CHAR* )"Uart_CMD",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
    Stu_home_Init();
    Stu_DisplayData_Init();
    Stu_DisplayDataPCha_Init();
    //  DataSave.Data_type.BlockAddr = 0;
    //主页
    ChanSetInit();
    SendSetMod(ChanSet);
    OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
    if(UartCmd!=CmdR_MOD)
    {
        InitRes[10] = 0xFF;
    }
    OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    SendSetSYNCTime(ChanSet);
    ChanSet.SYNC = 0;
    OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
    if(UartCmd!=CmdR_TIME)
    {
        InitRes[11] = 0xFF;
    }

    OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    RTC_work();
    GetStartDateTime();//初始化获得时间
    GetStartDateTimesamp();//初始化获得时间
}



//转换
float DoSW(const float  val,uint8 i)
{
    float x = 1.0F;
    float y = 0.0F;
    float res = 0;
    if(DisLog[2+i][0].index[1] == OFF)  //
    {
        ;
    }
    else if(DisLog[2+i][0].index[1] == VDC)  //电压模式
    {
        if(DataSave.Data_type.ComputeDC[i]==ON)
        {
            x =(DataSave.Data_type.DCUpEu[i].fval-DataSave.Data_type.DCLowEu[i].fval)/(DataSave.Data_type.DCUpMeas[i].fval-DataSave.Data_type.DCLowMeas[i].fval);
            y = DataSave.Data_type.DCUpEu[i].fval-(DataSave.Data_type.DCUpMeas[i].fval*x);
            res = val*x +y;
        }
        else
        {
            res = val;
        }
    }
    else if(DisLog[2+i][0].index[1] == VT)  //温度模式
    {
        if(DataSave.Data_type.ComputeT[i]==ON)
        {
            res = (DataSave.Data_type.TempAdjEu[i].fval/DataSave.Data_type.TempAdjMeas[i].fval)*val;
        }
        else
        {
            res = val;
        }
    }
    else if(DisLog[2+i][0].index[1] == VR)  //湿度模式 默认是电压可能是电流
    {
        if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==0)  //小电流mA-%
        {
            x =(DataSave.Data_type.VRUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VRLowB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.VRUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VRLowA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
            y = DataSave.Data_type.VRUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.VRUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
            res = val*1000.0F*x +y;
        }
        else if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==1)    //电压V-%
        {
            x =(DataSave.Data_type.VRUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VRLowB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.VRUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VRLowA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
            y = DataSave.Data_type.VRUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.VRUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
            res = val*x +y;
        }
        else if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==2)   //电压V-%
        {
            x =(DataSave.Data_type.VRUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VRLowB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.VRUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VRLowA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
            y = DataSave.Data_type.VRUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.VRUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
            res = val*x +y;
        }

    }
    else if(DisLog[2+i][0].index[1] == VI)  //电流进来A  统一单位是mA
    {
        if(DataSave.Data_type.ComputeDI[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i]==ON)
        {

            if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==0)  //小电流A-mA
            {
                x =(DataSave.Data_type.DIUpEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.DILowEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.DIUpMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.DILowMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
                y = DataSave.Data_type.DIUpEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.DIUpMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
                res = (val*1000.0F)*x +y;  //转化为mA
            }
            else if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==1)    //电压V-mA
            {
                x =(DataSave.Data_type.DIUpEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.DILowEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.DIUpMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.DILowMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
                y = DataSave.Data_type.DIUpEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.DIUpMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
                res = val*x +y;  //转化为mA
            }
            else if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==2)   //电压V-A
            {
                x =(DataSave.Data_type.DIUpEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.DILowEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.DIUpMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.DILowMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
                y = DataSave.Data_type.DIUpEu[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.DIUpMeas[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
                res = (val*x +y)*1000.0F;//转化为mA
            }

        }
        else//转化没开启时进来的只可以是A
        {
            res = val*1000;
        }

    }
    else if(DisLog[2+i][0].index[1] == VG)  //重力模式
    {

        if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==0)  //小电流mA-kg
        {
            x =(DataSave.Data_type.VGUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VGLowB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.VGUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VGLowA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
            y = DataSave.Data_type.VGUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.VGUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
            res = ((val*1000)*x +y)*1000;  //转化为g
        }
        else if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==1)    //电压V-g
        {
            x =(DataSave.Data_type.VGUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VGLowB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.VGUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VGLowA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
            y = DataSave.Data_type.VGUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.VGUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
            res = val*x +y;
        }
        else if(DisLog[2+i][1].index[DisLog[2+i][1].cind]==2)   //电压V-kg
        {
            x =(DataSave.Data_type.VGUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VGLowB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval)/(DataSave.Data_type.VGUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-DataSave.Data_type.VGLowA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval);
            y = DataSave.Data_type.VGUpB[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval-(DataSave.Data_type.VGUpA[DisLog[2+i][1].index[DisLog[2+i][1].cind]][i].fval*x);
            res = (val*x +y)*1000;//转化为g
        }


    }
    return res;
}

float DoSWHmd(const float  val,uint8 i)
{
    float x = 1.0F;
    float y = 0.0F;
    float res = 0;
	uint8 j,k;
	if(i==1)
	{
		j=11;
		k=10;
	}else if(i == 3){
		j=13;
		k=11;
	}else if(i == 5){
		j=14;
		k=12;
	}
    if(DisLog[j][0].index[1] == OFF)  //
    {
        ;
    }
    
    else if(DisLog[j][0].index[1] == VR)  //湿度模式 默认是电压可能是电流
    {
        if(DisLog[j][1].index[DisLog[j][1].cind]==0)  //小电流mA-%
        {
            x =(DataSave.Data_type.VRUpB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-DataSave.Data_type.VRLowB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval)/(DataSave.Data_type.VRUpA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-DataSave.Data_type.VRLowA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval);
            y = DataSave.Data_type.VRUpB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-(DataSave.Data_type.VRUpA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval*x);
//            res = val*1000.0F*x +y;
			res = val*x +y;
        }
        else if(DisLog[j][1].index[DisLog[j][1].cind]==1)    //电压V-%
        {
            x =(DataSave.Data_type.VRUpB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-DataSave.Data_type.VRLowB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval)/(DataSave.Data_type.VRUpA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-DataSave.Data_type.VRLowA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval);
            y = DataSave.Data_type.VRUpB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-(DataSave.Data_type.VRUpA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval*x);
            res = val*x +y;
        }
        else if(DisLog[j][1].index[DisLog[j][1].cind]==2)   //电压V-%
        {
            x =(DataSave.Data_type.VRUpB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-DataSave.Data_type.VRLowB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval)/(DataSave.Data_type.VRUpA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-DataSave.Data_type.VRLowA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval);
            y = DataSave.Data_type.VRUpB[DisLog[j][1].index[DisLog[j][1].cind]][k].fval-(DataSave.Data_type.VRUpA[DisLog[j][1].index[DisLog[j][1].cind]][k].fval*x);
            res = val*x +y;
        }

    }
    return res;
}

//滤波器
float DoFiler(const float  val, int span,uint8 ch)
{
    static char num[CHANNUM]= {0,0,0,0,0,0,0,0,0/*,0*/};
    static float  dataArr[CHANNUM][FilerMaxNum] = {0};
    uint8 i = 0;
    float  res;
    float  chushu;
    if(++num[ch]<span)
    {
        res = val;
        dataArr[ch][num[ch]-1] = val;
    }
    else
    {
        num[ch]=span;
        if(span>0)    //跨度大于0 时才有效
        {
            dataArr[ch][num[ch]-1] = val;
            chushu = 1/(float)span;
            res = 0;
            for(i=0; i<span; i++) //前移动
            {
                res+=dataArr[ch][i]*chushu;
                if(i<span-1)
                    dataArr[ch][i] = dataArr[ch][i+1];
            }

        }
        else
        {
            res = val;  //选OFF
        }
    }
    return res;
}

//滤波器算法
//void MovingAverage(const float  data[], int span,int Length)
//{
//    memset(smoothArray,0,sizeof(smoothArray));
//    int b = 0;
//    int c;
//    if (span % 2 == 1)
//        b = (span - 1) / 2;
//    else
//    {
//        span -= 1;
//        b = (span - 1) / 2;
//    }
//    if (Length > span)
//    {
//        for (int i = 0; i < Length; i++)
//        {
//            if (i < b)
//            {
//                smoothArray[i] = 0;
//                for (int j = -i; j < i + 1; j++)
//                {
//                    smoothArray[i] += data[i + j];
//                }
//                smoothArray[i] /= (2 * i + 1);
//            }
//            else if (i >= b && (Length - i) > b)
//            {
//                smoothArray[i] = 0;
//                for (int j = -b; j < b + 1; j++)
//                {
//                    smoothArray[i] += data[i + j];
//                }
//                smoothArray[i] /= span;
//            }
//            else
//            {
//                smoothArray[i] = 0;
//                c = Length - i - 1;
//                for (int j = -c; j < c + 1; j++)
//                {
//                    smoothArray[i] += data[i + j];
//                }
//                smoothArray[i] /= (2 * c + 1);
//            }
//        }
//    }
//}

void GetSDSaveDataFrq(void)
{
    uint8 i=0;
    //脉冲存储数据刷新一次
    for(i=0; i<2; i++)
    {
        //单位就是一个脉冲
        memcpy(Dsd.Data.FD[i].SDData.byte,ChanData.frq[i].byte,sizeof(ChanData.frq[i].byte));
        Dsd.Data.FD[i].SDData.vcind =ChanData.frq[i].vcind;
    }
}
void DoUart_task(void *p_arg)
{
    OS_ERR err;
    static uint32 starttick=0;
    static uint32 startticksamp=0;
    static uint32 startticksampOld=0;
    static uint32 endticksamp=0;
    static uint32 DoUarttick=0;
    uint32 time=0;
    uint32 i=0;
    static uint32 j=0;
    uint8 r = 0;
    uint8 r1 = 0;
    uint8 z = 0;
    uint8 CAMold = 0;
    uint8 ZoneCAMold = 0;
    static uint32 Dtime=100;
    uint8 JieTaoflag = 0;//接头处理
    uint32 endtick=0;
    uint8 AlarmCnt = 0;
    uint8 AlarmCntOld = 0xFF;
    static uint8 UartCmdT =  Cmd_NULL;
    static uint8 SampleCnt = 0;
    DoUart_Init();
    starttick = HAL_GetTick();
    startticksamp= starttick;
    LinenumFM=0;  //初始值
    LinenumSample=0;
    //ChanMData.Num=0;
    //ChanMData.ch = 1;  //第0个通道
    ChanMData.ch = 4;  //第3个通道 20200803 测试第四通道数据快读
    SampleTimeCur.CAM =DataSave.Data_type.SampleCAM;
    CompSample();
    OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
	CurChan.PAGE = 0;
    while(1)
    {
		sizewatch = sizeof(DataSave.Data_type);
        DoUarttick = HAL_GetTick(); //计数看收到数据的毫秒数
        //截屏功能
        DoScreen();
		
		SendReadTandH(ChanSet);
		time=GetTotalTime(&ChanSet);
		OSSemPend(&Uart_CMD,time,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
        UartCmdT = UartCmd;
		
		
        if(MDataF==ON)//处于快存状态
            SendReadFData(ChanMData.ch,10);
        else
            SendReadData(ChanSet);
        UartCmdT =  Cmd_NULL;
        DoMData();
        CAMold = SampleTimeCur.CAM;
        ZoneCAMold = ZoneCur.CAM;
        //启动bin文件
        // TrigFile();
        //  SendReSet();
        //  SendSetMod(ChanSet);
        //  SendSetSYNCTime(ChanSet);
        time=GetTotalTime(&ChanSet);
        OSSemPend(&Uart_CMD,time,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
        UartCmdT = UartCmd;
		
//		SendReadTandH(ChanSet);
//		time=GetTotalTime(&ChanSet);
//		OSSemPend(&Uart_CMD,time,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂起
//        UartCmdT = UartCmd;
        if( UartCmdT ==/*CmdR_TandH*/CmdR_DATA&&CAMold == SampleTimeCur.CAM&&ZoneCAMold ==ZoneCur.CAM)
        {
            if(DataSave.Data_type.PageFlag == FPageCh )//只有在画图页面时才画
            {
                SendReadTemp();
            }
            if(Linenum==0&&JieTaoflag==0)//意外回头
            {
                GetStartDateTime();//回头获得时间
                starttick = HAL_GetTick();
            }
            AlarmCnt = 0;
            ++SampleCnt;
            for(i=0; i<CHANNUM+3; i++)
            {
                UpLowComp(i);//上下限比较
                MaxMinComp(i);  //最大最小报警值比较
                if(ChanData.chalarm[i]!=0)
                    AlarmCnt++;   //报警输出脉冲计数
                if(SampleCnt>=SaveCnt)//到了存储节拍
                {
					if(i < 9)
					{
						//存储数据刷新一次
						endticksamp=JiSuanXYsamp(&ChanData.chd[i],i,startticksamp,SampletimeMs[DataSave.Data_type.SaveSample],
												 ZoneData[ ZoneCur.CAM],&r1,LinenumSample);//坐标确定
						GetSDSaveData(startticksamp,endticksamp,ChanData.chd[i],i,UartCmdT&0xf);
					}else if(i > 10){
						//存储数据刷新一次
						endticksamp=JiSuanXYsamp2(&ChanData.hmd[(i-10)*2+1],i,startticksamp,SampletimeMs[DataSave.Data_type.SaveSample],
												 ZoneData[ ZoneCur.CAM],&r1,LinenumSample);//坐标确定
						GetSDSaveData(startticksamp,endticksamp,ChanData.hmd[(i-10)*2+1],i,UartCmdT&0xf);
					}else if(i == 10){
						//存储数据刷新一次
						endticksamp=JiSuanXYsamp(&ChanData.hmd[1],9,startticksamp,SampletimeMs[DataSave.Data_type.SaveSample],
												 ZoneData[ ZoneCur.CAM],&r1,LinenumSample);//坐标确定
						GetSDSaveData(startticksamp,endticksamp,ChanData.hmd[1],i,UartCmdT&0xf);
					}
                }
                endtick=UpDataDDa(starttick,i);
            }
            if(SampleCnt>=SaveCnt)//到了存储节拍
            {
                GetSDSaveDataFrq();
            }
            if(DataSave.Data_type.PageFlag == FPageCh )
            {
                OSSemPend(&Uart_CMD,50,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂
                UpDataTemp();
            }
            if(AlarmCnt!= AlarmCntOld&& DataSave.Data_type.AlarmOutF==ON)
            {
                if(AlarmCnt==0)
                    SendSetALARM(AlarmCnt,AlarmCnt);
                else
                    SendSetALARM(11-AlarmCnt,11-AlarmCnt);
            }
//            UpDataAllDFrq();
            UpDataWIFI();//wifi数据上传 可以上传制标志任务里上传数据
            if(SampleCnt>=SaveCnt)//到了存储节拍
                GetSDSaveTime(RTC_DateStrStartsamp,RTC_TimeStrStartsamp);//获取保持时间
            if(AlarmCnt!= AlarmCntOld&& DataSave.Data_type.AlarmOutF==ON)
            {
                AlarmCntOld=AlarmCnt;
                OSSemPend(&Uart_CMD,50,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量挂
            }
            //Linenum++;
//当画图页面打开后画图在此处做不丢点不可放画图任务里做
            if(++Linenum<PointsNum)
            {
                if(Linenum>=1&&ShowTimeflag==0)//第一个点
                {
                    if(DataSave.Data_type.PageFlag == FPageGraph)//只有在画图页面时才画
                    {
                        ShowTimeLine(RTC_DateStrStart,RTC_TimeStrStart,10, 403, 100,&Font16,SampletimeMs[SampleTimeCur.CAM]/10);// 显示画图下方的时间
                    }
                    ShowTimeflag =1;
                }
                if(Linenum>0&&Linenum<PointsNum) // 1代表第一个点
                {
                    r = DrawGraph(CurChan,Linenum);//不全清   返回值有些超框有些没超

                }
                if(r==1)//回头接头处理
                {
                    DrawGraph_Flash(CurChan,Linenum);
                    Linenum=0;
                    GetStartDateTime();
                    starttick = HAL_GetTick();
                    JieTaoflag=1;
                    ShowTimeflag =0;
                    continue;
                }
            }
			else
			{
				DrawGraph_Flash(CurChan,Linenum);
                Linenum=0;
                GetStartDateTime();
                starttick = HAL_GetTick();
                JieTaoflag=1;
                ShowTimeflag =0;
                continue;
			
			
			}

            if(SampleCnt>=SaveCnt)
            {
                LinenumSample++;
                if(LinenumSample<=PointsPix)
                {
                    SaveSD();
                    SaveUSBEXL(filenameUSB,UartCmdT);
                }
                if(LinenumSample<PointsNum)
                {
                    //r1==1 还没满足400个点说明慢了 说明要补点
                    //400个点了 r1==0 说明快了 等一下
                    if(r1==1)//判断一帧是否完成
                    {
//                      while(LinenumSample<PointsPix)//少点快了
//                      {
//                          LinenumSample++;
//                          SaveSD();
//                          SaveUSBEXL(filenameUSB,UartCmdT);
//                      }
                        startticksampOld = startticksamp;
                        GetStartDateTimesamp();//初始化获得时间
                        while(++LinenumSample<=PointsPix)
                        {
                            for(i=0; i<CHANNUM; i++)
                                Dsd.Data.CHD[i].SDData.endtick=(LinenumSample*SampletimeMs[DataSave.Data_type.SaveSample])+startticksampOld;
								SaveSD();
								SaveUSBEXL(filenameUSB,UartCmdT);
                        }
						
                        GetSDSaveTime(RTC_DateStrStartsamp,RTC_TimeStrStartsamp);//获取保持时间
                        startticksamp= HAL_GetTick();
                        r1=0;
                        LinenumSample=0;
                    }

                }
                else
                {
                    GetStartDateTimesamp();//初始化获得时间
                    GetSDSaveTime(RTC_DateStrStartsamp,RTC_TimeStrStartsamp);//获取保持时间
                    startticksamp= HAL_GetTick();
                    r1=0;
                    LinenumSample=0;

                }
                SampleCnt=0;//存储完成
            }

            /* if(++SampletickCnt>=SaveCnt)//判断一帧是否完成
                      {
                          GetStartDateTimesamp();//初始化获得时间
                          startticksamp= starttick;
                          SampletickCnt=0;//回头开始了
                      }*/
        }
        else if( UartCmdT ==CmdR_FDATA&&CAMold == SampleTimeCur.CAM&&ZoneCAMold ==ZoneCur.CAM) //只存单个通道快存
        {

            DDa[ChanMData.ch-1][0].DP.Num = ChanMData.chmd[0].DfStr;//接收到数据更新 第一个数据
            DDa[ChanMData.ch-1][0].Uint[DDa[ChanMData.ch-1][0].vcind].Col = ChanData.chd[0].UintOne.Col;  // 显示单位刷新
            DDa[ChanMData.ch-1][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新
            if(FileRes == FILETFOpen||FileRes == FILEUSBOpen)   //打开了
            {
                for(i=0; i<10; i++) //连续存10个数据
                {
                    UpLowCompFM(i,ChanMData.ch-1);//上下限比较
                    for(z=0; z<10; z++)
                        Dsd.Data.CHD[z].SDData.vcind = OFF;//所有通道都清除
                    GetSDSaveData(starttick,starttick+(LinenumFM)*5,ChanMData.chmd[i],ChanMData.ch-1,
                                  (((ChanMData.ch-1)<<4)&0xf0|(UartCmdT&0xf)));//数据存入那个要存通道
                    memcpy(DsdFM[i].uD,Dsd.uD,sizeof(Dsd.uD));
                    if(++LinenumFM>=PointsPix)
                    {
                        LinenumFM=0;
                        Dsd.Data.Timestamp+=2;
                        starttick = HAL_GetTick();
                        //多存一个点画图可以匹配
                        /*
                        GetSDSaveData(starttick,starttick+(LinenumFM)*5,ChanMData.chmd[i],ChanMData.ch-1,
                                              (((ChanMData.ch-1)<<4)&0xf0|(UartCmdT&0xf)));//数据存入
                        SaveSD();
                        */
                    }
                }
                if(FileRes == FILETFOpen)
                    SaveSDFM();
                else if(FileRes == FILEUSBOpen)
                    SaveUSBEXL(filenameUSB,UartCmdT);
            }
            else
            {
                LinenumFM=0;
                GetStartDateTime();
                Dsd.Data.Timestamp=time2Stamp(RTC_DateStrStart, RTC_TimeStrStart) ;
                starttick = HAL_GetTick();
            }
        }
        DoUarttick = HAL_GetTick() - DoUarttick;
        Dtime = 50;//默认50ms
        if(SampletimeMs[SampleTimeCur.CAM]>=50)
        {
            if(SampletimeMs[SampleTimeCur.CAM] > DoUarttick)
                Dtime = SampletimeMs[SampleTimeCur.CAM] - DoUarttick;
        }
        else
        {
            Dtime -= DoUarttick;
        }
        Dtime/=10;
        OSTimeDly ( Dtime%10, OS_OPT_TIME_PERIODIC , & err );
        for(j=0; j<Dtime; j++)
        {
            IsReTrigFile(FileSaveNum);//大型数据分文件处理
            if(SaveTrig == TRIGStart)//启动存储 复位头 不复位会有随机性错误
            {
                SampleCnt = 0;
                r1 = 0;//储存回头
//              if(SaveCnt==1)
//              {
//                  startticksamp= starttick;//意外的回头
//                  RTC_TimeStrStartsamp=RTC_TimeStrStart;
//                  RTC_DateStrStartsamp=RTC_DateStrStart;
//                  LinenumSample=Linenum;
//              }
//              else
//              {
                startticksamp= HAL_GetTick();//意外的回头
                GetStartDateTimesamp();//初始化获得时间
                LinenumSample=0;
//              }
            }
            TrigFile();//启动文件
            OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );
            if(ZoneCAMold !=ZoneCur.CAM)   //跨度切换和区域切换直接跳出 重画
            {
                DrawGraph_FlashZone(CurGraph,Linenum+1);//回头
                Linenum=0;
                GetStartDateTime();
                GetStartDateTimesamp();//初始化获得时间
                starttick = HAL_GetTick();
                startticksamp= starttick;//意外的回头
                r1=0;//回头开始了
                LinenumSample=0;
                JieTaoflag=1;
                ShowTimeflag =0;
                break;
            }
            else if(CAMold != SampleTimeCur.CAM)   //跨度切换和区域切换直接跳出 重画
            {
                DrawGraph_Flash(CurChan,Linenum+1);//回头
                Linenum=0;
                GetStartDateTime();
                GetStartDateTimesamp();//初始化获得时间
                starttick = HAL_GetTick();
                startticksamp= starttick;//意外的回头
                r1=0;//回头开始了
                LinenumSample=0;
                JieTaoflag=1;
                ShowTimeflag =0;
                break;
            }
        }
    }
}



/**
 * @brief  RS232_USART GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
void RS232_USART_Config(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

    RS232_USART_RX_GPIO_CLK_ENABLE();
    RS232_USART_TX_GPIO_CLK_ENABLE();

    /* 配置串口3时钟源*/
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UARTx;
    RCC_PeriphClkInit.Usart1ClockSelection = RCC_UARTxCLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    /* 使能 UART 时钟 */
    RS232_USART_CLK_ENABLE();

    /* 配置Tx引脚为复用功能  */
    GPIO_InitStruct.Pin = RS232_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = RS232_USART_TX_AF;
    HAL_GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStruct);

    /* 配置Rx引脚为复用功能 */
    GPIO_InitStruct.Pin = RS232_USART_RX_PIN;
    GPIO_InitStruct.Alternate = RS232_USART_RX_AF;
    HAL_GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStruct);

    /* 配置串RS232_USART 模式 */
    UartHandle.Instance = RS232_USART;
    UartHandle.Init.BaudRate = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    UartHandle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    HAL_UART_Init(&UartHandle);

    /*串口中断初始化 */
    HAL_NVIC_SetPriority(RS232_USART_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(RS232_USART_IRQ);
    /*配置串口接收中断 */
    __HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);
}


/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
    unsigned int k=0;
    do
    {
        HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k),1,1000);
        k++;
    }
    while(*(str + k)!='\0');
}

void Usart_SendStringL( USART_TypeDef * pUSARTx, uint8_t *str,uint8_t len)
{
    unsigned int k=0;
    for(k=0; k<len; k++)
    {
        HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k),1,1000);
    }
}
///重定向c库函数printf到串口RS232_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口RS232_USART */
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);
    return (ch);
}

///重定向c库函数scanf到串口RS232_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    int ch;
    HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);
    return (ch);
}
/*********************************************END OF FILE**********************/

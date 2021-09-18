/*
  * @file  RTC.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   RTC����
 */
#include "include.h"
//������ƿ�
OS_TCB RTCTaskTCB;
//�����ջ
CPU_STK RTC_TASK_STK[RTC_STK_SIZE];
u8 date_time[6] = {0,1,1,0,0,0};
RTC_TimeTypeDef RTC_TimeStr;
RTC_DateTypeDef RTC_DateStr;
const u8 Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const u16 monDays[12] = {0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
extern u32 SaveStamp;
u32 time2Stamp(RTC_DateTypeDef date, RTC_TimeTypeDef time)    //����ʱ��תʱ���
{
    u32 result;
    u16 Year=date.Year+2000;
    result = (Year - 1970) * 365 * 24 * 3600 + (monDays[date.Month-1] + date.Date - 1) * 24 * 3600 + (time.Hours-8) * 3600 + time.Minutes * 60 + time.Seconds;

    result += (date.Month>2 && (Year % 4 == 0) && (Year % 100 != 0 || Year % 400 == 0))*24*3600;    //����

    Year -= 1970;
    result += (Year/4 - Year/100 + Year/400)*24 * 3600;     //����
    return result;
}

void stamp2Time(u32 stamp,RTC_DateTypeDef* d,RTC_TimeTypeDef* t)        //ʱ���ת����ʱ��
{
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    u32 days;
    u16 leap_num;
    time.Seconds = stamp % 60;
    stamp /= 60;    //��ȡ��
    time.Minutes = stamp % 60;
    stamp += 8 * 60 ;
    stamp /= 60;    //��ȡСʱ
    time.Hours = stamp % 24;
    days = stamp / 24;
    leap_num = (days + 365) / 1461;
    if( ((days + 366) % 1461) == 0)
    {
        date.Year = (days / 366)+ 1970 -2000;
        date.Month = 12;
        date.Date = 31;
    }
    else
    {
        days -= leap_num;
        date.Year = (days / 365) + 1970 -2000;
        days %= 365;
        days += 1;
        if(((date.Year%4) == 0 ) && (days==60))
        {
            date.Month = 2;
            date.Date = 29;
        }
        else
        {
            if(((date.Year%4) == 0 ) && (days > 60)) --days;
            for(date.Month = 0; Days[date.Month] < days; date.Month++)
            {
                days -= Days[date.Month];
            }
            ++date.Month;
            date.Date = days;
        }
    }
    *d = date;
    *t = time;
}

uint8 rtc_init(void)
{
	uint8 res = 0;
    /* RTC���ã�ѡ��ʱ��Դ������RTC_CLK�ķ�Ƶϵ�� */
    res = RTC_CLK_Config();

    if (HAL_RTCEx_BKUPRead(&Rtc_Handle,RTC_BKP_DRX) != 0X32F2)
    {
        /* ����ʱ������� */
        RTC_TimeAndDate_Set();
    }
    else
    {
        /* ����Ƿ��Դ��λ */
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
        {
            //  printf("\r\n ������Դ��λ....\r\n");
        }
        /* ����Ƿ��ⲿ��λ */
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
        {
            //  printf("\r\n �����ⲿ��λ....\r\n");
        }
        //printf("\r\n ����Ҫ��������RTC....\r\n");
        /* ʹ�� PWR ʱ�� */
        __HAL_RCC_PWR_CLK_ENABLE();
        /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
        HAL_PWR_EnableBkUpAccess();
        /* �ȴ� RTC APB �Ĵ���ͬ�� */
        HAL_RTC_WaitForSynchro(&Rtc_Handle);
    }
	return res;
}

void RTC_work(void)
{
    static uint8_t Rtctmp=0;
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
    if(Rtctmp != RTC_TimeStructure.Seconds)
    {
        //����ʱ��仯
        RTC_TimeStr = RTC_TimeStructure;
        RTC_DateStr = RTC_DateStructure;
        (void)RTC->DR;
    }
    Rtctmp = RTC_TimeStructure.Seconds;
}

void DoScrSaver(void)
{
    u32 Stamp  = 0;
    if(DataSave.Data_type.ScrSaver>=1&&DataSave.Data_type.ScrSaver<=8&&SaveFlag==0)
    {
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr);
        if(Stamp>SaveStamp)
        {
            SaveFlag=1;    //�����Ѿ�����
        }
    }
}
//����5�����ɳ�ʼ��
void InitScrSaver(void)
{
    static  u8 timecnt =0;
    static  u8 flag = 0;
    if(flag == 0)
    {
        if(timecnt++>10)
        {
            flag=1;
            SaveStamp =  SetSaveStamp();  //����ˢ��
        }
    }
}

void DoTriggerRecord(void)
{
    u32 StampS  = 0;
    u32 StampE  = 0;
    u32 StampNow  = 0;
    static uint8 Recordflag = 0;
    if( trigTflag==1)
    {
        StampS = time2Stamp(DateStart, TimeStart);
        StampE = time2Stamp(DateEnd, TimeEnd);
        if(StampE>StampS)
        {
            StampNow = time2Stamp(RTC_DateStr, RTC_TimeStr);
            if(StampNow>StampE &&FileRes ==FILETFOpen&&Recordflag == 1)
            {
                SaveTrig = TRIGStopTF;
                Recordflag = 0;
            }
			else if(StampNow>StampE &&FileRes ==FILEUSBOpen&&Recordflag == 1)
            {
                SaveTrig = TRIGStopUSB;
                Recordflag = 0;
            }
            else if(StampNow>StampS&&StampNow<StampE&&FileRes ==FILEReady&&Recordflag == 0)
            {
                SaveTrig = TRIGStart;
                Recordflag = 1;
            }

        }
    }
}
void FLashHidFlag(void)
{
    static u8 cnt = 0;
    if(++cnt>5)
    {
        if(HidCnt!=HidCntOld)
        {
            HidFlag=ON;
        }
        else
        {
            HidFlag=OFF;
        }
        HidCnt=HidCntOld;
        cnt=0;
    }
}

//����RTC ����500msˢ��һ��
void RTC_task(void *p_arg)
{
    OS_ERR  err;
    uint8 i;
    while(1)
    {
        RTC_work();
        InitScrSaver();
        DoScrSaver();
        udisk_scan();
        for(i=0; i<5; i++)
        {

            DoTriggerRecord();
            OSTimeDly ( 90, OS_OPT_TIME_DLY, & err );  //��ʱ90ms
        }
        FLashHidFlag();
    }
}

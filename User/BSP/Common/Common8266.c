
/*
* @file    Common.c
* @author  JK LJ
* @version V1.0.0
* @date    18-6-2020
* @brief   8266通用显示自编函数
*/
#include  "include.h"

//任务控制块
OS_TCB WIFICheckTaskTCB;
//任务堆栈
CPU_STK WIFICheck_TASK_STK[WIFICheck_STK_SIZE];



//任务控制块
OS_TCB WIFITaskTCB;
//任务堆栈
CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];

u8 Wifi__TX_BUF[WIFI_REC_LEN] = {0};  //AT指令缓存区
char *                 itoa                                ( int value, char * string, int radix );

__IO WIFI_DATA newwifi;
u8 WIFI_SEND_STA;  //wifi发送标志位   1正在发送   0没有发送

uartFunction wifi_uart =UART8266F;     //wifi的串口

volatile UCHAR  ucRTUBuf4000[MB_SER_PDU_SIZE_MAX] = {0};
const USHORT usSndBufferCount4000 = 0x20B;
USHORT WifiDataFlashFlag=0;
void wifiWork_Init(void)
{
    uint8_t IP[4] =  {0};
    char Name[21]=  {0};
    char PW[21]=  {0};
    uint8_t STA = 0;
//    OS_ERR err;
    char i;
    /*
    memset(DataSave.Data_type.parameters.Wifi_Name,0,sizeof(DataSave.Data_type.parameters.Wifi_Name));
    strcpy(DataSave.Data_type.parameters.Wifi_Name,"JK17\0");
    memset(DataSave.Data_type.parameters.Wifi_PW,0,sizeof(DataSave.Data_type.parameters.Wifi_PW));
    strcpy(DataSave.Data_type.parameters.Wifi_PW,"1123456789\0");
    memset(DataSave.Data_type.parameters.YuMing,0,sizeof(DataSave.Data_type.parameters.YuMing));
    strcpy(DataSave.Data_type.parameters.YuMing,"udp.jk17.com\0");

    DataSave.Data_type.parameters.Ser_Port = 8101;
    DataSave.Data_type.parameters.Port = 61310;
    DataSave.Data_type.parameters.modbus_id = 8;
    */
    ESP8266_Rst();
//  ESP8266_AT_Test ();
    send_wificmd(2000,"AT");       //100为6秒左右
    send_wificmd(2000,"AT");       //100为6秒左右
    send_wificmd(20000,RESTORE);       //100为6秒左右
    send_wificmd(20000,RST);
    send_wificmd(20000,"AT+CWMODE=1");
    send_wificmd(20000,RST);
    //  WIFI_delay_m(2000);
    for(i = 0; i<20; i++)
    {
        Name[i]=DataSave.Data_type.parameters.Wifi_Name[i];
        PW[i]=DataSave.Data_type.parameters.Wifi_PW[i];
    }




    for(i = 0; i<30&&STA!=1; i++)
    {
        STA = send_wificmd(4000,"AT+CWJAP=\"%s\",\"%s\"",Name,PW);
    }
//  if(STA!=1)
//  {
//      LED_RED; //链接失败
//      LED_STA = STA_RED;
//  }
//  else
//  {
//    LED_YELLOW; //
//      LED_STA = STA_YELLOW;
//  }
    send_wificmd(20000,CIFSR);
//  IP[0] =     ( UCHAR )(parameters.IP12>>8);
//  IP[1] =     ( UCHAR )(parameters.IP12&0xff);
//  IP[2] =     ( UCHAR )(parameters.IP34>>8);
//  IP[3] =     ( UCHAR )(parameters.IP34&0xff);
//0316更改 域名查询
    send_wificmd(40000,"AT+CIPSTART=\"UDP\",\"114.114.114.114\",%d,%d,2",53,DataSave.Data_type.parameters.Port);
    ChaXunSW();

    while(IPflag==0)
    {
        send_dataTo8266(Single_ID_0,ucChaXun, sizeof(ucChaXun));
        WIFI_delay_m(100);
    }
//IP从域名得到
    IP[0] =     ( uint8_t  )(DataSave.Data_type.parameters.IP12>>8);
    IP[1] =     ( uint8_t  )(DataSave.Data_type.parameters.IP12&0xff);
    IP[2] =     ( uint8_t  )(DataSave.Data_type.parameters.IP34>>8);
    IP[3] =     ( uint8_t  )(DataSave.Data_type.parameters.IP34&0xff);
    send_wificmd(20000,RESTORE);       //100为6秒左右
    send_wificmd(20000,RST);
    send_wificmd(20000,"AT+CWMODE=1");
    send_wificmd(20000,RST);
    WIFI_delay_m(2000);
    STA = 0;
    for(i = 0; i<30&&STA!=1; i++)
    {
        STA = send_wificmd(4000,"AT+CWJAP=\"%s\",\"%s\"",Name,PW);
    }
    send_wificmd(20000,CIFSR);
    Clear_wifi_data();
    send_wificmd(40000,"AT+CIPSTART=\"UDP\",\"%d.%d.%d.%d\",%d,%d,2",IP[0],IP[1],IP[2],IP[3],DataSave.Data_type.parameters.Ser_Port,DataSave.Data_type.parameters.Port);
}

void UpDataWIFI(void)
{
    uint32 i;
    uint32 Val;
    if(WifiDataFlashFlag == 1)//可以更新数据
    {
        for(i = 0; i<MB_SER_PDU_SIZE_MAX; i++)
        {

            ucRTUBuf4000[i] = 0;

        }
        ucRTUBuf4000[4] = 0x20;
        ucRTUBuf4000[5] = 0x5;
        ucRTUBuf4000[6] = 0x1;
        ucRTUBuf4000[7] = 0x3;
        ucRTUBuf4000[8] = 0x2;
        for(i = 0; i<10; i++) //  通道数值
        {
            if(DDa[i][0].DP.Num.fval <0)  //小于0取负数
            {
                ucRTUBuf4000[9+0+i*4] |=0x80;
                Val=(uint32)(-DDa[i][0].DP.Num.fval *10);
            }
            else
            {
                Val=(uint32)(DDa[i][0].DP.Num.fval *10);
            }
            if(ChanData.chalarm[i]==1) //上超
            {
                ucRTUBuf4000[9+0+i*4] |=0x40;
            }
            else if(ChanData.chalarm[i]==2) //下超
            {
                ucRTUBuf4000[9+0+i*4] |=0x20;
            }
            if(Val>0xFFFFFF)   // 最多只有24位
            {
                Val=0xFFFFFF;
            }
            ucRTUBuf4000[9+1+i*4] = (Val>>16)&0xFF;
            ucRTUBuf4000[9+2+i*4] = (Val>>8)&0xFF;
            ucRTUBuf4000[9+3+i*4] = (Val)&0xFF;

        }
        for(i = 10; i<12; i++) //频率数值
        {
            if(DFrq[i-10].DP.Num.fval  <0)  //小于0取负数
            {
                ucRTUBuf4000[9+0+i*4] |=0x80;
                Val=(uint32)(-DFrq[i-10].DP.Num.fval *10);
            }
            else
            {
                Val=(uint32)(DFrq[i-10].DP.Num.fval *10);
            }
            if(ChanData.chalarm[i]==1) //上超
            {
                ucRTUBuf4000[9+0+i*4] |=0x40;
            }
            else if(ChanData.chalarm[i]==2) //下超
            {
                ucRTUBuf4000[9+0+i*4] |=0x20;
            }
            if(Val>0xFFFFFF)   // 最多只有24位
            {
                Val=0xFFFFFF;
            }
            ucRTUBuf4000[9+1+i*4] = (Val>>16)&0xFF;
            ucRTUBuf4000[9+2+i*4] = (Val>>8)&0xFF;
            ucRTUBuf4000[9+3+i*4] = (Val)&0xFF;

        }
        for(i = 12; i<128; i++)
        {
            ucRTUBuf4000[9+2+i*4] = 0x4E;
            ucRTUBuf4000[9+3+i*4] = 0x1F;
        }
        ucRTUBuf4000[521] = DataSave.Data_type.parameters.modbus_id>>8;
        ucRTUBuf4000[522] = DataSave.Data_type.parameters.modbus_id&0xff;
        WifiDataFlashFlag = 2; // 数据已经更新
    }
}





wifi_data_t wifidatas;
void wifi_WriteSendBuf(u8 lianjie,u8* WriteData,u8 len)
{
    memcpy(wifidatas.rBuf[wifidatas.rPtr1],WriteData,len);
    wifidatas.wifilen[wifidatas.rPtr1] = len;
    wifidatas.wifilianjie[wifidatas.rPtr1] = lianjie;
    wifidatas.rPtr1++;
    wifidatas.rPtr1%=wifi_SIZE;       //发送缓冲区循环
}
u8 wifi_ReadSendBuf(void )
{
    u8 t;
    if(wifidatas.rPtr1!=wifidatas.rPtr2)    //发送缓冲区及缓冲区队列首尾
    {
        t = wifidatas.rPtr2;
        wifidatas.rPtr2++;
        wifidatas.rPtr2%=wifi_SIZE;  //发送缓冲区循环
    }
    return t;
}
u8 WIFI_RecBufEmpty(void)
{
    if(wifidatas.rPtr1!=wifidatas.rPtr2)
    {
        return 1;  //接收缓冲区非空
    }
    else
    {
        return 0;
    }
}


//0317 lj因IP查询添加
void Clear_wifi_data(void)
{
    //modbus_data.modbus_is_working = 0;
    memset((void *)&wifidatas,0,sizeof(wifidatas));

}

#ifdef EN_USART8266_RX        //如果使能了接收
struct uart8266  Uart8266;


void uart8266_init_data(void)
{
    Uart8266.rPtr1=0;
    Uart8266.rPtr2=0;
    Uart8266.sPtr1=0;
    Uart8266.sPtr2=0;
    Uart8266.RX_STA=0;   //接收状态标记，1表示收到新数据。
    Uart8266.TX_STA=0;   //发送状态标记，1表示没有数据发送。
}


#endif

void USART8266_SendByte(u8 Data)
{
    HAL_UART_Transmit(&ESP8266_UartHandle,&Data, 1, 0xFFFF);
    while ( __HAL_USART_GET_FLAG (&ESP8266_UartHandle, USART_FLAG_TXE ) == RESET );
}
/*******************************************************************************
* Function Name  : UART8266_SendBuf
* Description    : 将数据存入UART8266发送缓冲区并启动发送，中断发送方式。
* Input          : u8 Sdata
* Output         : None
* Return         : None
*******************************************************************************/
void UART8266_SendBuf(u8 sData)
{
    uint8 a=0;
    UART8266_WriteSendBuf(sData);   //写一个字节数据到发送缓冲区
    if(Uart8266.TX_STA) //发送状态标记，1表示没有数据发送
    {

        __HAL_UART_DISABLE_IT(&ESP8266_UartHandle, UART_IT_TC); //禁止USAR8266发送完成中断
        //  USART8266->DR=UART8266_ReadSendBuf();  //读发送缓冲区一个字节
        a=UART8266_ReadSendBuf();  //读发送缓冲区一个字节
        HAL_UART_Transmit(&ESP8266_UartHandle,&a, 1, 0xFFFF);
        while ( __HAL_USART_GET_FLAG (&ESP8266_UartHandle, USART_FLAG_TXE ) == RESET );
        Uart8266.TX_STA=0;  //发送状态标记
        __HAL_UART_ENABLE_IT(&ESP8266_UartHandle, UART_IT_TC);    //使能USAR8266发送完成中断
    }

}


/*******************************************************************************
* Function Name  : UART8266_ReadBuf
* Description    : 从UART8266读接收缓冲区一个字节
* Input          : None
* Output         : None
* Return         : u8 ReadData
*******************************************************************************/
u8 UART8266_ReadRecBuf(void)
{
    u8 ReadData=0;
    if(Uart8266.rPtr1!=Uart8266.rPtr2)
    {
        ReadData=Uart8266.rBuf[Uart8266.rPtr2];
        Uart8266.rPtr2++;
        Uart8266.rPtr2%=UART8266_RBUFSIZE;  //接收缓冲区循环
    }
    return ReadData;
}


/*******************************************************************************
* Function Name  : UART8266_WriteBuf
* Description    : 写一个字节数据到接收缓冲区
* Input          : u8 WriteData
* Output         : None
* Return         : None
*******************************************************************************/
void UART8266_WriteRecBuf(u8 WriteData)
{
    Uart8266.rBuf[Uart8266.rPtr1]=WriteData;   //接收缓冲区及缓冲区队列首尾
    Uart8266.rPtr1++;
    Uart8266.rPtr1%=UART8266_RBUFSIZE;       //接收缓冲区循环
}


/*******************************************************************************
* Function Name  : UART8266_ReadSendBuf
* Description    : 从UART8266读发送缓冲区一个字节
* Input          : None
* Output         : None
* Return         : u8 ReadData
*******************************************************************************/
u8 UART8266_ReadSendBuf(void)
{
    u8 ReadData=0;
    if(Uart8266.sPtr1!=Uart8266.sPtr2)  //发送缓冲区及缓冲区队列首尾
    {
        ReadData=Uart8266.sBuf[Uart8266.sPtr2];
        Uart8266.sPtr2++;
        Uart8266.sPtr2%=UART8266_SBUFSIZE;  //发送缓冲区循环
    }
    return ReadData;
}


/*******************************************************************************
* Function Name  : UART8266_WriteSendBuf
* Description    : 写一个字节数据到发送缓冲区
* Input          : u8 WriteData
* Output         : None
* Return         : None
*******************************************************************************/
void UART8266_WriteSendBuf(u8 WriteData)
{
    Uart8266.sBuf[Uart8266.sPtr1]=WriteData;
    Uart8266.sPtr1++;
    Uart8266.sPtr1%=UART8266_SBUFSIZE;       //发送缓冲区循环
}


/*******************************************************************************
* Function Name  : RecBufEmpty
* Description    : 接收缓冲区非空
* Input          : u8 WriteData
* Output         : None
* Return         : None
*******************************************************************************/
u8 UART8266_RecBufEmpty(void)
{
    if(Uart8266.rPtr1!=Uart8266.rPtr2)
    {
        return 1;  //接收缓冲区非空
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
* Function Name  : RecBufEmpty
* Description    : 发送缓冲区非空
* Input          : u8 WriteData
* Output         : None
* Return         : None
*******************************************************************************/
u8 UART8266_SendBufEmpty(void)
{
    if(Uart8266.sPtr1!=Uart8266.sPtr2)
    {
        return 1;  //发送缓冲区非空
    }
    else
    {
        return 0;
    }
}

void checkdata(u8 DATA)
{
    __IO static u8 shu=0;
    __IO static u32 wifii;
    __IO static u8 STAw =0;
//    char temp[30] = {0};
    u8 i;
    u8 flag = 1;
//  if(STAw==0)
//  {
//      testcnt++;
//      testcnt1=0;
//  }
//  if(testcnt1++<200)
//  {
//      testdata[testcnt1] = DATA;
//
//  }
//  else
//      testcnt1=0;

    switch(STAw)
    {
        case 0:
            if(DATA=='O')
                STAw=1;
            else if(DATA=='E')
                STAw=2;
            else if(DATA=='+')
                STAw=3;
//          else if(DATA==10)
//              STAw=22;
            else if(DATA=='C')
                STAw=9;
            break;
//      case 22:
//          if(DATA=='+')
//              STAw=3;
//          else
//              STAw=0;
//            break;
        case 1:
            if(DATA=='K')
            {
                STAw=0;
                WIFI_SEND_STA=1;
            }
            else
                STAw=0;
            break;
        case 2:
            if(DATA=='R')
            {
                STAw=0;
                WIFI_SEND_STA=2;
            }
            else
                STAw=0;
            break;
        case 3:
            if(DATA=='I')
                STAw=4;
            else
                STAw=0;
            break;
        case 4:
            if(DATA=='P')
                STAw=5;
            else
                STAw=0;
            break;
        case 5:
            if(DATA=='D')
                STAw=6;
            else
                STAw=0;
            break;
        case 6:
            if(DATA==',')
            {
                shu=1;
                if(shezhi_work ==1)
                {
                    STAw=21;
                }
                else
                {
                    STAw=7;
                }
            }
            else
                STAw=0;
            break;
        case 7:
            if(DATA!=',')
            {
                if(DATA!=':')
                {
                    if(shu==0)
                    {
                        newwifi.GET_DATANUM = DATA-0x30;
                    }
                    else if(shu==1)
                    {
                        newwifi.GET_DATANUM = (newwifi.GET_DATANUM)*10+(DATA-0x30);
                    }
                    else if(shu==2)
                    {
                        newwifi.GET_DATANUM = (newwifi.GET_DATANUM)*10+(DATA-0x30);
                    }
                    else if(shu==3)
                    {
                        newwifi.GET_DATANUM = (newwifi.GET_DATANUM)*10+(DATA-0x30);
                    }
                    shu++;
                }
                else
                {
                    //  uart_printf("获取%d:",newwifi.GET_DATANUM);
                    shu=0;
                    wifii=0;
                    STAw=8;
                    memset((char*)newwifi.getwifidata,0,sizeof(newwifi.getwifidata));
                }
            }
            else if(shu>10)
            {
                shu=0;
                STAw=0;
                newwifi.GET_DATANUM=0;
            }
            else
            {
                newwifi.CONNCET_NUM=newwifi.GET_DATANUM;
                shu=0;
                newwifi.GET_DATANUM=0;
            }
            break;
        case 8:
            if(wifii<newwifi.GET_DATANUM)
            {
                newwifi.getwifidata[wifii] = DATA;
                wifii++;
                if(wifii==newwifi.GET_DATANUM||wifii>=WIFI_REC_LEN)
                {
                    wifi_WriteSendBuf(newwifi.CONNCET_NUM,(u8*)&newwifi.getwifidata[0],wifii);
                    newwifi.isreaded = 1;
                    STAw=0;
                    wifii=0;
//              memset(testdata,0,200);
//              testcnt1=0;
                    //memcpy(wifi_json_data,(const char*)newwifi.getwifidata,sizeof(wifi_json_data));
                }
            }
            break;
        case 9:
            if(DATA=='L')                //CLOSED
                STAw=10;
            else if(DATA=='O')               //CONNECT FAIL/OK
                STAw=14;
            else
                STAw=0;
            break;
        case 10:
            if(DATA=='O')           //CLOSED
                STAw=11;
            else
                STAw=0;
            break;
        case 11:
            if(DATA=='S')           //CLOSED
                STAw=12;
            else
                STAw=0;
            break;
        case 12:
            if(DATA=='E')           //CLOSED
                STAw=13;
            else
                STAw=0;
            break;
        case 13:
            STAw=0;
            if(DATA=='D')           //CLOSED
            {
                STAw=0;
                newwifi.CONNECT_OK = 0;
            }
        case 14:
            if(DATA=='N')           //CONNECT FAIL/OK   服务端断开是CONNECT FAIL
                STAw=15;
            else
                STAw=0;
            break;
        case 15:
            if(DATA=='N')           //CONNECT FAIL/OK
                STAw=16;
            else
                STAw=0;
            break;
        case 16:
            if(DATA=='E')           //CONNECT FAIL/OK
                STAw=17;
            else
                STAw=0;
            break;
        case 17:
            if(DATA=='C')           //CONNECT FAIL/OK
                STAw=18;
            else
                STAw=0;
            break;
        case 18:
            if(DATA=='T')           //CONNECT FAIL/OK
                STAw=19;
            else
                STAw=0;
            break;
        case 19:
            if(DATA==0X0D)           //CONNECT FAIL/OK
            {
                STAw=0;
                newwifi.CONNECT_OK=1;
            }
            else if(DATA==' ')
                STAw=20;
            else
                STAw=0;
            break;
        case 20:
            if(DATA=='F')           //CONNECT FAIL/OK
                newwifi.CONNECT_OK=0;
            STAw=0;
        case 21:
            if(DATA!=','&&shu==0)
            {
                newwifi.CONNCET_NUM=DATA-0x30;
            }
            else
            {
                if(DATA!=',')
                {
                    if(DATA!=':')
                    {
                        if(shu==1)
                        {
                            newwifi.GET_DATANUM = DATA-0x30;
                        }
                        else if(shu==2)
                        {
                            newwifi.GET_DATANUM = (newwifi.GET_DATANUM)*10+(DATA-0x30);
                        }
                        else if(shu==3)
                        {
                            newwifi.GET_DATANUM = (newwifi.GET_DATANUM)*10+(DATA-0x30);
                        }
                        else if(shu==4)
                        {
                            newwifi.GET_DATANUM = (newwifi.GET_DATANUM)*10+(DATA-0x30);
                        }
                        shu++;
                    }
                    else
                    {
                        //uart_printf("获取%d:",newwifi.GET_DATANUM);
                        flag = 1;
                        for(i=12; i<30; i++)
                        {
                            if(ucChaXun[i]!=newwifi.getwifidata[i])
                            {
                                flag = 0;
                                break;
                            }
                        }
                        if (flag == 1)
                            if(newwifi.getwifidata[41] == 4)
                            {
                                DataSave.Data_type.parameters.IP12 = (newwifi.getwifidata[42]<<8)|newwifi.getwifidata[43];
                                DataSave.Data_type.parameters.IP34 = (newwifi.getwifidata[44]<<8)|newwifi.getwifidata[45];
                                IPflag =1;
                            }
                        shu=0;
                        wifii=0;
                        STAw=8;
                        memset((char*)newwifi.getwifidata,0,sizeof(newwifi.getwifidata));
                    }
                }
                else if(shu>10)
                {
                    shu=0;
                    STAw=0;
                    newwifi.GET_DATANUM=0;
                }
                else
                {
                    shu=1;
                    newwifi.GET_DATANUM=0;
                }
            }
            break;
    }
}
void WIFI_printf(char* fmt,...)
{
    u16 i,j;
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)Wifi__TX_BUF,fmt,ap);
    va_end(ap);
    i=strlen((const char*)Wifi__TX_BUF);
    //直接串口7发出去
    for(j=0; j<i; j++)
    {
        HAL_UART_Transmit(&ESP8266_UartHandle,(uint8_t *)(&Wifi__TX_BUF[j]), 1, 0xFFFF);
        while ( __HAL_USART_GET_FLAG (&ESP8266_UartHandle, USART_FLAG_TXE ) == RESET );
    }

    /*
        for(j=0;j<i;j++)
        {
          wifi_uart.SendByte(Wifi__TX_BUF[j]);
        }
    */
}


char * WIFI_SEND(char *p,int start,int len)
{
    int times = 100;         //100为6秒左右
    char *q = p+start;

    WIFI_SEND_STA =0;
    while(len-start)
    {
        wifi_uart.SendByte(*q);
        q++;
        len--;
    }
    while(WIFI_SEND_STA==0&&times)
    {
        WIFI_delay_m(100);
        times--;
    }
    return q;
}
void send_dataTo8266(u8 num,u8* buf,int len)
{
    //int len =strlen((const char*)buf);
    int times = len/WIFIDATALEN;
    int tim = len%WIFIDATALEN;
    int i ;
    char *p = (char *)buf;
    if(shezhi_work==1)
    {
        if(times)
        {
            for(i=0; i<times; i++)
            {
                if(send_wificmd(200,"AT+CIPSEND=%d,%d",num,WIFIDATALEN)==1)
                {
                    p = WIFI_SEND((char*)buf,WIFIDATALEN*i,WIFIDATALEN*(i+1));
                }
                else
                {
                    ;//modbus_data.modbus_is_working=0;
                }
            }
        }
        if(tim)
        {
            //  if(send_wificmd(200,"AT+CIPSEND=%d,%d",num,tim)==1)
            if(send_wificmd(200,"AT+CIPSEND=%d",tim)==1)
            {

                p =WIFI_SEND((char*)p,0,tim);
            }
            else
            {
                ;//modbus_data.modbus_is_working=0;
            }
        }
    }
    else
    {
        if(times)
        {
            for(i=0; i<times; i++)
            {
                if(send_wificmd(200,"AT+CIPSEND=%d",WIFIDATALEN)==1)
                {
                    p = WIFI_SEND((char*)buf,WIFIDATALEN*i,WIFIDATALEN*(i+1));
                }
            }
        }
        if(tim)
        {
            if(send_wificmd(200,"AT+CIPSEND=%d",tim)==1)
            {
                p =WIFI_SEND((char*)p,0,tim);
            }
        }
    }
}




void send_wificmd1(u16 waittime,char* cmd,...)     //waittime = 100为6秒左右
{
//    u16 times = waittime*2;
    va_list ap;
    va_start(ap,cmd);
    vsprintf((char*)Wifi__TX_BUF,cmd,ap);
    va_end(ap);
    WIFI_SEND_STA =0;
    WIFI_printf("%s\r\n",Wifi__TX_BUF); //发送命令
}
u8 send_wificmd(u16 waittime,char* cmd,...)     //waittime = 100为6秒左右
{
    u16 times = waittime*2;
    va_list ap;
    va_start(ap,cmd);
    vsprintf((char*)Wifi__TX_BUF,cmd,ap);
    va_end(ap);
    WIFI_SEND_STA =0;
    WIFI_printf("%s\r\n",Wifi__TX_BUF); //发送命令
    while(WIFI_SEND_STA==0&&times)
    {
        WIFI_delay_m(1);
        if(times-- == waittime)
        {
            WIFI_printf("%s\r\n",Wifi__TX_BUF);//再发一次命令
        }
    }
    /*
        if(WIFI_SEND_STA==2)
        {
            //uart_printf("执行%s失败\r\n",Wifi__TX_BUF);
            WIFI_SEND_STA=2;
        }
        else if(WIFI_SEND_STA==1)
        {
            //uart_printf("执行%s成功\r\n",Wifi__TX_BUF);
            WIFI_SEND_STA=1;

        }
        else
        {
            //uart_printf("执行%s超时\r\n",Wifi__TX_BUF);
    //              if(LED_STA==STA_YELLOW&&LED_STA==STA_GREEN)//不是黄色和红色
    //                      {
    //                          LED_PURPLE;
    //                          LED_STA = STA_PURPLE;
    //                      }
            WIFI_SEND_STA=WIFI_SEND_STA;
        }
    */
    return WIFI_SEND_STA;
}


uint8_t ucChaXun[30] = {0};
uint8_t    IPflag = 0;
void ChaXunSW(void)
{
    u8 i,numall,cur;
    u8 j=0;
    u8 num[3] = {0};
    ucChaXun[0] =0x12;
    ucChaXun[1] =0x34;
    ucChaXun[2] =0x01;
    ucChaXun[3] =0x00;
    ucChaXun[4] =0x00;
    ucChaXun[5] =0x01;
    memset(&ucChaXun[6],0,6);
    numall= sizeof(DataSave.Data_type.parameters.YuMing);
    for(i=0; i<numall&&j<3; i++)
    {
        if(DataSave.Data_type.parameters.YuMing[i]=='.')
            num[j++] = i;
        else if(DataSave.Data_type.parameters.YuMing[i]==0)
        {
            num[j++] = i;
        }

    }
    numall = 3;
    for(i=0; i<numall; i++)
    {
        num[i] -= i;
        if(i>1)
            num[i] -= (num[i-1]+num[i-2]);
        else if(i>0)
            num[i] -= num[i-1];
    }

    for(i=0; i<3; i++)
    {
        numall += num[i];
    }
    j=0;
    ucChaXun[12] = num[j++];
    for(i=0; i<numall; i++)
    {
        if(DataSave.Data_type.parameters.YuMing[i]=='.')
            ucChaXun[13+i] = num[j++];
        else
            ucChaXun[13+i] = DataSave.Data_type.parameters.YuMing[i];
    }
    cur = 13+i;

//  ucChaXun[15] =0x70;
//  ucChaXun[16] =0x4;
//  ucChaXun[17] =0x6a;
//  ucChaXun[18] =0x6b;
//  ucChaXun[19] =0x31;
//  ucChaXun[20] =0x37;
//  ucChaXun[21] =0x3;
//  ucChaXun[22] =0x63;
//  ucChaXun[23] =0x6f;
//  ucChaXun[24] =0x6d;
    ucChaXun[cur] =0x0;
    ucChaXun[cur++] =0x0;
    ucChaXun[cur++] =0x1;
    ucChaXun[cur++] =0x0;
    ucChaXun[cur++] =0x1;
}


/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */
char * itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

char * u16toa( uint16_t value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }


    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


void wifi_getdata()
{
    u8 DATA;
    if(wifi_uart.RecBufEmpty())  //接收缓冲区非空
    {
        DATA =wifi_uart.ReadRecBuf();
        if(shezhi_work==1)
        {
            checkdata(DATA);
        }
    }
}
//设置WIFI任务1ms刷新一次
void WIFICheck_task(void *p_arg)
{
    OS_ERR  err;
    while(1)
    {
        wifi_getdata();
        OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );  //延时
    }
}



//设置WIFI任务2s刷新一次
void WIFI_task(void *p_arg)
{
    OS_ERR  err;
    uint8 i;
    wifiWork_Init();
    WifiDataFlashFlag = 1;// 数据可以更新
    while(1)
    {
        for (i = 0; i <195; i ++)
        {
            if(WifiDataFlashFlag==2)
            {
                send_dataTo8266(Single_ID_0,(u8*)ucRTUBuf4000, usSndBufferCount4000);
                WifiDataFlashFlag=3; //发送1次就不发
            }
            OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );  //延时
        }
        WifiDataFlashFlag=1;
    }
}

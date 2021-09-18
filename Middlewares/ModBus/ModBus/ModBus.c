/*
    ��ModbusЭ����ʱֻ֧��RTUģʽ��ֻ֧����ΪModbus���豸��
    ��ʱ֧�ֵĹ����루16���ƣ����±���ʾ��
    01.����Ȧ״̬����������λ��״̬����Ч��λΪ0-31��
    02.������λ״̬�����������λ��״̬����Ч��λΪ0-31��
    03.�����ּĴ�������������ּĴ�������ֵ����Ч��λΪ0-99��
    04.������Ĵ��������������Ĵ�������ֵ����Ч��ַΪ0-1��
    05.ǿ�Ƶ�����Ȧ��ǿ�Ƶ������λ��״̬����Ч��λΪ0-31��
    06.Ԥ�Ƶ����Ĵ������趨һ���Ĵ�������ֵ����Ч��ַΪ0-99��
    0F.ǿ�ƶ����Ȧ��ǿ�ƶ�����λ��״̬����Ч��ַΪ0-31��
    10.Ԥ�ƶ���Ĵ������趨����Ĵ�������ֵ����Ч��ַΪ0-99��

    ��ʱ֧�ֵĴ������Ϊ��
    01 ���Ϸ����ܴ���ӻ����յ���һ�ֲ���ִ�й��ܴ��롣������ѯ����󣬸ô���ָʾ�޳����ܡ�����֧�ֵĹ��ܴ��룩
    02 ���Ϸ����ݵ�ַ���յ����ݵ�ַ���Ǵӻ�������ĵ�ַ������ʼ��ַ������Ч��Χ�ڣ�
    03 ���Ϸ����ݲ�ѯ��������ֵ�Ǵӻ��������ֵ��������ʼ��ַ�Ļ����ϣ���������ǲ��Ϸ��ģ�

    ʹ�õ�����Դ�У�
    1.��ʱ��1��������֡�жϣ�������ʹ��TIM2��
    2.���ڽ��շ���һ�ԡ����ڽ��պͷ������ݡ�

    ���û����õĺ����У�
    1.void ModInit(u16 id);//����Modbus��ʼ�����ں�������ǰ�������ʼ������������Main������
    2.void ModRcv(void);//����modbus��Ϣ���գ����ڴ��ڽ����ж�
    3.void ModSend(void);//����modbus��Ϣ����,���ڴ��ڷ����ж�

    ��Ҫ�û�ʵ�ֵĺ����У�
    TIM��ʱ�ĳ�ʼ��
    ���ڵĳ�ʼ����
    ������ʹ�õ�KEIL 5.10������оƬΪstm32,3.5��׼������ʵ�֣�
    ���û��йص���Ҫ��TIM2��ʱ����USART3���ڣ��û��������õ���������
    �ɽ��������е�TIM2��USART3�����Լ��ļ��ɡ�
                                                                     */

//����ͷ�ļ�
#include "modbus.h"
#include "include.h"
#include "main.h"
#include "usb_device.h"
//modbus��ͨѶ����
u8 Tim_Out;         //����3.5���ַ�ʱ�䣬����ȡ3ms (������9600��ʱ���Լ2�㼸����)
u8 Rcv_Complete;    //һ֡�Ƿ��Ѿ��������
u8 Send_Complete;   //һ֡�Ƿ��Ѿ��������
u8 Com_busy;        //ͨѶ��æ����ʾ��һ֡��δ�������
u8 Rcv_Buffer[210]; //������Ž��յ���������һ֡����    ����һ���ֽ�������Ž��յ�����Ч�ֽ�����Ҳ���������е���Ч�ֽ�����
u8 Send_Buffer[210];//������Ŵ����͵�������һ֡���ݣ���һ���ֽ�������Ŵ����͵���Ч�ֽ�����Ҳ���������е���Ч�ֽ�����
u8 Rcv_Data;        //������Ž��յ�һ���ֽ�
u8 Send_Data;       //�������Ҫ���͵�һ�ֽ�
u8 Mod_Id;          //������־��Ϊ��վ��վ��
u8 Rcv_Num;         //������ʾ���յ�һ֡����Ч�ֽ������ӹ����뵽CRCУ�飩
u8 Send_Num;        //������ʾ�����͵�һ֡���ֽ���
u8 *PointToRcvBuf;  //����ָ����յ����ݻ���
u8 *PointToSendBuf; //����ָ������͵����ݻ���
u8 Comu_Busy;       //������ʾ�ܷ������һ֡����
u8 HaveMes;         //�Ƿ�������Ϣ
u16 HoldReg[522];   //Modbus�Ĵ���
u8 Input_Buffer[INPUT_IO_LENGTH];       //����IO����
u8 Output_Buffer[OUTPUT_IO_LENGTH ];    //���IO����
const static BYTE JKName [12] = {0x4A, 0x4B, 0x38, 0x34, 0x30, 0x00, 0x00,  0xA, 0x00, 0x00, 0x00, 0x00};
uint8 ModBusMod[CHANNUM];
//CRCУ�����ò���
/* CRC ��λ�ֽ�ֵ��*/
const static u8 auchCRCHi[] =
{
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40
} ;
/* CRC��λ�ֽ�ֵ��*/
const static u8 auchCRCLo[] =
{
    0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,
    0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD,
    0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
    0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
    0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
    0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
    0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
    0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
    0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
    0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
    0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
    0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
    0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
    0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
    0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
    0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
    0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
    0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
    0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
    0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
    0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
    0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
    0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
    0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
    0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
    0x43,0x83,0x41,0x81,0x80,0x40
} ;
//������ƿ�
OS_TCB USBTaskTCB;
//�����ջ
CPU_STK USB_TASK_STK[USB_STK_SIZE];

OS_SEM  USB_SEM;        //����һ���ź�������������ͬ��
//����modbus�ĺ���
void ModInit(u8 Id);//����Modbus��ʼ��������IdΪվ�ţ�1-255��
void ModRcv(char Rdata);//����modbus��Ϣ����
void ModSend(void);//����modbus��Ϣ����
u8 MessageHandle(u8 *pointer_in,u8 *pointer_out);//�����յ�����Ϣ֡
void ReadOutputBit(u8 *pointer_1,u8 *pointer_2);//����Ȧ
void ReadInputBit(u8 *pointer_1,u8 *pointer_2);//������λ
void ReadHoldingReg(u8 *pointer_1,u8 *pointer_2);//�����ּĴ���
void ReadInputReg(u8 *pointer_1,u8 *pointer_2);//������Ĵ���
void ForceSingleCoil(u8 *pointer_1,u8 *pointer_2);//ǿ�Ƶ�����Ȧ
void PresetSingleReg(u8 *pointer_1,u8 *pointer_2);//Ԥ�Ƶ����Ĵ���
void ForceMulCoil(u8 *pointer_1,u8 *pointer_2);//ǿ�ƶ����Ȧ
void PresetMulReg(u8 *pointer_1,u8 *pointer_2);//Ԥ�ƶ���Ĵ���
void ErrorHandle(u8 Mode,u8 *Pointer);//������Ϣ֡����
u16 CRC16(u8 *puchMsgg,u8 usDataLen);//���ڼ���CRCУ����
void TimeHandler(void);
void GetHoldReg(void);
void SetSingalReg(u16 Address);
void SetMulReg(u16 startAddress, u16 Num);
//extern    void USART3_IRQHandler(void); //U2�Ĵ����жϣ���usart.c���档
//extern void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
//extern  ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
//extern    void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);
//extern uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
//extern void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);
/***************Modbus ��ʼ��*********************************
*   �������ƣ�Modbus ��ʼ��
*   �������ܣ�����Modbus��ʼ��
*   �������룺IdΪModbusվ�š�
*   �������أ���
*   ���ú���������������USART3��ʼ����֡�ж�ʱ��������ʱ��T2��ʼ����
*   ������д��user-F
*   ��д���ڣ�2015 �� 3 �� 15 ��
*   �����汾��
**************************************************************/
void ModInit(u8 Id)
{
    //modbus������ʼ��
    PointToRcvBuf=Rcv_Buffer;
    PointToSendBuf=Send_Buffer;
    Send_Num=1;//���͵�����˳���������ĵڼ�������
    Mod_Id=Id;//վ������
    Rcv_Buffer[1]=Mod_Id;
    Send_Buffer[1]=Mod_Id;
    Comu_Busy=0;
    //TIM2_Int_Init(35,7199); //ModBus֡�������ʱ��������ʱ������ʱ3.5ms����Ƶ7199.����Ƶ��10khz
    TIMx_Configuration();
    __HAL_TIM_DISABLE(&TIM_TimeBaseStructure);
    __HAL_TIM_SET_COUNTER(&TIM_TimeBaseStructure,0);
}


/***************Modbus ��Ϣ����*********************************
*   �������ƣ�ModRcv
*   �������ܣ�������Modbus��Ϣ����
*   �������룺�ޡ�
*   ������д��user-F
*   ��д���ڣ�2015 �� 3 �� 15 ��
*   �����汾��
**************************************************************/
void ModRcv(char Rdata)
{
    HaveMes=1;//��ʾ���յ�����Ϣ
//      Rcv_Data=USART_ReceiveData(USART3);
    Rcv_Data = Rdata;
    if(Comu_Busy!=1)//�����æ�����Խ�����һ֡��Ϣ
    {
        //TIM_Cmd(TIM2, DISABLE); //��ֹ����
        __HAL_TIM_DISABLE(&TIM_TimeBaseStructure);
//          TIM_SetCounter(TIM2,0); //��ʱ������

        __HAL_TIM_SET_COUNTER(&TIM_TimeBaseStructure,0);
        if((Tim_Out!=0)&&(Rcv_Data==Mod_Id))//������ʱ�䳬����3.5���ַ���ͬʱ���ܵ��ֽں��Լ���վ��һ�£�����Ϊ���տ�ʼ

        {
            Rcv_Complete=0;//��ʾ����֡���տ�ʼ
            Rcv_Num=0;//�������ݸ�����ʼ��
            Rcv_Num++;//ͬʱ������һ
        }
        if((0==Tim_Out)&&(0==Rcv_Complete))//������ڽ���һ֡������������
        {
            if(Rcv_Num<100)
            {
                Rcv_Buffer[Rcv_Num+1]=Rcv_Data;//�����ݷ������������
                Rcv_Num++;//ͬʱ������һ
            }
            else
            {
                Rcv_Complete=1;
                Comu_Busy=1;
                Rcv_Buffer[0]=Rcv_Num;
                *(PointToSendBuf+2)=*(PointToRcvBuf+2);//��ȡ������
                ErrorHandle(6,PointToSendBuf);//��ʾ�������ֽ���(�ӻ��豸æµ)
                Rcv_Num=0;
            }
        }
        Tim_Out=0;
//        TIM_Cmd(TIM2, ENABLE);//����4.5ms��ʱ���������ַ��ı��ع��ƣ�
        __HAL_TIM_ENABLE(&TIM_TimeBaseStructure);

    }

}

/*
�������ܣ�����Modbus��Ϣ����
�������룺�ޡ�
����������ޡ�
*/
void ModSend(void)
{
    u8 tsendbuf[64] = {0};
    u8 num = 0;
    uint8 i = 0;
    for(i = 0; i < 64; i++)
    {
        Send_Data=*(PointToSendBuf+Send_Num);
        //USART_SendData(USART3,Send_Data);
        tsendbuf[num++] = Send_Data;
        Send_Num++;
        if(Send_Num>(*PointToSendBuf))//�����Ѿ����
        {
            Comu_Busy=0;
            *PointToSendBuf=0;
            Rcv_Num=0;
            Send_Num=1;
            num = 0;
            //�������ݷ���
            //      USART_ITConfig(USART3, USART_IT_TC, DISABLE);//�ر����ݷ����ж�
            while(USBD_BUSY==USBD_CUSTOM_HID_SendReport_HS(tsendbuf,64));
            break;
        }
    }

}

/* �������ܣ�Modbusר�ö�ʱ����TIM2��
   �������룺�ޡ�
����������ޡ�
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static u8 Cnt = 0;
    OS_ERR  err;
    if(htim==(&TIM_TimeBaseStructure))
    {
        //TimeHandler();

        OSSemPost(&USB_SEM,OS_OPT_POST_1,&err);//�����ź���
    }
    if(htim==(&TIM_TimeBaseStructureAD))
    {
        BEEP_TOGGLE;
    }
    if(htim==(&TIM_TimeBaseStructure7))
    {
        Cnt++;
        if(DataSave.Data_type.Bright==1)
        {

            if(Cnt>=5)
            {
                LCD_PWM_TOGGLE ;//
                Cnt=0;
            }


        }
        else if(DataSave.Data_type.Bright==2||SaveFlag==1)
        {
            if(Cnt==1)
            {
                LCD_PWM_LO;//�ߵ�ƽ����
            }
            else if(Cnt==9)
            {
                LCD_PWM_HI;//�ߵ�ƽ����
                Cnt=0;
            }
        }
        else
        {
            if(Cnt==100)
            {
                LCD_PWM_HI;//�ߵ�ƽ����
                Cnt=0;
            }
        }
    }
}
void TimeHandler(void)
{
    uint32 i;


    Tim_Out=1;
    //  TIM_Cmd(TIM2,DISABLE);
    __HAL_TIM_DISABLE(&TIM_TimeBaseStructure);
    //  TIM_SetCounter(TIM2,0);
    __HAL_TIM_SET_COUNTER(&TIM_TimeBaseStructure,0);
    Rcv_Complete=1;
    Rcv_Buffer[0]=Rcv_Num;

    if(HaveMes!=0 && Rcv_Num>4)
    {
        Comu_Busy=1;

        for(i=4; i<Rcv_Num; i++)
        {
            Rcv_Buffer[0]=i;
            if(MessageHandle(PointToRcvBuf,PointToSendBuf)==1)
                break;
        }
    }


}


/* �������ܣ�CRCУ���ú���
   �������룺puchMsgg��Ҫ����CRCУ�����Ϣ��usDataLen����Ϣ���ֽ���
������������������CRCУ���롣
*/
u16 CRC16(u8 *puchMsgg,u8 usDataLen)//puchMsgg��Ҫ����CRCУ�����Ϣ��usDataLen����Ϣ���ֽ���
{
    u8 uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ��*/
    u8 uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ��*/
    u8 uIndex ; /* CRCѭ���е�����*/
    while (usDataLen--) /* ������Ϣ������*/
    {
        uIndex = uchCRCHi ^ *puchMsgg++ ; /* ����CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    return ((uchCRCHi << 8) | uchCRCLo) ;
}


/* �������ܣ����������Ϣ֡���д������չ����벻ͬ�����ò�ͬ�ĺ�������
   �������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
         pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
         �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�
*/
u8 MessageHandle(u8 *pointer_in,u8 *pointer_out)
{
    u16 CalKey;//���������У��ֵ
    u16 RcvKey;//���յ���У��ֵ
    u8 len;//���յ���У��ֵ
    uint8 res =0;
    HaveMes=0;//�����Ϣλ
    //��ȡ���յ���У��ֵ
    RcvKey=(u16)*(pointer_in+(*pointer_in-1));
    RcvKey=RcvKey<<8;
    RcvKey=RcvKey|(u16)*(pointer_in+(*pointer_in));

    CalKey=CRC16(pointer_in+1,*pointer_in-2);
    if(CalKey==RcvKey)
    {
        res = 1;
        len = *pointer_in+1;
        memset(pointer_in+len,0,210-len); //������0
        switch(*(pointer_in+2))//�ڶ����ֽ�Ϊ������
        {
            case 0x01:
                ReadOutputBit(pointer_in,pointer_out);//�������Ȧ
                break;
            case 0x02:
                ReadInputBit(pointer_in,pointer_out);//������λ
                break;
            case 0x03:
                ReadHoldingReg(pointer_in,pointer_out);//�����ּĴ���
                break;
            case 0x04:
                ReadInputReg(pointer_in,pointer_out);//������Ĵ���
                break;
            case 0x05:
                ForceSingleCoil(pointer_in,pointer_out);//ǿ�Ƶ�����Ȧ״̬
                break;
            case 0x06:
                PresetSingleReg(pointer_in,pointer_out);//Ԥ�Ƶ����Ĵ���
                break;
            case 0x0F:
                ForceMulCoil(pointer_in,pointer_out);//ǿ�ƶ����Ȧ
                break;
            case 0x10:
                PresetMulReg(pointer_in,pointer_out);//Ԥ�ƶ���Ĵ���
                break;
            default:
            {
                *(pointer_out+2)=*(pointer_in+2);//��ȡ������
                ErrorHandle(1,pointer_out);//���������
            }
            break;
        }
    }
    else
    {
        Comu_Busy=0;
        res = 0;
    }
    return res;
}


/***************��ȡ��Ȧ״̬*********************************
*   �������ƣ�ReadOutputBit������
*   �������ܣ���ȡ���뻺��Input_Buffer[40]��ֵ
*   �������룺����ָ�룬pointer_inָ���������������Ϣ֡�����飬
              pointer_out����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�������
*   ������д��user-F
*   ��д���ڣ�2015 �� 3 �� 25 ��
*   �����汾��V1.1����ԭ������
**************************************************************/
void ReadOutputBit(u8 *pointer_in,u8 *pointer_out)//pointer_in�������룬pointer_out�������
{
    u16 Address=0;//����ȡ��Ȧ��ʼ��ַ
    u16 Num=0;//Ҫ��ȡ����Ȧ����
    u8 Byte=0;//Ҫ��ȡ������λ�����ܹ�ռ�õ��ֽ�����
    u32 PortTemp[2] = {0,0}; //������ŴӶ˿�ȡ������PortTemp[0]���ݵ�32��
//  u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_in+3))*256+(*(pointer_in+4));//�ȵõ���Ȧ��ַ(*(pointer_in+3))*256 = (*(pointer_in+3)) ����8λ��
    Num=(u16)(*(pointer_in+5))*256+(*(pointer_in+6));//�ȵõ�Ҫ��ȡ����Ȧ����
    *(pointer_out+2)=0x01;//�������ֽ�Ϊ������

    if(*(pointer_in)==8)      //������յ����ֽ�������8��������һ������֡
    {
        if(Address < 24 ) //ֻҪ��ַС��24�����ǺϷ���ַ
        {
            if(Address + Num<=24 && Num>0) //ֻҪ��ַ����������0С��80�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;
                u16 k;
                Byte=Num/8; //Ҫ��ȡ����Ȧ�����ܹ�ռ�õ��ֽ���
                if(Num%8!=0)
                    Byte++;//����������Ļ�����һ���ֽڣ�ʣ��ĸ�λ����
                *(pointer_out+3)=Byte;//���ĸ��ֽ�ΪҪ���͵ĸ���
                *(pointer_out)=1+1+1+Byte+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+��Ȧ��Ϣ+CRCУ��

                //�����뻺�������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                for(i = 0,j = 0; i < 40; i++)
                {
                    if(i%32 == 0) j++;
                    PortTemp[j] = PortTemp[j] <<1;
                    PortTemp[j] |= (Output_Buffer[i] & 0x01);

                }
                //������һ���ֽ�һ���ֽ�װ���������ݻ���������
                for(i=4,j=0, k=Num; k>0; k--,i++)
                {
                    if( Num%32 == 0 ) j++; //����32���֣�������ʱ���������1
                    *(pointer_out+i)=(u8)(PortTemp[j]&0x00FF);//��ȡһ���ֽڵĳ��ȣ��������
                    PortTemp[j] = PortTemp[j]>>8;//�ٽ����������ƶ�8λ

                }

                //д��У����
                SendKey=CRC16(pointer_out+1,*pointer_out-2);
                //�����������У����װ��������ݻ�����
                *(pointer_out+(*pointer_out-1))=(u8)(SendKey>>8);
                *(pointer_out+(*pointer_out))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
                ModSend();
            }
            else
            {
                ErrorHandle(3,pointer_out);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_out);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}
/***************��ȡ�����ŷ�״̬*********************************
*   �������ƣ�ReadSERVO������
*   �������ܣ���ȡ���뻺��Input_Buffer[40]��ֵ
*   �������룺����ָ�룬pointer_inָ���������������Ϣ֡�����飬
        pointer_out����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�������
*   ������д��user-F
*   ��д���ڣ�2015 �� 3 �� 25 ��
*   �����汾��V1.1����ԭ������
**************************************************************/
void ReadSERVO(u8 *pointer_in,u8 *pointer_out)//pointer_in�������룬pointer_out�������
{
    u16 Address=0;//����ȡ�ŷ����
    u16 Num=0;//Ҫ��ȡ����Ȧ����
    u8 Byte=0;//Ҫ��ȡ������λ�����ܹ�ռ�õ��ֽ�����
    u32 PortTemp[2] = {0,0}; //������ŴӶ˿�ȡ������PortTemp[0]���ݵ�32��
//  u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_in+3))*256+(*(pointer_in+4));//�ȵõ���Ȧ��ַ(*(pointer_in+3))*256 = (*(pointer_in+3)) ����8λ��
    Num=(u16)(*(pointer_in+5))*256+(*(pointer_in+6));//�ȵõ�Ҫ��ȡ����Ȧ����
    *(pointer_out+2)=0x01;//�������ֽ�Ϊ������

    if(*(pointer_in)==8)      //������յ����ֽ�������8��������һ������֡
    {
        if(Address < 24 ) //ֻҪ��ַС��24�����ǺϷ���ַ
        {
            if(Address + Num<=24 && Num>0) //ֻҪ��ַ����������0С��80�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;
                u16 k;
                Byte=Num/8; //Ҫ��ȡ����Ȧ�����ܹ�ռ�õ��ֽ���
                if(Num%8!=0)
                    Byte++;//����������Ļ�����һ���ֽڣ�ʣ��ĸ�λ����
                *(pointer_out+3)=Byte;//���ĸ��ֽ�ΪҪ���͵ĸ���
                *(pointer_out)=1+1+1+Byte+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+��Ȧ��Ϣ+CRCУ��

                //�����뻺�������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                for(i = 0,j = 0; i < 40; i++)
                {
                    if(i%32 == 0) j++;
                    PortTemp[j] = PortTemp[j] <<1;
                    PortTemp[j] |= (Output_Buffer[i] & 0x01);

                }
                //������һ���ֽ�һ���ֽ�װ���������ݻ���������
                for(i=4,j=0, k=Num; k>0; k--,i++)
                {
                    if( Num%32 == 0 ) j++; //����32���֣�������ʱ���������1
                    *(pointer_out+i)=(u8)(PortTemp[j]&0x00FF);//��ȡһ���ֽڵĳ��ȣ��������
                    PortTemp[j] = PortTemp[j]>>8;//�ٽ����������ƶ�8λ

                }

                //д��У����
                SendKey=CRC16(pointer_out+1,*pointer_out-2);
                //�����������У����װ��������ݻ�����
                *(pointer_out+(*pointer_out-1))=(u8)(SendKey>>8);
                *(pointer_out+(*pointer_out))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
                ModSend();

            }
            else
            {
                ErrorHandle(3,pointer_out);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_out);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}

/***************��ȡ����״̬*********************************
*   �������ƣ�ReadInputBit������
*   �������ܣ���ȡ���뻺��Input_Buffer[40]��ֵ
*   �������룺����ָ�룬pointer_inָ���������������Ϣ֡�����飬
              pointer_out����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�������
*   ������д��user-F
*   ��д���ڣ�2015 �� 3 �� 25 ��
*   �����汾��V1.1����ԭ������
**************************************************************/
void ReadInputBit(u8 *pointer_in,u8 *pointer_out)//pointer_in�������룬pointer_out�������
{
    u16 Address=0;//����ȡ����λ��ʼ��ַ
    u16 Num=0;//Ҫ��ȡ������λ����
    u8 Byte=0;//Ҫ��ȡ������λ�����ܹ�ռ�õ��ֽ�����
    u32 PortTemp[2] = {0,0}; //������ŴӶ˿�ȡ������PortTemp[0]���ݵ�32��
//  u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_in+3))*256+(*(pointer_in+4));//�ȵõ�����λ��ַ(*(pointer_in+3))*256 = (*(pointer_in+3)) ����8λ��
    Num=(u16)(*(pointer_in+5))*256+(*(pointer_in+6));//�ȵõ�Ҫ��ȡ������λ����
    *(pointer_out+2)=0x01;//�������ֽ�Ϊ������

    if(*(pointer_in)==8)      //������յ����ֽ�������8��������һ������֡
    {
        if(Address < 40 ) //ֻҪ��ַС��24�����ǺϷ���ַ�����24����Ȧ
        {
            if(Address + Num<=40 && Num>0) //��ַ�Ϸ����ж�
            {
                //����forѭ��
                u16 i;
                u16 j;
                u16 k;
                Byte=Num/8; //Ҫ��ȡ������λ�����ܹ�ռ�õ��ֽ���
                if(Num%8!=0)
                    Byte++;//����������Ļ�����һ���ֽڣ�ʣ��ĸ�λ����

                *(pointer_out+3)=Byte;//���ĸ��ֽ�ΪҪ���͵ĸ���
                *(pointer_out)=1+1+1+Byte+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+����λ��Ϣ+CRCУ��

                //�����뻺�������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                for(i = 0,j = 0; i < 24; i++)
                {
                    if(i%32 == 0) j++;
                    PortTemp[j] = PortTemp[j] <<1;
                    PortTemp[j] |= (Input_Buffer[i] & 0x01);

                }
                //������һ���ֽ�һ���ֽ�װ���������ݻ���������
                for(i=4,j=0, k=Num; k>0; k--,i++)
                {
                    if( Num%32 == 0 ) j++; //����32���֣�������ʱ���������1
                    *(pointer_out+i)=(u8)(PortTemp[j]&0x00FF);//��ȡһ���ֽڵĳ��ȣ��������
                    PortTemp[j] = PortTemp[j]>>8;//�ٽ����������ƶ�8λ

                }

                //д��У����
                SendKey=CRC16(pointer_out+1,*pointer_out-2);
                //�����������У����װ��������ݻ�����
                *(pointer_out+(*pointer_out-1))=(u8)(SendKey>>8);
                *(pointer_out+(*pointer_out))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�

            }
            else
            {
                ErrorHandle(3,pointer_out);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_out);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}




/* �������ܣ���ȡ���ּĴ���
    �������룺����ָ�룬pointer_inָ���������������Ϣ֡�����飬
              pointer_out����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
              �����Ԫ�ذ���ModbusЭ����֯��
    ����������ޡ�
                                                                                          */
void ReadHoldingReg(u8 *pointer_in,u8 *pointer_out)//pointer_in�������룬pointer_out�������
{
    u16 Address=0;//����ȡ�Ĵ�����ʼ��ַ��HoldReg[i],iΪ0-99��Ӧ��ַ��0��99��
    u16 Num=0;//Ҫ��ȡ�ļĴ�������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ
    Address=(u16)(*(pointer_in+3))*256+(*(pointer_in+4));//�ȵõ��Ĵ�����ʼ��ַ
    Num=(u16)(*(pointer_in+5))*256+(*(pointer_in+6));//�ȵõ�Ҫ��ȡ�ļĴ�������
    *(pointer_out+2)=0x03;//�������ֽ�Ϊ������

    if(*(pointer_in)==8)      //������յ����ֽ�������8��������һ������֡
    {

        if(Address<522) //ֻҪ��ַС��100�����ǺϷ���ַ
        {
            GetHoldReg();//�Ĵ���ˢ��
            if(Address+Num<=522&&Num>0) //ֻҪ��ַ����������0С��100�����ǺϷ�����
            {

                //����forѭ��
                u16 i;
                u16 j;

                *(pointer_out+3)=Num*2;//���ĸ��ֽ�ΪҪ���͵��ֽڸ���
                *(pointer_out)=1+1+1+Num*2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+�Ĵ�����Ϣ+CRCУ��


                for(i=Address,j=4; i<Address+Num; i++,j+=2)
                {
                    *(pointer_out+j)=(u8)(HoldReg[i]>>8);//�ȷŸ�λ
                    *(pointer_out+j+1)=(u8)(HoldReg[i]&0x00FF);//�ٷŵ�λ
                }

                //д��У����
                SendKey=CRC16(pointer_out+1,*pointer_out-2);
                //�����������У����װ��������ݻ�����
                *(pointer_out+(*pointer_out-1))=(u8)(SendKey>>8);
                *(pointer_out+(*pointer_out))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
                ModSend();

            }
            else
            {
                ErrorHandle(3,pointer_out);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_out);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}



/***************��ȡ�ŷ�״̬*********************************
*   �������ƣ�ReadServoReg������
*   �������ܣ���ȡ���뻺��Servo_Buffer[7]��ֵ
*   �������룺����ָ�룬pointer_inָ���������������Ϣ֡�����飬
              pointer_out����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�������
*   ������д��user-F
*   ��д���ڣ�2015 �� 3 �� 25 ��
*   �����汾��V1.1����ԭ������
**************************************************************/

void ReadInputReg(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
{
    u16 Address=0;//����ȡ�Ĵ�����ʼ��ַ��HoldReg[i],iΪ0-99��Ӧ��ַ��0��99��
    u16 Num=0;//Ҫ��ȡ�ļĴ�������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ
    u32 PortTemp;//������ŴӶ˿�ȡ���������ݣ���ʱ������
    u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u32 CalTemp=0;//������ʱ����


    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ʼ��ַ
    Num=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�Ҫ��ȡ�ļĴ�������
    *(pointer_2+2)=0x04;//�������ֽ�Ϊ������

    if(*(pointer_1)==8)   //������յ����ֽ�������8��������һ������֡
    {
        if(Address<2) //ֻҪ��ַС��2�����ǺϷ���ַ
        {
            if(Address+Num<=2&&Num>0) //ֻҪ��ַ����������0С��2�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;

                *(pointer_2+3)=Num*2;//���ĸ��ֽ�ΪҪ���͵��ֽڸ���
                *(pointer_2)=1+1+1+Num*2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+�Ĵ�����Ϣ+CRCУ��

                //���˿�ADC1��ADC2������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                //ReadData=ADC_GetConversionValue(ADC2);
                PortTemp=(u32)(ReadData);
                PortTemp=PortTemp<<16;
//              ReadData=ADC_GetConversionValue(ADC1);
                PortTemp=PortTemp|(u32)(ReadData);

                //��PortTemp�е������Ƚ���Ԥ����
                PortTemp=PortTemp<<(Address*16);
                for(i=4,j=Num*2; j>0; i++,j-=2)
                {
                    CalTemp=(u16)(PortTemp<<16);
                    *(pointer_2+i)=(u8)(CalTemp>>8);//�ȷŸ�λ
                    *(pointer_2+i+1)=(u8)(CalTemp&0x00FF);//�ٷŵ�λ
                }

                //д��У����
                SendKey=CRC16(pointer_2+1,*pointer_2-2);
                //�����������У����װ��������ݻ�����
                *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
                *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�

            }
            else
            {
                ErrorHandle(3,pointer_2);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}


/* �������ܣ�ǿ�Ƶ�����Ȧ״̬
�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
        pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
        �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */

void ForceSingleCoil(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
{
    u16 Address=0;//��ǿ����Ȧ��ʼ��ַ��//GPIO_X,XΪA��B�����˿ڣ�ÿ���˿�16λ����Ӧ��ַ0����31��
    u16 Command=0;//����Ϊ0xFF00����0x0000��0xFF00ǿ����ȦON��0x0000ǿ����ȦOFF��
    u32 PortTemp;//������ŴӶ˿�ȡ����������
    u32 CalTemp;//��ʱ������
    u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ���Ȧ��ַ
    Command=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�����ֵ
    *(pointer_2+2)=0x05;//�������ֽ�Ϊ������

    if(*(pointer_1)==8)   //������յ����ֽ�������8��������һ������֡
    {

        if(Address<32) //ֻҪ��ַС��32�����ǺϷ���ַ
        {

            *(pointer_2)=1+1+2+2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+��Ȧ��ַ+��Ȧ����+CRCУ��
            *(pointer_2+3)=*(pointer_1+3);//����ֵַд������ļĴ�����
            *(pointer_2+4)=*(pointer_1+4);

            //���˿�C��D������Ԥ�ȶ��뵽��ʱ�����ݻ�����
            ReadData=//GPIO_ReadOutputData(//GPIOD);
                PortTemp=(u32)(ReadData);
            PortTemp=PortTemp<<16;
            ReadData=//GPIO_ReadOutputData(//GPIOC);
                PortTemp=PortTemp|(u32)(ReadData);

            if(Command==0xFF00||Command==0x0000)
            {
                //��PortTemp�е����ݴ������Ҫ��λ������װ��������ݻ�����
                switch(Command)
                {
                    case 0xFF00:
                    {
                        //����Ҫ��λ��һ
                        CalTemp=0x00000001<<Address;
                        PortTemp|=CalTemp;

                        //����Ϣ֡�������Ӧλд�϶�Ӧ����
                        *(pointer_2+5)=0xFF;
                        *(pointer_2+6)=0x00;
                    }
                    break;

                    case 0x0000:
                    {
                        //����Ҫ��λ����
                        CalTemp=~(0x00000001<<Address);
                        PortTemp&=CalTemp;

                        //����Ϣ֡�������Ӧλд�϶�Ӧ����
                        *(pointer_2+5)=0x00;
                        *(pointer_2+6)=0x00;
                    }
                    break;

                }

                //�ٽ�����д��
                //GPIO_Write(//GPIOC,(u16)(PortTemp&0x0000FFFF));
                //GPIO_Write(//GPIOD,(u16)(PortTemp>>16));

                //д��У����
                SendKey=CRC16(pointer_2+1,*pointer_2-2);
                //�����������У����װ��������ݻ�����
                *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
                *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
            }
            else
            {
                Comu_Busy=0;
            }

        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}


/* �������ܣ�Ԥ�Ƶ����Ĵ���
�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
       pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
       �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */

void PresetSingleReg(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
{
    u16 Address=0;//��Ԥ�ƼĴ�������ʼ��ַ��//GPIO_X,XΪA��B�����˿ڣ�ÿ���˿�16λ����Ӧ��ַ0����31��
    u16 PresetValue=0;//Ԥ����ֵ
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ַ
    PresetValue=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�Ԥ��ֵ
    *(pointer_2+2)=0x06;//�������ֽ�Ϊ������

    if(*(pointer_1)==8)   //������յ����ֽ�������8��������һ������֡
    {

        if(Address<522) //ֻҪ��ַС��100�����ǺϷ���ַ
        {

            *(pointer_2)=1+1+2+2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�Ĵ�����ַ+�Ĵ�����ֵ+CRCУ��
            *(pointer_2+3)=*(pointer_1+3);//����ֵַд������ļĴ�����
            *(pointer_2+4)=*(pointer_1+4);
            *(pointer_2+5)=*(pointer_1+5);//����ֵд������Ĵ�����
            *(pointer_2+6)=*(pointer_1+6);

            HoldReg[Address]=PresetValue;//��Ԥ��ֵд�뱣�ּĴ���

            //д��У����
            SendKey=CRC16(pointer_2+1,*pointer_2-2);
            //�����������У����װ��������ݻ�����
            *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
            *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);
            //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
            ModSend();
            SetSingalReg(Address);

        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }

    }
    else
    {
        Comu_Busy=0;
    }
}


/* �������ܣ�ǿ�ƶ����Ȧ״̬
�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
       pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
       �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */

void ForceMulCoil(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
{
    u16 Address=0;//��ǿ����Ȧ��ʼ��ַ��//GPIO_X,XΪC��D�����˿ڣ�ÿ���˿�16λ����Ӧ��ַ0����31��
    u16 Num=0;//Ҫǿ�Ƶ���Ȧ����
    u8  ByteCount;//����ֵ���ֽڸ���
    u32 CommandValue;//�������ֵ����Ч��ÿ��λ������λ���߸�λָ����ַ����Ȧ
    u32 PortTemp;//������ŴӶ˿�ȡ����������
    u32 CalTemp;//��ʱ������
    u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ
    u8  CountTemp;//����ʵ����Ҫ�������ֽ���

    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ���Ȧ��ַ
    Num=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ���Ȧ����
    ByteCount= *(pointer_1+7);//��ʾ����ֵ���ֽ���
    CountTemp=(u8)(Num/8);
    if(Num%8!=0)
        CountTemp++;
    *(pointer_2+2)=0x0F;//�������ֽ�Ϊ������


    if((*(pointer_1)==9+ByteCount)&&ByteCount>0&&ByteCount<=4&&CountTemp==ByteCount)//������յ����ֽ�������Ԥ���ĸ��������������ֽ���������
        //��Χ����һ������֡
    {

        if(Address<32) //ֻҪ��ַС��32�����ǺϷ���ַ
        {
            if(Address+Num<=32&&Num>0) //ֻҪ��ַ����������0С��32�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;

                //������ֵ��һЩ��������CommandValue
                CommandValue=0;
                for(i=0,j=ByteCount; j>0; i++,j--)
                {
                    CommandValue|=((u32)*(pointer_1+8+i))<<8*i;//���������ݻ����е����ݴ���CommandValue
                }
                CommandValue=CommandValue<<Address;//�ƶ�����Ӧ��ʼλλ��

                *(pointer_2)=1+1+2+2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+��Ȧ��ʼ��ַ+��Ȧ����+CRCУ��
                *(pointer_2+3)=*(pointer_1+3);//����ֵַд������ļĴ�����
                *(pointer_2+4)=*(pointer_1+4);
                *(pointer_2+5)=*(pointer_1+5);//����Ȧ����ֵд������ļĴ�����
                *(pointer_2+6)=*(pointer_1+6);

                //���˿�C��D������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                //ReadData=//GPIO_ReadOutputData(//GPIOD);
                PortTemp=(u32)(ReadData);
                PortTemp=PortTemp<<16;
                //ReadData=//GPIO_ReadOutputData(//GPIOC);
                PortTemp=PortTemp|(u32)(ReadData);

                //����Ҫ�����������ݰ�Ҫ����
                CalTemp=0xFFFFFFFF << (32-Num);
                CalTemp=CalTemp >> (32-Num-Address);
                CalTemp=~CalTemp;
                PortTemp&=CalTemp;
                PortTemp|=CommandValue;

                //�ٽ�����д��
                //GPIO_Write(//GPIOC,(u16)(PortTemp&0x0000FFFF));
                //GPIO_Write(//GPIOD,(u16)(PortTemp>>16));

                //д��У����
                SendKey=CRC16(pointer_2+1,*pointer_2-2);
                //�����������У����װ��������ݻ�����
                *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
                *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�

            }
            else
            {
                ErrorHandle(3,pointer_2);//�����ȡ����
            }
        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}


/* �������ܣ�Ԥ�ƶ���Ĵ���
�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
          pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
          �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */

void PresetMulReg(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
{
    u16 Address=0;//��Ԥ�ƼĴ�������ʼ��ַ��HoldReg[i],iΪ0-99��Ӧ��ַ��0��99��
    u16 Num=0;//ҪԤ�ƵļĴ�������
    u8  ByteCount;//Ԥ��ֵ���ֽڸ���
    u16 PresetValue=0;//Ԥ����ֵ
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ַ
    Num=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ���Ԥ�ƼĴ�������
    *(pointer_2+2)=0x10;//�������ֽ�Ϊ������
    ByteCount= *(pointer_1+7);//��ʾ����ֵ���ֽ���

    if((*(pointer_1)==9+ByteCount)&&ByteCount>0&&ByteCount<=200&&ByteCount==(u8)(Num*2))//������յ����ֽ�������Ԥ���ĸ��������������ֽ�����������Χ����һ������֡
    {

        if(Address<522) //ֻҪ��ַС��100�����ǺϷ���ַ
        {

            if(Address+Num<=522&&Num>0) //ֻҪ��ַ����������0С��100�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;

                *(pointer_2)=1+1+2+2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�Ĵ�����ַ+�Ĵ�������+CRCУ��
                *(pointer_2+3)=*(pointer_1+3);//����ֵַд������ļĴ�����
                *(pointer_2+4)=*(pointer_1+4);
                *(pointer_2+5)=*(pointer_1+5);//������д������Ĵ�����
                *(pointer_2+6)=*(pointer_1+6);

                for(i=0,j=0; i<Num; i++,j+=2)
                {
                    PresetValue=(u16)(*(pointer_1+8+j))*256+(*(pointer_1+9+j));//�ȵõ�Ԥ��ֵ
                    HoldReg[Address+i]=PresetValue;//��Ԥ��ֵд�뱣�ּĴ���
                }


                //д��У����
                SendKey=CRC16(pointer_2+1,*pointer_2-2);
                //�����������У����װ��������ݻ�����
                *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
                *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);

                //�������ݷ���
//              USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
                ModSend();
                SetMulReg(Address, Num);

            }
            else
            {
                ErrorHandle(3,pointer_2);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
}



/* �������ܣ�����֡��������1,2,3,6�����������1Ϊ���Ϸ������룬2���Ϸ����ݵ�ַ��3���Ϸ����ݣ�6�ӻ��豸æµ��
�������룺��һ������Mode����ָʾ��һ�����
          pointer����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
          �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */
void  ErrorHandle(u8 Mode,u8 *Pointer)
{
    u16 SendKey;//Ҫ�������ݵ�У��ֵ

    HaveMes=0;//�����Ϣλ
    //  TIM_Cmd(TIM2,DISABLE);
    // TIM_SetCounter(TIM2,0);
    __HAL_TIM_DISABLE(&TIM_TimeBaseStructure);
    __HAL_TIM_SET_COUNTER(&TIM_TimeBaseStructure,0);
    Rcv_Complete=1;
    Comu_Busy=1;
    Rcv_Buffer[0]=Rcv_Num;
    switch(Mode)
    {
        case 1:
            *(Pointer+3)=0x01;//��������
            break;
        case 2:
            *(Pointer+3)=0x02;//�����ַ
            break;
        case 3:
            *(Pointer+3)=0x03;//��������
            break;
        case 6:
            *(Pointer+3)=0x06;//���豸æ
            break;
    }
    *Pointer=0x05;//����Ĵ�����Ч���ݸ���
    *(Pointer+2)|=0x80;//���������λ��һ
    //д��У����
    SendKey=CRC16(Pointer+1,*Pointer-2);
    //�����������У����װ��������ݻ�����
    *(Pointer+(*Pointer-1))=(u8)(SendKey>>8);
    *(Pointer+(*Pointer))=(u8)(SendKey&0x00FF);

    //�������ݷ���
//    USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�
    ModSend();
}



/*******************************************************************************************
��������: String2BcdTime
��   �� : ���ļ����ַ���תΪBCD��ʱ��
�������: ��Ҫת���ı����ļ����ַ��������� ���Լ�ת��֮��Ҫ����bcdʱ�������
�������: ��
��   �� : ��
********************************************************************************************/
void String2BcdTime(BYTE name_buf[], BYTE bcd_buf[])
{
    /* �ļ�����ʽΪ�� ��2018-09-15 12:25:26.mp4�� */
    /* BCD ʱ���ʽΪ ��180915122526�� */
    bcd_buf[0] = 0x20;
    bcd_buf[1] = (BYTE)((name_buf[2] - '0')<<4) + (name_buf[3] - '0');
    bcd_buf[2] = (BYTE)((name_buf[5] - '0')<<4) + (name_buf[6] - '0');
    bcd_buf[3] = (BYTE)((name_buf[8] - '0')<<4) + (name_buf[9] - '0');
    bcd_buf[4] = (BYTE)((name_buf[11] - '0')<<4) + (name_buf[12] - '0');
    bcd_buf[5] = (BYTE)((name_buf[14] - '0')<<4) + (name_buf[15] - '0');
    bcd_buf[6] = (BYTE)((name_buf[17] - '0')<<4) + (name_buf[18] - '0');
    bcd_buf[7] = 0x20;
}

void Bytes2Reg(u16* Reg,uint8* byte)
{
    uint16 stemp =0;
    *Reg = byte[0];
    stemp = byte[1];
    stemp <<=8;
    stemp |=byte[2];
    *(Reg+1) = stemp;
}

void GetHoldReg(void)
{
    uint32 i,j;
    uint8 bufNull[3]= {0x7f,0x7f,0x7f};
    uint8 buf[4]= {0};
    char str[50]= {0};
    BYTE bcd_buf[8] = {0};
    union u16To2_u8 d;
    memset(HoldReg,0,sizeof(HoldReg));
    for(i=0; i<5; i++)
    {
        // d.buf[1] = ChanSet.Mod[i*2];
        //  d.buf[0] = ChanSet.Mod[i*2+1];
        d.buf[1] = ModBusMod[i*2];
        d.buf[0] = ModBusMod[i*2+1];
        HoldReg[i] =d.Date;
    }
    HoldReg[32] = ChanSet.SYNC;
    for(i=33,j=0; i<33+5; i++,j++)
    {
        d.buf[1] = ChanSet.Time[j*2];
        d.buf[0] = ChanSet.Time[j*2+1];
        HoldReg[i] =d.Date;
    }
    HoldReg[65] = ChanSet.alarm;
    //memcpy( &HoldReg[66],&ChanData.frq[0].byte,sizeof(ChanData.frq[0].byte));
    //memcpy( &HoldReg[68],&ChanData.frq[1].byte,sizeof(ChanData.frq[1].byte));
    Bytes2Reg(&HoldReg[66],ChanData.frq[0].byte);
    Bytes2Reg(&HoldReg[68],ChanData.frq[1].byte);
    for(i=0; i<CHANNO; i++)
    {
        //ͨ���ɼ�����
        if(DataSave.Data_type.DisLog[2+i][0].index[1] == OFF)  //
        {
            Bytes2Reg(&HoldReg[70+i*2],bufNull);
        }
        else
            Bytes2Reg(&HoldReg[70+i*2],ChanData.chd[i].byte);
        //ͨ��������
        if(DataSave.Data_type.DisLog[2+i][0].index[1] == OFF)  //
        {
            FloattoBYTE4(DataSave.Data_type.TempMax[i].fval,buf);

            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.TempMin[i].fval,buf);

            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
        else if(DataSave.Data_type.DisLog[2+i][0].index[1] == VDC)  //��ѹģʽ
        {
            FloattoBYTE4(DataSave.Data_type.DCMax[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.DCMin[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
        else if(DataSave.Data_type.DisLog[2+i][0].index[1] == VT)  //�¶�ģʽ
        {
            FloattoBYTE4(DataSave.Data_type.TempMax[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.TempMin[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
        else if(DataSave.Data_type.DisLog[2+i][0].index[1] == VI)  //����ģʽ
        {
            FloattoBYTE4(DataSave.Data_type.DIMax[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.DIMin[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
        else if(DataSave.Data_type.DisLog[2+i][0].index[1] == VG)  //����ģʽ
        {
            FloattoBYTE4(DataSave.Data_type.VGMax[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.VGMin[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
        else if(DataSave.Data_type.DisLog[2+i][0].index[1] == VR)  //ʪ��ģʽ
        {
            FloattoBYTE4(DataSave.Data_type.VRMax[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.VRMin[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
        else
        {
            FloattoBYTE4(DataSave.Data_type.TempMax[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4],buf);
            memset(buf,0,sizeof(buf));
            FloattoBYTE4(DataSave.Data_type.TempMin[i].fval,buf);
            Bytes2Reg(&HoldReg[198+i*4+2],buf);
            memset(buf,0,sizeof(buf));
        }
    }
    sprintf(str,"20%0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",
            RTC_DateStr.Year,
            RTC_DateStr.Month,
            RTC_DateStr.Date,
            RTC_TimeStr.Hours,
            RTC_TimeStr.Minutes,
            RTC_TimeStr.Seconds);
    String2BcdTime((BYTE*)str,bcd_buf);
    for(i=512,j=0 ; i<512+4; i++,j++)
    {
        d.buf[1] = bcd_buf[j*2];
        d.buf[0] = bcd_buf[j*2+1];
        HoldReg[i] =d.Date;
    }
    for(i=516,j=0 ; i<516+6; i++,j++)
    {
        d.buf[1] = JKName[j*2];
        d.buf[0] = JKName[j*2+1];
        HoldReg[i] =d.Date;
    }
}

void ModBusMod2ChanSetMod(uint32 i)
{

    if(ModBusMod[i] ==1)
    {

        ChanSet.Mod[i] =ModBusMod[i];
    }
    else if(ModBusMod[i] >1&&ModBusMod[i] <16)//�¶�ģʽ
    {
        ChanSet.Mod[i] = ModBusMod[i];

    }
    else if(ModBusMod[i] <18)//��ѹģʽ
    {
        ChanSet.Mod[i] = ModBusMod[i];

    }
    else if(ModBusMod[i] ==18)//С����ģʽ
    {
        ChanSet.Mod[i] = ModBusMod[i];

    }
    else if(ModBusMod[i] ==19)//����ģʽ
    {
        ChanSet.Mod[i] = 16;

    }
    else if(ModBusMod[i] ==20)//����ģʽ
    {
        ChanSet.Mod[i] = 17;

    }
    else if(ModBusMod[i] ==21)//С����ģʽ
    {
        ChanSet.Mod[i] = 18;

    }
    else if(ModBusMod[i] ==22)//��ѹģʽ
    {
        ChanSet.Mod[i] = 16;

    }
    else if(ModBusMod[i] ==23)//��ѹģʽ
    {
        ChanSet.Mod[i] = 17;

    }
    else if(ModBusMod[i] ==24)//С����ģʽ
    {
        ChanSet.Mod[i] = 18;

    }
    else if(ModBusMod[i] ==25)//��ѹģʽ
    {
        ChanSet.Mod[i] = 16;

    }
    else if(ModBusMod[i] ==26)//��ѹģʽ
    {
        ChanSet.Mod[i] = 17;

    }
}



void SwSetChanSetMod(void)
{
    uint32 i=0;
    for(i=0; i<10; i++)
    {
        if(ModBusMod[i] ==1)
        {
            DisLog[2+i][0].index[1] = OFF;
            ChanSet.Mod[i] =ModBusMod[i];
        }
        else if(ModBusMod[i] >1&&ModBusMod[i] <16)//�¶�ģʽ
        {
            ChanSet.Mod[i] = ModBusMod[i];
            DisLog[2+i][0].index[1] = VT;
            DisLog[2+i][1].index[2] = ModBusMod[i]-2;
        }
        else if(ModBusMod[i] <18)//��ѹģʽ
        {
            ChanSet.Mod[i] = ModBusMod[i];
            DisLog[2+i][0].index[1] = VDC;
            DisLog[2+i][2].index[3] = ModBusMod[i]-16;
        }
        else if(ModBusMod[i] ==18)//С����ģʽ
        {
            ChanSet.Mod[i] = ModBusMod[i];
            DisLog[2+i][0].index[1] = VI;
            DisLog[2+i][1].index[22]=0;//С����
        }
        else if(ModBusMod[i] ==19)//����ģʽ
        {
            ChanSet.Mod[i] = 16;
            DisLog[2+i][0].index[1] = VI;
            DisLog[2+i][1].index[22]=1;//
            DisLog[2+i][2].index[3] = 0;
        }
        else if(ModBusMod[i] ==20)//����ģʽ
        {
            ChanSet.Mod[i] = 17;
            DisLog[2+i][0].index[1] = VI;
            DisLog[2+i][1].index[22]= 2;//
            DisLog[2+i][2].index[3] = 1;
        }
        else if(ModBusMod[i] ==21)//С����ģʽ
        {
            ChanSet.Mod[i] = 18;
            DisLog[2+i][0].index[1] = VR;
            DisLog[2+i][1].index[21]=0;//С����
        }
        else if(ModBusMod[i] ==22)//��ѹģʽ
        {
            ChanSet.Mod[i] = 16;
            DisLog[2+i][0].index[1] = VR;
            DisLog[2+i][1].index[21]=1;//
            DisLog[2+i][2].index[3] = 0;
        }
        else if(ModBusMod[i] ==23)//��ѹģʽ
        {
            ChanSet.Mod[i] = 17;
            DisLog[2+i][0].index[1] = VR;
            DisLog[2+i][1].index[21]= 2;//
            DisLog[2+i][2].index[3] = 1;
        }
        else if(ModBusMod[i] ==24)//С����ģʽ
        {
            ChanSet.Mod[i] = 18;
            DisLog[2+i][0].index[1] = VG;
            DisLog[2+i][1].index[23]=0;//С����
        }
        else if(ModBusMod[i] ==25)//��ѹģʽ
        {
            ChanSet.Mod[i] = 16;
            DisLog[2+i][0].index[1] = VG;
            DisLog[2+i][1].index[23]=1;//
            DisLog[2+i][2].index[3] = 0;
        }
        else if(ModBusMod[i] ==26)//��ѹģʽ
        {
            ChanSet.Mod[i] = 17;
            DisLog[2+i][0].index[1] = VG;
            DisLog[2+i][1].index[23]= 2;//
            DisLog[2+i][2].index[3] = 1;
        }

        /*
        if(ChanSet.Mod[i] ==1)
            {
                DisLog[2+i][0].index[1] = OFF;
            }
            else if(ChanSet.Mod[i] >1&&ChanSet.Mod[i] <16)//�¶�ģʽ
            {
                DisLog[2+i][0].index[1] = VT;
                DisLog[2+i][1].index[2] = ChanSet.Mod[i]-2;
            }
            else if(ChanSet.Mod[i] <18)//��ѹģʽ
            {
                DisLog[2+i][0].index[1] = VDC;
                DisLog[2+i][2].index[3] = ChanSet.Mod[i]-16;
            }
            else if(ChanSet.Mod[i] ==18)//����ģʽ
            {
                DisLog[2+i][0].index[1] = VI;
            }
            */
    }
}





//�����Ĵ����Ĳ���
void SetSingalReg(u16 Address)
{
    uint32 i;
    union u16To2_u8 d;
    OS_ERR  err;
    if(Address<5)
    {
        d.Date = HoldReg[Address];
//        ChanSet.Mod[Address*2] = d.buf[0];
//        ChanSet.Mod[Address*2+1] =  d.buf[1];
        ModBusMod[Address*2] = d.buf[0];
        ModBusMod[Address*2+1] =  d.buf[1];
        OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
        SwSetChanSetMod();//ȫ����ת����
        SendSetMod(ChanSet);
        OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
        SaveAll();
        OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
    }
    else if(Address<32)
    {
        return;
    }
    else if(Address==32)
    {
        ChanSet.SYNC=(uint8)HoldReg[Address];
        OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
        SendSetSYNCTime(ChanSet);
        OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
        OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
    }
    else if(Address<33+5)
    {
        d.Date = HoldReg[Address];
        i=Address-33;
        ChanSet.Time[i*2] = d.buf[0];
        ChanSet.Time[i*2+1] =  d.buf[1];
        if(ChanSet.Time[i*2]>ChanSet.Time[i*2+1])
            DataSave.Data_type.SampleCAM=ChanSet.Time[i*2] ;
        else
            DataSave.Data_type.SampleCAM=ChanSet.Time[i*2+1] ;
        SampleTimeCur.CAM =DataSave.Data_type.SampleCAM;
        OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
        SetChanSetTime();
        SendSetSYNCTime(ChanSet);
        OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
        SaveAllPara();
        OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);

    }
    else if(Address<65)
    {
        return;
    }
    else if(Address==65)
    {
        ChanSet.alarm=(uint8)(HoldReg[Address]&0xff);  //����ָʾ���� ������ʱ����Զ����µ�
    }
    else if(Address<511)
    {
        return;
    }
    else if(Address==511)
    {
        if(HoldReg[Address]==1)
        {
            OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
            SendReSet();
            OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
            OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
        }

    }
}


void DealMulReg(uint8* flagb)
{
    OS_ERR  err;
    uint8 flag =0;
    if(flagb[0]==1)
    {
        OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
        SwSetChanSetMod();
        SendSetMod(ChanSet);
        OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
        SaveAll();
        OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);


    }

    if(flagb[1]==1)
    {
        OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
        SetChanSetTime();
        SendSetSYNCTime(ChanSet);
        OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
        if(flag ==0)
        {
            SaveAllPara();
            flag =1;
        }

        OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
    }

    if(flagb[2]==1)
    {
        OSTaskSuspend((OS_TCB*)&DoUartTaskTCB,&err);//��������
        SendReSet();
        OSSemPend(&Uart_CMD,100,OS_OPT_PEND_BLOCKING,0,&err); //�����ź�������
        OSTaskResume((OS_TCB*)&DoUartTaskTCB,&err);
    }
    if(flagb[3]==1)
    {
        OSSchedLock(&err);
        RTC_TimeAndDate_Set_Otur(DataSave.Data_type.Date, DataSave.Data_type.Time);
        if(flag ==0)
        {
            SaveAllPara();
            flag =1;
        }
        OSSchedUnlock(&err);
    }

    if(flagb[4]==1)
    {
        if(flag ==0)
        {
            SaveAllPara();
            flag =1;
        }
    }
}
uint8 testval = 0;
//����Ĵ����Ĳ���
void SetMulReg(u16 startAddress, u16 Num)
{
    uint32 i;
    union u16To2_u8 d;
//    OS_ERR  err;
    struct RegStru RegST[256] = {0};
    uint8 flagb[5]= {0};
    uint8 buf[3]= {0};
    uint8 itemp;
    u16 Address = startAddress;
    memset(flagb,0,sizeof(flagb));
    for(i=0; i<256; i++)
    {
        RegST[i].flag = 0;
    }
    for(Address =startAddress; Address<startAddress+Num; Address++)
    {
        if(Address<5)
        {
            d.Date = HoldReg[Address];

            ModBusMod[Address*2] = d.buf[0];
            ModBusMod[Address*2+1] =  d.buf[1];
            ModBusMod2ChanSetMod(Address*2);
			ModBusMod2ChanSetMod(Address*2+1);
            //ChanSet.Mod[Address*2] = d.buf[1];
            //ChanSet.Mod[Address*2+1] =  d.buf[0];
            flagb[0]=1;
        }
        else if(Address<32)
        {
            continue;
        }
        else if(Address==32)
        {
            ChanSet.SYNC=(uint8)HoldReg[Address];
            flagb[1]=1;
        }
        else if(Address<33+5)
        {
            d.Date = HoldReg[Address];
            i=Address-33;
            ChanSet.Time[i*2] = d.buf[1];
            ChanSet.Time[i*2+1] =  d.buf[0];
            if(ChanSet.Time[i*2]>ChanSet.Time[i*2+1])
                DataSave.Data_type.SampleCAM=ChanSet.Time[i*2] ;
            else
                DataSave.Data_type.SampleCAM=ChanSet.Time[i*2+1] ;
            SampleTimeCur.CAM =DataSave.Data_type.SampleCAM;
            flagb[1]=1;
        }
        else if(Address<65)
        {
            continue;
        }
        else if(Address==65)
        {
            ChanSet.alarm=(uint8)(HoldReg[Address]&0xff);  //����ָʾ���� ������ʱ����Զ����µ�
        }
        else if(Address<198)
        {
            continue;
        }
        else if(Address<=453&&Address>=198)
        {
            d.Date = HoldReg[Address];
            RegST[Address-198].buf[0] = d.buf[1];
            RegST[Address-198].buf[1] = d.buf[0];
            RegST[Address-198].flag = 1;
            flagb[4]=1;
        }
        else if(Address<511)
        {
            continue;
        }
        else if(Address==511)
        {
            if(HoldReg[Address]==1)
                flagb[2]=1;
        }
        else if(Address==512)
        {
            d.Date = HoldReg[Address];
            //RegST[Address-198].buf[0] = d.buf[0];
            testval = (d.buf[1]>>4)&0xF;
            testval = testval*10;
            DataSave.Data_type.Date.Year = (((d.buf[1]>>4)&0xF)*10)+(d.buf[1]&0xF);
            flagb[3]=1;
        }
        else if(Address==513)
        {
            d.Date = HoldReg[Address];
            DataSave.Data_type.Date.Month = (((d.buf[1]>>4)&0xF)*10)+(d.buf[1]&0xF);
            DataSave.Data_type.Date.Date = (((d.buf[0]>>4)&0xF)*10)+(d.buf[0]&0xF);
            flagb[3]=1;
        }
        else if(Address==514)
        {
            d.Date = HoldReg[Address];
            DataSave.Data_type.Time.Hours = (((d.buf[1]>>4)&0xF)*10)+(d.buf[1]&0xF);
            DataSave.Data_type.Time.Minutes = (((d.buf[0]>>4)&0xF)*10)+(d.buf[0]&0xF);
            flagb[3]=1;
        }
        else if(Address==515)
        {
            d.Date = HoldReg[Address];
            DataSave.Data_type.Time.Seconds = (((d.buf[1]>>4)&0xF)*10)+(d.buf[1]&0xF);
            flagb[3]=1;
        }
    }

    for(i=0; i<256; i+=2)
    {
        if(RegST[i].flag ==1&& RegST[i+1].flag ==1)
        {
            itemp=i/4;
            if(DataSave.Data_type.DisLog[2+itemp][0].index[1] == VDC)  //
            {
                memset(buf,0,sizeof(buf));
                // buf[0]=RegST[i].buf[0] ;
                buf[0]= RegST[i].buf[1] ;
                buf[1]= RegST[i+1].buf[0] ;
                buf[2]= RegST[i+1].buf[1] ;
                if(i%4==0)
                {
                    DataSave.Data_type.DCMax[itemp].fval = BYTE3toFloat(buf);
                    SwitchChanalData (&DataSave.Data_type.DCMax[itemp],8);//С�����8λ����
                }
                else
                {
                    DataSave.Data_type.DCMin[itemp].fval = BYTE3toFloat(buf);
                    SwitchChanalData (&DataSave.Data_type.DCMin[itemp],8);//С�����8λ����

                }

            }
            else if(DataSave.Data_type.DisLog[2+itemp][0].index[1] == VT)  //
            {
                memset(buf,0,sizeof(buf));
                // buf[0]=RegST[i].buf[0] ;
                buf[0]= RegST[i].buf[1] ;
                buf[1]= RegST[i+1].buf[0] ;
                buf[2]= RegST[i+1].buf[1] ;
                if(i%4==0)
                {
                    DataSave.Data_type.TempMax[itemp].fval = BYTE3toFloat(buf);
                    SwitchChanalData (&DataSave.Data_type.TempMax[itemp],1);//С�����8λ����
                }
                else
                {
                    DataSave.Data_type.TempMin[itemp].fval = BYTE3toFloat(buf);
                    SwitchChanalData (&DataSave.Data_type.TempMin[itemp],1);//С�����8λ����

                }

            }
            else if(DataSave.Data_type.DisLog[2+itemp][0].index[1] == VI)  //
            {
                memset(buf,0,sizeof(buf));
                buf[0]= RegST[i].buf[1] ;
                buf[1]= RegST[i+1].buf[0] ;
                buf[2]= RegST[i+1].buf[1] ;
                if(i%4==0)
                {
                    DataSave.Data_type.DIMax[itemp].fval = BYTE3toFloat(buf);
                    SwitchChanalData (&DataSave.Data_type.DIMax[itemp],1);//С�����8λ����
                }
                else
                {
                    DataSave.Data_type.DIMin[itemp].fval = BYTE3toFloat(buf);
                    SwitchChanalData (&DataSave.Data_type.DIMin[itemp],1);//С�����8λ����
                }
            }
        }
    }
    DealMulReg(flagb);
}

//����USB ����
void USB_task(void *p_arg)
{
    OS_ERR  err;
    //����һ���ź���
    OSSemCreate ((OS_SEM*   )&USB_SEM,
                 (CPU_CHAR* )"Key_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
    while(1)
    {

        OSSemPend(&USB_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���

        ++HidCnt;

        //USBSend();
        TimeHandler();
        //   OSTimeDly ( 500, OS_OPT_TIME_DLY, & err );  //��ʱ500ms
    }
}



/* �������ܣ���ȡ��Ȧ״̬
�������룺����ָ�룬pointer_inָ���������������Ϣ֡�����飬
        pointer_out����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
        �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */
/*
 void ReadOutputBit(u8 *pointer_in,u8 *pointer_out)//pointer_in�������룬pointer_out�������
 {
    u16 Address=0;//����ȡ��Ȧ��ʼ��ַ��//GPIO_X,XΪA��B�����˿ڣ�ÿ���˿�16λ����Ӧ��ַ0����31��
    u16 Num=0;//Ҫ��ȡ����Ȧ����
    u8 Byte=0;//Ҫ��ȡ����Ȧ�����ܹ�ռ�õ��ֽ�����
    u32 PortTemp;//������ŴӶ˿�ȡ����������
    u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_in+3))*256+(*(pointer_in+4));//�ȵõ���Ȧ��ַ(*(pointer_in+3))*256 = (*(pointer_in+3)) ����8λ��
    Num=(u16)(*(pointer_in+5))*256+(*(pointer_in+6));//�ȵõ�Ҫ��ȡ����Ȧ����
    *(pointer_out+2)=0x01;//�������ֽ�Ϊ������

    if(*(pointer_in)==8)      //������յ����ֽ�������8��������һ������֡
    {
        if(Address<32) //ֻҪ��ַС��32�����ǺϷ���ַ
        {
            if(Address + Num<=32 && Num>0) //ֻҪ��ַ����������0С��80�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;

                Byte=Num/8; //Ҫ��ȡ����Ȧ�����ܹ�ռ�õ��ֽ���
                if(Num%8!=0)
                    Byte++;//����������Ļ�����һ���ֽڣ�ʣ��ĸ�λ����
                *(pointer_out+3)=Byte;//���ĸ��ֽ�ΪҪ���͵ĸ���
                *(pointer_out)=1+1+1+Byte+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+��Ȧ��Ϣ+CRCУ��

                //���˿�C��D������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                ReadData=//GPIO_ReadOutputData(//GPIOD);
                PortTemp=(u32)(ReadData);
                PortTemp=PortTemp<<16;
                ReadData=//GPIO_ReadOutputData(//GPIOC);
                PortTemp=PortTemp|(u32)(ReadData);

                //��PortTemp�е����ݴ������Ҫ��λ������װ��������ݻ�����
                PortTemp=PortTemp<<(31-(Address+Num-1));
                PortTemp=PortTemp>>((31-(Address+Num-1))+Address);

                //������һ���ֽ�һ���ֽ�װ���������ݻ���������

                for(i=4,j=Byte;j>0;j--,i++)
                {
                    *(pointer_out+i)=(u8)(PortTemp&0x00FF);//��ȡһ���ֽڵĳ��ȣ��������
                    PortTemp=PortTemp>>8;//�ٽ����������ƶ�8λ
                }

                //д��У����
                SendKey=CRC16(pointer_out+1,*pointer_out-2);
                //�����������У����װ��������ݻ�����
                *(pointer_out+(*pointer_out-1))=(u8)(SendKey>>8);
                *(pointer_out+(*pointer_out))=(u8)(SendKey&0x00FF);

                //�������ݷ���
                USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�

            }
            else
            {
                 ErrorHandle(3,pointer_out);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_out);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
 }
*/
/* �������ܣ���ȡ����״̬
   �������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
         pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
         �����Ԫ�ذ���ModbusЭ����֯��
����������ޡ�                                                                                      */
/*
 void ReadInputBit(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
 {
    u16 Address=0;//����ȡ����λ��ʼ��ַ��//GPIO_X,XΪC��D�����˿ڣ�ÿ���˿�16λ����Ӧ��ַ0-31��
    u16 Num=0;//Ҫ��ȡ������λ����
    u8 Byte=0;//Ҫ��ȡ������λ�����ܹ�ռ�õ��ֽ�����
    u32 PortTemp;//������ŴӶ˿�ȡ���������ݣ���ʱ������
    u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ


    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ�����λ��ַ
    Num=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�Ҫ��ȡ������λ����
    *(pointer_2+2)=0x02;//�������ֽ�Ϊ������

    if(*(pointer_1)==8)   //������յ����ֽ�������8��������һ������֡
    {

        if(Address<32) //ֻҪ��ַС��32�����ǺϷ���ַ
        {
            if(Address+Num<=32&&Num>0) //ֻҪ��ַ����������0С��80�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;

                Byte=Num/8;
                if(Num%8!=0)
                    Byte++;//����������Ļ�����һ���ֽڣ�ʣ��ĸ�λ����
                *(pointer_2+3)=Byte;//���ĸ��ֽ�ΪҪ���͵ĸ���
                *(pointer_2)=1+1+1+Byte+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+����λ��Ϣ+CRCУ��

                //���˿�A��B������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                ReadData=//GPIO_ReadInputData(//GPIOB);
                PortTemp=(u32)(ReadData);
                PortTemp=PortTemp<<16;
                ReadData=//GPIO_ReadInputData(//GPIOA);
                PortTemp=PortTemp|(u32)(ReadData);

                //��PortTemp�е����ݴ������Ҫ��λ������װ��������ݻ�����
                PortTemp=PortTemp<<(31-(Address+Num-1));
                PortTemp=PortTemp>>((31-(Address+Num-1))+Address);

                //������һ���ֽ�һ���ֽ�װ���������ݻ���������
                for(i=4,j=Byte;j>0;j--,i++)
                {
                    *(pointer_2+i)=(u8)(PortTemp&0x00FF);//��ȡһ���ֽڵĳ��ȣ��������
                    PortTemp=PortTemp>>8;//�ٽ����������ƶ�8λ
                }

                //д��У����
                SendKey=CRC16(pointer_2+1,*pointer_2-2);
                //�����������У����װ��������ݻ�����
                *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
                *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);

                //�������ݷ���
                USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�

            }
            else
            {
                 ErrorHandle(3,pointer_2);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
 }
*/

/* �������ܣ���ȡ����Ĵ�����ģ�������룩
    �������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
              pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
              �����Ԫ�ذ���ModbusЭ����֯��
    ����������ޡ�                                                                                        */
/*
 void ReadInputReg(u8 *pointer_1,u8 *pointer_2)//pointer_1�������룬pointer_2�������
 {
    u16 Address=0;//����ȡ�Ĵ�����ʼ��ַ��HoldReg[i],iΪ0-99��Ӧ��ַ��0��99��
    u16 Num=0;//Ҫ��ȡ�ļĴ�������
    u16 SendKey;//Ҫ�������ݵ�У��ֵ
    u32 PortTemp;//������ŴӶ˿�ȡ���������ݣ���ʱ������
    u16 ReadData=0;//������ʱ��ŴӶ˿ڶ���������
    u32 CalTemp=0;//������ʱ����


    Address=(u16)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ʼ��ַ
    Num=(u16)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�Ҫ��ȡ�ļĴ�������
    *(pointer_2+2)=0x04;//�������ֽ�Ϊ������

    if(*(pointer_1)==8)   //������յ����ֽ�������8��������һ������֡
    {
        if(Address<2) //ֻҪ��ַС��2�����ǺϷ���ַ
        {
            if(Address+Num<=2&&Num>0) //ֻҪ��ַ����������0С��2�����ǺϷ�����
            {
                //����forѭ��
                u16 i;
                u16 j;

                *(pointer_2+3)=Num*2;//���ĸ��ֽ�ΪҪ���͵��ֽڸ���
                *(pointer_2)=1+1+1+Num*2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+�Ĵ�����Ϣ+CRCУ��

                //���˿�ADC1��ADC2������Ԥ�ȶ��뵽��ʱ�����ݻ�����
                ReadData=ADC_GetConversionValue(ADC2);
                PortTemp=(u32)(ReadData);
                PortTemp=PortTemp<<16;
                ReadData=ADC_GetConversionValue(ADC1);
                PortTemp=PortTemp|(u32)(ReadData);

                //��PortTemp�е������Ƚ���Ԥ����
                PortTemp=PortTemp<<(Address*16);
                for(i=4,j=Num*2;j>0;i++,j-=2)
                {
                    CalTemp=(u16)(PortTemp<<16);
                    *(pointer_2+i)=(u8)(CalTemp>>8);//�ȷŸ�λ
                    *(pointer_2+i+1)=(u8)(CalTemp&0x00FF);//�ٷŵ�λ
                }

                //д��У����
                SendKey=CRC16(pointer_2+1,*pointer_2-2);
                //�����������У����װ��������ݻ�����
                *(pointer_2+(*pointer_2-1))=(u8)(SendKey>>8);
                *(pointer_2+(*pointer_2))=(u8)(SendKey&0x00FF);

                //�������ݷ���
                USART_ITConfig(USART3, USART_IT_TC, ENABLE);//�������ݷ����ж�

            }
            else
            {
                 ErrorHandle(3,pointer_2);//�����ȡ����
            }

        }
        else
        {
            ErrorHandle(2,pointer_2);//������ʼ��ַ
        }
    }
    else
    {
        Comu_Busy=0;
    }
 }*/


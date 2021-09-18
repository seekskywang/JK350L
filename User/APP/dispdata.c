/*
  * @file    Dispdata.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   ����չʾ�Ľ���
 */
#include "include.h"
//������ƿ�
OS_TCB DisplayDataTaskTCB;
//�����ջ
CPU_STK DisplayData_TASK_STK[DisplayData_STK_SIZE];
struct RDispData  DDa[DCOL+2][ DCAM];
struct RDispData  DDaP[DCOL+2][ DCAMP];
uint8 CAM1=CAMMax;
uint8 CAM2=CAMMin;
float DSum[CHANNUM+2] = {0};
struct RDispData  DFrq[2];//+�Ƿ�����Ƶ��ֵ
struct RDispData  DHmd[3];//3ͨ��ʪ��
struct RDispData  DTemp;//�ڲ��¶�ֵ
uint8 PageFlagold=0;
uint8 PageFlagSetRe=FPageCh; //���÷���
const struct  UintARR ConstUint[VALCIND]=
{
    {"  \0"},//off ״̬
    {"MV\0","kV\0"," V\0","mV\0","uV\0"},//��ѹ��ʾ��λ
    {"$\0","&\0"},//���϶�
    {"%\0"},//ʪ��
    {"mA\0"," A\0"},//����
    {" g\0","kg\0"},//����
    {"  \0"," k\0"," M\0"," G\0"},//����

};
const struct  UintARR ConstUintF[VALCIND]=
{
    {"  \0"},//off ״̬
    {"MV\0","kV\0"," V\0","mV\0","uV\0"},//��ѹ��ʾ��λ
    {"��\0","�H\0"},//���϶�
    {"%\0"},//ʪ��
    {"mA\0"," A\0"},//����
    {" g\0","kg\0"},//����
    {"  \0"," k\0"," M\0"," G\0"},//����

};
//10��ͨ����������ɫ 10�� ��Щ���Զ����   + ����ͨ��Ƶ��
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
//�����˵���ʾ
static const struct HelpStr helpS[1]=
{
    {"��ʾ:��ʾ���������ڵĲ���ֵ��\0"," Displag parameter values in operation cycle.\0"},
};
static struct PStr PS[DCAMP]=
{
    {"���ֵ\0"," Max\0"},
    {"��Сֵ\0"," Min\0"},
    {"ƽ��ֵ\0"," Ave\0"},
    {"���ֵ\0"," P-P\0"},
    {"���ֵ\0"," PV \0"},
    {"�ȵ�ֵ\0"," VV \0"},
};

void Stu_DisplayDataFrq_Init(void)
{
    uint8 m,n,i,j;
    for(m=0; m<2; m++)
    {
        DFrq[m].DP.col = m;
        DFrq[m].DP.cam = n;
        DFrq[m].DP.lang = DataSave.Data_type.LANG;

        DFrq[m].vcind = VFRQ ;//����
        for(i=0; i<VALCIND; i++)
            for(j=0; j<UintCOL; j++)
            {
                strncpy(DFrq[m].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //��ѹ���� ����ĵ�λ
                DFrq[m].Uint[i].Col =0;   // Ƶ������Ϊ0
            }
        DFrq[m].DP.Num.fval = 0.0;//�ȸ���Ĭ��ֵ1.1v ������� �¶�ʱĬ����1.1���϶�
        DFrq[m].DP.Num.bit =1;//����λ1λ
        DFrq[m].DP.Num.dot = 1;//С����һλ��
        DFrq[m].DP.Num.Decimalbit = 1;//С��λ1λ

        DFrq[m].DP.flag = 1;
        DFrq[m].DP.disp=2;
        DFrq[m].DP.dispold=0;

        DFrq[m].DP.Box.flag = 0;
        DFrq[m].DP.FontEN.flag = 0;
        //���嵥λ
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
            DDaP[m][n].vcind = DisLog[m+2][0].index[1];//ǰ���� ��һ�����л����� �ڶ�����ALL ������ ��Ҫ��2
            //   DDaP[m][n].vcind = VT;//����
            for(i=0; i<VALCIND; i++)
                for(j=0; j<UintCOL; j++)
                {
                    strncpy(DDaP[m][n].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //��ѹ���� ����ĵ�λ
                    if(DDaP[m][n].vcind == VDC)   //��ѹ����Ĭ��ѡΪ��λ��v �������
                        DDaP[m][n].Uint[i].Col = 2;  //0 MV 1KV 2 V 3 mV 4 uV
                    else  if(DDaP[m][n].vcind == VI)   //����
                        DDaP[m][n].Uint[i].Col = 0;  //0
                    else  if(DDaP[m][n].vcind == VT)   //�¶ȵ�λ����
                        DDaP[m][n].Uint[i].Col = DataSave.Data_type.TUnit; //�������ϻ���
                    else  if(DDaP[m][n].vcind == VG)   //����
                        DDaP[m][n].Uint[i].Col = 0;  //0
                    else
                        DDaP[m][n].Uint[i].Col =0;   // ��������ֻ��1�� ��Ϊ0
                }
            DDaP[m][n].DP.Num.fval = 0.0;//�ȸ���Ĭ��ֵ1.1v ������� �¶�ʱĬ����1.1���϶�
            DDaP[m][n].DP.Num.bit =1;//����λ1λ
            DDaP[m][n].DP.Num.dot = 1;//С����һλ��
            DDaP[m][n].DP.Num.Decimalbit = 1;//С��λ1λ
            if(DDaP[m][n].vcind==0)//off ״̬�²���ʾ
            {
                DDaP[m][n].DP.flag = 0;
                DDaP[m][n].DP.disp=0;
                DDaP[m][n].DP.dispold=0;
            }
            else//���в���״̬�� 1 �������ֵ  2������Сֵ ��Ҫ���θ�ֵ
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
            DDa[m][n].vcind = DisLog[m+2][0].index[1];//ǰ���� ��һ�����л����� �ڶ�����ALL ������ ��Ҫ��2
            //   DDa[m][n].vcind = VT;//����
            for(i=0; i<VALCIND; i++)
                for(j=0; j<UintCOL; j++)
                {
                    strncpy(DDa[m][n].Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //��ѹ���� ����ĵ�λ
                    if(DDa[m][n].vcind == VDC)   //��ѹ����Ĭ��ѡΪ��λ��v �������
                        DDa[m][n].Uint[i].Col = 2;  //0 MV 1KV 2 V 3 mV 4 uV
                    else  if(DDa[m][n].vcind == VI)   //����
                        DDa[m][n].Uint[i].Col = 0;  //0
                    else  if(DDa[m][n].vcind == VT)   //�¶ȵ�λ����
                        DDa[m][n].Uint[i].Col = DataSave.Data_type.TUnit; //�������ϻ���
                    else  if(DDa[m][n].vcind == VG)   //����
                        DDa[m][n].Uint[i].Col = 0;  //0
                    else
                        DDa[m][n].Uint[i].Col =0;   // ��������ֻ��1�� ��Ϊ0
                }
            //��λ���ͳ�ʼ��
            if(n==0)
            {
                ChanData.chd[m].vcind =DDa[m][n].vcind;  //10��ͨ���Ľ����������Ͷ��� 1Ϊ��ѹ
				ChanData.hmd[1].vcind =3;  //10��ͨ���Ľ����������Ͷ��� 1Ϊ��ѹ
				ChanData.hmd[3].vcind =3;  //10��ͨ���Ľ����������Ͷ��� 1Ϊ��ѹ
				ChanData.hmd[5].vcind =3;  //10��ͨ���Ľ����������Ͷ��� 1Ϊ��ѹ
                for(j=0; j<UintCOL; j++)
                    strncpy(ChanData.chd[m].UintOne.Ch[j],ConstUint[ChanData.chd[m].vcind ].Ch[j],sizeof(ConstUint[ChanData.chd[m].vcind ].Ch[j]));//10��ͨ���Ľ���������ʾ��λ�������ܵ���
            }
            if( DDa[m][n].vcind ==VDC ) //��ѹ����
            {
                //�����ǰ�ǵ�ѹ�����͸���

                DDa[m][n].Up[DDa[m][n].vcind]=DataSave.Data_type.DCMax[m] ;
                DDa[m][n].Up[DDa[m][n].vcind].dot=3;  //С������1λ֮��
                DDa[m][n].Up[DDa[m][n].vcind].bit=3;//����λ����
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=3;//С��λ����

                //ֵ�������䷶Χ��

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.DCMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=3;  //С������1λ֮��
                DDa[m][n].Low[DDa[m][n].vcind].bit=3;//����λ����
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=3;//С��λ����


            }
            if( DDa[m][n].vcind ==VI ) //��������
            {
                //�����ǰ�ǵ�ѹ�����͸���

                DDa[m][n].Up[DDa[m][n].vcind]=DataSave.Data_type.DIMax[m] ;
                DDa[m][n].Up[DDa[m][n].vcind].dot=6;  //С������1λ֮��
                DDa[m][n].Up[DDa[m][n].vcind].bit=6;//����λ����
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=2;//С��λ����

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.DIMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=6;  //С������1λ֮��
                DDa[m][n].Low[DDa[m][n].vcind].bit=6;//����λ����
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=2;//С��λ����


            }
            if( DDa[m][n].vcind ==VG ) //��������
            {
                //�����ǰ�ǵ�ѹ�����͸���

                DDa[m][n].Up[DDa[m][n].vcind]=DataSave.Data_type.VGMax[m] ;
                DDa[m][n].Up[DDa[m][n].vcind].dot=6;  //С������1λ֮��
                DDa[m][n].Up[DDa[m][n].vcind].bit=6;//����λ����
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=3;//С��λ����

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.VGMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=6;  //С������1λ֮��
                DDa[m][n].Low[DDa[m][n].vcind].bit=6;//����λ����
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=3;//С��λ����


            }
            else if(DDa[m][n].vcind ==VT)//�¶�����
            {

                //�����ǰ���¶Ȳ����͸���

                DDa[m][n].Up[DDa[m][n].vcind]= DataSave.Data_type.TempMax[m];
                DDa[m][n].Up[DDa[m][n].vcind].dot=4;  //С������4λ֮��
                DDa[m][n].Up[DDa[m][n].vcind].bit=4;//����λ����
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=1;//С��λ����

                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.TempMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=4;  //С������4λ֮��
                DDa[m][n].Low[DDa[m][n].vcind].bit=4;//����λ����
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=1;//С��λ����

            }
            else if(DDa[m][n].vcind ==VR)//ʪ������
            {

                DDa[m][n].Up[DDa[m][n].vcind]= DataSave.Data_type.VRMax[m];
                DDa[m][n].Up[DDa[m][n].vcind].dot=3;  //С������3λ֮��
                DDa[m][n].Up[DDa[m][n].vcind].bit=3;//����λ����
                DDa[m][n].Up[DDa[m][n].vcind].Decimalbit=1;//С��λ����
                DDa[m][n].Low[DDa[m][n].vcind]=DataSave.Data_type.VRMin[m];
                DDa[m][n].Low[DDa[m][n].vcind].dot=3;  //С������3λ֮��
                DDa[m][n].Low[DDa[m][n].vcind].bit=3;//����λ����
                DDa[m][n].Low[DDa[m][n].vcind].Decimalbit=1;//С��λ����
            }

            DDa[m][n].DP.Num.fval = 0.0;//�ȸ���Ĭ��ֵ1.1v ������� �¶�ʱĬ����1.1���϶�
            DDa[m][n].DP.Num.bit =1;//����λ1λ
            DDa[m][n].DP.Num.dot = 1;//С����һλ��
            DDa[m][n].DP.Num.Decimalbit = 1;//С��λ1λ

            if(DDa[m][n].vcind==0)//off ״̬�²���ʾ
            {
                DDa[m][n].DP.flag = 0;
                DDa[m][n].DP.disp=0;
                DDa[m][n].DP.dispold=0;
            }
            else//���в���״̬�� 1 �������ֵ  2������Сֵ ��Ҫ���θ�ֵ
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

            DDa[9][n].Up[VT].dot=4;  //С������4λ֮��
            DDa[9][n].Up[VT].bit=4;//����λ����
            DDa[9][n].Up[VT].Decimalbit=1;//С��λ����
            DDa[9][n].Low[VT].dot=4;  //С������4λ֮��
            DDa[9][n].Low[VT].bit=4;//����λ����
            DDa[9][n].Low[VT].Decimalbit=1;//С��λ����
        }
    }
}


void DisplayDataTemp_Init(void)
{
    uint8 i,j;
    DTemp.DP.lang = DataSave.Data_type.LANG;
    DTemp.vcind = VT ;//����
    for(i=0; i<VALCIND; i++)
        for(j=0; j<UintCOL; j++)
        {
            strncpy(DTemp.Uint[i].Ch[j],ConstUint[i].Ch[j],sizeof(ConstUint[i].Ch[j]));   //��ѹ���� ����ĵ�λ
            DTemp.Uint[i].Col =0;   // Ƶ������Ϊ0
        }
    DTemp.DP.Num.fval = 0.0;//�ȸ���Ĭ��ֵ1.1v ������� �¶�ʱĬ����1.1���϶�
    DTemp.DP.Num.bit =1;//����λ1λ
    DTemp.DP.Num.dot = 1;//С����һλ��
    DTemp.DP.Num.Decimalbit = 2;//С��λ2λ
    DTemp.DP.flag = 1;
    DTemp.DP.disp=2;
    DTemp.DP.dispold=0;
    DTemp.DP.Box.flag = 0;
    DTemp.DP.FontEN.flag = 0;
    //�¶ȵ�λ
    chTemp.vcind =DTemp.vcind;
}

void Stu_DisplayData_Init(void)
{
    Stu_DisplayDataCha_Init();
    Stu_DisplayDataFrq_Init();
    DisplayDataTemp_Init();

}
uint8 ModifyData(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // ��ֵƥ�亯������while��ѭ���з���ƥ��
{
    PageFlagold=DataSave.Data_type.PageFlag ;
    *curold=*cur;
    cur->OPN=keytmp;                        //����ֵ����������־��������ǰ�Ĳ���״̬
    switch(cur->OPN)
    {
        case KEY_FUNC1://����10ch
            DataSave.Data_type.PageFlag =  FPageCh ;
            break;
        case KEY_FUNC2://����data
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageGraph  ;
            break;

        case KEY_FUNC4://����3ch
            DataSave.Data_type.PageFlag =  FPageCh3 ;
            break;
        case KEY_FUNC5://�����ļ�
            if(FileRes ==0)
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

//��������չʾ����
void DisplayData_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    static RTC_TimeTypeDef TimeStr;
    //��ҳ
    Stu_DisplayData_Init(); //����չʾҳ�����ݳ�ʼ��
    page_DisplayData();//����չʾҳ���ʼ��
    while(1)
    {
        //����
        keytmp = KeyTaskCreate(ALL_ReTurn);//20ms���ؼ�ֵ ������
        keytmp=ModifyData(keytmp,&CurDisp,&CurDispOld);     //�����ֵ
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
            //ɾ���Լ�����
        }
        page_DisplayData_cycle(); //����չʾҳ�洦��
        if(TimeStr.Seconds!=RTC_TimeStr.Seconds)
        {
            ShowDateTime(RTC_DateStr,RTC_TimeStr,2,5,&Font16);   //��ʾʱ��
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


        LCD_DisplayStringLineLJ(x+Xdiv,y, (uint8_t *)"��ֵ",FONTTYPE16);
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
        COMmenuInit(DataSave.Data_type.LANG,(uint8_t *)"ȫͨ��",(uint8_t *)"������ʾ",(uint8_t *)"����",(uint8_t *)"��ͨ��",(uint8_t *)"�ļ�",1);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLineLJ(x+Xdiv,y, (uint8_t *)"��ֵ",FONTTYPE16);
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
//�ӵ�λ
void AddUint2Char(const struct RDispData RD,char* ptr,uint8 setoff)
{
    uint8 i;
    uint8 j=setoff;
    if(RD.vcind==VDC)//�˵�λ�ǵ�ѹ��λ��ѹ��λ��5��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VT)//�˵�λ���¶ȵ�λ2��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VI)//�˵�λ���¶ȵ�λ2��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VG)//�˵�λ��������λ2��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else  //�˵�λֻ��1��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[0],sizeof(RD.Uint[RD.vcind].Ch[0]));//ʪ��
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
    if(RD.vcind==VDC)//�˵�λ�ǵ�ѹ��λ��ѹ��λ��5��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[2],sizeof(RD.Uint[RD.vcind].Ch[2]));
    else if(RD.vcind==VT)//�˵�λ���¶ȵ�λ2��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VI)//�˵�λ���¶ȵ�λ2��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else if(RD.vcind==VG)//�˵�λ��������λ2��
        strncpy(ptr+setoff,RD.Uint[RD.vcind].Ch[RD.Uint[RD.vcind].Col],sizeof(RD.Uint[RD.vcind].Ch[0]));
    else  //�˵�λֻ��1��
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
			if(DSum[i]==0)   //���¼�������ֵ
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
			if(DSum[i]==0)   //���¼�������ֵ
				DDaSL[i][n].fStr= ChanData.hmd[1].fStr;
			DDaSL[i][n].vcind= DDa[i][0].vcind;
			DDaP[i][n].vcind = DDa[i][0].vcind;
		}else{
			if(DDaP[i][n].vcind != DDa[i][0].vcind)
			{
				DSum[i]=0;
			}
			if(DSum[i]==0)   //���¼�������ֵ
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
		//ƽ��ֵ
		DDaSL[i+1][CAMAV].fStr.fval =  DDaSL[i+1][CAMAV].fStr.fval-DDaSL[i+1][CAMAV].fStr.fval/DSum[i]+DDaSL[i+1][0].fStr.fval/DSum[i];
		//���ֵ
		DDaSL[i+1][CAMPP].fStr.fval = DDaSL[i+1][CAMMax].fStr.fval -DDaSL[i+1][CAMMin].fStr.fval;
		//��ֵ
		DDaSL[i+1][CAMP].fStr.fval = DDaSL[i+1][CAMMax].fStr.fval -DDaSL[i+1][CAMAV].fStr.fval;
		//��ֵ
		DDaSL[i+1][CAMG].fStr.fval = -DDaSL[i][CAMMin].fStr.fval+DDaSL[i+1][CAMAV].fStr.fval;
	}else{
		//ƽ��ֵ
		DDaSL[i][CAMAV].fStr.fval =  DDaSL[i][CAMAV].fStr.fval-DDaSL[i][CAMAV].fStr.fval/DSum[i]+DDaSL[i][0].fStr.fval/DSum[i];
		//���ֵ
		DDaSL[i][CAMPP].fStr.fval = DDaSL[i][CAMMax].fStr.fval -DDaSL[i][CAMMin].fStr.fval;
		//��ֵ
		DDaSL[i][CAMP].fStr.fval = DDaSL[i][CAMMax].fStr.fval -DDaSL[i][CAMAV].fStr.fval;
		//��ֵ
		DDaSL[i][CAMG].fStr.fval = -DDaSL[i][CAMMin].fStr.fval+DDaSL[i][CAMAV].fStr.fval;
	}
    for(n=0; n<DCAMP; n++)
    {
		if(i > 9)
		{
			fStr2DfStr(&DDaSL[i+1][n]);
			DDaP[i+1][n].DP.Num = DDaSL[i+1][n].DfStr;//���յ����ݸ���
			DDaP[i+1][n].Uint[DDaP[i+1][n].vcind].Col = DDaSL[i+1][n].UintOne.Col;  // ��ʾ��λˢ���ǽ��յ�ָ����Զ��л���
		}else{
			fStr2DfStr(&DDaSL[i][n]);
			DDaP[i][n].DP.Num = DDaSL[i][n].DfStr;//���յ����ݸ���
			DDaP[i][n].Uint[DDaP[i][n].vcind].Col = DDaSL[i][n].UintOne.Col;  // ��ʾ��λˢ���ǽ��յ�ָ����Զ��л���
		}
    }
    /*
      DSum+=1;
      if(DDaP[i][CAMMax].DP.Num.fval<DDa[i][0].DP.Num.fval)
          DDaP[i][CAMMax].DP.Num = DDa[i][0].DP.Num;
      if(DDaP[i][CAMMin].DP.Num.fval>DDa[i][0].DP.Num.fval)
          DDaP[i][CAMMin].DP.Num = DDa[i][0].DP.Num;
      //ƽ��ֵ
      DDaP[i][CAMAV].DP.Num.fval =  DDaP[i][CAMAV].DP.Num.fval-DDaP[i][CAMAV].DP.Num.fval/DSum+DDa[i][0].DP.Num.fval/DSum;
      //���ֵ
      DDaP[i][CAMPP].DP.Num.fval = DDaP[i][CAMMax].DP.Num.fval -DDaP[i][CAMMin].DP.Num.fval;
      //��ֵ
      DDaP[i][CAMP].DP.Num.fval = DDaP[i][CAMMax].DP.Num.fval -DDaP[i][CAMAV].DP.Num.fval;
      //��ֵ
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
    // DSum+=1;  //����
    for(i=0; i<DCOL+2-1; i++)
    {
        
		if(i>9)
		{
			DDa[i+1][0].DP.dispold = 0;//ˢ��Ҫ���±�־
			DDa[i+1][1].DP.dispold = 0;
			DDa[i+1][2].DP.dispold = 0;
			DDa[i+1][0].DP.Box.x1 =x-5+Xdiv;
			DDa[i+1][0].DP.Box.y1 =y+i*Ydiv;
		}else{
			DDa[i][0].DP.dispold = 0;//ˢ��Ҫ���±�־
			DDa[i][1].DP.dispold = 0;
			DDa[i][2].DP.dispold = 0;
			DDa[i][0].DP.Box.x1 =x-5+Xdiv;
			DDa[i][0].DP.Box.y1 =y+i*Ydiv;
		}
        memset(ptr,0,sizeof(ptr));
        
		if(i>9)
		{
			 if(i==7&&DDa[i+1][0].DP.Num.fval<1)//������ʾ
				i=i;
			 NumFloat2Char(&DDa[i+1][0],&DDa[i+1][0].DP.Num,ptr);  //�������ݸ�����ת��Ϊ�ַ���
			 AddUint2Char(DDa[i+1][0],ptr,9);    // �ַ���β���ϼӵ�λ
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i+1][0].DP,DDa[i+1][0].DP.Box.x1,DDa[i+1][0].DP.Box.y1+2,ptr,Font20);
		}else{
			 if(i==7&&DDa[i][0].DP.Num.fval<1)//������ʾ
				i=i;
			 NumFloat2Char(&DDa[i][0],&DDa[i][0].DP.Num,ptr);  //�������ݸ�����ת��Ϊ�ַ���
			 AddUint2Char(DDa[i][0],ptr,9);    // �ַ���β���ϼӵ�λ
			 LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][0].DP,DDa[i][0].DP.Box.x1,DDa[i][0].DP.Box.y1+2,ptr,Font20);
		}
       

        ComputerP(i);
        //���ֵ�Ĳ���
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
        //��Сֵ�Ĳ���

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
    //����Ҫ�ӵ�λ
//    for(i=0; i<DCOL-1; i++)
//    {
//        LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][0].DP,DDa[i][0].DP.Box.x1+100,DDa[i][0].DP.Box.y1+2,DDa[i][0].Uint[DDa[i][0].vcind].Ch[DDa[i][0].Uint[DDa[i][0].vcind].Col],Font20);
//        LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][1].DP,DDa[i][1].DP.Box.x1+100,DDa[i][1].DP.Box.y1+2,DDa[i][1].Uint[DDa[i][1].vcind].Ch[DDa[i][1].Uint[DDa[i][1].vcind].Col],Font20);
//        LCD_DisplayNumLine_EN_CH_LJ_Home(&DDa[i][2].DP,DDa[i][2].DP.Box.x1+100,DDa[i][2].DP.Box.y1+2,DDa[i][2].Uint[DDa[i][2].vcind].Ch[DDa[i][1].Uint[DDa[i][1].vcind].Col],Font20);
//    }

}

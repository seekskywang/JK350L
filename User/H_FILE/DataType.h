#ifndef  DADATYPE_H
#define  DADATYPE_H
typedef unsigned          char Uint8;
typedef unsigned short     int Uint16;
typedef unsigned           int Uint32;

typedef unsigned          char uint8;
typedef unsigned short     int uint16;
typedef unsigned           int uint32;

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;

#define EPSF0       (0.000001F)    //�ӿ����ý���
#define EPSFMAX       (1000000000.0F)    //�ӿ����ý���


#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#define  TEMPMAXVAL         1800
#define  TEMPMINVAL       (-200)

#define  VRMAXVAL         100
#define  VRMINVAL       0

#define  VGMAXVAL       999
#define  VGMINVAL       0


#define RStrNUM    24   //10��ͨ�������ý���
#define OturNUM   9    //����������Ŀ�����ý���

#define OturCOL    9    //����������Ŀ�����ý���
#define HDataNUM   7    //���ݴ������ý���
#define HDataCOL    7    //���ݴ������ý���


#define IFNUM   7    //�ӿ����ý���
#define IFCOL    7    //�ӿ����ý���

#define NUMCOL    15   // ������
#define  FPageGraph            1   //��ͼҳ��
#define  FPageDisplayData  2  //����չʾҳ��
#define  FPageHome            3  //����ҳ��
#define  FPageCh          4  //С��������չʾҳ��
#define  FPageCh3          5  //������������ʾ
#define  FPageHDATA          6  //
#define  FPageOtur          7  //
#define  FPageIF          8  //
#define  FPageFile          9 //
#define  FPageRecord        10 //�ؿ�ҳ��
#define  StrNUM 10
#define  HCOL 13
#define  HCAM 6
#define  InPutCOL 6
#define  SensorCOL 5
#define  SensorCAM  3
#define  RangeCOL   2



#define  SenI     0
#define  SenSV  1
#define  SenBV  2
#define  VRVGSensorCOL  3



#define  FilterCOL  6
#define  DiaTEMPCOL   3
#define  DiaTEMPCAM  2


#define  DisTiShiCAM    2
#define  FilerMaxNum     40

#define  DiaDICOL   4
#define  DiaDICAM  2


#define  DiaVGCOL   4
#define  DiaVGCAM  2


#define  DiaVRCOL   4
#define  DiaVRCAM  2

#define  FileCOL 11
#define  FileCAM 2
#define  FileNUM 11
#define  FilePageMax 8
#define  FilePageMin 0

#define  DiaTEMPRangeCOL   3
#define  DiaTEMPRangeCAM  2

#define  DiaTEMPDcCOL   3
#define  DiaTEMPDcCAM  2

#define  TEMPOnOffCOL 2
#define  LimitCOL 4


#define  DiaDCCOL   4
#define  DiaDCCAM  2
#define  DCOnOffCOL 2

#define  SaverOnOffCOL 9
#define  BrightnessCOL 3
#define  PowerOnCOL 2
#define  BeepOnCOL 2
#define  RoomCOL 2
#define  UnitCOL 2
#define  LangCOL 2
#define  ReturnCOL 2


#define  SamplingCOL 18

#define  USBIDCOL 10
#define  WifiSWCOL 2

#define  ExternCOL 2
#define  BackUpCOL 2
#define  DiaWifiCOL   4
#define  DiaWifiCAM  2

#define  DiaSoftKeyCOL   7
#define  DiaSoftKeyCAM  13

#define  DiaDTCOL   4
#define  DiaDTCAM  2

#define  CHANNUM   10  //ͨ����(�¶�)
#define  CHANNUMH  3  //ͨ����(ʪ��)

#define  DCOL 11    //��
#define  DCAM 5    //��

#define  DCAMP 6    //���ֵ��Сֵƽ��ֵ ���ֵ��ֵ��ֵ

#define  CAMMax        0     //
#define  CAMMin        1     //
#define  CAMAV        2    //
#define  CAMPP          3   //
#define  CAMP          4  //
#define  CAMG          5 //



#define  CINDNUM    3    //��ʾ����
#define  VAL        0     //��ֵ 
#define  MAX        1//
#define  MIN        2//


#define VALCIND 7    //����
#define  OFF        0     //��
#define  VDC        1     //��ѹ
#define  VT          2   //�¶�
#define  VR          3  //ʪ��
#define  VI         4     //����
#define  VG         5     //����
#define  VFRQ        6 //Ƶ��


#define  FILEReady    0
#define  FILETFOpen   1
#define  FILEUSBOpen  2
#define  SDERR        3     //sd����ʼ������

#define  TRIGReady    0
#define  TRIGStart    1
#define  TRIGStopTF   2
#define  TRIGStopUSB  3


#define  UintCOL  5    //��
#define  UintCAM  3    //��

#define  SETON    1
#define  SETOFF  0

#define NUMBER_OF_BLOCKS      15625000

#define  SDSaveMAX      500000
#define  USBSaveMAX    500000
//========================WIFI��������=======================
#define shezhi_work  1            //0�ǹ���״̬ 1������ģʽ
#define WIFI_REC_LEN            1000    //ATָ���������  
#define WIFI_SEND_LEN           1000    //WIFI���ͻ��������� 

#define     wifi_RBUFSIZE  256
#define     wifi_SIZE        4
#pragma pack (1)
struct NumStru   //13���ֽ�
{

    uint8 dot;  //С����
    uint8 bit;  //��������λ��
    uint8 Decimalbit;  //С�����λ��
    uint8 Symbol;  //���ű�ʶ1Ϊ����0Ϊ��
    uint8 Changflag; // ��ֵ�Ƿ񱻸ı��
    float fval;  //��cind����ΪNUMCOL
    float fvalold;  // ��һ�εĸ�������
};
#pragma pack ()
struct UintStr
{
    uint8 Col;//�α�ѡ�е�λ�е���
    char Ch[ UintCOL][UintCAM ];  //��λ����
};


struct UintARR
{
    char Ch[ UintCOL][UintCAM ];  //��λ����
};

struct SLData
{
    uint8 flag; //�Ƿ���Ч
    uint8 byte[3]; //���յ������ֽ�
    struct NumStru fStr;//���պ����������ԭʼ����
    struct NumStru DfStr;//���պ����ݿ��ܵ����ٸ���ʾ
    struct UintStr UintOne;  //���պ��ѹ��λ���ܸ�����ֵ����
    char vcind; //�����������ֵ�ѹ�¶�ʪ��
    Point Dxy;
};

#define     SDLEN 262
#pragma pack (1)
union DataSD_union
{
    //14���ֽ�
    struct
    {
        char vcind; //�����������ֵ�ѹ�¶�ʪ��
        uint8 byte[3]; //���յ������ֽ
        uint8 MaxB[3] ;
        uint8 MinB[3] ;
        // Point Dxy;
        uint32 endtick;

    } SDData ;
    unsigned char uData[14];
};

union SD_U
{
    struct
    {
        uint8 DataType; //   ��������
        uint8 SampleTimeCurCAM;
        //  uint8 ZoneCurCAM;
        uint32 Timestamp;
        uint32 starttick;
        union DataSD_union FD[2];     //����Ƶ��
        union DataSD_union CHD[10];  //10��ͨ�� ��Ԥ��
		union DataSD_union HMD[6];  //3��ʪ��ͨ�� ��Ԥ��

    } Data;
    unsigned char uD[SDLEN]; //�ֽ�
};
#pragma pack ()
struct ChanalData
{
    uint8 alarm;
    struct SLData frq[2];
    struct SLData chd[CHANNUM];
	struct SLData hmd[6];
    uint8  chalarm[CHANNUM+3];   //ÿ��ͨ���ı������� 0 ��1�ϳ� 2 �³� 3 ����
};

typedef struct
{
    u8 rBuf[wifi_SIZE][wifi_RBUFSIZE];  //���ջ�����
    u8 wifilen[wifi_SIZE];
    u8 wifilianjie[wifi_SIZE];
    u16 rPtr1;    //���ջ�����������
    u16 rPtr2;    //���ջ���������β
} wifi_data_t;
struct ChanalMData
{
    uint8 ch;     //�ڼ���ͨ��
    // uint16 Num;    //�洢��ʼ�ĵڼ�������
    struct SLData chmd[10];     //  1�ѱ�׼10������
};


#pragma pack (1)
struct ChanalSet  //62���ֽ�
{
    uint8 Mod[CHANNUM];  //10���ֽ�
    uint8 Time[CHANNUM]; //10���ֽ�
    uint8 SYNC;
    uint8 alarm;
    uint32 TimeVal[CHANNUM]; //40���ֽ�
};

struct ChanalHSet  //2���ֽ�
{
    uint8 Mod[CHANNUMH];  //3���ֽ�
    uint8 Time[CHANNUMH]; //3���ֽ�
    uint8 SYNC;
    uint8 alarm;
    uint32 TimeVal[CHANNUMH]; //12���ֽ�
};
#pragma pack ()
#pragma pack (1)
struct RDispParaSave
{
    char cind;                  //��ʾ���� home ��RStrNUM��
    char index[RStrNUM];                //��ʾ�ڼ���
};
#pragma pack ()

#pragma pack (1)
typedef struct
{
    u8   uart_stop;
    u8   uart_len;
    u8   uart_check;
    u8   SW;
    u16  modbus_id;
    u16  fun_gensui;
    u16  IP12;
    u16  IP34;
    u16  Ser_Port;
    u16  Port;
    u32  uart_BaudRate;
    char Wifi_Name[20];
    char Wifi_PW[20];
    char YuMing[40];
} PARAMETERS_t;                 //
#pragma pack ()
union Data_union
{
#pragma pack (1)
    struct
    {
        uint8 PageFlag;                       //ҳ���־ ����ҳ���ʶ ��ͼҳ��  1�ֽ�
        uint8 LANG;       //
        uint8 Bright;    //����
        uint8 ScrSaver;    //��Ļ����
        uint8 PowerOn; //�����ϵ�
        uint8 RoomT;   //�¶ȱ���
        uint8 TUnit;   //�¶ȵ�λ
        uint8 SW;   //��ʾ�߼��ͱ���������л�
        uint8 USBId;    //��Ļ����
        uint8 BeepF;    //��Ļ����
        uint8   SampleCAM;
        uint8   SaveSample;
        uint8   ZoneCAM;      //
        uint8   FormatF;
        uint8   ShotF;   //
        uint8   AlarmOutF;   //
        uint8   BlockAddr;                     //�ֽ�
        uint8   SYNF;    //ͬ����־
        uint8   PLSF[2];   //
        RTC_DateTypeDef Date;  //   4���ֽ�����ʱ��
        char PName[11]; //��Ʒ�ͺ�
        char PSN[11]; //��Ʒ���к�
        uint8 TraceF[CHANNUM+2+1];        //
        uint8 ComputeT[CHANNUM+2+1];        //
        uint8 ComputeDC[CHANNUM+2+1];        //

        uint8 Alarm[CHANNUM+2+1];     //��������
        RTC_TimeTypeDef Time;   //20���ֽ�����ʱ��
        uint8 ComputeDI[VRVGSensorCOL][CHANNUM+2+1];        //
        uint8 ComputeVR[VRVGSensorCOL][CHANNUM+2+1];        //
        uint8 ComputeVG[VRVGSensorCOL][CHANNUM+2+1];        //
        char ChName[CHANNUM+2][6];
//      float DCCheck[CHANNUM][4];
//      float TempCheck[CHANNUM][4];
        struct ChanalSet    ChanSet;
        struct NumStru      TempAdjMeas[CHANNUM+2+1];                                 //
        struct NumStru      TempAdjEu[CHANNUM+2+1];                                     //
        struct NumStru      DCUpMeas[CHANNUM+2+1];                                 //
        struct NumStru      DCUpEu[CHANNUM+2+1];                                    //
        struct NumStru      DCLowMeas[CHANNUM+2+1];                                 //
        struct NumStru      DCLowEu[CHANNUM+2 +1];                                     //
        struct NumStru      TempMax[CHANNUM+2+1];                                 //
        struct NumStru      TempMin[CHANNUM+2+1];                                     //
        struct NumStru      DCMax[CHANNUM+2+1];                                 //
        struct NumStru      DCMin[CHANNUM+2+1];                                     //130 �ֽ�
        struct NumStru      TempLMax[CHANNUM+2+1];                                 //
        struct NumStru      TempLMin[CHANNUM+2+1];                                     //
        struct NumStru      DCLMax[CHANNUM+2+1];                                 //
        struct NumStru      DCLMin[CHANNUM+2+1];                                     //52�ֽ�

        //
        struct NumStru      DIMax[CHANNUM+2+1];                                 //
        struct NumStru      DIMin[CHANNUM+2+1];                                     //130 �ֽ�
        struct NumStru      DILMax[CHANNUM+2+1];                                 //
        struct NumStru      DILMin[CHANNUM+2+1];                                     //52�ֽ�


        struct NumStru      VRMax[CHANNUM+2+1];                                 //
        struct NumStru      VRMin[CHANNUM+2+1];                                     //130 �ֽ�
        struct NumStru      VRLMax[CHANNUM+2+1];                                 //
        struct NumStru      VRLMin[CHANNUM+2+1];                                     //52�ֽ�

        struct NumStru      VGMax[CHANNUM+2+1];                                 //
        struct NumStru      VGMin[CHANNUM+2+1];                                     //130 �ֽ�
        struct NumStru      VGLMax[CHANNUM+2+1];                                 //
        struct NumStru      VGLMin[CHANNUM+2+1];                                     //52�ֽ�

        PARAMETERS_t parameters;
        struct NumStru      DIUpMeas[VRVGSensorCOL][CHANNUM+2+1];                                 //
        struct NumStru      DIUpEu[VRVGSensorCOL][CHANNUM+2+1];                                     //
        struct NumStru      DILowMeas[VRVGSensorCOL][CHANNUM+2+1];                                 //
        struct NumStru      DILowEu[VRVGSensorCOL][CHANNUM+2+1];

        struct NumStru      VRUpA[VRVGSensorCOL][CHANNUM+2+1];                                 //���ֵ
        struct NumStru      VRUpB[VRVGSensorCOL][CHANNUM+2+1];                                     //��ʾֵ
        struct NumStru      VRLowA[VRVGSensorCOL][CHANNUM+2+1];                                 //���ֵ
        struct NumStru      VRLowB[VRVGSensorCOL][CHANNUM+2+1];                                     //��ʾֵ


        struct NumStru      VGUpA[VRVGSensorCOL][CHANNUM+2+1];                                 //���ֵ
        struct NumStru      VGUpB[VRVGSensorCOL][CHANNUM+2+1];                                     //��ʾֵ
        struct NumStru      VGLowA[VRVGSensorCOL][CHANNUM+2+1];                                 //���ֵ
        struct NumStru      VGLowB[VRVGSensorCOL][CHANNUM+2+1];                                     //��ʾֵ

        struct RDispParaSave DisLog[HCOL+2-1][HCAM-2];  // 48*20 == 960
		struct ChanalHSet    ChanHSet;
    } Data_type;
#pragma pack ()
    unsigned char uData[12371];
};
typedef struct
{
    u8 CONNCET_NUM;   //������ʱ���ӱ��
    u16 GET_DATANUM;  //���յ������ݳ���
    u8 CONNECT_OK;    //����״̬
    char getwifidata[WIFI_SEND_LEN];//���յ�����������
    u8 isreaded;            //���������Ƿ��Ѷ�
} WIFI_DATA;   //����״̬
//union u32To4_u8
//{
//    unsigned int Date;
//    unsigned char buf[4];
//};

typedef  u8 (*uartno)(void);
typedef  void (*uartone)(u8 sData);
typedef struct
{
    uartone SendByte;
    uartno RecBufEmpty; //����һ��RecBufEmptyָ��
    uartno ReadRecBuf;
    uartone SendBuf;
}   uartFunction;
union doubleTo4_u8
{
    double Date;
    unsigned char buf[4];
};


union u32To4_u8
{
    u32 Date;
    unsigned char buf[4];
};

union u16To2_u8
{
    unsigned short Date;
    unsigned char buf[2];
};
union u32To2_u16
{
    unsigned int Date;
    unsigned short buf[2];
};

struct RegStru
{
    uint8 buf[2];
    uint8 flag;
};


extern union Data_union DataSave;


struct BoxFram     //�������
{
    uint16 x1;
    uint16 y1;
    uint16 x2;
    uint16 y2;
    uint16 h;
    uint16 w;
    char flag;
};
struct FontFram    //�������
{
    uint16 x1;
    uint16 y1;
    char flag;
};






struct RDispPara
{
    uint8 col;                   //��
    uint8 cam;                  //��
//  int len;
    char disp;                 //�Ƿ���ʾ
    char dispold;                 //�Ƿ���ʾ
    char flag;                 //�Ƿ��ʼ��
    char sel;                 //�Ƿ�ѡ��
    char selold;                 //�ϴ��Ƿ�ѡ��
//  uint32_t max;
//  uint32_t val;
    char lang;                //ʹ������
    char cind;                  //��ʾ���� home ��RStrNUM��
    char index[RStrNUM];                //��ʾ�ڼ���
    uint8 Ceng;                 //�����˵����ٲ�
    char Name[6];                 //�����˵����ٲ�
    struct NumStru Num;          //������ʾ �ṹ��
    struct BoxFram  Box;       //��ʾ�ַ�������λ��
    struct FontFram  FontCH;    //�����ַ���
    struct FontFram  FontEN;    // Ӣ���ַ���
    Point Dxy;
};








#define  CindStr    0    //Ӣ�ļ�����
#define  CindNum   1   //����


struct RDispStr
{
    uint8 col;                   //��
    uint8 cam;                  //��
    char disp;                 //�Ƿ���ʾ
    char dispold;                 //�Ƿ���ʾ
    char flag;                 //�Ƿ��ʼ��
    char sel;                 //�Ƿ�ѡ��
    char selold;                 //�ϴ��Ƿ�ѡ��
    char lang;                //ʹ������
    char cind;                  // Ӣ�ļ�����  0     �������� 1
    char index[RStrNUM];                //��ʾ�ڼ���
    struct BoxFram  Box;       //��ʾ�ַ�������λ��
    struct FontFram  FontCH;    //�����ַ���
    struct FontFram  FontEN;    // Ӣ���ַ���
    Point Dxy;
    char Str[40];
};


struct CUR     //�α�
{
    uint8 COL;   //  ��
    uint8 CAM;   //��
    uint8 OPN;   //������������ ��������
	uint8 PAGE;   //��ҳ
};
struct HearStr
{

    char Ch[2][40];
};


struct RStr
{
    char Ch[14][10];
};

struct HelpStr
{

    char Ch[2][70];
};


struct PStr
{

    char Ch[2][7];
};
struct TipStr
{

    char Ch[2][10];
};
struct HDataStr
{

    char Ch[18][10];
};



struct FileStr
{
    char Ch[99][13];
};

/*
struct NumD
{
        char cind;                //��ʾ����

    float fval;  //��cind����ΪNUMCOL
    uint8 dot;  //С����
    uint8 bit;  //��������λ��
    uint8 Decimalbit;  //С�����λ��
    uint8 Symbol;  //���ű�ʶ1Ϊ����0Ϊ��
    uint8 Changflag; // ��ֵ�Ƿ񱻸ı��
         struct  UintStr Uint[VALCIND];


};
*/


struct RDispData
{
    /*
     uint8 col;                   //???
    uint8 cam;

    char disp;                 //�Ƿ���ʾ
    char dispold;                 //�Ƿ���ʾ
    char flag;                 //�Ƿ��ʼ��
    char sel;                 //�Ƿ�ѡ��
    char selold;                 //�ϴ��Ƿ�ѡ��
      char lang;                //ʹ������
      char cind;                 //��ʾ���� home ��RStrNUM��
      char index[RStrNUM];               //��ʾ�ڼ���
      uint8 Ceng;
      struct NumStru Num;
      struct BoxFram  Box;
      struct FontFram  FontCH;
      struct FontFram  FontEN;
            char vcind;
       struct NumD    Val[VALCIND];
      */
    char vcind;// ���Է�Ϊ4��
    //  int v[CINDNUM];
    struct  UintStr Uint[VALCIND]; // 4������ off ��ѹ �¶�ʪ��  ��λҲ��4��
    struct RDispPara DP;
    struct NumStru      Up[VALCIND];                                 // 4������ off ��ѹ�����¶����� ʪ������
    struct NumStru      Low[VALCIND];                              // 4������  ����


};
#endif
/*************************** End of file ****************************/

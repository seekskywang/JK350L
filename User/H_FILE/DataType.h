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

#define EPSF0       (0.000001F)    //½Ó¿ÚÉèÖÃ½çÃæ
#define EPSFMAX       (1000000000.0F)    //½Ó¿ÚÉèÖÃ½çÃæ


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


#define RStrNUM    24   //10¸öÍ¨µÀµÄÉèÖÃ½çÃæ
#define OturNUM   9    //ÆäËüÉèÖÃÏîÄ¿µÄÉèÖÃ½çÃæ

#define OturCOL    9    //ÆäËüÉèÖÃÏîÄ¿µÄÉèÖÃ½çÃæ
#define HDataNUM   7    //Êı¾İ´¢´æÉèÖÃ½çÃæ
#define HDataCOL    7    //Êı¾İ´¢´æÉèÖÃ½çÃæ


#define IFNUM   7    //½Ó¿ÚÉèÖÃ½çÃæ
#define IFCOL    7    //½Ó¿ÚÉèÖÃ½çÃæ

#define NUMCOL    15   // Êı×ÖĞĞ
#define  FPageGraph            1   //»­Í¼Ò³Ãæ
#define  FPageDisplayData  2  //Êı¾İÕ¹Ê¾Ò³Ãæ
#define  FPageHome            3  //ÉèÖÃÒ³Ãæ
#define  FPageCh          4  //Ğ¡×ÖÌåÊı¾İÕ¹Ê¾Ò³Ãæ
#define  FPageCh3          5  //´ó×ÖÌåÊı¾İÏÔÊ¾
#define  FPageHDATA          6  //
#define  FPageOtur          7  //
#define  FPageIF          8  //
#define  FPageFile          9 //
#define  FPageRecord        10 //»Ø¿´Ò³Ãæ
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

#define  CHANNUM   10  //Í¨µÀÊı(ÎÂ¶È)
#define  CHANNUMH  3  //Í¨µÀÊı(Êª¶È)

#define  DCOL 11    //ĞĞ
#define  DCAM 5    //ÁĞ

#define  DCAMP 6    //×î´óÖµ×îĞ¡ÖµÆ½¾ùÖµ ·å·åÖµ·åÖµ¹ÈÖµ

#define  CAMMax        0     //
#define  CAMMin        1     //
#define  CAMAV        2    //
#define  CAMPP          3   //
#define  CAMP          4  //
#define  CAMG          5 //



#define  CINDNUM    3    //ÏÔÊ¾ÀàĞÍ
#define  VAL        0     //ÊıÖµ 
#define  MAX        1//
#define  MIN        2//


#define VALCIND 7    //ÀàĞÍ
#define  OFF        0     //¹Ø
#define  VDC        1     //µçÑ¹
#define  VT          2   //ÎÂ¶È
#define  VR          3  //Êª¶È
#define  VI         4     //µçÁ÷
#define  VG         5     //ÖØÁ¦
#define  VFRQ        6 //ÆµÂÊ


#define  FILEReady    0
#define  FILETFOpen   1
#define  FILEUSBOpen  2
#define  SDERR        3     //sd¿¨³õÊ¼»¯´íÎó

#define  TRIGReady    0
#define  TRIGStart    1
#define  TRIGStopTF   2
#define  TRIGStopUSB  3


#define  UintCOL  5    //ĞĞ
#define  UintCAM  3    //ÁĞ

#define  SETON    1
#define  SETOFF  0

#define NUMBER_OF_BLOCKS      15625000

#define  SDSaveMAX      500000
#define  USBSaveMAX    500000
//========================WIFI¹¤×÷²ÎÊı=======================
#define shezhi_work  1            //0ÊÇ¹¤×÷×´Ì¬ 1ÊÇÅäÖÃÄ£Ê½
#define WIFI_REC_LEN            1000    //ATÖ¸Áî»º´æÇø³¤¶È  
#define WIFI_SEND_LEN           1000    //WIFI·¢ËÍ»º´æÇø³¤¶È 

#define     wifi_RBUFSIZE  256
#define     wifi_SIZE        4
#pragma pack (1)
struct NumStru   //13¸ö×Ö½Ú
{

    uint8 dot;  //Ğ¡Êıµã
    uint8 bit;  //ÕûÊı²¿·ÖÎ»Êı
    uint8 Decimalbit;  //Ğ¡ÊıµãºóÎ»Êı
    uint8 Symbol;  //·ûºÅ±êÊ¶1Îª¸ºÊı0ÎªÕı
    uint8 Changflag; // ÊıÖµÊÇ·ñ±»¸Ä±ä¹ı
    float fval;  //µ±cindÀàĞÍÎªNUMCOL
    float fvalold;  // ÉÏÒ»´ÎµÄ¸¡µãÊı¾İ
};
#pragma pack ()
struct UintStr
{
    uint8 Col;//ÓÎ±êÑ¡ÖĞµ¥Î»ÖĞµÄÁĞ
    char Ch[ UintCOL][UintCAM ];  //µ¥Î»ÄÚÈİ
};


struct UintARR
{
    char Ch[ UintCOL][UintCAM ];  //µ¥Î»ÄÚÈİ
};

struct SLData
{
    uint8 flag; //ÊÇ·ñÓĞĞ§
    uint8 byte[3]; //½ÓÊÕµÄÈı¸ö×Ö½Ú
    struct NumStru fStr;//½ÓÊÕºó½âÎöµÄÊı¾İÔ­Ê¼Êı¾İ
    struct NumStru DfStr;//½ÓÊÕºóÊı¾İ¿ÉÄÜµ÷ÕûÔÙ¸øÏÔÊ¾
    struct UintStr UintOne;  //½ÓÊÕºóµçÑ¹µ¥Î»¿ÉÄÜ¸ù¾İÊıÖµµ÷Õû
    char vcind; //Êı¾İÀàĞÍÇø·ÖµçÑ¹ÎÂ¶ÈÊª¶È
    Point Dxy;
};

#define     SDLEN 262
#pragma pack (1)
union DataSD_union
{
    //14¸ö×Ö½Ú
    struct
    {
        char vcind; //Êı¾İÀàĞÍÇø·ÖµçÑ¹ÎÂ¶ÈÊª¶È
        uint8 byte[3]; //½ÓÊÕµÄÈı¸ö×Ö½
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
        uint8 DataType; //   Êı¾İÀàĞÍ
        uint8 SampleTimeCurCAM;
        //  uint8 ZoneCurCAM;
        uint32 Timestamp;
        uint32 starttick;
        union DataSD_union FD[2];     //Á½¸öÆµÂÊ
        union DataSD_union CHD[10];  //10¸öÍ¨µÀ ²»Ô¤Áô
		union DataSD_union HMD[6];  //3¸öÊª¶ÈÍ¨µÀ ²»Ô¤Áô

    } Data;
    unsigned char uD[SDLEN]; //×Ö½Ú
};
#pragma pack ()
struct ChanalData
{
    uint8 alarm;
    struct SLData frq[2];
    struct SLData chd[CHANNUM];
	struct SLData hmd[6];
    uint8  chalarm[CHANNUM+3];   //Ã¿¸öÍ¨µÀµÄ±¨¾¯ÀàĞÍ 0 ¹Ø1ÉÏ³¬ 2 ÏÂ³¬ 3 ³¬³ö
};

typedef struct
{
    u8 rBuf[wifi_SIZE][wifi_RBUFSIZE];  //½ÓÊÕ»º³åÇø
    u8 wifilen[wifi_SIZE];
    u8 wifilianjie[wifi_SIZE];
    u16 rPtr1;    //½ÓÊÕ»º³åÇø¶ÓÁĞÊ×
    u16 rPtr2;    //½ÓÊÕ»º³åÇø¶ÓÁĞÎ²
} wifi_data_t;
struct ChanalMData
{
    uint8 ch;     //µÚ¼¸¸öÍ¨µÀ
    // uint16 Num;    //´æ´¢¿ªÊ¼µÄµÚ¼¸¸öÊı¾İ
    struct SLData chmd[10];     //  1°Ñ±ê×¼10¸öÊı¾İ
};


#pragma pack (1)
struct ChanalSet  //62¸ö×Ö½Ú
{
    uint8 Mod[CHANNUM];  //10¸ö×Ö½Ú
    uint8 Time[CHANNUM]; //10¸ö×Ö½Ú
    uint8 SYNC;
    uint8 alarm;
    uint32 TimeVal[CHANNUM]; //40¸ö×Ö½Ú
};

struct ChanalHSet  //2¸ö×Ö½Ú
{
    uint8 Mod[CHANNUMH];  //3¸ö×Ö½Ú
    uint8 Time[CHANNUMH]; //3¸ö×Ö½Ú
    uint8 SYNC;
    uint8 alarm;
    uint32 TimeVal[CHANNUMH]; //12¸ö×Ö½Ú
};
#pragma pack ()
#pragma pack (1)
struct RDispParaSave
{
    char cind;                  //ÏÔÊ¾ÖÖÀà home ÓĞRStrNUMÖÖ
    char index[RStrNUM];                //ÏÔÊ¾µÚ¼¸Ïî
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
        uint8 PageFlag;                       //Ò³Ãæ±êÖ¾ Çø·ÖÒ³Ãæ±êÊ¶ »­Í¼Ò³Ãæ  1×Ö½Ú
        uint8 LANG;       //
        uint8 Bright;    //ÁÁ¶È
        uint8 ScrSaver;    //ÆÁÄ»±£»¤
        uint8 PowerOn; //¿ª»úÉÏµç
        uint8 RoomT;   //ÎÂ¶È±£»¤
        uint8 TUnit;   //ÎÂ¶Èµ¥Î»
        uint8 SW;   //ÏÔÊ¾Âß¼­ºÍ±¨¾¯½çÃæµÄÇĞ»»
        uint8 USBId;    //ÆÁÄ»±£»¤
        uint8 BeepF;    //ÆÁÄ»±£»¤
        uint8   SampleCAM;
        uint8   SaveSample;
        uint8   ZoneCAM;      //
        uint8   FormatF;
        uint8   ShotF;   //
        uint8   AlarmOutF;   //
        uint8   BlockAddr;                     //×Ö½Ú
        uint8   SYNF;    //Í¬²½±êÖ¾
        uint8   PLSF[2];   //
        RTC_DateTypeDef Date;  //   4¸ö×Ö½ÚÈÕÆÚÊ±¼ä
        char PName[11]; //²úÆ·ĞÍºÅ
        char PSN[11]; //²úÆ·ĞòÁĞºÅ
        uint8 TraceF[CHANNUM+2+1];        //
        uint8 ComputeT[CHANNUM+2+1];        //
        uint8 ComputeDC[CHANNUM+2+1];        //

        uint8 Alarm[CHANNUM+2+1];     //±¨¾¯ÀàĞÍ
        RTC_TimeTypeDef Time;   //20¸ö×Ö½ÚÈÕÆÚÊ±¼ä
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
        struct NumStru      DCMin[CHANNUM+2+1];                                     //130 ×Ö½Ú
        struct NumStru      TempLMax[CHANNUM+2+1];                                 //
        struct NumStru      TempLMin[CHANNUM+2+1];                                     //
        struct NumStru      DCLMax[CHANNUM+2+1];                                 //
        struct NumStru      DCLMin[CHANNUM+2+1];                                     //52×Ö½Ú

        //
        struct NumStru      DIMax[CHANNUM+2+1];                                 //
        struct NumStru      DIMin[CHANNUM+2+1];                                     //130 ×Ö½Ú
        struct NumStru      DILMax[CHANNUM+2+1];                                 //
        struct NumStru      DILMin[CHANNUM+2+1];                                     //52×Ö½Ú


        struct NumStru      VRMax[CHANNUM+2+1];                                 //
        struct NumStru      VRMin[CHANNUM+2+1];                                     //130 ×Ö½Ú
        struct NumStru      VRLMax[CHANNUM+2+1];                                 //
        struct NumStru      VRLMin[CHANNUM+2+1];                                     //52×Ö½Ú

        struct NumStru      VGMax[CHANNUM+2+1];                                 //
        struct NumStru      VGMin[CHANNUM+2+1];                                     //130 ×Ö½Ú
        struct NumStru      VGLMax[CHANNUM+2+1];                                 //
        struct NumStru      VGLMin[CHANNUM+2+1];                                     //52×Ö½Ú

        PARAMETERS_t parameters;
        struct NumStru      DIUpMeas[VRVGSensorCOL][CHANNUM+2+1];                                 //
        struct NumStru      DIUpEu[VRVGSensorCOL][CHANNUM+2+1];                                     //
        struct NumStru      DILowMeas[VRVGSensorCOL][CHANNUM+2+1];                                 //
        struct NumStru      DILowEu[VRVGSensorCOL][CHANNUM+2+1];

        struct NumStru      VRUpA[VRVGSensorCOL][CHANNUM+2+1];                                 //Êä³öÖµ
        struct NumStru      VRUpB[VRVGSensorCOL][CHANNUM+2+1];                                     //ÏÔÊ¾Öµ
        struct NumStru      VRLowA[VRVGSensorCOL][CHANNUM+2+1];                                 //Êä³öÖµ
        struct NumStru      VRLowB[VRVGSensorCOL][CHANNUM+2+1];                                     //ÏÔÊ¾Öµ


        struct NumStru      VGUpA[VRVGSensorCOL][CHANNUM+2+1];                                 //Êä³öÖµ
        struct NumStru      VGUpB[VRVGSensorCOL][CHANNUM+2+1];                                     //ÏÔÊ¾Öµ
        struct NumStru      VGLowA[VRVGSensorCOL][CHANNUM+2+1];                                 //Êä³öÖµ
        struct NumStru      VGLowB[VRVGSensorCOL][CHANNUM+2+1];                                     //ÏÔÊ¾Öµ

        struct RDispParaSave DisLog[HCOL+2-1][HCAM-2];  // 48*20 == 960
		struct ChanalHSet    ChanHSet;
    } Data_type;
#pragma pack ()
    unsigned char uData[12371];
};
typedef struct
{
    u8 CONNCET_NUM;   //·şÎñÆ÷Ê±Á¬½Ó±àºÅ
    u16 GET_DATANUM;  //½ÓÊÕµ½µÄÊı¾İ³¤¶È
    u8 CONNECT_OK;    //Á¬½Ó×´Ì¬
    char getwifidata[WIFI_SEND_LEN];//½ÓÊÕµ½µÄÊı¾İÄÚÈİ
    u8 isreaded;            //±¾´ÎÊı¾İÊÇ·ñÒÑ¶Á
} WIFI_DATA;   //ÍøÂç×´Ì¬
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
    uartno RecBufEmpty; //¶¨ÒåÒ»¸öRecBufEmptyÖ¸Õë
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


struct BoxFram     //¿òµÄ×ø±ê
{
    uint16 x1;
    uint16 y1;
    uint16 x2;
    uint16 y2;
    uint16 h;
    uint16 w;
    char flag;
};
struct FontFram    //¿òµÄ×ø±ê
{
    uint16 x1;
    uint16 y1;
    char flag;
};






struct RDispPara
{
    uint8 col;                   //ĞĞ
    uint8 cam;                  //ÁĞ
//  int len;
    char disp;                 //ÊÇ·ñÏÔÊ¾
    char dispold;                 //ÊÇ·ñÏÔÊ¾
    char flag;                 //ÊÇ·ñ³õÊ¼»¯
    char sel;                 //ÊÇ·ñÑ¡ÖĞ
    char selold;                 //ÉÏ´ÎÊÇ·ñÑ¡ÖĞ
//  uint32_t max;
//  uint32_t val;
    char lang;                //Ê¹ÓÃÓïÑÔ
    char cind;                  //ÏÔÊ¾ÖÖÀà home ÓĞRStrNUMÖÖ
    char index[RStrNUM];                //ÏÔÊ¾µÚ¼¸Ïî
    uint8 Ceng;                 //ÏÂÀ­²Ëµ¥¶àÉÙ²ã
    char Name[6];                 //ÏÂÀ­²Ëµ¥¶àÉÙ²ã
    struct NumStru Num;          //Êı¾İÏÔÊ¾ ½á¹¹Ìå
    struct BoxFram  Box;       //ÏÔÊ¾×Ö·û´®¿òÌåÎ»ÖÃ
    struct FontFram  FontCH;    //ÖĞÎÄ×Ö·û´®
    struct FontFram  FontEN;    // Ó¢ÎÄ×Ö·û´®
    Point Dxy;
};








#define  CindStr    0    //Ó¢ÎÄ¼ÓÊı×Ö
#define  CindNum   1   //Êı×Ö


struct RDispStr
{
    uint8 col;                   //ĞĞ
    uint8 cam;                  //ÁĞ
    char disp;                 //ÊÇ·ñÏÔÊ¾
    char dispold;                 //ÊÇ·ñÏÔÊ¾
    char flag;                 //ÊÇ·ñ³õÊ¼»¯
    char sel;                 //ÊÇ·ñÑ¡ÖĞ
    char selold;                 //ÉÏ´ÎÊÇ·ñÑ¡ÖĞ
    char lang;                //Ê¹ÓÃÓïÑÔ
    char cind;                  // Ó¢ÎÄ¼ÓÊı×Ö  0     ½ö½öÊı×Ö 1
    char index[RStrNUM];                //ÏÔÊ¾µÚ¼¸Ïî
    struct BoxFram  Box;       //ÏÔÊ¾×Ö·û´®¿òÌåÎ»ÖÃ
    struct FontFram  FontCH;    //ÖĞÎÄ×Ö·û´®
    struct FontFram  FontEN;    // Ó¢ÎÄ×Ö·û´®
    Point Dxy;
    char Str[40];
};


struct CUR     //ÓÎ±ê
{
    uint8 COL;   //  ĞĞ
    uint8 CAM;   //ÁĞ
    uint8 OPN;   //°´¼ü²Ù×÷ÀàĞÍ ÉÏÏÂ×óÓÒ
	uint8 PAGE;   //·ÖÒ³
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
        char cind;                //ÏÔÊ¾ÖÖÀà

    float fval;  //µ±cindÀàĞÍÎªNUMCOL
    uint8 dot;  //Ğ¡Êıµã
    uint8 bit;  //ÕûÊı²¿·ÖÎ»Êı
    uint8 Decimalbit;  //Ğ¡ÊıµãºóÎ»Êı
    uint8 Symbol;  //·ûºÅ±êÊ¶1Îª¸ºÊı0ÎªÕı
    uint8 Changflag; // ÊıÖµÊÇ·ñ±»¸Ä±ä¹ı
         struct  UintStr Uint[VALCIND];


};
*/


struct RDispData
{
    /*
     uint8 col;                   //???
    uint8 cam;

    char disp;                 //ÊÇ·ñÏÔÊ¾
    char dispold;                 //ÊÇ·ñÏÔÊ¾
    char flag;                 //ÊÇ·ñ³õÊ¼»¯
    char sel;                 //ÊÇ·ñÑ¡ÖĞ
    char selold;                 //ÉÏ´ÎÊÇ·ñÑ¡ÖĞ
      char lang;                //Ê¹ÓÃÓïÑÔ
      char cind;                 //ÏÔÊ¾ÖÖÀà home ÓĞRStrNUMÖÖ
      char index[RStrNUM];               //ÏÔÊ¾µÚ¼¸Ïî
      uint8 Ceng;
      struct NumStru Num;
      struct BoxFram  Box;
      struct FontFram  FontCH;
      struct FontFram  FontEN;
            char vcind;
       struct NumD    Val[VALCIND];
      */
    char vcind;// ¿ÉÒÔ·ÖÎª4Àà
    //  int v[CINDNUM];
    struct  UintStr Uint[VALCIND]; // 4ÖÖÀàĞÍ off µçÑ¹ ÎÂ¶ÈÊª¶È  µ¥Î»Ò²ÓĞ4Àà
    struct RDispPara DP;
    struct NumStru      Up[VALCIND];                                 // 4ÖÖÀàĞÍ off µçÑ¹ÉÏÏŞÎÂ¶ÈÉÏÏŞ Êª¶ÈÉÏÏŞ
    struct NumStru      Low[VALCIND];                              // 4ÖÖÀàĞÍ  ÏÂÏŞ


};
#endif
/*************************** End of file ****************************/

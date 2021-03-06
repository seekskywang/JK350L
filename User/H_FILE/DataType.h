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

#define EPSF0       (0.000001F)    //接口设置界面
#define EPSFMAX       (1000000000.0F)    //接口设置界面


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


#define RStrNUM    24   //10个通道的设置界面
#define OturNUM   9    //其它设置项目的设置界面

#define OturCOL    9    //其它设置项目的设置界面
#define HDataNUM   7    //数据储存设置界面
#define HDataCOL    7    //数据储存设置界面


#define IFNUM   7    //接口设置界面
#define IFCOL    7    //接口设置界面

#define NUMCOL    15   // 数字行
#define  FPageGraph            1   //画图页面
#define  FPageDisplayData  2  //数据展示页面
#define  FPageHome            3  //设置页面
#define  FPageCh          4  //小字体数据展示页面
#define  FPageCh3          5  //大字体数据显示
#define  FPageHDATA          6  //
#define  FPageOtur          7  //
#define  FPageIF          8  //
#define  FPageFile          9 //
#define  FPageRecord        10 //回看页面
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

#define  CHANNUM   10  //通道数(温度)
#define  CHANNUMH  3  //通道数(湿度)

#define  DCOL 11    //行
#define  DCAM 5    //列

#define  DCAMP 6    //最大值最小值平均值 峰峰值峰值谷值

#define  CAMMax        0     //
#define  CAMMin        1     //
#define  CAMAV        2    //
#define  CAMPP          3   //
#define  CAMP          4  //
#define  CAMG          5 //



#define  CINDNUM    3    //显示类型
#define  VAL        0     //数值 
#define  MAX        1//
#define  MIN        2//


#define VALCIND 7    //类型
#define  OFF        0     //关
#define  VDC        1     //电压
#define  VT          2   //温度
#define  VR          3  //湿度
#define  VI         4     //电流
#define  VG         5     //重力
#define  VFRQ        6 //频率


#define  FILEReady    0
#define  FILETFOpen   1
#define  FILEUSBOpen  2
#define  SDERR        3     //sd卡初始化错误

#define  TRIGReady    0
#define  TRIGStart    1
#define  TRIGStopTF   2
#define  TRIGStopUSB  3


#define  UintCOL  5    //行
#define  UintCAM  3    //列

#define  SETON    1
#define  SETOFF  0

#define NUMBER_OF_BLOCKS      15625000

#define  SDSaveMAX      500000
#define  USBSaveMAX    500000
//========================WIFI工作参数=======================
#define shezhi_work  1            //0是工作状态 1是配置模式
#define WIFI_REC_LEN            1000    //AT指令缓存区长度  
#define WIFI_SEND_LEN           1000    //WIFI发送缓存区长度 

#define     wifi_RBUFSIZE  256
#define     wifi_SIZE        4
#pragma pack (1)
struct NumStru   //13个字节
{

    uint8 dot;  //小数点
    uint8 bit;  //整数部分位数
    uint8 Decimalbit;  //小数点后位数
    uint8 Symbol;  //符号标识1为负数0为正
    uint8 Changflag; // 数值是否被改变过
    float fval;  //当cind类型为NUMCOL
    float fvalold;  // 上一次的浮点数据
};
#pragma pack ()
struct UintStr
{
    uint8 Col;//游标选中单位中的列
    char Ch[ UintCOL][UintCAM ];  //单位内容
};


struct UintARR
{
    char Ch[ UintCOL][UintCAM ];  //单位内容
};

struct SLData
{
    uint8 flag; //是否有效
    uint8 byte[3]; //接收的三个字节
    struct NumStru fStr;//接收后解析的数据原始数据
    struct NumStru DfStr;//接收后数据可能调整再给显示
    struct UintStr UintOne;  //接收后电压单位可能根据数值调整
    char vcind; //数据类型区分电压温度湿度
    Point Dxy;
};

#define     SDLEN 262
#pragma pack (1)
union DataSD_union
{
    //14个字节
    struct
    {
        char vcind; //数据类型区分电压温度湿度
        uint8 byte[3]; //接收的三个字?
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
        uint8 DataType; //   数据类型
        uint8 SampleTimeCurCAM;
        //  uint8 ZoneCurCAM;
        uint32 Timestamp;
        uint32 starttick;
        union DataSD_union FD[2];     //两个频率
        union DataSD_union CHD[10];  //10个通道 不预留
		union DataSD_union HMD[6];  //3个湿度通道 不预留

    } Data;
    unsigned char uD[SDLEN]; //字节
};
#pragma pack ()
struct ChanalData
{
    uint8 alarm;
    struct SLData frq[2];
    struct SLData chd[CHANNUM];
	struct SLData hmd[6];
    uint8  chalarm[CHANNUM+3];   //每个通道的报警类型 0 关1上超 2 下超 3 超出
};

typedef struct
{
    u8 rBuf[wifi_SIZE][wifi_RBUFSIZE];  //接收缓冲区
    u8 wifilen[wifi_SIZE];
    u8 wifilianjie[wifi_SIZE];
    u16 rPtr1;    //接收缓冲区队列首
    u16 rPtr2;    //接收缓冲区队列尾
} wifi_data_t;
struct ChanalMData
{
    uint8 ch;     //第几个通道
    // uint16 Num;    //存储开始的第几个数据
    struct SLData chmd[10];     //  1把标准10个数据
};


#pragma pack (1)
struct ChanalSet  //62个字节
{
    uint8 Mod[CHANNUM];  //10个字节
    uint8 Time[CHANNUM]; //10个字节
    uint8 SYNC;
    uint8 alarm;
    uint32 TimeVal[CHANNUM]; //40个字节
};

struct ChanalHSet  //2个字节
{
    uint8 Mod[CHANNUMH];  //3个字节
    uint8 Time[CHANNUMH]; //3个字节
    uint8 SYNC;
    uint8 alarm;
    uint32 TimeVal[CHANNUMH]; //12个字节
};
#pragma pack ()
#pragma pack (1)
struct RDispParaSave
{
    char cind;                  //显示种类 home 有RStrNUM种
    char index[RStrNUM];                //显示第几项
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
        uint8 PageFlag;                       //页面标志 区分页面标识 画图页面  1字节
        uint8 LANG;       //
        uint8 Bright;    //亮度
        uint8 ScrSaver;    //屏幕保护
        uint8 PowerOn; //开机上电
        uint8 RoomT;   //温度保护
        uint8 TUnit;   //温度单位
        uint8 SW;   //显示逻辑和报警界面的切换
        uint8 USBId;    //屏幕保护
        uint8 BeepF;    //屏幕保护
        uint8   SampleCAM;
        uint8   SaveSample;
        uint8   ZoneCAM;      //
        uint8   FormatF;
        uint8   ShotF;   //
        uint8   AlarmOutF;   //
        uint8   BlockAddr;                     //字节
        uint8   SYNF;    //同步标志
        uint8   PLSF[2];   //
        RTC_DateTypeDef Date;  //   4个字节日期时间
        char PName[11]; //产品型号
        char PSN[11]; //产品序列号
        uint8 TraceF[CHANNUM+2+1];        //
        uint8 ComputeT[CHANNUM+2+1];        //
        uint8 ComputeDC[CHANNUM+2+1];        //

        uint8 Alarm[CHANNUM+2+1];     //报警类型
        RTC_TimeTypeDef Time;   //20个字节日期时间
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
        struct NumStru      DCMin[CHANNUM+2+1];                                     //130 字节
        struct NumStru      TempLMax[CHANNUM+2+1];                                 //
        struct NumStru      TempLMin[CHANNUM+2+1];                                     //
        struct NumStru      DCLMax[CHANNUM+2+1];                                 //
        struct NumStru      DCLMin[CHANNUM+2+1];                                     //52字节

        //
        struct NumStru      DIMax[CHANNUM+2+1];                                 //
        struct NumStru      DIMin[CHANNUM+2+1];                                     //130 字节
        struct NumStru      DILMax[CHANNUM+2+1];                                 //
        struct NumStru      DILMin[CHANNUM+2+1];                                     //52字节


        struct NumStru      VRMax[CHANNUM+2+1];                                 //
        struct NumStru      VRMin[CHANNUM+2+1];                                     //130 字节
        struct NumStru      VRLMax[CHANNUM+2+1];                                 //
        struct NumStru      VRLMin[CHANNUM+2+1];                                     //52字节

        struct NumStru      VGMax[CHANNUM+2+1];                                 //
        struct NumStru      VGMin[CHANNUM+2+1];                                     //130 字节
        struct NumStru      VGLMax[CHANNUM+2+1];                                 //
        struct NumStru      VGLMin[CHANNUM+2+1];                                     //52字节

        PARAMETERS_t parameters;
        struct NumStru      DIUpMeas[VRVGSensorCOL][CHANNUM+2+1];                                 //
        struct NumStru      DIUpEu[VRVGSensorCOL][CHANNUM+2+1];                                     //
        struct NumStru      DILowMeas[VRVGSensorCOL][CHANNUM+2+1];                                 //
        struct NumStru      DILowEu[VRVGSensorCOL][CHANNUM+2+1];

        struct NumStru      VRUpA[VRVGSensorCOL][CHANNUM+2+1];                                 //输出值
        struct NumStru      VRUpB[VRVGSensorCOL][CHANNUM+2+1];                                     //显示值
        struct NumStru      VRLowA[VRVGSensorCOL][CHANNUM+2+1];                                 //输出值
        struct NumStru      VRLowB[VRVGSensorCOL][CHANNUM+2+1];                                     //显示值


        struct NumStru      VGUpA[VRVGSensorCOL][CHANNUM+2+1];                                 //输出值
        struct NumStru      VGUpB[VRVGSensorCOL][CHANNUM+2+1];                                     //显示值
        struct NumStru      VGLowA[VRVGSensorCOL][CHANNUM+2+1];                                 //输出值
        struct NumStru      VGLowB[VRVGSensorCOL][CHANNUM+2+1];                                     //显示值

        struct RDispParaSave DisLog[HCOL+2-1][HCAM-2];  // 48*20 == 960
		struct ChanalHSet    ChanHSet;
    } Data_type;
#pragma pack ()
    unsigned char uData[12371];
};
typedef struct
{
    u8 CONNCET_NUM;   //服务器时连接编号
    u16 GET_DATANUM;  //接收到的数据长度
    u8 CONNECT_OK;    //连接状态
    char getwifidata[WIFI_SEND_LEN];//接收到的数据内容
    u8 isreaded;            //本次数据是否已读
} WIFI_DATA;   //网络状态
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
    uartno RecBufEmpty; //定义一个RecBufEmpty指针
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


struct BoxFram     //框的坐标
{
    uint16 x1;
    uint16 y1;
    uint16 x2;
    uint16 y2;
    uint16 h;
    uint16 w;
    char flag;
};
struct FontFram    //框的坐标
{
    uint16 x1;
    uint16 y1;
    char flag;
};






struct RDispPara
{
    uint8 col;                   //行
    uint8 cam;                  //列
//  int len;
    char disp;                 //是否显示
    char dispold;                 //是否显示
    char flag;                 //是否初始化
    char sel;                 //是否选中
    char selold;                 //上次是否选中
//  uint32_t max;
//  uint32_t val;
    char lang;                //使用语言
    char cind;                  //显示种类 home 有RStrNUM种
    char index[RStrNUM];                //显示第几项
    uint8 Ceng;                 //下拉菜单多少层
    char Name[6];                 //下拉菜单多少层
    struct NumStru Num;          //数据显示 结构体
    struct BoxFram  Box;       //显示字符串框体位置
    struct FontFram  FontCH;    //中文字符串
    struct FontFram  FontEN;    // 英文字符串
    Point Dxy;
};








#define  CindStr    0    //英文加数字
#define  CindNum   1   //数字


struct RDispStr
{
    uint8 col;                   //行
    uint8 cam;                  //列
    char disp;                 //是否显示
    char dispold;                 //是否显示
    char flag;                 //是否初始化
    char sel;                 //是否选中
    char selold;                 //上次是否选中
    char lang;                //使用语言
    char cind;                  // 英文加数字  0     仅仅数字 1
    char index[RStrNUM];                //显示第几项
    struct BoxFram  Box;       //显示字符串框体位置
    struct FontFram  FontCH;    //中文字符串
    struct FontFram  FontEN;    // 英文字符串
    Point Dxy;
    char Str[40];
};


struct CUR     //游标
{
    uint8 COL;   //  行
    uint8 CAM;   //列
    uint8 OPN;   //按键操作类型 上下左右
	uint8 PAGE;   //分页
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
        char cind;                //显示种类

    float fval;  //当cind类型为NUMCOL
    uint8 dot;  //小数点
    uint8 bit;  //整数部分位数
    uint8 Decimalbit;  //小数点后位数
    uint8 Symbol;  //符号标识1为负数0为正
    uint8 Changflag; // 数值是否被改变过
         struct  UintStr Uint[VALCIND];


};
*/


struct RDispData
{
    /*
     uint8 col;                   //???
    uint8 cam;

    char disp;                 //是否显示
    char dispold;                 //是否显示
    char flag;                 //是否初始化
    char sel;                 //是否选中
    char selold;                 //上次是否选中
      char lang;                //使用语言
      char cind;                 //显示种类 home 有RStrNUM种
      char index[RStrNUM];               //显示第几项
      uint8 Ceng;
      struct NumStru Num;
      struct BoxFram  Box;
      struct FontFram  FontCH;
      struct FontFram  FontEN;
            char vcind;
       struct NumD    Val[VALCIND];
      */
    char vcind;// 可以分为4类
    //  int v[CINDNUM];
    struct  UintStr Uint[VALCIND]; // 4种类型 off 电压 温度湿度  单位也有4类
    struct RDispPara DP;
    struct NumStru      Up[VALCIND];                                 // 4种类型 off 电压上限温度上限 湿度上限
    struct NumStru      Low[VALCIND];                              // 4种类型  下限


};
#endif
/*************************** End of file ****************************/

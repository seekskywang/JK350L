#ifndef SLCZRF_H
#define SLCZRF_H

#define ChrStart         0xAC




#define Cmd_RST           0x01
#define Cmd_MOD          0x02
#define Cmd_TIME         0x03
#define Cmd_DATA        0x04
#define Cmd_FDATA      0x05
#define Cmd_ALARM      0x06
#define Cmd_TEMP      0x07
#define Cmd_TandH      0x08
#define Cmd_HMOD     0x09

#define CmdR_RST           0x41
#define CmdR_MOD          0x42
#define CmdR_TIME         0x43
#define CmdR_DATA        0x44
#define CmdR_FDATA      0x45
#define CmdR_ALARM      0x46
#define CmdR_TEMP      0x47
#define CmdR_TandH     0x48
#define CmdR_HMOD    0x49

#define Cmd_ERR            0xFF
#define Cmd_NULL          0xFE
void NumTest(void);

extern uint8 chrStr[256];
extern uint8 cpyStr[256];

uint8 SerialRemoteHandle(uint8 len,uint8 *buf,struct ChanalData *pCmd,struct ChanalMData *pMD);
uint32 JiSuanXYsamp(struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD,uint8 * pr1,uint16 XLen);
uint32 JiSuanXYsamp2(struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD,uint8 * pr1,uint16 XLen);
void SendReSet(void);
void SendReadTemp(void);

//发送设置通道模式指令
float BYTE3toFloat(uint8 *buf);
void SendSetMod(const struct ChanalSet D);
void SendSetHMod(const struct ChanalHSet D);
//发送设置通道时间指令
void FloattoBYTE4(float fval,uint8 *buf);
void SendSetSYNCTime(const struct ChanalSet D);
void SendReadData(const struct ChanalSet D);
void SendReadTandH(const struct ChanalSet D);
void SwitchChanalData ( struct NumStru *p    , uint8 decimalbit);
void SendReadFData(uint8 ch,uint8 Dnum);
void SendSetALARM(uint8 TIMON,uint8 TIMOFF);
void   fStr2DfStr(struct SLData *p);//接收后解析的数据原始数据转换为数据显示
uint32 JiSuanXY (struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD);
uint32 JiSuanXY2(struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD);
uint8 IsDigitChar(uint8 a);
#endif



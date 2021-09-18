
#ifndef  HOMEFILE_H_
#define  HOMEFILE_H_
#include  "DataType.h"

/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/
/*USB状态*/
#define UNKNOWN		     2
#define UNCONNECTED		 0
#define CONNECTED		 1
//任务控制块
extern OS_TCB DoFileTaskTCB;
//任务堆栈
extern CPU_STK DoFile_TASK_STK[DoFile_STK_SIZE];
uint8 GetNewName(char *path,char *name,const char* head);
void Stu_file_Init(void);
u8 NewFile(const char* head,const char* type);
void DoFile_task(void *p_arg);
void page_file_cycle(const struct CUR cur);
void TrigFile(void);
void EndSD(void);
void FormatSD(void);
void SaveSDFM(void);
void LCD_DisplayNumLine_EN_CH_LJ_Home_File(struct RDispStr *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType);
void LCD_DisplayStringLineLJ_Home_File(struct RDispStr* sel,uint16_t x,uint16_t y,uint8_t FontType);
void Disp_Box_Home_File(struct RDispStr *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color);
void LCD_DisplayStringLine_EN_CH_LJ_File(struct RDispPara *sel,uint16_t x,uint16_t y);
void LCD_DisplayStringLine_EN_CH_LJ_Home_File(struct RDispStr *sel,uint16_t x,uint16_t y);
void udisk_scan(void);
uint8 FatReadAllDir(char *path,char *str);
void InitUSBEXL(void);
void SerialRemoteHandleR(uint8 *buf,struct ChanalData *pCmd);
void SaveSD(void);
void SaveUSBEXL(char* TarName,uint8_t  UartCmd);
void TiShiDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 type);
void ModifyTiShi(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCam);
void IsReTrigFile(uint32 FileSaveNum);
extern struct RDispPara DisTiShi[DisTiShiCAM];
extern char SaveTrig;
extern u8 usbstatus;
extern char FileRes; 
extern char FileUsbRes;
extern char filenameUSB[20];
extern u8 FileHao;
#endif                                                          /* End of module include.                               */


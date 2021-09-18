

#ifndef  HOMEIF_H_
#define  HOMEIF_H_
#include  "DataType.h"

/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

void page_IF_cycle(void);
void page_IF(void);

extern const struct RStr IFCh[IFNUM];
extern const struct RStr IFEn[IFNUM];

void Stu_IF_Init(void);
void Stu_IF_cycle(struct CUR *sle,struct CUR *sleold);
uint8 ModifyIF(uint8 keytmp,struct CUR* cur,struct CUR* curold);             // ��ֵƥ�亯������while��ѭ���з���ƥ��

void LCD_DisplayStringLineLJ_IF(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType,uint32_t SetTextColor);
void DoIF_task(void *p_arg);
void Stu_MinIF_Init(void);
void page_IF_cycle(void);
void ModifySoftKey(uint8 keytmp,struct RDispStr* RD,struct RDispStr* RDOld,struct CUR* cur,char MaxCol,char MaxCam,uint8 MaxNum);
void SoftKeyDia(uint8 keytmp,struct RDispStr* RD,struct CUR* cur);
void SoftKeyDiaInit(uint8 keytmp,struct RDispStr* RD,struct CUR* cur);
void LCD_DisplayNumLine_EN_CH_LJ_Home_Wifi(struct RDispStr *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType);
//�α�  ���к���
extern struct CUR CurIF;
extern struct CUR CurIFOld;

extern struct RDispStr DisSoftKey[DiaSoftKeyCOL][DiaSoftKeyCAM];//�ӶԻ����ѹ�Ի���Ŀؼ�����


//������ƿ�
extern OS_TCB DoIFTaskTCB;
//�����ջ
extern CPU_STK DoIF_TASK_STK[DoIF_STK_SIZE];

extern const char FuHao[DiaSoftKeyCAM];
//�������õ������˵�
extern struct RDispPara DisIF[IFCOL];
#endif                                                          /* End of module include.                               */


/**
  ******************************************************************************
  * @file    Templates/Inc/main.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WORK_H
#define __WORK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include  "DataType.h"
#include  "home.h"
//DoWork����

//������ƿ�
extern OS_TCB DoWorkTaskTCB;
//�����ջ
extern CPU_STK DoWork_TASK_STK[DoWork_STK_SIZE];
//DoWork����
void DoWork_task(void *p_arg);
uint8 Modify(uint8 keytmp,struct CUR* cur,struct CUR* curold);
void page_Mindle(void);
void ModifyCAM0(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol) ;
void InPutManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void FilterManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur) ;
void ModifyCAM3(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol) ;
void ModifyCAM1(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol,char hCol);
void ModifyCAM2(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol) ;
void SensorManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur) ;
void RangeManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur) ;
void DCDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur) ;
void ModifyTEMP(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol,char MaxCam)  ;
void ModifyOnOff(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol) ;                 // ��ֵƥ�亯������while��ѭ���з���ƥ��
void TEMPOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)  ;
void ModifyTEMPNum(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char* ptr)   ;
void DCOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)  ;
void ModifyDCNum(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char* ptr)   ;
void float2char(float slope,char*buffer,int n);
void NumFloat2Char(struct RDispData* RD,struct NumStru *Num,char* ptr);
void LCD_DisplayNumLine_EN_CH_LJ(struct RDispPara* RD, char *ptr,uint32 SetTextColor,uint32 SetBackColor,sFONT FontType);
void ModifyDC(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)  ;
uint8 KeyTaskCreate(uint8 key_ReTurn);
// ��ѹ��Χ�Ի����ʼ��
void DCRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//�¶ȷ�Χ�Ի����ʼ��
void TEMPRangeDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//��ѹ��Χ�Ի���˶Ի�����¶ȶԻ���һ�����õ�����Ҳһ��
void DCRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//�¶ȷ�Χ�Ի���
void TEMPRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//0n Off�л�������
void TEMPRangeOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//�¶ȷ�Χ������ļ�ֵ������
void ModifyRangeTEMP(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam);


// ��ѹ��Χ�Ի����ʼ��
void DCLimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//�¶ȷ�Χ�Ի����ʼ��
void TEMPLimitDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//��ѹ��Χ�Ի���˶Ի�����¶ȶԻ���һ�����õ�����Ҳһ��
void DCLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//�¶ȷ�Χ�Ի���
void TEMPLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//0n Off�л�������
void TEMPLimitOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//�¶ȷ�Χ������ļ�ֵ������
void ModifyLimitTEMP(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam);
//������������
void power_on(void);
void DrawIniALL(uint8 i,uint32 time);
void SaveAll(void);
void NumFloat2CharS(struct NumStru *Num,char* ptr);
void DILimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void DIRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void DIDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
//����U������ͼ��
void DrawUdisk(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag);
void DrawHid(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag);
void DrawWIFI(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag);
void DrawBat(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff1,uint8_t flag);
void DrawSave(uint16_t Xpos, uint16_t Ypos,uint8_t OnOff,uint8_t flag);
void ModifyDI(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam);
void ModifyVG(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam);
void SetChanFilter(void);
void VGDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VGRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VGLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VGDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VGOnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VRDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VRRangeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VRLimitDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VRDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void VROnOff(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
void ModifyVR(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam);
void VRVGDISensorManu(uint8 keytmp,struct RDispPara* RD,struct CUR* cur);
extern uint8 HidFlag;
extern OS_SEM  Page_SEM;   
extern struct RDispPara DisInPut[DiaTEMPCOL][DiaTEMPCAM];
extern struct RDispPara DisDC[DiaDCCOL][DiaDCCAM];
extern uint32 HidCnt ;
extern uint32 HidCntOld ;
extern char FilerIndex[CHANNUM];
//�˲����Ŀ�� 0�ǹر�״̬
extern const int FilterVal[6] ;

extern uint8 InitRes[14];
#endif /* __WORK_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#ifndef __MODBUS_H
#define __MODBUS_H
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#define CHANNO 64  //��λ��Э��ͨ����
#define INPUT_IO_LENGTH 1044
#define OUTPUT_IO_LENGTH  1044
void ModInit(u8 Id);//����Modbus��ʼ��������IdΪվ�ţ�1-255��
void ModRcv(char Rdata);//����modbus��Ϣ����
void ModSend(void);//����modbus��Ϣ����
u8 MessageHandle(u8 *pointer_in,u8 *pointer_out);//�����յ�����Ϣ֡
void ReadOutputBit(u8 *pointer_1,u8 *pointer_2);//����Ȧ
void ReadInputBit(u8 *pointer_1,u8 *pointer_2);//������λ
void ReadHoldingReg(u8 *pointer_1,u8 *pointer_2);//�����ּĴ���	
//void ReadInputReg(u8 *pointer_1,u8 *pointer_2);//������Ĵ���
void ForceSingleCoil(u8 *pointer_1,u8 *pointer_2);//ǿ�Ƶ�����Ȧ
void PresetSingleReg(u8 *pointer_1,u8 *pointer_2);//Ԥ�Ƶ����Ĵ���
void ForceMulCoil(u8 *pointer_1,u8 *pointer_2);//ǿ�ƶ����Ȧ
void PresetMulReg(u8 *pointer_1,u8 *pointer_2);//Ԥ�ƶ���Ĵ���
void ErrorHandle(u8 Mode,u8 *Pointer);//������Ϣ֡����
u16 CRC16(u8 *puchMsgg,u8 usDataLen);//���ڼ���CRCУ����

extern u8 ModBusMod[10];
#define MODBUS_ENABLE  1                  //����MODBUS_ENABLE ����modbus
//#define LAN_SERIANET   1                  //��������͸������MODBUS_ENABLEֻ�ܶ���һ����ͬʱ���壬���ȿ���modbus 
#endif

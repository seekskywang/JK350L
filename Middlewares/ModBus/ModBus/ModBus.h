#ifndef __MODBUS_H
#define __MODBUS_H
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#define CHANNO 64  //上位机协议通道数
#define INPUT_IO_LENGTH 1044
#define OUTPUT_IO_LENGTH  1044
void ModInit(u8 Id);//用于Modbus初始化，参数Id为站号（1-255）
void ModRcv(char Rdata);//用于modbus信息接收
void ModSend(void);//用于modbus信息发送
u8 MessageHandle(u8 *pointer_in,u8 *pointer_out);//处理收到的信息帧
void ReadOutputBit(u8 *pointer_1,u8 *pointer_2);//读线圈
void ReadInputBit(u8 *pointer_1,u8 *pointer_2);//读输入位
void ReadHoldingReg(u8 *pointer_1,u8 *pointer_2);//读保持寄存器	
//void ReadInputReg(u8 *pointer_1,u8 *pointer_2);//读输入寄存器
void ForceSingleCoil(u8 *pointer_1,u8 *pointer_2);//强制单个线圈
void PresetSingleReg(u8 *pointer_1,u8 *pointer_2);//预制单个寄存器
void ForceMulCoil(u8 *pointer_1,u8 *pointer_2);//强制多个线圈
void PresetMulReg(u8 *pointer_1,u8 *pointer_2);//预制多个寄存器
void ErrorHandle(u8 Mode,u8 *Pointer);//错误信息帧处理
u16 CRC16(u8 *puchMsgg,u8 usDataLen);//用于计算CRC校验码

extern u8 ModBusMod[10];
#define MODBUS_ENABLE  1                  //定义MODBUS_ENABLE 开启modbus
//#define LAN_SERIANET   1                  //开启网络透传，与MODBUS_ENABLE只能定义一个，同时定义，优先开启modbus 
#endif

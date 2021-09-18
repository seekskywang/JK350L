#ifndef __COMMON8266_H
#define __COMMON8266_H



#include "stm32f7xx.h"
// 服务端参数定义
#define   CWMODE_3    "AT+CWMODE=2"
#define   CIPMUX      "AT+CIPMUX=1"  // 0是单路连接 1是多路连接
#define		CIPSTO			"AT+CIPSTO=2880"
#define		RESTORE 		"AT+RESTORE"
#define   RST         "AT+RST"
#define   CIFSR       "AT+CIFSR"
#define   CIPSTATUS   "AT+CIPSTATUS"



#define EN_USART8266_RX //使能串口7中断接收

#ifdef EN_USART8266_RX		  //如果使能了接收
extern struct uart8266  Uart8266;
void uart8266_init_data(void);
#endif
//////////////////串口8266/////////////////
#define UART8266_RBUFSIZE   500     //接收缓冲区大小
#define UART8266_SBUFSIZE   400     //发送缓冲区大小
#define  RBUFSIZE 100

#define    WIFIDATALEN           	 2048         //一次发送最大长度----最多设置为2048


#define MB_SER_PDU_SIZE_MAX     780     /*!< Maximum size of a Modbus RTU frame. */

#define UART8266F {USART8266_SendByte,UART8266_RecBufEmpty,UART8266_ReadRecBuf,UART8266_SendBuf}
void USART8266_SendByte(u8 dat);
void USART8266Write(u8* data);
char *                 itoa                                ( int value, char * string, int radix );
char * u16toa( uint16_t value, char *string, int radix );
struct uart8266
{
	u8 rBuf[UART8266_RBUFSIZE];  //接收缓冲区
	u16 rPtr1;	  //接收缓冲区队列首
	u16 rPtr2;	  //接收缓冲区队列尾
	u8 sBuf[UART8266_SBUFSIZE];  //发送缓冲区
	u16 sPtr1;	  //发送缓冲区队列首
	u16 sPtr2;	  //发送缓冲区队列尾
	u8 RX_STA;    //接收状态标记，1表示收到新数据。
	u8 TX_STA;    //发送状态标记，1表示没有数据发送。
};






void uart8266_init_data(void);
void UART8266_SendBuf(u8 sData);  //将数据存入UART8266发送缓冲区并启动发送。
u8 UART8266_ReadRecBuf(void);	  //读接收缓冲区一个字节
void UART8266_WriteRecBuf(u8 WriteData);   //写一个字节数据到接收缓冲区
u8 UART8266_ReadSendBuf(void);   //读发送缓冲区一个字节
void UART8266_WriteSendBuf(u8 WriteData);  //写一个字节数据到发送缓冲区
u8 UART8266_RecBufEmpty(void);  //接收缓冲区非空
u8 UART8266_SendBufEmpty(void);  //发送缓冲区非空
////////////////////////////////////////

/********************************** 函数声明 ***************************************/
void                     USART_printf                       ( USART_TypeDef * USARTx, char * Data, ... );
extern uartFunction wifi_uart;
extern uint8_t ucChaXun[30];
extern uint8_t IPflag ;
void ChaXunSW(void);
void WIFI_printf(char* fmt,...) ;
void Clear_wifi_data(void);
char * WIFI_SEND(char *p,int start,int len);
u8 send_wificmd(u16 waittime,char* cmd,...);     //waittime = 100为6秒左右


void send_dataTo8266(u8 num,u8* buf,int len);

void WIFI_task(void *p_arg);
//任务控制块
extern OS_TCB WIFITaskTCB;
//任务堆栈
extern CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];

void WIFICheck_task(void *p_arg);
void UpDataWIFI(void);
//任务控制块
extern OS_TCB WIFICheckTaskTCB;
//任务堆栈
extern CPU_STK WIFICheck_TASK_STK[WIFICheck_STK_SIZE];

extern volatile UCHAR  ucRTUBuf4000[MB_SER_PDU_SIZE_MAX];
extern const USHORT usSndBufferCount4000;
extern USHORT WifiDataFlashFlag;
#endif /* __COMMON8266_H */


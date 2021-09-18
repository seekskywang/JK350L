#ifndef __COMMON8266_H
#define __COMMON8266_H



#include "stm32f7xx.h"
// ����˲�������
#define   CWMODE_3    "AT+CWMODE=2"
#define   CIPMUX      "AT+CIPMUX=1"  // 0�ǵ�·���� 1�Ƕ�·����
#define		CIPSTO			"AT+CIPSTO=2880"
#define		RESTORE 		"AT+RESTORE"
#define   RST         "AT+RST"
#define   CIFSR       "AT+CIFSR"
#define   CIPSTATUS   "AT+CIPSTATUS"



#define EN_USART8266_RX //ʹ�ܴ���7�жϽ���

#ifdef EN_USART8266_RX		  //���ʹ���˽���
extern struct uart8266  Uart8266;
void uart8266_init_data(void);
#endif
//////////////////����8266/////////////////
#define UART8266_RBUFSIZE   500     //���ջ�������С
#define UART8266_SBUFSIZE   400     //���ͻ�������С
#define  RBUFSIZE 100

#define    WIFIDATALEN           	 2048         //һ�η�����󳤶�----�������Ϊ2048


#define MB_SER_PDU_SIZE_MAX     780     /*!< Maximum size of a Modbus RTU frame. */

#define UART8266F {USART8266_SendByte,UART8266_RecBufEmpty,UART8266_ReadRecBuf,UART8266_SendBuf}
void USART8266_SendByte(u8 dat);
void USART8266Write(u8* data);
char *                 itoa                                ( int value, char * string, int radix );
char * u16toa( uint16_t value, char *string, int radix );
struct uart8266
{
	u8 rBuf[UART8266_RBUFSIZE];  //���ջ�����
	u16 rPtr1;	  //���ջ�����������
	u16 rPtr2;	  //���ջ���������β
	u8 sBuf[UART8266_SBUFSIZE];  //���ͻ�����
	u16 sPtr1;	  //���ͻ�����������
	u16 sPtr2;	  //���ͻ���������β
	u8 RX_STA;    //����״̬��ǣ�1��ʾ�յ������ݡ�
	u8 TX_STA;    //����״̬��ǣ�1��ʾû�����ݷ��͡�
};






void uart8266_init_data(void);
void UART8266_SendBuf(u8 sData);  //�����ݴ���UART8266���ͻ��������������͡�
u8 UART8266_ReadRecBuf(void);	  //�����ջ�����һ���ֽ�
void UART8266_WriteRecBuf(u8 WriteData);   //дһ���ֽ����ݵ����ջ�����
u8 UART8266_ReadSendBuf(void);   //�����ͻ�����һ���ֽ�
void UART8266_WriteSendBuf(u8 WriteData);  //дһ���ֽ����ݵ����ͻ�����
u8 UART8266_RecBufEmpty(void);  //���ջ������ǿ�
u8 UART8266_SendBufEmpty(void);  //���ͻ������ǿ�
////////////////////////////////////////

/********************************** �������� ***************************************/
void                     USART_printf                       ( USART_TypeDef * USARTx, char * Data, ... );
extern uartFunction wifi_uart;
extern uint8_t ucChaXun[30];
extern uint8_t IPflag ;
void ChaXunSW(void);
void WIFI_printf(char* fmt,...) ;
void Clear_wifi_data(void);
char * WIFI_SEND(char *p,int start,int len);
u8 send_wificmd(u16 waittime,char* cmd,...);     //waittime = 100Ϊ6������


void send_dataTo8266(u8 num,u8* buf,int len);

void WIFI_task(void *p_arg);
//������ƿ�
extern OS_TCB WIFITaskTCB;
//�����ջ
extern CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];

void WIFICheck_task(void *p_arg);
void UpDataWIFI(void);
//������ƿ�
extern OS_TCB WIFICheckTaskTCB;
//�����ջ
extern CPU_STK WIFICheck_TASK_STK[WIFICheck_STK_SIZE];

extern volatile UCHAR  ucRTUBuf4000[MB_SER_PDU_SIZE_MAX];
extern const USHORT usSndBufferCount4000;
extern USHORT WifiDataFlashFlag;
#endif /* __COMMON8266_H */


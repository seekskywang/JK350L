#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f7xx.h"


/* ������������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶķ��������� */
#define BEEP_GPIO_PORT    	    GPIOC			              /* GPIO�˿� */
#define BEEP_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()  /* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN		  	GPIO_PIN_1			          /* ���ӵ���������GPIO */

/* �ߵ�ƽʱ���������� */
#define ON  1
#define OFF 0

/* ���κ꣬��������������һ��ʹ�� */
#define BEEP(a)	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,a)
					
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR = i;}			        //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRR = (uint32_t)i << 16;;}  //����͵�ƽ
#define digitalToggle(p,i)	    {p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define BEEP_TOGGLE		digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON	        digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF		digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
void Beep_task(void *p_arg);
extern OS_SEM  Beep_SEM;        //����һ���ź�������������ͬ��

//������ƿ�
extern OS_TCB BeepTaskTCB;
//�����ջ
extern CPU_STK Beep_TASK_STK[Beep_STK_SIZE];					
#endif /* __BEEP_H */

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "DataType.h"

//引脚定义
/*******************************************************/




#define P1_PIN                  GPIO_PIN_5                 
#define P1_GPIO_PORT            GPIOB                     
#define P1_GPIO_CLK            __GPIOB_CLK_ENABLE()

#define P2_PIN                  GPIO_PIN_7                 
#define P2_GPIO_PORT            GPIOB                      
#define P2_GPIO_CLK              __GPIOB_CLK_ENABLE()

#define P3_PIN                  GPIO_PIN_3                 
#define P3_GPIO_PORT            GPIOI                      
#define P3_GPIO_CLK              __GPIOI_CLK_ENABLE()

#define P4_PIN                  GPIO_PIN_13                 
#define P4_GPIO_PORT            GPIOD                      
#define P4_GPIO_CLK             __GPIOD_CLK_ENABLE()

#define P5_PIN                  GPIO_PIN_4                 
#define P5_GPIO_PORT            GPIOH                      
#define P5_GPIO_CLK             __GPIOH_CLK_ENABLE()

#define P6_PIN                  GPIO_PIN_4                 
#define P6_GPIO_PORT            GPIOB                      
#define P6_GPIO_CLK             __GPIOB_CLK_ENABLE()

#define P7_PIN                  GPIO_PIN_4                 
#define P7_GPIO_PORT            GPIOD                      
#define P7_GPIO_CLK             __GPIOD_CLK_ENABLE()

#define P8_PIN                  GPIO_PIN_13                 
#define P8_GPIO_PORT            GPIOG                      
#define P8_GPIO_CLK             __GPIOG_CLK_ENABLE()


#define P9_PIN                  GPIO_PIN_3                 
#define P9_GPIO_PORT            GPIOB                      
#define P9_GPIO_CLK              __GPIOB_CLK_ENABLE()


#define P10_PIN                  GPIO_PIN_14                 
#define P10_GPIO_PORT            GPIOG                      
#define P10_GPIO_CLK              __GPIOG_CLK_ENABLE()



#define P11_PIN                  GPIO_PIN_3                 
#define P11_GPIO_PORT            GPIOD                      
#define P11_GPIO_CLK             __GPIOD_CLK_ENABLE()

#define P12_PIN                  GPIO_PIN_15                 
#define P12_GPIO_PORT            GPIOA                      
#define P12_GPIO_CLK             __GPIOA_CLK_ENABLE()

#define P13_PIN                  GPIO_PIN_12                 
#define P13_GPIO_PORT            GPIOD                     
#define P13_GPIO_CLK             __GPIOD_CLK_ENABLE()

#define P14_PIN                  GPIO_PIN_1                 
#define P14_GPIO_PORT            GPIOB                      
#define P14_GPIO_CLK              __GPIOB_CLK_ENABLE()

#define P15_PIN                  GPIO_PIN_3                 
#define P15_GPIO_PORT            GPIOH                      
#define P15_GPIO_CLK              __GPIOH_CLK_ENABLE()

#define P16_PIN                  GPIO_PIN_2                 
#define P16_GPIO_PORT            GPIOD                      
#define P16_GPIO_CLK            __GPIOD_CLK_ENABLE()

#define P17_PIN                  GPIO_PIN_12                 
#define P17_GPIO_PORT            GPIOA                      
#define P17_GPIO_CLK             __GPIOA_CLK_ENABLE()

#define P18_PIN                  GPIO_PIN_11                 
#define P18_GPIO_PORT            GPIOD                      
#define P18_GPIO_CLK            __GPIOD_CLK_ENABLE()

#define P19_PIN                  GPIO_PIN_0                 
#define P19_GPIO_PORT            GPIOB                      
#define P19_GPIO_CLK             __GPIOB_CLK_ENABLE()

#define P20_PIN                  GPIO_PIN_2                 
#define P20_GPIO_PORT            GPIOH                      
#define P20_GPIO_CLK             __GPIOH_CLK_ENABLE()

#define P21_PIN                  GPIO_PIN_12                 
#define P21_GPIO_PORT            GPIOC                      
#define P21_GPIO_CLK             __GPIOC_CLK_ENABLE()

#define P22_PIN                  GPIO_PIN_11                 
#define P22_GPIO_PORT            GPIOA                      
#define P22_GPIO_CLK              __GPIOA_CLK_ENABLE()

#define P23_PIN                  GPIO_PIN_2                 
#define P23_GPIO_PORT            GPIOB                      
#define P23_GPIO_CLK              __GPIOB_CLK_ENABLE()

#define P24_PIN                  GPIO_PIN_5                 
#define P24_GPIO_PORT            GPIOH                      
#define P24_GPIO_CLK             __GPIOH_CLK_ENABLE()
/*******************************************************/




#define RP_PIN(Port,Pin)     (HAL_GPIO_ReadPin(Port,Pin))   







/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0
/*******************************************************/

 /** 键值
	* 
	*/
/*	
#define KEY_FUNC1	0x15         1
#define KEY_FUNC2	0x0B         2
#define KEY_FUNC3	0x10         3
#define KEY_FUNC4	0x06         4
#define KEY_FUNC5	0x05		5

#define KEY_UP		0x0F 	6
#define KEY_DOWN	0x07	7
#define KEY_LEFT	0x17	8
#define KEY_RIGHT	0x02	9
#define KEY_ACC		0x12	10

#define KEY_1		0x16    11
#define KEY_2		0x0C	12
#define KEY_3		0x11	13
#define KEY_BACK	0x0A	14
#define KEY_ESC		0x01	15


#define KEY_4		0x18	16
#define KEY_5		0x0D	17
#define KEY_6		0x14	18
#define KEY_0		0x08	19
#define KEY_ENTER	0x03	20


#define KEY_7		0x19	21
#define KEY_8		0x0E	22
#define KEY_9		0x13	23
#define KEY_DOT		0x09	24
*/
#define KEY_NULL	0xFF  //空键
#define KEY_1		0x16
#define KEY_2		0x0C
#define KEY_3		0x11
#define KEY_4		0x18
#define KEY_5		0x0D
#define KEY_6		0x14
#define KEY_7		0x19
#define KEY_8		0x0E
#define KEY_9		0x13
#define KEY_0		0x08
#define KEY_DOT		0x09
#define KEY_BACK	0x0A
#define KEY_FUNC1	0x15
#define KEY_FUNC2	0x0B
#define KEY_FUNC3	0x10
#define KEY_FUNC4	0x06
#define KEY_FUNC5	0x05
#define KEY_UP		0x0F
#define KEY_DOWN	0x07
#define KEY_LEFT	0x17
#define KEY_RIGHT	0x02
#define KEY_ESC		0x01
#define KEY_ENTER	0x03
#define KEY_ACC		0x12
#define KEY_MANU	0x1A

#define KEY_HIDE1		0x20
#define KEY_HIDE2		0x21
#define ALL_ReTurn    1
#define NoNull_ReTurn    0
extern uint8 KEY_ReTurn;
//任务控制块
extern OS_TCB KEY0TaskTCB;
//任务堆栈
extern CPU_STK KEY0_TASK_STK[KEY0_STK_SIZE];
//key0任务
void KEY0_task(void *p_arg);
void KeyPin_GPIO_Config_Full(GPIO_TypeDef  *GPIOx,uint32_t Pin,uint32_t Mode,uint32_t Pull,uint32_t Speed);

extern OS_SEM	Key_SEM;		//定义一个信号量，用于任务同步
extern uint8 KEY_value;
extern uint32 KEY_cont0;
void Key_GPIO_Config(void);

#endif


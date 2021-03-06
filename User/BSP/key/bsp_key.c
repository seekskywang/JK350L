/*
  * @file    Bsp_key.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   键盘的驱动和键盘扫描任务业务
 */
 
#include "include.h"

OS_SEM  Key_SEM;        //定义一个信号量，用于任务同步
extern u32 SaveStamp;
uint8 KEY_ReTurn = 0;
uint8 KEY_value = 0xFF;
uint32 KEY_cont0 = 0;
//任务控制块
OS_TCB KEY0TaskTCB;
//任务堆栈
CPU_STK KEY0_TASK_STK[KEY0_STK_SIZE];
/// 不精确的延时
void Key_Delay(__IO u32 nCount)
{
    for(; nCount != 0; nCount--);
}
void KeyPin_GPIO_Config(GPIO_TypeDef  *GPIOx,uint32_t Pin)
{

    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=Pin; //
    HAL_GPIO_Init(GPIOx,&GPIO_Initure);
}


void KeyPin_GPIO_Config_Full(GPIO_TypeDef  *GPIOx,uint32_t Pin,uint32_t Mode,uint32_t Pull,uint32_t Speed)
{

    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin=Pin; //
    GPIO_Initure.Mode=Mode;  //推挽输出
    GPIO_Initure.Pull=Pull;          //上拉
    GPIO_Initure.Speed=Speed;     //高速
    HAL_GPIO_Init(GPIOx,&GPIO_Initure);
}

void Key_GPIO_Config(void)
{

    P1_GPIO_CLK;
    P2_GPIO_CLK;
    P3_GPIO_CLK;
    P4_GPIO_CLK;
    P5_GPIO_CLK;
    P6_GPIO_CLK;
    P7_GPIO_CLK;
    P8_GPIO_CLK;
    P9_GPIO_CLK;
    P10_GPIO_CLK;

    P11_GPIO_CLK;
    P12_GPIO_CLK;
    P13_GPIO_CLK;
    P14_GPIO_CLK;
    P15_GPIO_CLK;
    P16_GPIO_CLK;
    P17_GPIO_CLK;
    P18_GPIO_CLK;
    P19_GPIO_CLK;
    P20_GPIO_CLK;

    P21_GPIO_CLK;
    P22_GPIO_CLK;
    P23_GPIO_CLK;
    P24_GPIO_CLK;

    //推挽输入
//上拉
//高速
    KeyPin_GPIO_Config_Full(P1_GPIO_PORT,P1_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P2_GPIO_PORT,P2_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P3_GPIO_PORT,P3_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P4_GPIO_PORT,P4_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P5_GPIO_PORT,P5_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P6_GPIO_PORT,P6_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P7_GPIO_PORT,P7_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P8_GPIO_PORT,P8_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P9_GPIO_PORT,P9_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P10_GPIO_PORT,P10_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);

    KeyPin_GPIO_Config_Full(P11_GPIO_PORT,P11_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P12_GPIO_PORT,P12_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P13_GPIO_PORT,P13_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P14_GPIO_PORT,P14_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P15_GPIO_PORT,P15_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P16_GPIO_PORT,P16_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P17_GPIO_PORT,P17_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P18_GPIO_PORT,P18_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P19_GPIO_PORT,P19_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P20_GPIO_PORT,P20_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);

    KeyPin_GPIO_Config_Full(P21_GPIO_PORT,P21_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P22_GPIO_PORT,P22_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P23_GPIO_PORT,P23_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);
    KeyPin_GPIO_Config_Full(P24_GPIO_PORT,P24_PIN,GPIO_MODE_INPUT,GPIO_PULLUP ,GPIO_SPEED_FAST);

}

//扫描键值
uint8 Key_Scan(void)
{
    uint8 key_value;
    key_value = 0XFF;
    if(RP_PIN(P14_GPIO_PORT,P14_PIN)==0&&RP_PIN(P11_GPIO_PORT,P11_PIN)==0)
		key_value =KEY_HIDE1;
    else if(RP_PIN(P14_GPIO_PORT,P14_PIN)==0&&RP_PIN(P12_GPIO_PORT,P12_PIN)==0)
		key_value =KEY_HIDE2;
    else if(RP_PIN(P1_GPIO_PORT,P1_PIN)==0)
        key_value =KEY_FUNC1;
    else if(RP_PIN(P2_GPIO_PORT,P2_PIN)==0)
        key_value =KEY_FUNC2;
    else if(RP_PIN(P3_GPIO_PORT,P3_PIN)==0)
        key_value =KEY_FUNC3;
    else if(RP_PIN(P4_GPIO_PORT,P4_PIN)==0)
        key_value =KEY_FUNC4;
    else if(RP_PIN(P5_GPIO_PORT,P5_PIN)==0)
        key_value =KEY_FUNC5;
    else if(RP_PIN(P6_GPIO_PORT,P6_PIN)==0)
        key_value =KEY_UP;
    else if(RP_PIN(P7_GPIO_PORT,P7_PIN)==0)
        key_value =KEY_DOWN;
    else if(RP_PIN(P8_GPIO_PORT,P8_PIN)==0)
        key_value =KEY_LEFT;
    else if(RP_PIN(P9_GPIO_PORT,P9_PIN)==0)
        key_value =KEY_RIGHT;
    else if(RP_PIN(P10_GPIO_PORT,P10_PIN)==0)
        key_value =KEY_ENTER;
    else if(RP_PIN(P11_GPIO_PORT,P11_PIN)==0)
        key_value =KEY_1;
    else if(RP_PIN(P12_GPIO_PORT,P12_PIN)==0)
        key_value =KEY_2;
    else if(RP_PIN(P13_GPIO_PORT,P13_PIN)==0)
        key_value =KEY_3;
    else if(RP_PIN(P14_GPIO_PORT,P14_PIN)==0)
        key_value =KEY_BACK;
    else if(RP_PIN(P15_GPIO_PORT,P15_PIN)==0)
        key_value =KEY_ESC;
    else if(RP_PIN(P16_GPIO_PORT,P16_PIN)==0)
        key_value =KEY_4;
    else if(RP_PIN(P17_GPIO_PORT,P17_PIN)==0)
        key_value =KEY_5;
    else if(RP_PIN(P18_GPIO_PORT,P18_PIN)==0)
        key_value =KEY_6;
    else if(RP_PIN(P19_GPIO_PORT,P19_PIN)==0)
        key_value =KEY_0;
    else if(RP_PIN(P20_GPIO_PORT,P20_PIN)==0)
        key_value =KEY_MANU;
    else if(RP_PIN(P21_GPIO_PORT,P21_PIN)==0)
        key_value =KEY_7;
    else if(RP_PIN(P22_GPIO_PORT,P22_PIN)==0)
        key_value =KEY_8;
    else if(RP_PIN(P23_GPIO_PORT,P23_PIN)==0)
        key_value =KEY_9;
    else if(RP_PIN(P24_GPIO_PORT,P24_PIN)==0)
        key_value =KEY_DOT;
    return key_value;
}



//KEY0任务
void KEY0_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytemp;
    static uint32 cont0Key=0;
    uint8 value;
    //创建一个信号量
    OSSemCreate ((OS_SEM*   )&Key_SEM,
                 (CPU_CHAR* )"Key_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*   )&err);
    while(1)
    {
        value = Key_Scan();
        if(value != KEY_NULL)
        {
            if(cont0Key==0&&DataSave.Data_type.BeepF==1)//第一次按下使蜂鸣器
                OSSemPost(&Beep_SEM,OS_OPT_POST_1,&err);//发送信号量
            cont0Key++;//已经按下
            keytemp = value;
            KEY_value=KEY_NULL;
            if( KEY_ReTurn==ALL_ReTurn)
                OSSemPost(&Key_SEM,OS_OPT_POST_1,&err);//发送信号量
        }
        else
        {
            if(cont0Key!=0)  //按下后松开
            {
                KEY_cont0=cont0Key; //长按统计
                KEY_value=keytemp;
                keytemp=KEY_NULL;
                OSSemPost(&Key_SEM,OS_OPT_POST_1,&err);//发送信号量
                SaveStamp =SetSaveStamp();
            }
            else  // 没有按按键
            {
                KEY_value=KEY_NULL;
                if( KEY_ReTurn==ALL_ReTurn)
                    OSSemPost(&Key_SEM,OS_OPT_POST_1,&err);//发送信号量
            }
            cont0Key=0;
        }
        OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_PERIODIC,&err);//延时20ms
    }
}









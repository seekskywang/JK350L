/*
  ******************************************************************************
  * @file    bsp.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   系统初始化相关
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F767 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/


/*
*********************************************************************************************************
*                                             包含的文件
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  "os.h"
#include  "bsp.h"
#include "./sdram/bsp_sdram.h"
#include "bsp_lcd.h"
#include  "stm32f7xx_hal.h"
#include  "include.h"
#include "main.h"
#include "usb_device.h"

void Delay(__IO uint32_t nCount)     //简单的延时函数
{
    for(; nCount != 0; nCount--);
}
/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
    /*演示显示变量*/
    static uint8_t testCNT = 0;
    char dispBuff[100];


    /*选择字体*/
    LCD_SetFont(&LCD_DEFAULT_FONT);
    LCD_DisplayStringLineEx(600,5,16,16,(uint8_t* )"F42916*16",0);
    LCD_SelectLayer(0);

    /* 清屏，显示全黑 */
    LCD_Clear(LCD_COLOR_BLACK);
    /*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/

    /*使用c标准库把变量转化成字符串*/
    sprintf(dispBuff,"显示变量例子: testCount = %d ",testCNT);
    LCD_ClearLine(7);
    /*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    /*然后显示该字符串即可，其它变量也是这样处理*/
    LCD_DisplayStringLine_EN_CH(7,(uint8_t* )dispBuff);


    /* 画直线 */
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"画线:");
    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_DrawLine(50,250,750,250);
    LCD_DrawLine(50,300,750,300);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_DrawLine(300,250,400,400);
    LCD_DrawLine(600,250,600,400);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


    /*画矩形*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"画矩形:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_DrawRect(200,250,200,100);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_DrawRect(350,250,200,50);

    LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    LCD_DrawRect(200,350,50,200);

    Delay(0xFFFFFF);


    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


    /*填充矩形*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"填充矩形:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_FillRect(200,250,200,100);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_FillRect(350,250,200,50);

    LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    LCD_FillRect(200,350,50,200);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

    /* 画圆 */
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"画圆:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
    LCD_DrawCircle(200,350,50);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
    LCD_DrawCircle(350,350,75);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


    /*填充圆*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"填充圆:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_FillCircle(300,350,50);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_FillCircle(450,350,75);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

    LCD_ClearLine(8);

    /*透明效果 前景层操作*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"双层透明效果：");

    /*设置前景层不透明度*/
    LCD_SetTransparency(1, 128);

    /* 选择LCD第一层 */
    LCD_SelectLayer(1);

    /* 清屏，显示全黑 */
    LCD_Clear(LCD_COLOR_BLACK);
    /*在前景画一个红色圆*/
    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
    LCD_FillCircle(400,350,75);

    Delay(0xFFFFFF);

    /*透明效果 背景层操作*/

    /* 选择LCD背景层 */
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    /*设置背景层不透明*/
    LCD_SetTransparency(1, 0);


    /*在背景画一个绿色圆*/
    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_FillCircle(450,350,75);

    /*在背景画一个蓝色圆*/
    LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    LCD_FillCircle(350,350,75);

    Delay(0xFFFFFF);
    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


}


void SDRAM_LCD_Init(void)
{
    SDRAM_GPIO_Config();
    LCD_GPIO_Config();
    SDRAM_Init();
    LCD_Init();
}




/*
*********************************************************************************************************
* 函数名 : BSP_Init
* 描述   : 所有的硬件设备都应该放在这个函数里边初始化
* 形参   : 无
* 返回值 : 无
*********************************************************************************************************
*/
void  BSP_Init (void)
{

    BSP_OSTickInit();                  //初始化 OS 时钟源
    SDRAM_LCD_Init();
    /* 使能LCD，包括开背光 */
    LCD_LayerInit(0, LCD_FB1_ADDRESS, RGB565);
    LCD_DisplayOn();
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);
    /* 选择LCD第一层 */
    LCD_SelectLayer(0);

    /* 第一层清屏，显示全黑 */
    LCD_Clear(LCD_COLOR_BLUE);


    /* 第一层清屏，显示全黑 */
    LCD_Clear(LCD_COLOR_BLACK);
    /* 选择LCD第二层 */
    LCD_SelectLayer(1);




    LCD_Clear(LCD_COLOR_TRANSPARENT);
    /* 配置第一和第二层的透明度,最小值为0，最大值为255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);

    /* 选择LCD第一层 */
    LCD_SelectLayer(0);

    /* 清屏，显示全黑 */
    LCD_Clear(LCD_COLOR_BLACK);
    /*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    /*选择字体*/
    LCD_SetFont(&LCD_DEFAULT_FONT);
   
    CH376_GPIO_Init();
	memset(InitRes,0,sizeof(InitRes));
    power_on();
	Delay(0xffffff);
    Delay(0xffffff);
  //  LCD_Clear(LCD_COLOR_BLACK);
    InitRes[0] = rtc_init();
    /*------------------------------ SD 初始化 ------------------------------ */

    Key_GPIO_Config();//按键初始化
    //RS232_USART_Config(); //前端232初始化
    /*蜂鸣器端口初始化 */
    BEEP_GPIO_Config();

    GPIO_ResetBits(CH376_RST_PORT,CH376_RST_PIN);
    QSPI_FLASH_Init();
    ESP8266_Init();
    ModInit(1);
    MX_GPIO_Init();
    InitRes[4] = MX_USB_DEVICE_Init();
    InitRes[5] = FileInit(); //文件系统初始化
//	Delay(0xffffff);
//    Delay(0xffffff);
    RS232_USART_Config(); //前端232初始化
    BAT_USART_Config(); //电池232
    if(USB_INT_SUCCESS !=Init_CH376())
		InitRes[8] = 0xff;
    TIMx_Configuration7();
    TIMx_ConfigurationAD();
	
	

}









/*
*********************************************************************************************************
* 函数名 : BSP_SystemClkCfg
* 描述   : 系统时钟初始化
* 形参   : 无
* 返回值 : 无
*********************************************************************************************************
*/

void  BSP_SystemClkCfg (void)
{
    RCC_OscInitTypeDef  RCC_OscInit;
    RCC_ClkInitTypeDef  RCC_ClkInit;
    HAL_StatusTypeDef   hal_status;

    RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInit.HSEState       = RCC_HSE_ON;
    RCC_OscInit.HSIState       = RCC_HSI_OFF;
    RCC_OscInit.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInit.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInit.PLL.PLLM       = 25u;                              //外部晶振频率 HSE freq     = 25MHz
    RCC_OscInit.PLL.PLLN       = 432u;                             //倍频时钟频率 VCO out-freq = HSE * (PLLN / PLLM)     = 432MHz
    RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;                    //系统时钟频率 PLLCLK       = (VCO out-freq) / PLLP   = 216MHz
    RCC_OscInit.PLL.PLLQ       = 9;                                //外设时钟频率 PLL_Q out freq = (VCO out-freq) / PLLQ = 48MHz

    hal_status = HAL_RCC_OscConfig(&RCC_OscInit);
    if (hal_status != HAL_OK)
    {
        while (DEF_TRUE)                                           //如果出错则停止
        {
            ;
        }
    }

    hal_status = HAL_PWREx_EnableOverDrive();                      //等待时钟频率稳定为 216 Mhz
    if (hal_status != HAL_OK)
    {
        while (DEF_TRUE)                                           //如果出错则停止
        {
            ;
        }
    }

    RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_HCLK   |
                                 RCC_CLOCKTYPE_PCLK1  |
                                 RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;                   //AHB时钟频率  HCLK= AHBCLK    = PLLCLK / AHBPRES(1) = 216MHz
    RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;                     //APB1外设总线时钟频率 APB1CLK = AHBCLK  / APB1DIV(4)=  54MHz
    RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;                     //APB2外设总线时钟频率 APB2CLK = AHBCLK  / APB2DIV(2)= 108MHz

    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_7);
    if (hal_status != HAL_OK)
    {
        while (DEF_TRUE)                                            //如果出错则停止
        {
            ;
        }
    }
}


/*
*********************************************************************************************************
* 函数名 : BSP_ClkFreqGet
* 描述   : 这个函数用来检索系统时钟频率
* 形参   : clk_id    系统时钟标识符
*                    BSP_CLK_ID_SYSCLK     系统时钟频率。
*                    BSP_CLK_ID_HCLK       CPU时钟频率。
*                    BSP_CLK_ID_PCLK1      APB1总线时钟频率。
*                    BSP_CLK_ID_PCLK2      APB2总线时钟频率。
* 返回值 : 无
*********************************************************************************************************
*/
CPU_INT32U  BSP_ClkFreqGet (BSP_CLK_ID  clk_id)
{
    CPU_INT32U  clk_freq;


    switch (clk_id)
    {
        case BSP_CLK_ID_SYSCLK:
            clk_freq = HAL_RCC_GetSysClockFreq();
            break;


        case BSP_CLK_ID_HCLK:
            clk_freq = HAL_RCC_GetHCLKFreq();
            break;


        case BSP_CLK_ID_PCLK1:
            clk_freq = HAL_RCC_GetPCLK1Freq();
            break;


        case BSP_CLK_ID_PCLK2:
            clk_freq = HAL_RCC_GetPCLK2Freq();
            break;


        default:
            clk_freq = 1u;                                     //没有有效时钟频率
            break;
    }

    return (clk_freq);
}
/*
*********************************************************************************************************
* 函数名 : BSP_OSTickInit
* 描述   : 初始化 OS 嘀嗒时钟中断
* 形参   : 无
* 返回值 : 无
*********************************************************************************************************
*/
void  BSP_OSTickInit (void)
{
    CPU_INT32U  cpu_clk_freq;


    cpu_clk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);             //获取CPU时钟，时间戳是以该时钟计数

    OS_CPU_SysTickInitFreq(cpu_clk_freq);                       //初始化uC/OS 周期时钟源 (SysTick)
}

/*
*********************************************************************************************************
* 函数名 : HAL_InitTick
* 描述   : 覆盖STM32F7xx HAL 库中的HAL_InitTick函数，因为Micrium实时系统有自己的Systick 初始化，
*          必须在多任务启动后才初始化滴答时钟
* 形参   : TickPriority     滴答中断优先级
* 返回值 : 无
*********************************************************************************************************
*/
HAL_StatusTypeDef  HAL_InitTick (uint32_t  TickPriority)
{
    HAL_NVIC_SetPriorityGrouping(0);

    return (HAL_OK);
}

/*
*********************************************************************************************************
* 函数名 : HAL_GetTick
* 描述   : 覆盖STM32F7xx HAL HAL_GetTick函数，因为Micrium实时系统有自己的滴答计时器的值
* 形参   : 无
* 返回值 : 滴答计时器的值
* 注意   ：请确保滴答时钟任务比应用程序启动任务具有更高的优先级
*********************************************************************************************************
*/
uint32_t  HAL_GetTick(void)
{
    CPU_INT32U  os_tick_ctr;
#if (OS_VERSION >= 30000u)
    OS_ERR      os_err;


    os_tick_ctr = OSTimeGet(&os_err);
#else
    os_tick_ctr = OSTimeGet();
#endif

    return os_tick_ctr;
}

/*
  ******************************************************************************
  * @file    bsp.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ϵͳ��ʼ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F767 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/


/*
*********************************************************************************************************
*                                             �������ļ�
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

void Delay(__IO uint32_t nCount)     //�򵥵���ʱ����
{
    for(; nCount != 0; nCount--);
}
/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
    /*��ʾ��ʾ����*/
    static uint8_t testCNT = 0;
    char dispBuff[100];


    /*ѡ������*/
    LCD_SetFont(&LCD_DEFAULT_FONT);
    LCD_DisplayStringLineEx(600,5,16,16,(uint8_t* )"F42916*16",0);
    LCD_SelectLayer(0);

    /* ��������ʾȫ�� */
    LCD_Clear(LCD_COLOR_BLACK);
    /*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/

    /*ʹ��c��׼��ѱ���ת�����ַ���*/
    sprintf(dispBuff,"��ʾ��������: testCount = %d ",testCNT);
    LCD_ClearLine(7);
    /*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    /*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
    LCD_DisplayStringLine_EN_CH(7,(uint8_t* )dispBuff);


    /* ��ֱ�� */
    LCD_SetTextColor(LCD_COLOR_BLUE);

    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"����:");
    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_DrawLine(50,250,750,250);
    LCD_DrawLine(50,300,750,300);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_DrawLine(300,250,400,400);
    LCD_DrawLine(600,250,600,400);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


    /*������*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"������:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_DrawRect(200,250,200,100);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_DrawRect(350,250,200,50);

    LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    LCD_DrawRect(200,350,50,200);

    Delay(0xFFFFFF);


    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


    /*������*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"������:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_FillRect(200,250,200,100);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_FillRect(350,250,200,50);

    LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    LCD_FillRect(200,350,50,200);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

    /* ��Բ */
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"��Բ:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
    LCD_DrawCircle(200,350,50);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
    LCD_DrawCircle(350,350,75);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


    /*���Բ*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"���Բ:");

    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
    LCD_FillCircle(300,350,50);

    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_FillCircle(450,350,75);

    Delay(0xFFFFFF);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

    LCD_ClearLine(8);

    /*͸��Ч�� ǰ�������*/
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_ClearLine(8);
    LCD_DisplayStringLine_EN_CH(8,(uint8_t* )"˫��͸��Ч����");

    /*����ǰ���㲻͸����*/
    LCD_SetTransparency(1, 128);

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(1);

    /* ��������ʾȫ�� */
    LCD_Clear(LCD_COLOR_BLACK);
    /*��ǰ����һ����ɫԲ*/
    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
    LCD_FillCircle(400,350,75);

    Delay(0xFFFFFF);

    /*͸��Ч�� ���������*/

    /* ѡ��LCD������ */
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    /*���ñ����㲻͸��*/
    LCD_SetTransparency(1, 0);


    /*�ڱ�����һ����ɫԲ*/
    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    LCD_FillCircle(450,350,75);

    /*�ڱ�����һ����ɫԲ*/
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
* ������ : BSP_Init
* ����   : ���е�Ӳ���豸��Ӧ�÷������������߳�ʼ��
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/
void  BSP_Init (void)
{

    BSP_OSTickInit();                  //��ʼ�� OS ʱ��Դ
    SDRAM_LCD_Init();
    /* ʹ��LCD������������ */
    LCD_LayerInit(0, LCD_FB1_ADDRESS, RGB565);
    LCD_DisplayOn();
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);
    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);

    /* ��һ����������ʾȫ�� */
    LCD_Clear(LCD_COLOR_BLUE);


    /* ��һ����������ʾȫ�� */
    LCD_Clear(LCD_COLOR_BLACK);
    /* ѡ��LCD�ڶ��� */
    LCD_SelectLayer(1);




    LCD_Clear(LCD_COLOR_TRANSPARENT);
    /* ���õ�һ�͵ڶ����͸����,��СֵΪ0�����ֵΪ255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);

    /* ��������ʾȫ�� */
    LCD_Clear(LCD_COLOR_BLACK);
    /*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    /*ѡ������*/
    LCD_SetFont(&LCD_DEFAULT_FONT);
   
    CH376_GPIO_Init();
	memset(InitRes,0,sizeof(InitRes));
    power_on();
	Delay(0xffffff);
    Delay(0xffffff);
  //  LCD_Clear(LCD_COLOR_BLACK);
    InitRes[0] = rtc_init();
    /*------------------------------ SD ��ʼ�� ------------------------------ */

    Key_GPIO_Config();//������ʼ��
    //RS232_USART_Config(); //ǰ��232��ʼ��
    /*�������˿ڳ�ʼ�� */
    BEEP_GPIO_Config();

    GPIO_ResetBits(CH376_RST_PORT,CH376_RST_PIN);
    QSPI_FLASH_Init();
    ESP8266_Init();
    ModInit(1);
    MX_GPIO_Init();
    InitRes[4] = MX_USB_DEVICE_Init();
    InitRes[5] = FileInit(); //�ļ�ϵͳ��ʼ��
//	Delay(0xffffff);
//    Delay(0xffffff);
    RS232_USART_Config(); //ǰ��232��ʼ��
    BAT_USART_Config(); //���232
    if(USB_INT_SUCCESS !=Init_CH376())
		InitRes[8] = 0xff;
    TIMx_Configuration7();
    TIMx_ConfigurationAD();
	
	

}









/*
*********************************************************************************************************
* ������ : BSP_SystemClkCfg
* ����   : ϵͳʱ�ӳ�ʼ��
* �β�   : ��
* ����ֵ : ��
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
    RCC_OscInit.PLL.PLLM       = 25u;                              //�ⲿ����Ƶ�� HSE freq     = 25MHz
    RCC_OscInit.PLL.PLLN       = 432u;                             //��Ƶʱ��Ƶ�� VCO out-freq = HSE * (PLLN / PLLM)     = 432MHz
    RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;                    //ϵͳʱ��Ƶ�� PLLCLK       = (VCO out-freq) / PLLP   = 216MHz
    RCC_OscInit.PLL.PLLQ       = 9;                                //����ʱ��Ƶ�� PLL_Q out freq = (VCO out-freq) / PLLQ = 48MHz

    hal_status = HAL_RCC_OscConfig(&RCC_OscInit);
    if (hal_status != HAL_OK)
    {
        while (DEF_TRUE)                                           //���������ֹͣ
        {
            ;
        }
    }

    hal_status = HAL_PWREx_EnableOverDrive();                      //�ȴ�ʱ��Ƶ���ȶ�Ϊ 216 Mhz
    if (hal_status != HAL_OK)
    {
        while (DEF_TRUE)                                           //���������ֹͣ
        {
            ;
        }
    }

    RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_HCLK   |
                                 RCC_CLOCKTYPE_PCLK1  |
                                 RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;                   //AHBʱ��Ƶ��  HCLK= AHBCLK    = PLLCLK / AHBPRES(1) = 216MHz
    RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;                     //APB1��������ʱ��Ƶ�� APB1CLK = AHBCLK  / APB1DIV(4)=  54MHz
    RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;                     //APB2��������ʱ��Ƶ�� APB2CLK = AHBCLK  / APB2DIV(2)= 108MHz

    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_7);
    if (hal_status != HAL_OK)
    {
        while (DEF_TRUE)                                            //���������ֹͣ
        {
            ;
        }
    }
}


/*
*********************************************************************************************************
* ������ : BSP_ClkFreqGet
* ����   : ���������������ϵͳʱ��Ƶ��
* �β�   : clk_id    ϵͳʱ�ӱ�ʶ��
*                    BSP_CLK_ID_SYSCLK     ϵͳʱ��Ƶ�ʡ�
*                    BSP_CLK_ID_HCLK       CPUʱ��Ƶ�ʡ�
*                    BSP_CLK_ID_PCLK1      APB1����ʱ��Ƶ�ʡ�
*                    BSP_CLK_ID_PCLK2      APB2����ʱ��Ƶ�ʡ�
* ����ֵ : ��
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
            clk_freq = 1u;                                     //û����Чʱ��Ƶ��
            break;
    }

    return (clk_freq);
}
/*
*********************************************************************************************************
* ������ : BSP_OSTickInit
* ����   : ��ʼ�� OS ���ʱ���ж�
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/
void  BSP_OSTickInit (void)
{
    CPU_INT32U  cpu_clk_freq;


    cpu_clk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);             //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���

    OS_CPU_SysTickInitFreq(cpu_clk_freq);                       //��ʼ��uC/OS ����ʱ��Դ (SysTick)
}

/*
*********************************************************************************************************
* ������ : HAL_InitTick
* ����   : ����STM32F7xx HAL ���е�HAL_InitTick��������ΪMicriumʵʱϵͳ���Լ���Systick ��ʼ����
*          �����ڶ�����������ų�ʼ���δ�ʱ��
* �β�   : TickPriority     �δ��ж����ȼ�
* ����ֵ : ��
*********************************************************************************************************
*/
HAL_StatusTypeDef  HAL_InitTick (uint32_t  TickPriority)
{
    HAL_NVIC_SetPriorityGrouping(0);

    return (HAL_OK);
}

/*
*********************************************************************************************************
* ������ : HAL_GetTick
* ����   : ����STM32F7xx HAL HAL_GetTick��������ΪMicriumʵʱϵͳ���Լ��ĵδ��ʱ����ֵ
* �β�   : ��
* ����ֵ : �δ��ʱ����ֵ
* ע��   ����ȷ���δ�ʱ�������Ӧ�ó�������������и��ߵ����ȼ�
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

/*
  * @file    CHUSB.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-8-2020
  * @brief   U盘操作
 */
#include "include.h"

SPI_HandleTypeDef hspi1;

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;

//static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void xWriteCH376Data(u8 mData);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
u8 xReadCH376Data(void);
static void Delay(__IO uint32_t nCount)  //简单的延时函数
{
    for(; nCount != 0; nCount--);
}
/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hspi->Instance==SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }

}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if(hspi->Instance==SPI1)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }

}
void CH376_GPIO_Init(void)
{
    CH376_RST_CLK;
    KeyPin_GPIO_Config_Full(CH376_RST_PORT,CH376_RST_PIN,GPIO_MODE_OUTPUT_PP,GPIO_PULLDOWN,GPIO_SPEED_HIGH);
    GPIO_SetBits(CH376_RST_PORT,CH376_RST_PIN);
    CH376_INT_CLK;
    KeyPin_GPIO_Config_Full(CH376_INT_PORT,CH376_INT_PIN,GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_HIGH);


    CH376_SCS_CLK;
    KeyPin_GPIO_Config_Full(CH376_SCS_PORT,CH376_SCS_PIN,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_HIGH);

    MX_GPIO_Init();
    MX_SPI1_Init();


}
//void Error_Handler(void)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */

//  /* USER CODE END Error_Handler_Debug */
//}
static void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        // Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */

}





/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

}









/*******************************************************************************
* 描      述      : 延时指定微秒时间,根据单片机主频调整,不精确.
*******************************************************************************/
void mDelayuS( u8 us )
{
    while ( us -- );
}

/*******************************************************************************
* 描      述      : 延时指定毫秒时间,根据单片机主频调整,不精确
*******************************************************************************/
void mDelaymS( u8 ms )
{
    while ( ms -- )
    {
        mDelayuS( 250 );
        mDelayuS( 250 );
        mDelayuS( 250 );
        mDelayuS( 250 );
    }
}

/*******************************************************************************
* 描      述      : 向CH376写  命令.
* 输      入      : UINT8 mCmd:要发送的命令.
*******************************************************************************/
void xWriteCH376Cmd(u8 mCmd)
{
	char i;
//  SPI_CH376_SCS_HIGH();    /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
    GPIO_SetBits(CH376_SCS_PORT,CH376_SCS_PIN);
    Delay(10);
    Delay(10);
    /* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出
    *  方向,SPI_SDO为输入方向 */
    GPIO_ResetBits(CH376_SCS_PORT,CH376_SCS_PIN);/* SPI片选有效 */
//  SPI_CH376_SCS_LOW();      /* SPI片选有效 */
    SPIx_ReadWriteByte(mCmd);  /* 发出命令码 */
    Delay(10);
    Delay(10);
    Delay(10);   /* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
    Delay(10);
	
//	for ( i = 40; i != 0; -- i ) 
//	{ /* 状态查询,等待CH376不忙,或者上面一行的延时1.5uS代替 */
//		if ( ( xReadCH376Status( ) & PARA_STATE_BUSY ) == 0 ) 
//			break; /* 检查状态端口的忙标志位 */
//	}
}


/*******************************************************************************
* 描      述      : 向CH376写   数据.
* 输      入      : UINT8 mData:
*                   要发送的数据.
*******************************************************************************/
void xWriteCH376Data(u8 mData)
{
    SPIx_ReadWriteByte(mData);
    Delay(10);  /* 确保读写周期大于0.6uS */
}

/*******************************************************************************
* 函  数  名      : xReadCH376Data
* 描      述      : 从CH376读数据.
*******************************************************************************/
u8 xReadCH376Data(void)
{
    u8 i;
    Delay(10);
    i = SPIx_ReadWriteByte( 0xFF );
    return( i );
}

/*******************************************************************************
* 函  数  名      : xEndCH376Cmd   结束命令.
*******************************************************************************/

void xEndCH376Cmd(void)
{
    SPI_CH376_SCS_HIGH();/* 禁止SPI片选 */
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);
    return Rxdata; //返回收到的数据
}

/*******************************************************************************
* 描      述      : 查询CH376中断(INT#低电平).
* 返      回      : FALSE:无中断.       TRUE:有中断.
*******************************************************************************/
u8 Query376Interrupt(void)
{
    u8 i;
    i = (GPIO_ReadInputDataBit(CH376_INT_PORT,CH376_INT_PIN) ? FALSE : TRUE );  /* 如果连接了CH376的中断引脚则直接查询中断引脚 */
    return( i );
}

/*******************************************************************************
* 描      述      : 查询CH376复位(Rest#低电平).
* 返      回      : FALSE:无中断.       TRUE:有中断.
*******************************************************************************/
u8 Rest376Interrupt(void)
{
    u8 i;
    i = (GPIO_ReadInputDataBit(CH376_INT_PORT,CH376_INT_PIN) ? FALSE : TRUE );  /* 如果连接了CH376的中断引脚则直接查询中断引脚 */
    return( i );
}
/*******************************************************************************
* 描      述      : 初始化CH376.
* 返      回      : FALSE:无中断.  TRUE:有中断.
*******************************************************************************/
u8 Init_CH376(void)
{
    u8 res;

    Delay(200);
    Delay(200);
    Delay(200);

    GPIO_SetBits(CH376_SCS_PORT,CH376_SCS_PIN);/* 禁止SPI片选 */
    GPIO_SetBits(CH376_INT_PORT,CH376_INT_PIN);

    xWriteCH376Cmd( CMD11_CHECK_EXIST );
    xWriteCH376Data(0x55);

    res = xReadCH376Data();

    xEndCH376Cmd();
    if (res != 0xAA)
    {
        return(ERR_USB_UNKNOWN);  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
    }
    xWriteCH376Cmd( CMD11_SET_USB_MODE ); /* 设备USB工作模式 */
    xWriteCH376Data( 0x06 );
    Delay(20);
    res = xReadCH376Data();
    xEndCH376Cmd();

    if ( res == CMD_RET_SUCCESS )  //RES=51  命令操作成功
    {
        return( USB_INT_SUCCESS ); //USB事务或者传输操作成功
    }
    else
    {
        return( ERR_USB_UNKNOWN );/* 设置模式错误 */
    }
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
//static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
//{
//    /* 等待超时后的处理,输出错误信息 */
//    FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
//    return 0;
//}


#ifndef __BSP_LCD_H
#define	__BSP_LCD_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "./sdram/bsp_sdram.h"
#include "./fonts/fonts.h"

/* LCD �������ش�С (��Ⱥ͸߶�) */
//#define  LCD_PIXEL_WIDTH    ((uint16_t)800)
//#define  LCD_PIXEL_HEIGHT   ((uint16_t)480)


#define  LCD_PIXEL_WIDTH    ((uint16_t)640)
#define  LCD_PIXEL_HEIGHT   ((uint16_t)480)


#define LCD_FRAME_BUFFER       ((uint32_t)0xD0000000)
#define BUFFER_OFFSET          ((uint32_t)640*480*3)    //sdramΪ16λ��,Һ��RGB888
#define LCD_PIXCELS            ((uint32_t)640*480) 

/** 
  * @brief  LCD Direction  
  */ 
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

/**
  * @}
  */ 

/** 
  * @brief  LCD Layer  
  */ 
#define LCD_BACKGROUND_LAYER     0x0000
#define LCD_FOREGROUND_LAYER     0x0001

/* LCD �����ظ�ʽ*/
#define ARGB8888 	LTDC_PIXEL_FORMAT_ARGB8888  /*!< ARGB8888 LTDC���ظ�ʽ */
#define RGB888 		LTDC_PIXEL_FORMAT_RGB888    /*!< RGB888 LTDC���ظ�ʽ   */
#define RGB565 		LTDC_PIXEL_FORMAT_RGB565    /*!< RGB565 LTDC���ظ�ʽ   */
#define ARGB1555 	LTDC_PIXEL_FORMAT_ARGB1555  /*!< ARGB1555 LTDC���ظ�ʽ */
#define ARGB4444 	LTDC_PIXEL_FORMAT_ARGB4444  /*!< ARGB4444 LTDC���ظ�ʽ */

typedef struct 
{ 
  uint32_t TextColor; 
  uint32_t BackColor;  
  sFONT    *pFont;
}LCD_DrawPropTypeDef;   
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 

/** 
  * @brief  �������ģʽ  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* ���ж��� */
  RIGHT_MODE              = 0x02,    /* �Ҷ���   */
  LEFT_MODE               = 0x03     /* �����   */
}Text_AlignModeTypdef;

#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */
/** 
  * @brief  LCD status structure definition  
  */     
#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)
#define LCD_TIMEOUT            ((uint8_t)0x02)

/** 
  * @brief  LCD FB_StartAddress  
  */

#define LCD_FB1_ADDRESS     (0xC0000000ul)
#define LCD_FB2_ADDRESS     (0xC0096000ul)
//#define LCD_FB_START_ADDRESS       ((uint32_t)0xD0000000)
/** 
  * @brief  LCD color  
  */

//#define LCD_COLOR_BLUE          ((uint32_t)0x001F )
//#define LCD_COLOR_GREEN         ((uint32_t)0x07E0)
//#define LCD_COLOR_RED           ((uint32_t)0xF800)
//#define LCD_COLOR_CYAN          ((uint32_t) 0x07FF )
//#define LCD_COLOR_MAGENTA       ((uint32_t)0xF81F )
//#define LCD_COLOR_YELLOW        ((uint32_t)0xFFE0)
////#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
//#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0x7BE0)
////#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
////#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
////#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
////#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
//#define LCD_COLOR_DARKBLUE      ((uint32_t)0x000F)
//#define LCD_COLOR_DARKGREEN     ((uint32_t)0x03E0)
//#define LCD_COLOR_DARKRED       ((uint32_t)0x7800)
//#define LCD_COLOR_DARKCYAN      ((uint32_t)0x03EF)
//#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0x780F)
////#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
//#define LCD_COLOR_WHITE         ((uint32_t)0xFFFF)
//#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xC618)
////#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
//#define LCD_COLOR_DARKGRAY      ((uint32_t)0x7BEF)
//#define LCD_COLOR_BLACK         ((uint32_t)0x0000)
////#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
////#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
//#define LCD_COLOR_TRANSPARENT   ((uint32_t)0x0000)


//#define LCD_COLOR_HUI           ((uint32_t)0x7BEF)
//#define LCD_COLOR_MIDGRAY       ((uint32_t)0xC618)



#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)


//lj 20200623  ʹLCD_COLOR_DARKGREEN��LCD_COLOR_VERYDARKGREEN  һ��
//#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)

#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF00A300)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)

#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_GRAY1          ((uint32_t)0xFF707070)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)
#define LCD_COLOR_LITTLEDARKCYAN      ((uint32_t)0xFF005050)
#define LCD_COLOR_VERYDARKRED       ((uint32_t)0xFF600000)
#define LCD_COLOR_HUI           ((uint32_t)0xC0303030)
#define LCD_COLOR_VERYDARKGREEN     ((uint32_t)0xFF00A300)
#define LCD_COLOR_MIDGRAY       ((uint32_t)0xFFA3A3A3)
#define LCD_COLOR_VERYDARKCYAN      ((uint32_t)0xFF00A3A3)
#define LCD_COLOR_VERYDARKYELLOW    ((uint32_t)0xFFA3A300)
#define LCD_COLOR_VERYDARKGRAY      ((uint32_t)0xFF002000)  //������ɫ


#define LCD_COLOR_GR7          	   ((uint32_t)0xFFFF9300)
/** 
  * @brief LCD Ĭ������
  */ 
#define LCD_DEFAULT_FONT        Font24 





// LCD�ܽ�
// LCD����
#define LCD_PWM_GPIO_PORT     GPIOI
#define LCD_PWM_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LCD_PWM_GPIO_PIN      GPIO_PIN_11


// ��ɫ������
#define LTDC_R2_GPIO_PORT     GPIOH
#define LTDC_R2_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_R2_GPIO_PIN      GPIO_PIN_8
#define LTDC_R2_AF            GPIO_AF14_LTDC

#define LTDC_R3_GPIO_PORT     GPIOH
#define LTDC_R3_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_R3_GPIO_PIN      GPIO_PIN_9
#define LTDC_R3_AF            GPIO_AF14_LTDC

#define LTDC_R4_GPIO_PORT     GPIOH
#define LTDC_R4_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_R4_GPIO_PIN      GPIO_PIN_10
#define LTDC_R4_AF            GPIO_AF14_LTDC

#define LTDC_R5_GPIO_PORT     GPIOH
#define LTDC_R5_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_R5_GPIO_PIN      GPIO_PIN_11
#define LTDC_R5_AF            GPIO_AF14_LTDC

#define LTDC_R6_GPIO_PORT     GPIOH
#define LTDC_R6_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_R6_GPIO_PIN      GPIO_PIN_12
#define LTDC_R6_AF            GPIO_AF14_LTDC

#define LTDC_R7_GPIO_PORT     GPIOG
#define LTDC_R7_GPIO_CLK      __GPIOG_CLK_ENABLE()
#define LTDC_R7_GPIO_PIN      GPIO_PIN_6
#define LTDC_R7_AF            GPIO_AF14_LTDC

// ��ɫ������
#define LTDC_G2_GPIO_PORT     GPIOH
#define LTDC_G2_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_G2_GPIO_PIN      GPIO_PIN_13
#define LTDC_G2_AF            GPIO_AF14_LTDC

#define LTDC_G3_GPIO_PORT     GPIOH
#define LTDC_G3_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_G3_GPIO_PIN      GPIO_PIN_14
#define LTDC_G3_AF            GPIO_AF14_LTDC

#define LTDC_G4_GPIO_PORT     GPIOH
#define LTDC_G4_GPIO_CLK      __GPIOH_CLK_ENABLE()
#define LTDC_G4_GPIO_PIN      GPIO_PIN_15
#define LTDC_G4_AF            GPIO_AF14_LTDC

#define LTDC_G5_GPIO_PORT     GPIOI
#define LTDC_G5_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_G5_GPIO_PIN      GPIO_PIN_0
#define LTDC_G5_AF            GPIO_AF14_LTDC

#define LTDC_G6_GPIO_PORT     GPIOI
#define LTDC_G6_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_G6_GPIO_PIN      GPIO_PIN_1
#define LTDC_G6_AF            GPIO_AF14_LTDC

#define LTDC_G7_GPIO_PORT     GPIOI
#define LTDC_G7_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_G7_GPIO_PIN      GPIO_PIN_2
#define LTDC_G7_AF            GPIO_AF14_LTDC

// ��ɫ������
#define LTDC_B2_GPIO_PORT     GPIOA
#define LTDC_B2_GPIO_CLK      __GPIOA_CLK_ENABLE()
#define LTDC_B2_GPIO_PIN      GPIO_PIN_3
#define LTDC_B2_AF            GPIO_AF9_LTDC

#define LTDC_B3_GPIO_PORT     GPIOA
#define LTDC_B3_GPIO_CLK      __GPIOA_CLK_ENABLE()
#define LTDC_B3_GPIO_PIN      GPIO_PIN_8
#define LTDC_B3_AF            GPIO_AF13_LTDC

#define LTDC_B4_GPIO_PORT     GPIOI
#define LTDC_B4_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_B4_GPIO_PIN      GPIO_PIN_4
#define LTDC_B4_AF            GPIO_AF14_LTDC

#define LTDC_B5_GPIO_PORT     GPIOI
#define LTDC_B5_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_B5_GPIO_PIN      GPIO_PIN_5
#define LTDC_B5_AF            GPIO_AF14_LTDC

#define LTDC_B6_GPIO_PORT     GPIOI
#define LTDC_B6_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_B6_GPIO_PIN      GPIO_PIN_6
#define LTDC_B6_AF            GPIO_AF14_LTDC

#define LTDC_B7_GPIO_PORT     GPIOI
#define LTDC_B7_GPIO_CLK      __GPIOI_CLK_ENABLE()
#define LTDC_B7_GPIO_PIN      GPIO_PIN_7
#define LTDC_B7_AF            GPIO_AF14_LTDC

//�����ź���
#define LTDC_CLK_GPIO_PORT    GPIOG
#define LTDC_CLK_GPIO_CLK     __GPIOG_CLK_ENABLE()
#define LTDC_CLK_GPIO_PIN     GPIO_PIN_7
#define LTDC_CLK_AF           GPIO_AF14_LTDC

#define LTDC_VSYNC_GPIO_PORT  GPIOI
#define LTDC_VSYNC_GPIO_CLK   __GPIOI_CLK_ENABLE()
#define LTDC_VSYNC_GPIO_PIN   GPIO_PIN_9
#define LTDC_VSYNC_AF         GPIO_AF14_LTDC

#define LTDC_HSYNC_GPIO_PORT  GPIOI
#define LTDC_HSYNC_GPIO_CLK   __GPIOI_CLK_ENABLE()
#define LTDC_HSYNC_GPIO_PIN   GPIO_PIN_10
#define LTDC_HSYNC_AF         GPIO_AF14_LTDC
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR = i;}			        //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRR = (uint32_t)i << 16;;}  //����͵�ƽ
#define digitalToggle(p,i)	    {p->ODR ^=i;}			//�����ת״̬
/* �������IO�ĺ� */
#define LCD_PWM_TOGGLE        digitalToggle(LCD_PWM_GPIO_PORT, LCD_PWM_GPIO_PIN)
#define LCD_PWM_HI                 digitalHi(LCD_PWM_GPIO_PORT, LCD_PWM_GPIO_PIN)
#define LCD_PWM_LO            	   digitalLo(LCD_PWM_GPIO_PORT, LCD_PWM_GPIO_PIN)




//#define LTDC_BL_GPIO_PORT        	GPIOI
//#define LTDC_BL_GPIO_CLK_ENABLE()  	__GPIOI_CLK_ENABLE()
//#define LTDC_BL_GPIO_PIN         	GPIO_PIN_11
//#define BL_PIN(n)		(n?HAL_GPIO_WritePin(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN,GPIO_PIN_RESET))


void  LCD_Init(void);
uint8_t  LCD_DeInit(void);
uint32_t LCD_GetXSize(void);
uint32_t LCD_GetYSize(void);
void     LCD_SetXSize(uint32_t imageWidthPixels);
void     LCD_SetYSize(uint32_t imageHeightPixels);

/* Functions using the LTDC controller */
void     LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
void     LCD_LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address);
void     LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
void     LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
void     LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
void     LCD_ResetColorKeying(uint32_t LayerIndex);
void     LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void     LCD_SelectLayer(uint32_t LayerIndex);
void     LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State);

void     LCD_SetTextColor(uint32_t Color);
uint32_t LCD_GetTextColor(void);
void     LCD_SetBackColor(uint32_t Color);
uint32_t LCD_GetBackColor(void);

void LCD_SetColors(uint32_t TextColor, uint32_t BackColor);
    
void     LCD_SetFont(sFONT *fonts);
sFONT    *LCD_GetFont(void);

uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t pixel);
void     LCD_Clear(uint32_t Color);
void     LCD_ClearLine(uint32_t Line);
void     LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void 	 LCD_DisplayStringLine_EN_CH(uint16_t Line, uint8_t *ptr);

void     LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void     LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void     LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);

void     LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void     LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     LCD_DisplayOff(void);
void     LCD_DisplayOn(void);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void     LCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params);

void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat);
/*����*/
void LCD_DisplayStringLineEx(uint16_t x, //��ʾ��x��
														 uint16_t y, //��ʾ��y��
														 uint16_t Font_width,	//Ҫ��ʾ�������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
														 uint16_t Font_Heig,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
														 uint8_t *ptr,				//��ʾ���ַ�����
														 uint16_t DrawModel);  //�Ƿ�ɫ��ʾ

void LCD_GPIO_Config(void);
void LCD_SetLayer(uint32_t Layerx);
void LCD_DrawPixel_LJ(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void DrawCharLJ(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
void LCD_DisplayStringLine_EN_CH_LJ(uint16_t x,uint16_t y, uint8_t *ptr);
uint16_t RGBTo16RGB(uint32_t RGB_Code);
void LCD_DisplayStringLineLJ(uint16_t x,uint16_t y, uint8_t *ptr,uint8_t FontType);
void LCD_ShowFontCN_40_55(uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint8_t * image);
#endif /* __BSP_LCD_H */

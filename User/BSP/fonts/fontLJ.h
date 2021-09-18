/*
  * @file    fontLJ.h
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   Header for fontLJ.c file
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTLJ_H
#define __FONTLJ_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "./fonts/fonts.h"


/* ͨ�����ض��� */
#define ch_on           0
#define ch_off          1



/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */

/** @addtogroup FONTS
  * @{
  */ 

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
	
	/*********************************************************************************************************
������ģ�����ݽṹ����
*********************************************************************************************************/
typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[72];                                              /* ����������                   */
}FNT_GB24;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[81];                                              /* ����������                   */
}FNT_GB27;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[108];                                              /* ����������                   */
}FNT_GB32x27;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[124];                                              /* ����������                   */
}FNT_GB32x31;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[128];                                              /* ����������                   */
}FNT_GB32x32;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[57];                                              /* ����������                   */
}FNT_GB24x19;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[32];                                              /* ����������                   */
}FNT_GB16x16;

typedef struct {                                                 /* ������ģ���ݽṹ             */
    unsigned char Index[2];                                             /* ������������                 */
    unsigned char Msk[40];                                              /* ����������                   */
}FNT_GB16x20;



//typedef struct _tFont
//{    
//  const uint8_t *table;
//  uint16_t Width;
//  uint16_t Height;
//  
//} sFONT;

void DrawCharLJ(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
extern const FNT_GB24 Chinese24x24_Table[];
extern const FNT_GB27 Chinese24x27_Table[];
extern const FNT_GB32x27 Chinese32x27_Table[];
extern const FNT_GB32x31 Chinese32x31_Table[];
extern const FNT_GB32x32 Chinese32x32_Table[];
extern const FNT_GB24x19 Chinese24x19_Table[];
extern const FNT_GB16x20 Chinese16x20_Table[];
extern const FNT_GB16x16 Chinese16x16_Table[];
extern const FNT_GB16x16 SONG_16x16_Table[];


extern sFONT Font16x24;
extern sFONT Font16x31;
extern sFONT FontC_8x16;
extern sFONT FontC_8x20;
extern sFONT FontC_8x20B;
extern sFONT FontC_16x24;
extern sFONT FontC_16x32;
extern sFONT FontC_8x14;
extern sFONT FontT_24x40;
extern sFONT FontT_24x34;
extern sFONT FontT_16x28;
extern sFONT FontT_16x24;
extern sFONT Font12x12;
extern sFONT Font8x12;
extern sFONT Font8x8;
extern sFONT Font24x24;
extern sFONT C_Font24x27;
//extern sFONT C_Font32x27;
//extern sFONT C_Font32x31;
//extern sFONT C_Font32x32;
extern sFONT C_Font24x19;
extern sFONT C_Font16x16;
extern sFONT C_Font16x20;
extern sFONT FontT_INS;
extern sFONT FontT_SYS;
extern sFONT SONG_Font16x16;



void DISP_TEMP_M(uint16_t x,uint16_t y, uint8_t *ptr,uint8_t sw);
//0��ʾʹ��SD����ģ��1��ʾFLASH��ģ,3�Խ�����������SD����ģ���ļ�ϵͳ���ٶ����ܶࡣ
void LCD_DisplayStringLineLJ(uint16_t x,uint16_t y, uint8_t *ptr,uint8_t FontType);




void DISP_SYS(uint16_t x,uint16_t y, uint8_t *ptr);

void DISP_CNL_S(uint16_t x,uint16_t y, uint8_t *ptr);
void DISP_INS(uint16_t x,uint16_t y, uint8_t *ptr);
#define GBKCODE_FLASH 						3  
#endif /* __FONTLJ_H */
 
/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */      

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

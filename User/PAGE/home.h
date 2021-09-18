
#ifndef  HOME_H_
#define  HOME_H_
#include  "DataType.h"
void page_home_cycle(const struct CUR cur);
void page_home(void);

extern const struct RStr HCh[RStrNUM];
extern const struct RStr HEn[RStrNUM];
void Stu_home_Init(void);
void Stu_home_cycle(struct CUR *sle,struct CUR *sleold);
void LCD_DisplayStringLineLJ_Zi(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType);
void LCD_DisplayStringLine_EN_CH_LJ_Zi(struct RDispPara *sel,uint16_t x,uint16_t y);
void Disp_Box_Zi(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void Disp_Box_Zhi2(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t Biancolor);
void TEMPDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur) ;
void LCD_DisplayNumLine_EN_CH_LJ_Home(struct RDispPara *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType);
void Disp_Box_Home(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color);
void LCD_DisplayStringLineLJ_Home(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType);
void LCD_DisplayStringLine_EN_CH_LJ_Home(struct RDispPara *sel,uint16_t x,uint16_t y);
void Stu_Mid_Init(void);
void LCD_DisplayStringLineLJ(uint16_t x,uint16_t y, uint8_t *ptr,uint8_t FontType);
void Display_Help(uint16_t x,uint16_t y,const char *ptr);
void QFlashDataSave_home_Init(void);
void Stu_home_Init(void);
extern struct CUR CurDisp;
extern struct CUR CurDispOld;
extern struct RDispPara DisLog[HCOL+2][HCAM];
extern union Data_union DataSave;

#endif          /* End of module include.                               */


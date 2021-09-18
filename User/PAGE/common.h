

#ifndef  COMMON_H_
#define  COMMON_H_

extern uint8_t savedata[200];
#define CHI  0
#define ENG  1




#define   XCOR						corpara[0]  
#define   XOffset					corpara[1]
#define   YCOR						corpara[2]
#define   YOffset					corpara[3]
#define   Polar1					corpara[4]
#define   Polar2					corpara[5]

#define  FONTTYPE24       0
#define  FONTTYPE16       1
#define  FONTTYPE12       2
void Disp_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color);
void Print_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t color);
uint32_t RGBCOLOR(uint8_t red,uint8_t green,uint8_t blue);
void COMmenu(struct RDispPara *sel1,struct RDispPara *sel2,struct RDispPara *sel3,struct RDispPara *sel4,struct RDispPara *sel5);

void LCD_DrawRectShade( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 ,uint8_t red,uint8_t green,uint8_t blue,uint8_t big_small,uint8_t colorflag );
void Disp_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color);
void Disp_Box_Zhi(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t Biancolor,uint32_t color);
void Print_Box_Zhi(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color);
void COMmenuInit(char chs,uint8_t *ptr1,uint8_t *ptr2,uint8_t *ptr3,uint8_t *ptr4,uint8_t *ptr5,uint8_t sle);
void LCD_DisplayStringLineLJ(uint16_t x,uint16_t y, uint8_t *ptr,uint8_t FontType);
void Clear_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color);
void SetmenuInit(char chs,uint8_t *ptr1,uint8_t *ptr2,uint8_t *ptr3,uint8_t *ptr4,uint8_t *ptr5);
void SetmenuInitP(char chs,uint8_t *ptr1,uint8_t *ptr2,uint8_t *ptr3,uint8_t *ptr4,uint8_t *ptr5
	,uint16 x1,uint16 x2,uint16 x3,uint16 x4,uint16 x5);

#endif                                                          /* End of module include.                               */


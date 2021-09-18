/*
  * @file  bsp_bmp.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief  ��ͼ
 */

#include "include.h"
#include "main.h"
#include <time.h>

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)    | ((B)>>3)))
#define RGB24TORGB16LJ(R,G,B) ((unsigned short int)((((R))<<11) | (((G))<<5)    | ((B))))
/*
�������Ҫ��ӡbmp��ص���ʾ��Ϣ,��printfע�͵�����
��Ҫ��printf()���轫���������ļ���������
 *
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)

extern __IO int32_t OS_TimeMS;
BYTE pColorData[960];                   /* һ�����ɫ���ݻ��� 320 * 3 = 960 */
FATFS bmpfs[2];
FIL bmpfdst,bmpfsrc;
FIL* bmpfile;
FRESULT bmpres;



/*
 * ��ʾbmpͼƬ, 24λ���ɫ
 * ͼƬ��С���ܳ���320*240
 */
//void Lcd_show_bmp(unsigned short int x, unsigned short int y,unsigned char *pic_name)
//{
//  int i, j, k;
//  int width, height, l_width;
//  BYTE red,green,blue;
//  BITMAPFILEHEADER bitHead;
//  BITMAPINFOHEADER bitInfoHead;
//  WORD fileType;
//  unsigned int read_num;
//  unsigned char tmp_name[20];
//  sprintf((char*)tmp_name,"0:%s",pic_name);

//  //f_mount(0, &bmpfs[0]);
//  f_mount(&bmpfs[0],"0:",1);
//  BMP_DEBUG_PRINTF("file mount ok \r\n");
//  bmpres = f_open( &bmpfsrc , (char *)tmp_name, FA_OPEN_EXISTING | FA_READ);

//  if(bmpres == FR_OK)
//  {
//      BMP_DEBUG_PRINTF("Open file success\r\n");

//      /* ��ȡλͼ�ļ�ͷ��Ϣ  �����ֽ�*/
//      f_read(&bmpfsrc,&fileType,sizeof(WORD),&read_num);

//      /* �ж��ǲ���bmp�ļ� */
//      if(fileType != 0x4d42)
//      {
//          BMP_DEBUG_PRINTF("file is not .bmp file!\r\n");
//          return;
//      }
//      else
//      {
//          BMP_DEBUG_PRINTF("Ok this is .bmp file\r\n");
//      }

//      /* ��ȡBMP�ļ�ͷ��Ϣ*/
//      f_read(&bmpfsrc,&bitHead,sizeof(tagBITMAPFILEHEADER),&read_num);


//      /* ��ȡλͼ��Ϣͷ��Ϣ */
//      f_read(&bmpfsrc,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);
//
//  }
//  else
//  {
//      BMP_DEBUG_PRINTF("file open fail!\r\n");
//      return;
//  }

//  width = bitInfoHead.biWidth;
//  height = bitInfoHead.biHeight;

//  /* ����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı��� */
//  l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);

////    if(l_width > 960)
////    {
////        BMP_DEBUG_PRINTF("\n SORRY, PIC IS TOO BIG (<=320)\n");
////        return;
////    }
//
//  /* ����Lcd Gram ɨ�跽��Ϊ: ���½�->���Ͻ� */
//  //Lcd_GramScan( 3 );
//
//  /* ��һ��ͼƬ��С�Ĵ���*/
////    sLCD_OpenWindow(x, y, width, height);
//
//  /* �ж��Ƿ���24bit���ɫͼ */
//  if(bitInfoHead.biBitCount >= 24)
//  {
//      OSIntEnter();
//      for(i=0;i< height; i++)
//      {
//          /* ��ȡһ��bmp�����ݵ�����pColorData���� */
//          #if 0
//              for(j=0; j< l_width; j++)
//              {
//                  f_read(&bmpfsrc,pColorData+j,1,&read_num);
//              }
//          #elif 1
//              f_read(&bmpfsrc,pColorData,l_width/2,&read_num);
//              f_read(&bmpfsrc,pColorData+l_width/2,l_width/2,&read_num);
//              #else
//              f_read(&bmpfsrc,pColorData,l_width,&read_num);
//          #endif

//          for(j=0; j<width; j++)                                             //һ����Ч��Ϣ
//          {
//              k = j*3;                                                                     //һ���е�K�����ص����
//              red = pColorData[k+2];
//              green = pColorData[k+1];
//              blue =  pColorData[k];
//              LCD_DrawPixel_LJ(j,i,RGB24TORGB16(red,green,blue));
//              //  sLCD_WR_Data(RGB24TORGB16(red,green,blue)); //д��LCD-GRAM
//          }
//      }
//      OSIntExit();
//  }
//  else
//  {
//      BMP_DEBUG_PRINTF("SORRY, THIS PIC IS NOT A 24BITS REAL COLOR");
//  return ;
//  }
//  f_close(&bmpfsrc);
//}

/**********************************************************
 * ��������Screen_shot
 * ����  ����ȡLCDָ��λ��  ָ����ߵ����� ����Ϊ24λ���ɫbmp��ʽͼƬ
 * ����  :  x                               ---ˮƽλ��
 *                  y                               ---��ֱλ��
 *                  Width                       ---ˮƽ���
 *                  Height                  ---��ֱ�߶�
 *                  filename                ---�ļ���
 * ���  �� 0       ---�ɹ�
 *                  -1      ---ʧ��
 *              8           ---�ļ��Ѵ���
 * ����  ��Screen_shot(0, 0, 320, 240, "/myScreen");-----ȫ����ͼ
 * ע��  ��x��Χ[0,319]  y��Χ[0,239]  Width��Χ[0,320-x]  Height��Χ[0,240-y]
 *                  ����ļ��Ѵ���,��ֱ�ӷ���
 **************************************************************/
/*
 * bmp�ļ�ͷ��54���ֽڣ�����ǰ14���ֽ����ļ�ͷ��Ϣ����40���ֽ���λͼ��Ϣͷ��Ϣ
 * bmp�ļ�ͷ֮����Ǿ����������Ϣ
 * 0x42 0x4d :bm
 * 54        :ʵ��λͼ���ݵ�ƫ���ֽ���
 * 40        :λͼ��Ϣͷ�ṹ��ĳ���
 * 1         :ƽ����Ϊ1
 * 24        :24bit���ɫ
 */
int Screen_shot(unsigned short int x,\
                unsigned short int y,\
                unsigned short int Width,\
                unsigned short int Height,\
                unsigned char *filename)
{
    /* bmp  �ļ�ͷ 54���ֽ� */
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0,
        0, 0, 0, 0, 54, 0,
        0, 0, 40,0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 24, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0
    };
    int i;
    int j;
    long file_size;
    long width;
    long height;
    unsigned char r,g,b;
    unsigned char tmp_name[30];
    unsigned int mybw;
    unsigned int read_data;
    char kk[4]= {0,0,0,0};

    /* ��*�� +������ֽ� + ͷ����Ϣ */
    file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;

    /* �ļ���С 4���ֽ� */
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    /* λͼ�� 4���ֽ� */
    width=Width;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;

    /* λͼ�� 4���ֽ� */
    height = Height;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;

    /* ��filename ����һ���ĸ�ʽ������ tmp_name */
    sprintf((char*)tmp_name,"%s",filename);
//  bmpfile  = (FIL*)Mem_malloc(INSRAM, sizeof(FIL));
    /* ע��һ�����������߼���Ϊ0 */
//  f_mount(0, &bmpfs[0]);
    f_mount(&bmpfs[0],"0:",1);
    /* �½�һ���ļ� */
    bmpres = f_open( &bmpfsrc, (char*)tmp_name, FA_CREATE_NEW | FA_WRITE );

    /* �½��ļ�֮��Ҫ�ȹر��ٴ򿪲���д�� */
    f_close(&bmpfsrc);


    bmpres = f_open( &bmpfsrc, (char*)tmp_name, FA_OPEN_ALWAYS | FA_WRITE );
    if ( bmpres == FR_OK )
    {
        OSIntEnter();
        /* ��Ԥ�ȶ���õ�bmpͷ����Ϣд���ļ����� */
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);

        /* �����ǽ�ָ�����ڵ����ݶ�����д���ļ�����ȥ */
        for(i=0; i<Height; i++)
        {
            if( !(Width%4) )                /* �պ���4�ֽڶ��� */
            {
              
                for(j=Width-1; j>=0; j--)
                {
                    read_data = LCD_ReadPixel(y+j, x+i);

                    r =  GETR_FROM_RGB16(read_data);
                    g =  GETG_FROM_RGB16(read_data);
                    b =  GETB_FROM_RGB16(read_data);

                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                }
            }
            else
            {
          
                for(j=Width-1; j>=0; j--)
                {
                    read_data = LCD_ReadPixel(y+j, x+i);

                    r =  GETR_FROM_RGB16(read_data);
                    g =  GETG_FROM_RGB16(read_data);
                    b =  GETB_FROM_RGB16(read_data);

                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                }
                /* ����4�ֽڶ�������Ҫ���� */
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);
            }
        }/* ������� */
        OSIntExit();
        f_close(&bmpfsrc);
        return 0;
    }
    else if ( bmpres == FR_EXIST )  //����ļ��Ѿ�����
    {
        return FR_EXIST;                        //8
    }
    else/* ����ʧ�� */
    {
        return -1;
    }
}


/* -------------------------------------end of file--------------------------------- */






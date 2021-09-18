/*
  * @file    homeOtur.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   ÆäËüÑ¡ÏîÉèÖÃ½çÃæµÄÒµÎñ
 */
//±¾ÎÄ¼þÎªÆäËüÑ¡ÏîµÄÉèÖÃÒ³Ãæ
#include  "include.h"
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB DoOturTaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK DoOtur_TASK_STK[DoOtur_STK_SIZE];
void Stu_MinOtur_Init(void);
//ÓÎ±ê  ·ÖÐÐºÍÁÐ
struct CUR CurOtur = {0};
struct CUR CurOturOld= {0};
//ÆäËüÉèÖÃµÄÏÂÀ­²Ëµ¥
struct RDispPara DisOtur[OturCOL]= {0};
//10¸öÍ¨µÀÀïµÄÉèÖÃ
static const struct RStr OturCh[OturNUM] =
{
    {"Ã÷ÁÁ","ÖÐµÈ","»Ò°µ"},   //LCD ÁÁ¶È
    {"¹Ø ","10s  ","30s  ","1min","2min","5min","10min","30min","60min"},  //ÆÁÄ»±£»¤
    {"¹Ø ","¿ª "},//¿ªÊ¼µçÔ´¿ª
    {"ÄÚ²¿","Íâ²¿"},//ÎÂ¶È²¹³¥
    {"$","&"},//ÎÂ¶Èµ¥Î»
    {">"},//ÈÕÆÚ
    {"ÖÐÎÄ","Ó¢ÎÄ"},//ÓïÑÔ
    {"±£³Ö","»Ö¸´"},//»Ö¸´³ö³§ÉèÖÃ
    {">"},//ÐÅÏ¢
};    //

static const struct RStr OturEn[OturNUM]=
{
    {"Light","Medium","Dark"},   //LCD ÁÁ¶È
    {"Off ","10s  ","30s  ","1min","2min","5min","10min","30min","60min"},  //ÆÁÄ»±£»¤
    {"Off ","On "},//·äÃùÆ÷
    {"Inter","Exter"},//ÎÂ¶È²¹³¥
    {"$","&"},//ÎÂ¶Èµ¥Î»
    {">"},
    {"Chinese","English"},
    {"No","Yes"},//»Ö¸´³ö³§ÉèÖÃ
    {">"},//ÐÅÏ¢
};

static const struct HearStr OturHear[OturNUM]=
{
    {"ÁÁ¶È:","BRIGHTNESS:"},   //LCD ÁÁ¶È
    {"ÆÁ±£:","SCREEN SAVER:"},  //ÆÁÄ»±£»¤
    {"°´¼üÒô:","KEY TONE:"},//·äÃùÆ÷
    {"ÎÂ¶È²¹³¥:","ROOM TEMP:"},//ÎÂ¶È²¹³¥
    {"ÎÂ¶Èµ¥Î»:","TEMP UNIT:"},//ÎÂ¶Èµ¥Î»
    {"ÈÕÆÚÊ±¼ä:","DATE/TIME:"},
    {"ÓïÑÔ:","LANGUAGE:"},
    {"»Ö¸´ÉèÖÃ:","DEFAULT SETTINGS:"},//»Ö¸´³ö³§ÉèÖÃ
    {"ÐÅÏ¢:","INFORMATION:"},//ÐÅÏ¢
};    //


//°ïÖú²Ëµ¥ÏÔÊ¾
static const struct HelpStr helpS[9]=
{
    {"°ïÖú:µ÷½ÚÆÁÄ»ÁÁ¶È£¬»Ò°µ¿ÉÒÔÑÓ³¤µç³ØÊ¹ÓÃÊ±¼ä¡£\0","Adjust brightness and the screen will be used longger.\0"},
    {"°ïÖú:Éè¶¨ÆÁ±£¹¦ÄÜ¡£\0","Please set the screen saver function.\0"},
    {"°ïÖú:Éè¶¨·äÃùÆ÷¹¦ÄÜ¡£\0","Please set the buzzer function.\0"},
    {"°ïÖú:Éè¶¨ÄÚ²¿Ê±ÓÉÈÈµç½øÐÐÊÒÄÚ²¹³¥¡£","Inter thermoelectric is used for indoor compensation."},
    {"°ïÖú:ÎÂ¶Èµ¥Î»Ñ¡Ôñ¡£\0","Temperature unit selection.\0"},
    {"°ïÖú:Éè¶¨ÏÔÊ¾Ê±¼ä£¬Äê·Ý½öºóÁ½Î»¡£\0","Set the display time,year is only two digits.\0"},
    {"°ïÖú:ÖÐÓ¢ÎÄÇÐ»»¡£\0","Switch between Chinese and English.\0"},
    {"°ïÖú:³ö³§»Ö¸´¡£\0","Factory recovery.\0"},
    {"°ïÖú:ÏÔÊ¾Éè±¸ÐÅÏ¢¡£\0","Display device information.\0"},
};

//°æ±¾ºÅ

static uint8_t TypeNum[10] = "V20201109\0";

u32 SaveStamp = 0;
u32 SaveFlag = 2;  //³õÊ¼ÖµÎª2
struct RDispPara DisDT[DiaDTCOL][DiaDTCAM] = {0};//×Ó¶Ô»°¿òµçÑ¹¶Ô»°¿òµÄ¿Ø¼ü¾ØÕó
static void LCD_DisplayStringLine_EN_CH_LJ_Otur(struct RDispPara *sel,uint16_t x,uint16_t y,uint32_t SetTextColor);
//ÎÂ¶È¶Ô»°¿ò³õÊ¼»¯
void InForDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{

    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    uint32 BackColor;
    x1y1.Box.x1 =  100;//È¡µã
    x1y1.Box.y1 =  100;//È¡
    x2y2.Box.x2=  500;//È¡µã
    x2y2.Box.y2 = 400 ;//È¡µã

    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//±êÌâ
    LCD_DrawRect(x1y1.Box.x1+2,x1y1.Box.y1+Ydiv*2,x2y2.Box.x2-x1y1.Box.x1-4, 24); //ÖÐ¿ò
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //±êÌâ¿òÌîÉ«
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+2+1,x1y1.Box.y1+Ydiv*2+1,x2y2.Box.x2-x1y1.Box.x1-4-2, 24-2);//ÖÐ¿ò¿òÌîÉ«
    LCD_SetTextColor(LCD_COLOR_WHITE);
    Ydiv=32;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, (uint8_t *)"ÐÅÏ¢",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"°æ±¾",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*3, (uint8_t *)"ÁªÏµ·½Ê½",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, (uint8_t *)"ÍøÖ·",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*5, (uint8_t *)"ÓÊÏä",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*6, (uint8_t *)"²úÆ·ÐÍºÅ",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, (uint8_t *)"ÐòÁÐºÅ",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, (uint8_t *)"Information");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"Tpye:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*3, (uint8_t *)"Contact:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, (uint8_t *)"Website:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*5, (uint8_t *)"Mailbox:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*6, (uint8_t *)"ID:");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, (uint8_t *)"Serial N:");
    }
    LCD_SetFont(&Font20);
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+Ydiv, TypeNum);
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*3, (uint8_t *)"TEL4001128155");
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*4, (uint8_t *)"www.jk17.com");
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*5, (uint8_t *)"mailjk17@126.com");
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*6, (uint8_t *)DataSave.Data_type.PName);
    LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+120, x1y1.Box.y1+2+Ydiv*7, (uint8_t *)DataSave.Data_type.PSN);
    Disp_Box(260,374,340,396,0,LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
    Ydiv=32;
    LCD_SetFont(&Font20);
    BackColor=LCD_GetBackColor();
    LCD_SetFont(&Font20);
    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
    LCD_DisplayStringLine_EN_CH_LJ(285,376,(uint8_t* )"OK");
    LCD_SetBackColor(BackColor);
}
void SoftKeyDiaInForInit(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(m=0; m<DiaSoftKeyCOL; m++)
        for(n=0; n<DiaSoftKeyCAM; n++)
        {
            memset(DisSoftKey[m][m].Str,0,sizeof(DisSoftKey[m][m].Str));
            DisSoftKey[m][n].flag = 1;
            DisSoftKey[m][n].disp=0;
            DisSoftKey[m][n].dispold=0;
            DisSoftKey[m][n].lang =DataSave.Data_type.LANG;
        }
    DisSoftKey[0][0].disp=1;
    DisSoftKey[0][0].cind =  RD->cind  ;//Ãû³Æ

    strncpy(DisSoftKey[0][0].Str,RD->Str,sizeof(RD->Str)); //
    for(n=0; n<DiaSoftKeyCAM; n++)
    {
        DisSoftKey[1][n].disp=1;
        DisSoftKey[1][n].cind  = CindStr ;
        DisSoftKey[1][n].Str[0]  = 'A'+n ;
        DisSoftKey[2][n].disp=1;
        DisSoftKey[2][n].cind  = CindStr ;
        DisSoftKey[2][n].Str[0]  = 'A'+n +DiaSoftKeyCAM;
        DisSoftKey[3][n].disp=1;
        DisSoftKey[3][n].cind  = CindStr ;
        DisSoftKey[3][n].Str[0]  = 'a'+n ;
        DisSoftKey[4][n].disp=1;
        DisSoftKey[4][n].cind  = CindStr ;
        DisSoftKey[4][n].Str[0]  = 'a'+n +DiaSoftKeyCAM;
        DisSoftKey[5][n].disp=1;
        DisSoftKey[5][n].cind  = CindStr ;
        DisSoftKey[5][n].Str[0]  = FuHao[n];
    }


    DisSoftKey[6][0].disp=1;
    DisSoftKey[6][0].cind=16;
    DisSoftKey[6][0].index[DisSoftKey[6][0].cind] = 0;
    DisSoftKey[6][1].disp=1;
    DisSoftKey[6][1].cind=16;
    DisSoftKey[6][1].index[DisSoftKey[6][1].cind] = 1;

    x1y1.Box.x1 =  30;//È¡µã
    x1y1.Box.y1 =  65;//È¡
    x2y2.Box.x2=  630;//È¡µã
    x2y2.Box.y2 = 420 ;//È¡µã

    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//±êÌâ
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //±êÌâ¿òÌîÉ«
    LCD_SetTextColor(LCD_COLOR_WHITE);

    Ydiv=24;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Ó¢ÎÄÈí¼þÅÌ",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"²úÆ·ÐÍºÅ",FONTTYPE16);

    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Soft Key");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv,(uint8_t *)"ID");
    }
}
//Èí¼þ¼üÅÌ  ¶Ô»°¿ò
static void SoftKeyDiaInFor(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
{
    OS_ERR  err;
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    uint16 HChar=40;
    static struct RDispStr DisLogOld= {0};
    struct RDispStr x1y1;
    struct RDispStr x2y2;
    static struct CUR Cur;
    Cur.CAM=0;
    Cur.COL=0;
    DisLogOld = *RD;
    strncpy(DisLogOld.Str,RD->Str,sizeof(RD->Str)) ;
    x1y1.Box.x1 =  30;//È¡µã
    x1y1.Box.y1 =  65;//
    x2y2.Box.x2=  630;//È¡µã
    x2y2.Box.y1 = 396;//È¡µã
    x2y2.Box.y2 = 420;//È¡µã
    SoftKeyDiaInForInit(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home_File(&DisSoftKey[0][0],x1y1.Box.x1+Xdiv+10,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+290,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        for(n=0; n<DiaSoftKeyCAM; n++)
        {
            Disp_Box_Home_File(&DisSoftKey[1][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*3,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*3+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[2][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*5,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*5+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[3][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*7+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[4][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*9,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*9+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            Disp_Box_Home_File(&DisSoftKey[5][n],x1y1.Box.x1+n*HChar+24,x1y1.Box.y1+2+Ydiv*11,x1y1.Box.x1+n*HChar+24+28,x1y1.Box.y1+Ydiv*11+HChar,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        }
        Disp_Box_Home_File(&DisSoftKey[6][0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home_File(&DisSoftKey[6][1],x1y1.Box.x1+Xdiv*4+16,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*4+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);

        for(n=0; n<DiaSoftKeyCAM; n++)
            for(m=0; m<DiaSoftKeyCOL; m++)
            {
                if(m==0&&n==0)
                {

                    LCD_DisplayNumLine_EN_CH_LJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+2,DisSoftKey[m][n].Box.y1+2,DisSoftKey[m][n].Str,Font20);
                    DisSoftKey[m][n].sel=1;
                    DisSoftKey[m][n].selold=0;

                }
                else if(m==6)
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+15,DisSoftKey[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+15,DisSoftKey[m][n].Box.y1+2);
                }
                else if(m>=1&&m<=5)
                {

                    LCD_DisplayNumLine_EN_CH_LJ_Home_File(&DisSoftKey[m][n],DisSoftKey[m][n].Box.x1+6,DisSoftKey[m][n].Box.y1+8,DisSoftKey[m][n].Str,Font24);

                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==DiaSoftKeyCOL-1)&&keytmp!=KEY_ESC); //DCµÄ¶Ô»°¿òµÚ4ÐÐÊÇÈ·¶¨ºÍÈ¡Ïû°´Å¥
    OSSchedLock(&err);
    Stu_Otur_Init();
    ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    page_Otur();
    OSSchedUnlock(&err);
}

void ModifyInFor(uint8 keytmp,struct CUR* cur)
{
    OS_ERR err;
    static struct RDispStr RDtemp;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
            }
            break;
        case KEY_HIDE1://
            memset(RDtemp.Str,0,sizeof(RDtemp.Str));
            strncpy(RDtemp.Str,DataSave.Data_type.PName,sizeof(DataSave.Data_type.PName));
            SoftKeyDiaInFor(keytmp,&RDtemp,cur);
            strncpy(DataSave.Data_type.PName,RDtemp.Str,sizeof(DataSave.Data_type.PName));
            break;
        case KEY_HIDE2://
            memset(RDtemp.Str,0,sizeof(RDtemp.Str));
            strncpy(RDtemp.Str,DataSave.Data_type.PSN,sizeof(DataSave.Data_type.PSN));
            SoftKeyDiaInFor(keytmp,&RDtemp,cur);
            strncpy(DataSave.Data_type.PSN,RDtemp.Str,sizeof(DataSave.Data_type.PSN));
            break;

    }
}



//ÐÅÏ¢¶Ô»°¿ò
void InForDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    InForDiaInit(keytmp,RD, cur) ;
    do
    {
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyInFor(keytmp,cur);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC
          &&keytmp!=KEY_HIDE1&&keytmp!=KEY_HIDE2);
    Stu_MinOtur_Init();
    page_Otur();
}


void Stu_Otur_InitLang(void)
{
    uint8 m,n;

    for(m=0; m<OturCOL; m++)
    {

        DisOtur[m].selold = 1;//ÒÔÇ°È«Ñ¡ÖÐÏÖÔÚÈ«Ã»Ñ¡ÖÐ Ë¢ÐÂ
        DisOtur[m].sel= 0;     //µ±Ç°Ã»ÓÐÑ¡ÖÐ

        DisOtur[m].lang = DataSave.Data_type.LANG;


    }
    DisOtur[6].selold = 0;//ÒÔÇ°È«Ñ¡ÖÐÏÖÔÚÈ«Ã»Ñ¡ÖÐ Ë¢ÐÂ
    DisOtur[6].sel= 1;     //µ±Ç°Ã»ÓÐÑ¡ÖÐ
    for(n=0; n<HCAM; n++)
    {
        DisLog[12][n].sel= 0;
        DisLog[12][n].selold = 1;
        DisLog[12][n].lang = DataSave.Data_type.LANG;
    }
    DisLog[12][4].sel= 1;
    DisLog[12][4].selold = 2;


}

void Stu_Otur_Init(void)
{
    uint8 m;

    for(m=0; m<OturCOL; m++)
    {
        DisOtur[m].col = m;//ÐÐ
        DisOtur[m].cam = 0;//ÁÐ
        DisOtur[m].lang = DataSave.Data_type.LANG;
        DisOtur[m].flag = 1;  //³õÊ¼»¯µÚÒ»ÏÔÊ¾±êÖ¾
        DisOtur[m].selold = 1;//ÒÔÇ°È«Ñ¡ÖÐÏÖÔÚÈ«Ã»Ñ¡ÖÐ Ë¢ÐÂ
        DisOtur[m].sel= 0;     //µ±Ç°Ã»ÓÐÑ¡ÖÐ
        DisOtur[m].disp=1;  // ÊÇ·ñÏÔÊ¾1 ÏÔÊ¾ 0²»ÏÔÊ¾
        DisOtur[m].dispold=1; //Ö®Ç°ÊÇ·ñÏÔÊ¾
        DisOtur[m].Box.flag = 0;  //×Ö·û´®ÊÇ·ñÓÐ¿ò 0 Ã»ÓÐ¿ò
        DisOtur[m].FontCH.flag = 0;  //ÖÐÎÄ±êÖ¾
        DisOtur[m].FontEN.flag = 0;  //Ó¢ÎÄ±êÖ¾
        DisOtur[m].cind=m;//ÀàÐÍ
        DisOtur[m].Ceng= 0;

        if(DisOtur[m].cind==0)
        {
            DisOtur[m].Ceng=3;
        }
        else if(DisOtur[m].cind==1)
        {
            DisOtur[m].Ceng=9; //×ÓÏÂÀ­¿òÓÐ¼¸¸öÑ¡Ïî 9¸ö
        }
        else if(DisOtur[m].cind==2)
        {
            DisOtur[m].Ceng=2;//×ÓÏÂÀ­¿òÓÐ¼¸¸öÑ¡Ïî 2¸ö
        }
        else if(DisOtur[m].cind==3)
        {
            DisOtur[m].Ceng=2;
        }
        else if(DisOtur[m].cind==4)
        {
            DisOtur[m].Ceng=2;
        }
        else if(DisOtur[m].cind==6)
        {
            DisOtur[m].Ceng=2;
        }
        else if(DisOtur[m].cind==7)
        {
            DisOtur[m].Ceng=2;
        }
    }
    DisOtur[0].flag= 0 ;
    DisOtur[0].sel= 1;//µ±Ç°ÎªÑ¡ÖÐ×´Ì¬
    DisOtur[0].selold = 0;//ÉÏÒ»´ÎÎªÑ¡ÖÐ×´Ì¬


//Ö®Ç°ÉèÖÃÖµ»¹Ô­
    DisOtur[0].index[DisOtur[0].cind] =DataSave.Data_type.Bright;
    DisOtur[1].index[DisOtur[1].cind] =DataSave.Data_type.ScrSaver;
    DisOtur[2].index[DisOtur[2].cind] =DataSave.Data_type.BeepF;
    DisOtur[3].index[DisOtur[3].cind] =DataSave.Data_type.RoomT;
    DisOtur[4].index[DisOtur[4].cind] =DataSave.Data_type.TUnit;
    DisOtur[6].index[DisOtur[6].cind] =DataSave.Data_type.LANG;
    CurOtur.COL = 0;
    CurOtur.CAM = 0;
}

void Stu_HomeManu_Init(uint8 a)
{
    uint8 n;
    for(n=0; n<HCAM; n++)
    {
        DisLog[12][n].sel= 0;
        DisLog[12][n].selold = 1;
        DisLog[12][n].lang = DataSave.Data_type.LANG;
    }
    DisLog[12][a].sel= 1;
    DisLog[12][a].selold = 2;
}



//ÖÐ¿òÊý¾ÝµÄ³õÊ¼»¯
void Stu_MinOtur_Init(void)
{
    uint8 m,n;
    for(m=0; m<OturCOL ; m++)
    {
        DisOtur[m].lang = DataSave.Data_type.LANG;
        DisOtur[m].flag = 1;
        DisOtur[m].selold = 1;//ÒÔÇ°È«Ñ¡ÖÐÏÖÔÚÈ«Ã»Ñ¡ÖÐ Ë¢ÐÂ
        DisOtur[m].sel= 0;
        if(m==0&&n==0)
        {
            DisOtur[m].selold = 0;
        }
    }
    DisOtur[CurOtur.COL].sel= 1;
    DisOtur[CurOtur.COL].selold = 0;
    DisOtur[CurOtur.COL].flag = 0;

}

//Õû¸öÉèÖÃ½çÃæµÄ³õÊ¼»¯
void Stu_Otur_cycle(struct CUR *sle,struct CUR *sleold)
{
    uint8 m;
    for(m=0; m<OturCOL; m++)
    {
        DisOtur[m].flag = 0;
        DisOtur[m].selold = 0;
        DisOtur[m].sel= 0;
    }
    if(sleold->COL!=sle->COL)
    {
        DisOtur[sle->COL].selold = 0;//Ë¢ÐÂ
        DisOtur[sle->COL].sel= 1;
        DisOtur[sleold->COL].selold = 1;//Ë¢ÐÂ
        DisOtur[sleold->COL].sel= 0;
    }
}

//³õÊ¼»¯
void page_Otur(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;

    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);
    LCD_DrawRect(0, 2, 639, 430);
    LCD_DrawHLine(0, 380, 639);
    Stu_HomeManu_Init(4);  //µÚÎå¸ö°´Å¥¸ßÁÁ
    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//´Ë´¦µ÷ÓÃÖ÷½çÃæ
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(2,5, "ÉèÖÃ",FONTTYPE24);
        LCD_DisplayStringLineLJ(x,y, "ÉèÖÃÏî",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+4*Xdiv,y, "²ÎÊý",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_DisplayStringLineLJ(3,5, "OTHR",FONTTYPE24);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "Settings:");
        LCD_DisplayStringLine_EN_CH_LJ(x+4*Xdiv,y, "Parameter");
    }
    LCD_SetFont(&Font20);

    page_Otur_cycle();
}



void Lang_Flash(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    LCD_Clear(LCD_COLOR_BLACK);


    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(2,5, "ÉèÖÃ",FONTTYPE24);

        LCD_DisplayStringLineLJ(x,y, "ÉèÖÃÏî",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+4*Xdiv,y, "²ÎÊý",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_DisplayStringLineLJ(3,5, "OTHR",FONTTYPE24);

        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "Settings:");
        LCD_DisplayStringLine_EN_CH_LJ(x+4*Xdiv,y, "Parameter");
    }
    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//´Ë´¦µ÷ÓÃÖ÷½çÃæ
    y = 60;
    Ydiv =28;

    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);
    LCD_DrawRect(0, 2, 639, 430);
    LCD_DrawHLine(0, 380, 639);

    y = 60;
    Ydiv =28;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(x-5,y+Ydiv*11+20, "°ïÖú",FONTTYPE16);
    }
    else
    {
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*11+20, "Help?");
    }


    Xdiv=90;
    for(i=0; i<OturCOL; i++)
    {
        Disp_Box_Home(&DisOtur[i],x+Xdiv*4+20,y+i*Ydiv+2,x-15+Xdiv*4+130+30,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
    }
    for(i=0; i<OturNUM; i++)
    {
        if(CHI == DataSave.Data_type.LANG )
        {

            LCD_DisplayStringLineLJ(x-5,y+Ydiv*i,(uint8_t* )OturHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24);
        }
        else if(ENG == DataSave.Data_type.LANG)
        {
            LCD_SetFont(&Font20);
            LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )OturHear[i].Ch[DataSave.Data_type.LANG]);
        }
    }
    y = 64;
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
    for(i=0; i<OturCOL; i++)
    {
        if(i==1||i==4||i==5||i==8)
        {
            LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisOtur[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
        else
        {
            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_Otur(&DisOtur[i],x+30+Xdiv*4,y+i*Ydiv,FONTTYPE16,LCD_COLOR_HUI );
            else
                LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisOtur[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
}

static void LCD_DisplayStringLine_EN_CH_LJ_Otur(struct RDispPara *sel,uint16_t x,uint16_t y,uint32_t SetTextColor)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontEN.flag ==0)
    {
        sel->FontEN.x1=x;
        sel->FontEN.y1=y;
        sel->FontEN.flag = 1;
    }
    BackColor=LCD_GetBackColor();
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_SetFont(&Font20);
        LCD_SetBackColor(SetTextColor);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )OturEn[sel->cind].Ch[sel->index[sel->cind]]);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                LCD_SetFont(&Font20);
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )OturEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )OturEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )OturEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )OturEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}


//ÖÐÎÄ
void LCD_DisplayStringLineLJ_Otur(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType,uint32_t SetTextColor)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontCH.flag ==0)
    {
        sel->FontCH.x1=x;
        sel->FontCH.y1=y;

        sel->FontCH.flag = 1;
    }
    BackColor=LCD_GetBackColor();
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_SetBackColor(SetTextColor);
        LCD_DisplayStringLineLJ(x,y,(uint8_t *)OturCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)OturCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)OturCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            LCD_DisplayStringLineLJ(x,y,(uint8_t *)OturCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )OturEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}

void Disp_Head(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint8_t *ptr,uint8_t FontType,sFONT fonts)
{
    uint32 TextColor;
    char flag = sel->flag;
    if(sel->disp==1&&flag==1)
    {
        TextColor=LCD_GetTextColor();
        LCD_SetTextColor(LCD_COLOR_WHITE);
        if(CHI == DataSave.Data_type.LANG )
        {
            LCD_DisplayStringLineLJ(x1,y1,ptr,FontType);
        }
        else if(ENG == DataSave.Data_type.LANG)
        {
            LCD_SetFont(&fonts);
            LCD_DisplayStringLine_EN_CH_LJ(x1,y1,ptr);
        }
        LCD_SetTextColor(TextColor);
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                if(sel->sel==1)
                {
                    TextColor=LCD_GetTextColor();
                    LCD_SetTextColor(LCD_COLOR_DARKGREEN);
                    if(CHI == DataSave.Data_type.LANG )
                    {
                        LCD_DisplayStringLineLJ(x1,y1,ptr,FontType);
                    }
                    else if(ENG == DataSave.Data_type.LANG)
                    {
                        LCD_SetFont(&fonts);
                        LCD_DisplayStringLine_EN_CH_LJ(x1,y1,ptr);
                    }
                    LCD_SetTextColor(TextColor);
                }
                else
                {
                    TextColor=LCD_GetTextColor();
                    LCD_SetTextColor(LCD_COLOR_WHITE);
                    if(CHI == DataSave.Data_type.LANG )
                    {
                        LCD_DisplayStringLineLJ(x1,y1,ptr,FontType);
                    }
                    else if(ENG == DataSave.Data_type.LANG)
                    {
                        LCD_SetFont(&fonts);
                        LCD_DisplayStringLine_EN_CH_LJ(x1,y1,ptr);
                    }
                    LCD_SetTextColor(TextColor);
                }
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {

        }
        else if(sel->disp==2&&sel->dispold==0)
        {

        }
    }

}

void page_Otur_cycle(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=90;
    uint16 Ydiv=90;
    uint16 i;

    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);//´Ë´¦µ÷ÓÃÖ÷½çÃæ
    y = 60;
    Ydiv =28;
    Display_Help(x-2,y+Ydiv*11+20,helpS[CurOtur.COL].Ch[DataSave.Data_type.LANG]);

    for(i=0; i<OturCOL; i++)
    {
        Disp_Box_Home(&DisOtur[i],x+Xdiv*4+20,y+i*Ydiv+2,x-15+Xdiv*4+130+30,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);

    }

    for(i=0; i<OturNUM; i++)
    {
        if(CHI == DataSave.Data_type.LANG )
        {
            Disp_Head(&DisOtur[i],x-5,y+Ydiv*i,(uint8_t* )OturHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24,Font20);
        }
        else if(ENG == DataSave.Data_type.LANG)
        {
            Disp_Head(&DisOtur[i],x-5,y+Ydiv*i,(uint8_t* )OturHear[i].Ch[DataSave.Data_type.LANG],FONTTYPE24,Font20);
        }
    }
    y = 64;
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
    for(i=0; i<OturCOL; i++)
    {
        if(i==1||i==4||i==5||i==8)
        {
            LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisOtur[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
        else
        {
            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_Otur(&DisOtur[i],x+30+Xdiv*4,y+i*Ydiv,FONTTYPE16,LCD_COLOR_HUI );
            else
                LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisOtur[i],x+30+Xdiv*4,y+i*Ydiv,LCD_COLOR_HUI );
        }
    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
}


//µÚÒ»ÁÐÏÂÀ­¿òµÄ¼üÖµ´¦Àíº¯Êý
void ModifySaver(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* sel,char MaxCol)
{
    OS_ERR err;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
            }
            break;
        case KEY_UP://ÉÏ
            if((RD->index[RD->cind])==0)
                (RD->index[RD->cind]) = MaxCol-1;
            else
                RD->index[RD->cind]--;
            break;
        case KEY_DOWN:
            if( ++(RD->index[RD->cind])>MaxCol-1)
                (RD->index[RD->cind]) =0 ;
            break;
        case KEY_ACC:
            (RD->index[RD->cind])+=2;
            if((RD->index[RD->cind])>MaxCol-1)
                (RD->index[RD->cind]) =0 ;
            break;
        case KEY_ENTER:
            if(sel->COL<OturCOL-1)
            {
                DisOtur[sel->COL].disp=1;
                DisOtur[sel->COL].dispold=0;
                DisOtur[sel->COL].index[DisOtur[sel->COL].cind] = RD->index[RD->cind];

            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];

    }
}
void Disp_Box_Otur(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    if(sel->disp==1)
    {
        if(sel->sel!=sel->selold)
        {
            if(sel->sel==1)
            {
                Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_DARKGREEN);
            }
            else
            {
                Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
            }
        }
    }
    else if(sel->disp==0&&sel->dispold==1)
    {
        Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    }
}
void ComBoxLang(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[SaverOnOffCOL] = {0};   //Ñ¡×î´óµÄ
    static struct RDispPara DisLogOld;
    for(m=0; m<MaxCol; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=RD->cind;
        DisInPut[m].index[DisInPut[m].cind]=m;
    }
    DisLogOld = *RD;
    do
    {
        box=RD->Box;
        Ceng = RD->Ceng;
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        for(m=0; m<MaxCol; m++)
        {
            DisInPut[m].flag = 1;
            DisInPut[m].disp=1;
            DisInPut[m].dispold=0;
            DisInPut[m].sel = 0;
            DisInPut[m].selold=1;
            DisInPut[m].Box.x1 = RD->Box.x1;
            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1);
            DisInPut[m].Box.x2 = RD->Box.x2 ;
            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1);
            DisInPut[m].FontCH.x1 = RD->FontCH.x1;
            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
            DisInPut[m].FontEN.x1 = RD->FontEN.x1;
            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
            {
                DisInPut[m].sel = 1;
                DisInPut[m].selold=0;
                DisInPut[m].flag = 0;
            }
            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);


            if(cur->COL==1||cur->COL==4|cur->COL==5||cur->COL==8)
            {
                LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);

            }
            else
            {
                if(CHI == DataSave.Data_type.LANG )
                    LCD_DisplayStringLineLJ_Otur(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16,LCD_COLOR_BLACK);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifySaver(keytmp,RD,&DisLogOld,cur,MaxCol);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    DataSave.Data_type.LANG =DisOtur[cur->COL].index[DisOtur[cur->COL].cind];
    SaveAllPara();
    Stu_Otur_InitLang();
    Lang_Flash();
}


//SaverÇÐ»»ÏÂÀ­¿ò¿ÉÍ¨ÓÃ
static void ComBoxS(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 MaxCol)
{
    struct BoxFram  box;
    uint8 Ceng =0 ;
    uint8 m;
    struct RDispPara DisInPut[SaverOnOffCOL] = {0};   //Ñ¡×î´óµÄ
    static struct RDispPara DisLogOld;
    for(m=0; m<MaxCol; m++)
    {
        DisInPut[m].flag = 0;
        DisInPut[m].disp=0;
        DisInPut[m].dispold=0;
        DisInPut[m].lang = DataSave.Data_type.LANG;
        DisInPut[m].cind=RD->cind;
        DisInPut[m].index[DisInPut[m].cind]=m;
    }
    DisLogOld = *RD;
    do
    {
        box=RD->Box;
        Ceng = RD->Ceng;
        Disp_Box_Zhi( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        for(m=0; m<MaxCol; m++)
        {
            DisInPut[m].flag = 1;
            DisInPut[m].disp=1;
            DisInPut[m].dispold=0;
            DisInPut[m].sel = 0;
            DisInPut[m].selold=1;
            DisInPut[m].Box.x1 = RD->Box.x1;
            DisInPut[m].Box.y1 = RD->Box.y1+(box.h+8)*(m+1);
            DisInPut[m].Box.x2 = RD->Box.x2 ;
            DisInPut[m].Box.y2 = RD->Box.y2+(box.h+8)*(m+1);
            DisInPut[m].FontCH.x1 = RD->FontCH.x1;
            DisInPut[m].FontCH.y1 = RD->FontCH.y1+(box.h+8)*(m+1);
            DisInPut[m].FontEN.x1 = RD->FontEN.x1;
            DisInPut[m].FontEN.y1 = RD->FontEN.y1+(box.h+8)*(m+1);
            if(RD->index[RD->cind]==DisInPut[m].index[DisInPut[m].cind])
            {
                DisInPut[m].sel = 1;
                DisInPut[m].selold=0;
                DisInPut[m].flag = 0;
            }
            Disp_Box_Zi(&DisInPut[m],DisInPut[m].Box.x1,DisInPut[m].Box.y1,DisInPut[m].Box.x2,DisInPut[m].Box.y2);


            if(cur->COL==1||cur->COL==4|cur->COL==5||cur->COL==8)
            {
                LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);

            }
            else
            {
                if(CHI == DataSave.Data_type.LANG )
                    LCD_DisplayStringLineLJ_Otur(&DisInPut[m],DisInPut[m].FontCH.x1,DisInPut[m].FontCH.y1,FONTTYPE16,LCD_COLOR_BLACK);
                else
                    LCD_DisplayStringLine_EN_CH_LJ_Otur(&DisInPut[m],DisInPut[m].FontEN.x1,DisInPut[m].FontEN.y1,LCD_COLOR_BLACK);
            }
        }
        Disp_Box_Zhi2( box.x1,box.y2,box.x2,box.y2+(box.h+8)*Ceng,LCD_COLOR_WHITE);
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifySaver(keytmp,RD,&DisLogOld,cur,MaxCol);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    Stu_MinOtur_Init();
    ModifySaver(keytmp,RD,&DisLogOld,cur,MaxCol);
    page_Otur();
}

// Ê±¼ä¶Ô»°¿ò³õÊ¼»¯
void DTDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(m=0; m<DiaDTCOL; m++)
        for(n=0; n<DiaDTCAM; n++)
        {
            DisDT[m][n].flag = 1;
            DisDT[m][n].disp=0;
            DisDT[m][n].dispold=0;
            DisDT[m][n].lang =DataSave.Data_type.LANG;
            DisDT[m][n].Num.bit = 2;
            DisDT[m][n].Num.dot= 2;
            DisDT[m][n].Num.Decimalbit= 0;
        }
    DisDT[0][0].cind=17;
    DisDT[0][0].disp=1;
    DisDT[0][1].cind=17;
    DisDT[0][1].disp=1;
    DisDT[1][0].cind=17;
    DisDT[1][0].disp=1;
    DisDT[1][1].cind=17;
    DisDT[1][1].disp=1;
    DisDT[2][0].cind=17;
    DisDT[2][0].disp=1;
    DisDT[2][1].cind=17;
    DisDT[2][1].disp=1;
    DisDT[3][0].cind=16;
    DisDT[3][0].index[DisDT[3][0].cind] = 0;
    DisDT[3][0].disp=1;
    DisDT[3][1].cind=16;
    DisDT[3][1].index[DisDT[3][1].cind] = 1;
    DisDT[3][1].disp=1;


    DisDT[0][0].Num.fval = (float)DataSave.Data_type.Date.Year ; //Äê
    DisDT[0][1].Num.fval = (float)DataSave.Data_type.Date.Month ;//ÔÂ
    DisDT[1][0].Num.fval = (float)DataSave.Data_type.Date.Date ;//ÈÕ
    DisDT[1][1].Num.fval = (float)DataSave.Data_type.Time.Hours ;
    DisDT[2][0].Num.fval = (float)DataSave.Data_type.Time.Minutes ;
    DisDT[2][1].Num.fval = (float)DataSave.Data_type.Time.Seconds ;
    x1y1.Box.x1 =  100;//È¡µã
    x1y1.Box.y1 =  100;//È¡µ
    x2y2.Box.x2=  500;//È¡µã
    x2y2.Box.y2 = 400 ;//È¡µã
    Xdiv=80;
    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//±êÌâ
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //±êÌâ¿òÌîÉ«
    LCD_SetTextColor(LCD_COLOR_WHITE);

    Ydiv=24;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Ê±¼äÉèÖÃ",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "Äê",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+Ydiv, "ÔÂ",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "ÈÕ",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*4, "Ê±",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "·Ö",FONTTYPE16);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*7, "Ãë",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Time Setting");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, "Year");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv, "Mon");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*4, "Date");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*4, "Hour");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2+Ydiv*7, "Min");
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2+Xdiv*2+15, x1y1.Box.y1+2+Ydiv*7, "Sec");
    }
}
void NumFrameOtur(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    char  ptr[5] = "     ";
    static struct RDispPara DisLogOld;
    DisLogOld = *RD;
    RD->Num.fval=0;
    RD->Num.Decimalbit = 0;
    RD->Num.Changflag = 0;
    RD->Num.bit = 0;
    RD->Num.dot=0;
    RD->sel = 1;
    RD->selold = 0;
    LCD_DisplayNumLine_EN_CH_LJ_Home(RD,RD->Box.x1+2,RD->Box.y1+2,ptr,Font20);
    do
    {
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyTEMPNum(keytmp,RD,&DisLogOld,cur,ptr);
        NumFloat2CharS(&RD->Num,ptr);
        LCD_DisplayNumLine_EN_CH_LJ(RD,ptr,LCD_COLOR_WHITE,LCD_COLOR_HUI,Font20);
    }
    while(keytmp!=KEY_ENTER&&keytmp!=KEY_ESC);
    RD->sel = 1;
    RD->selold = 0;
    LCD_DisplayNumLine_EN_CH_LJ_Home(RD,RD->Box.x1+2,RD->Box.y1+2,ptr,Font20);
}

//Ê±¼äÏÂÀ­¿òµÄ¼üÖµ´¦Àíº¯Êý
void ModifyDT(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCol,char MaxCam)
{
    OS_ERR err;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
            }
            break;
        case KEY_UP://ÉÏ
            DisDT[cur->COL][cur->CAM].selold=1;
            DisDT[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->COL==0)
                    cur->COL= MaxCol-1;
                else
                    cur->COL--;
            }
            while(DisDT[cur->COL][cur->CAM].disp!=1);  //²»ÏÔÊ¾Ìø¹ý
            DisDT[cur->COL][cur->CAM].selold=0;
            DisDT[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_DOWN:
            DisDT[cur->COL][cur->CAM].selold=1;
            DisDT[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDT[cur->COL][cur->CAM].disp!=1);  //²»ÏÔÊ¾Ìø¹ý
            DisDT[cur->COL][cur->CAM].selold=0;
            DisDT[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_LEFT://ÉÏ
            DisDT[cur->COL][cur->CAM].selold=1;
            DisDT[cur->COL][cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisDT[cur->COL][cur->CAM].disp!=1);  //²»ÏÔÊ¾Ìø¹ý
            DisDT[cur->COL][cur->CAM].selold=0;
            DisDT[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisDT[cur->COL][cur->CAM].selold=1;
            DisDT[cur->COL][cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisDT[cur->COL][cur->CAM].disp!=1);  //²»ÏÔÊ¾Ìø¹ý
            DisDT[cur->COL][cur->CAM].selold=0;
            DisDT[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ACC:
            DisDT[cur->COL][cur->CAM].selold=1;
            DisDT[cur->COL][cur->CAM].sel=0;
            do
            {
                cur->COL+=2;
                if(cur->COL>MaxCol-1)
                    cur->COL =0 ;
            }
            while(DisDT[cur->COL][cur->CAM].disp!=1);  //²»ÏÔÊ¾Ìø¹ý
            DisDT[cur->COL][cur->CAM].selold=0;
            DisDT[cur->COL][cur->CAM].sel=1;
            break;
        case KEY_ENTER:
            if(cur->COL<3)
            {
                NumFrameOtur(keytmp,&DisDT[cur->COL][cur->CAM],cur);
            }
            else if(cur->CAM==0&&cur->COL==3)
            {

                RD->index[RD->cind] = DisDT[0][0].index[DisDT[0][0].cind];
            }
            else if(cur->CAM==1&&cur->COL==3)
            {

                RD->index[RD->cind] = RDOld->index[RDOld->cind];
            }
            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }


}

// ±äÕûÊý
void F2UintChar(char* ptr,uint8 setoff)
{
    uint8 i;
    for(i=0; i<setoff; i++)
    {
        if(ptr[i]=='.')
        {
            ptr[i]='\0';

        }
        else if(ptr[i]=='+')
        {
            ptr[i]=' ';
        }
    }
    ptr[i]='\0';
}


/**
  * @brief  ÉèÖÃÊ±¼äºÍÈÕÆÚ
  * @param  ÎÞ
  * @retval ÎÞ
  */
void RTC_TimeAndDate_Set_Otur(RTC_DateTypeDef  Date,RTC_TimeTypeDef  Time)
{
    RTC_DateTypeDef  RTC_DateStructure;
    RTC_TimeTypeDef  RTC_TimeStructure;
    // ³õÊ¼»¯Ê±¼ä
    RTC_TimeStructure.TimeFormat = RTC_H12_AMorPM;
    RTC_TimeStructure.Hours = Time.Hours;
    RTC_TimeStructure.Minutes = Time.Minutes;
    RTC_TimeStructure.Seconds = Time.Seconds ;
    HAL_RTC_SetTime(&Rtc_Handle,&RTC_TimeStructure, RTC_FORMAT_BIN);
    // ³õÊ¼»¯ÈÕÆÚ
    RTC_DateStructure.WeekDay = WEEKDAY;
    RTC_DateStructure.Date = Date.Date;
    RTC_DateStructure.Month = Date.Month;
    RTC_DateStructure.Year = Date.Year;
    HAL_RTC_SetDate(&Rtc_Handle,&RTC_DateStructure, RTC_FORMAT_BIN);

    HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX, RTC_BKP_DATA);
}


//Ê±¼ä¶Ô»°¿ò
void DateTimeDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur)
{
    OS_ERR  err;
    uint8 m,n;
    uint16 Xdiv=60;
    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    static struct CUR Cur;
    char ptr[10] = {0};
    Cur.CAM=0;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  100;//È¡µã
    x1y1.Box.y1 =  100;//
    x2y2.Box.x2=  500;//È¡µã
    x2y2.Box.y1 = 376;//È¡µã
    x2y2.Box.y2 = 400;//È¡µã
    DTDiaInit(keytmp,RD, cur) ;
    Xdiv=80;
    do
    {
        Ydiv=24;
        Disp_Box_Home(&DisDT[0][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[0][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[1][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[1][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv*4,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*5,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[2][0],x1y1.Box.x1+Xdiv,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv+80,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[2][1],x1y1.Box.x1+Xdiv*3,x1y1.Box.y1+2+Ydiv*7,x1y1.Box.x1+Xdiv*3+80,x1y1.Box.y1+2+Ydiv*8,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[3][0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisDT[3][1],x1y1.Box.x1+Xdiv*3,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*3+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Ydiv=32;
        LCD_SetFont(&Font20);
        for(m=0; m<DiaDTCOL; m++)
            for(n=0; n<DiaDTCAM; n++)
            {

                if(m<3)
                {
                    NumFloat2CharS(&DisDT[m][n].Num,ptr);
                    F2UintChar(ptr,4);
                    LCD_DisplayNumLine_EN_CH_LJ_Home(&DisDT[m][n],DisDT[m][n].Box.x1+2,DisDT[m][n].Box.y1+2,ptr,Font20);
                }
                else
                {
                    if(CHI == DataSave.Data_type.LANG )
                        LCD_DisplayStringLineLJ_Home(&DisDT[m][n],DisDT[m][n].Box.x1+15,DisDT[m][n].Box.y1+2,FONTTYPE16);
                    else
                        LCD_DisplayStringLine_EN_CH_LJ_Home(&DisDT[m][n],DisDT[m][n].Box.x1+15,DisDT[m][n].Box.y1+2);
                }
            }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyDT(keytmp,RD,&DisLogOld,&Cur,DiaDCCOL,DiaDCCAM);
    }
    while(!(keytmp==KEY_ENTER&&Cur.COL==3)&&keytmp!=KEY_ESC); //DCµÄ¶Ô»°¿òµÚ4ÐÐÊÇÈ·¶¨ºÍÈ¡Ïû°´Å¥
    OSSchedLock(&err);
    DataSave.Data_type.Date.Year = (uint8)DisDT[0][0].Num.fval; //Äê
    DataSave.Data_type.Date.Month = (uint8)DisDT[0][1].Num.fval ;//ÔÂ
    DataSave.Data_type.Date.Date =(uint8)DisDT[1][0].Num.fval;//ÈÕ
    DataSave.Data_type.Time.Hours =  (uint8)DisDT[1][1].Num.fval;
    DataSave.Data_type.Time.Minutes = (uint8)DisDT[2][0].Num.fval ;
    DataSave.Data_type.Time.Seconds = (uint8)DisDT[2][1].Num.fval ;
    RTC_TimeAndDate_Set_Otur(DataSave.Data_type.Date, DataSave.Data_type.Time);
    Stu_MinOtur_Init();
    ModifyDT(keytmp,RD,&DisLogOld,&Cur,DiaDTCOL,DiaDTCAM);
    page_Otur();
    SaveAllPara();
    OSSchedUnlock(&err);


}

u32 SetSaveStamp(void)
{
    u32 Stamp  = 0;
    SaveFlag=0;//ÆÁ±£ÖØÐÂ¼ÇÊ±
    if(DataSave.Data_type.ScrSaver==1)  //10s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+10;
    else if(DataSave.Data_type.ScrSaver==2)  //30s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+30;
    else if(DataSave.Data_type.ScrSaver==3)  //60s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+60;
    else if(DataSave.Data_type.ScrSaver==4)  //120s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+120;
    else if(DataSave.Data_type.ScrSaver==5)  //300s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+300;
    else if(DataSave.Data_type.ScrSaver==6)  //600s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+600;
    else if(DataSave.Data_type.ScrSaver==7)  //1800s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+1800;
    else if(DataSave.Data_type.ScrSaver==8)  //3600s
        Stamp = time2Stamp(RTC_DateStr, RTC_TimeStr)+3600;
    return Stamp  ;
}
void FlashUnit(void)
{
    u8 m,n;
    for(m=0; m<DCOL; m++)
    {
        for(n=0; n<DCAM; n++)
        {
            DDa[m][n].Uint[VT].Col = DataSave.Data_type.TUnit;
        }
    }
}

void ReturnST_Init(void)
{
    uint8 m,i;
    char buf[6]= {0};
    memset(DataSave.uData,0,sizeof(DataSave.uData));
    DataSave.Data_type.SW=0;
    DataSave.Data_type.SampleCAM=0;
    DataSave.Data_type.ZoneCAM=0;
    memset(DataSave.Data_type.PName,0,sizeof(DataSave.Data_type.PName));
    strcpy(DataSave.Data_type.PName,"JK350\0");
    for(m=0; m<CHANNUM+1; m++)
    {
		if(m<CHANNUM)
		{
			sprintf(buf,"CH%02d",m+1);
			strcpy(DataSave.Data_type.ChName[m],buf);
		}
        
        DataSave.Data_type.TraceF[m] = OFF;  //¸ú×Ù¹ØÁË
        DataSave.Data_type.ComputeT[m] = OFF;  //²¹³¥¼ÆËã
        DataSave.Data_type.ComputeDC[m] = OFF;  //²¹³¥¼ÆËã
        DataSave.Data_type.Alarm[m] = OFF;
        DataSave.Data_type.TempAdjMeas[m].bit=4;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.TempAdjMeas[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.TempAdjMeas[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.TempAdjMeas[m].fval = TEMPMAXVAL;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.TempAdjEu[m].bit=4;
        DataSave.Data_type.TempAdjEu[m].Decimalbit=0;
        DataSave.Data_type.TempAdjEu[m].Symbol=0;
        DataSave.Data_type.TempAdjEu[m].fval =TEMPMAXVAL;
		
        DataSave.Data_type.DCUpMeas[m].bit=1;
        DataSave.Data_type.DCUpMeas[m].Decimalbit=0;
        DataSave.Data_type.DCUpMeas[m].Symbol=0;
        DataSave.Data_type.DCUpMeas[m].fval = 1;
        DataSave.Data_type.DCUpEu[m].bit=1;
        DataSave.Data_type.DCUpEu[m].Decimalbit=0;
        DataSave.Data_type.DCUpEu[m].Symbol=0;
        DataSave.Data_type.DCUpEu[m].fval = 1;
        DataSave.Data_type.DCLowMeas[m].bit=1;
        DataSave.Data_type.DCLowMeas[m].Decimalbit=0;
        DataSave.Data_type.DCLowMeas[m].Symbol=0;
        DataSave.Data_type.DCLowMeas[m].fval = 1;
        DataSave.Data_type.DCLowEu[m].bit=1;
        DataSave.Data_type.DCLowEu[m].Decimalbit=0;
        DataSave.Data_type.DCLowEu[m].Symbol=0;
        DataSave.Data_type.DCLowEu[m].fval = 1;

      
//  ×ø±êyÖáµÄ·¶Î§
        DataSave.Data_type.TempMax[m].bit=4;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.TempMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.TempMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.TempMax[m].fval =TEMPMAXVAL;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.TempMin[m].bit=4;
        DataSave.Data_type.TempMin[m].Decimalbit=0;
        DataSave.Data_type.TempMin[m].Symbol=0;
        DataSave.Data_type.TempMin[m].fval =TEMPMINVAL;
        if(DataSave.Data_type.TempMax[m].fval-DataSave.Data_type.TempMin[m].fval<=1)
        {
            DataSave.Data_type.TempMax[m].fval =TEMPMAXVAL;
            DataSave.Data_type.TempMin[m].fval =TEMPMINVAL;
        }
        DataSave.Data_type.DCMax[m].bit=1;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.DCMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.DCMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.DCMax[m].fval = 1;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.DCMin[m].bit=1;
        DataSave.Data_type.DCMin[m].Decimalbit=0;
        DataSave.Data_type.DCMin[m].Symbol=0;
        DataSave.Data_type.DCMin[m].fval =-1;

        if(DataSave.Data_type.DCMax[m].fval-DataSave.Data_type.DCMin[m].fval<=(float)0.1)
        {
            DataSave.Data_type.DCMax[m].fval = 1;
            DataSave.Data_type.DCMin[m].fval =-1;
        }


        DataSave.Data_type.DIMax[m].bit=1;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.DIMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.DIMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.DIMax[m].fval = 1;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.DIMin[m].bit=1;
        DataSave.Data_type.DIMin[m].Decimalbit=0;
        DataSave.Data_type.DIMin[m].Symbol=0;
        DataSave.Data_type.DIMin[m].fval =-1;

        if(DataSave.Data_type.DIMax[m].fval-DataSave.Data_type.DIMin[m].fval<=(float)0.1)
        {
            DataSave.Data_type.DIMax[m].fval = 1;
            DataSave.Data_type.DIMin[m].fval =-1;
        }
        //±¨¾¯ÉÁË¸µÄ×î´ó×îÐ¡Öµ
        DataSave.Data_type.TempLMax[m].bit=4;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.TempLMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.TempLMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.TempLMax[m].fval =TEMPMAXVAL;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.TempLMin[m].bit=4;
        DataSave.Data_type.TempLMin[m].Decimalbit=0;
        DataSave.Data_type.TempLMin[m].Symbol=0;
        DataSave.Data_type.TempLMin[m].fval =TEMPMINVAL;
        if(DataSave.Data_type.TempLMax[m].fval-DataSave.Data_type.TempLMin[m].fval<=(float)1)
        {
            DataSave.Data_type.TempLMax[m].fval =TEMPMAXVAL;
            DataSave.Data_type.TempLMin[m].fval =TEMPMINVAL;
        }
        DataSave.Data_type.DCLMax[m].bit=1;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.DCLMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.DCLMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.DCLMax[m].fval = 1;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.DCLMin[m].bit=1;
        DataSave.Data_type.DCLMin[m].Decimalbit=0;
        DataSave.Data_type.DCLMin[m].Symbol=0;
        DataSave.Data_type.DCLMin[m].fval =-1;
        if(DataSave.Data_type.DCLMax[m].fval-DataSave.Data_type.DCLMin[m].fval<=(float)0.1)
        {
            DataSave.Data_type.DCLMax[m].fval = 1;
            DataSave.Data_type.DCLMin[m].fval =-1;
        }
        DataSave.Data_type.DILMax[m].bit=1;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.DILMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.DILMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.DILMax[m].fval = 1;   //  ÓÐ·ûºÅ¸¡µãÐÍ 2000.0
        DataSave.Data_type.DILMin[m].bit=1;
        DataSave.Data_type.DILMin[m].Decimalbit=0;
        DataSave.Data_type.DILMin[m].Symbol=0;
        DataSave.Data_type.DILMin[m].fval =-1;
        if(DataSave.Data_type.DILMax[m].fval-DataSave.Data_type.DILMin[m].fval<=(float)0.1)
        {
            DataSave.Data_type.DILMax[m].fval = 1;
            DataSave.Data_type.DILMin[m].fval =-1;
        }


        DataSave.Data_type.VGLMax[m].bit=1;           //ÕûÊýÎ»¸öÊý
        DataSave.Data_type.VGLMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
        DataSave.Data_type.VGLMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
        DataSave.Data_type.VGLMin[m].bit=1;
        DataSave.Data_type.VGLMin[m].Decimalbit=0;
        DataSave.Data_type.VGLMin[m].Symbol=0;

        if(DataSave.Data_type.VGLMax[m].fval-DataSave.Data_type.VGLMin[m].fval<=0.1F)
        {
            DataSave.Data_type.VGLMax[m].fval = 100;
            DataSave.Data_type.VGLMin[m].fval = 0;
        }
		DataSave.Data_type.VRLMax[m].bit=1;           //ÕûÊýÎ»¸öÊý
		DataSave.Data_type.VRLMax[m].Decimalbit=0;   //Ð¡ÊýÎ»¸öÊý
		DataSave.Data_type.VRLMax[m].Symbol=0;     //·ûºÅ 0 ÎªÕý 1Îª¸ººÅ
		DataSave.Data_type.VRLMin[m].bit=1;
		DataSave.Data_type.VRLMin[m].Decimalbit=0;
		DataSave.Data_type.VRLMin[m].Symbol=0;
		for(i=0;i<VRVGSensorCOL;i++)
		{

			DataSave.Data_type.ComputeDI[i][m] = ON;  //²¹³¥¼ÆËã
			DataSave.Data_type.ComputeVR[i][m] = ON;  //²¹³¥¼ÆËã
			DataSave.Data_type.ComputeVG[i][m] = ON;  //²¹³¥¼ÆËã
			
			DataSave.Data_type.DIUpMeas[i][m].bit=1;
			DataSave.Data_type.DIUpMeas[i][m].Decimalbit=0;
			DataSave.Data_type.DIUpMeas[i][m].Symbol=0;
			DataSave.Data_type.DIUpMeas[i][m].fval = 1;
			DataSave.Data_type.DIUpEu[i][m].bit=1;
			DataSave.Data_type.DIUpEu[i][m].Decimalbit=0;
			DataSave.Data_type.DIUpEu[i][m].Symbol=0;
			DataSave.Data_type.DIUpEu[i][m].fval = 1;
			
			
			DataSave.Data_type.DILowMeas[i][m].bit=1;
			DataSave.Data_type.DILowMeas[i][m].Decimalbit=0;
			DataSave.Data_type.DILowMeas[i][m].Symbol=0;
			DataSave.Data_type.DILowMeas[i][m].fval = 1;
			DataSave.Data_type.DILowEu[i][m].bit=1;
			DataSave.Data_type.DILowEu[i][m].Decimalbit=0;
			DataSave.Data_type.DILowEu[i][m].Symbol=0;
			DataSave.Data_type.DILowEu[i][m].fval = 1;
			
			DataSave.Data_type.VGUpA[i][m].bit=1;
			DataSave.Data_type.VGUpA[i][m].Decimalbit=0;
			DataSave.Data_type.VGUpA[i][m].Symbol=0;
			DataSave.Data_type.VGUpA[i][m].fval = 1.0F;

			DataSave.Data_type.VGUpB[i][m].bit=1;
			DataSave.Data_type.VGUpB[i][m].Decimalbit=0;
			DataSave.Data_type.VGUpB[i][m].Symbol=0;
			DataSave.Data_type.VGUpB[i][m].fval = 0.0F;

			DataSave.Data_type.VGLowA[i][m].bit=1;
			DataSave.Data_type.VGLowA[i][m].Decimalbit=0;
			DataSave.Data_type.VGLowA[i][m].Symbol=0;
			DataSave.Data_type.VGLowA[i][m].fval = 1.0F;

			DataSave.Data_type.VGLowB[i][m].bit=1;
			DataSave.Data_type.VGLowB[i][m].Decimalbit=0;
			DataSave.Data_type.VGLowB[i][m].Symbol=0;
			DataSave.Data_type.VGLowB[i][m].fval = 0.0F;
			

		   
			
			DataSave.Data_type.VRUpA[i][m].bit=3;
			DataSave.Data_type.VRUpA[i][m].Decimalbit=0;
			DataSave.Data_type.VRUpA[i][m].Symbol=0;
			DataSave.Data_type.VRUpA[i][m].fval = 100.0F;

			DataSave.Data_type.VRUpB[i][m].bit=3;
			DataSave.Data_type.VRUpB[i][m].Decimalbit=0;
			DataSave.Data_type.VRUpB[i][m].Symbol=0;
			DataSave.Data_type.VRUpB[i][m].fval = 100.0F;

			DataSave.Data_type.VRLowA[i][m].bit=1;
			DataSave.Data_type.VRLowA[i][m].Decimalbit=0;
			DataSave.Data_type.VRLowA[i][m].Symbol=0;
			DataSave.Data_type.VRLowA[i][m].fval = 0.0F;

			DataSave.Data_type.VRLowB[i][m].bit=1;
			DataSave.Data_type.VRLowB[i][m].Decimalbit=0;
			DataSave.Data_type.VRLowB[i][m].Symbol=0;
			DataSave.Data_type.VRLowB[i][m].fval = 0.0F;
		
		}
       
		
		
		 if(DataSave.Data_type.VRLMax[m].fval-DataSave.Data_type.VRLMin[m].fval<=0.1F)
        {
            DataSave.Data_type.VRLMax[m].fval = 100;
            DataSave.Data_type.VRLMin[m].fval = 0;
        }
    }
}

//ÌáÊ¾¶Ô»°¿ò
static uint8 TiShiDia(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 type)
{
    OS_ERR  err;
    uint8 n;
    uint16 Xdiv=60;
//    uint16 Ydiv=32;
    static struct RDispPara DisLogOld;
    struct RDispPara x1y1;
    struct RDispPara x2y2;
    static struct CUR Cur;
   // char ptr[10] = {0};
    Cur.CAM=1;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  100;//È¡µã
    x1y1.Box.y1 =  100;//
    x2y2.Box.x2=  500;//È¡µã
    x2y2.Box.y1 = 276;//È¡µã
    x2y2.Box.y2 = 300;//È¡µã
    TiShiDiaInit(keytmp,RD, cur,type) ;
    Xdiv=80;
//    Ydiv=24;

    do
    {
//        Ydiv=24;
        Disp_Box_Home(&DisTiShi[0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisTiShi[1],x1y1.Box.x1+Xdiv*3,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*3+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
//        Ydiv=32;
        LCD_SetFont(&Font20);
        for(n=0; n<DisTiShiCAM; n++)
        {

            if(CHI == DataSave.Data_type.LANG )
                LCD_DisplayStringLineLJ_Home(&DisTiShi[n],DisTiShi[n].Box.x1+15,DisTiShi[n].Box.y1+2,FONTTYPE16);
            else
                LCD_DisplayStringLine_EN_CH_LJ_Home(&DisTiShi[n],DisTiShi[n].Box.x1+15,DisTiShi[n].Box.y1+2);

        }
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        ModifyTiShi(keytmp,RD,&DisLogOld,&Cur,DisTiShiCAM);
    }
    while(!(keytmp==KEY_ENTER)&&keytmp!=KEY_ESC);  //
    OSSchedLock(&err);
    Stu_MinOtur_Init();
    ModifyTiShi(keytmp,RD,&DisLogOld,&Cur,DisTiShiCAM);
    page_Otur();
    OSSchedUnlock(&err);
    return Cur.CAM;
}



uint8 ModifyOtur(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // ¼üÖµÆ¥Åäº¯Êý£¬ÔÚwhile´óÑ­»·ÖÐ·´¸´Æ¥Åä
{
    OS_ERR err;
    *curold=*cur;
    cur->OPN=keytmp;                        //½«¼üÖµ¸³¸ø²Ù×÷±êÖ¾£¬±êÃ÷µ±Ç°µÄ²Ù×÷×´Ì¬
    switch(cur->OPN)
    {
        case KEY_FUNC1://½øÈëÏÔÊ¾ÉèÖÃ

            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
        case KEY_FUNC2://½øÈëdata ´¦Àí
            DataSave.Data_type.PageFlag =  FPageHDATA ;
            break;
        case KEY_FUNC3:
            DataSave.Data_type.PageFlag =  FPageFile ;
            break;
        case KEY_FUNC4:// ½Ó¿Ú
            DataSave.Data_type.PageFlag =  FPageIF  ;
            break;
        case KEY_FUNC5://½øÈëÉèÖÃ

            break;
        case      KEY_ESC:
            DataSave.Data_type.PageFlag =  PageFlagSetRe ;
            break;
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
            }
            break;
        case KEY_UP://ÉÏ
            if(cur->COL>0)
                cur->COL--;
            else
                cur->COL=OturCOL-1;
            break;

        case KEY_DOWN:
            if(cur->COL<OturCOL-1) //×îºóÒ»ÐÐÊÇ¹¦ÄÜ¼ü
                cur->COL++;
            else
                cur->COL=0;
            break;

        case KEY_ACC:
            cur->COL+=2;
            if(cur->COL>OturCOL-1)
                cur->COL=1;
            break;

        case KEY_ENTER:
            if(cur->COL==0)  //µÚÒ»ÐÐ
            {
                ComBoxS(keytmp,&DisOtur[cur->COL],cur,BrightnessCOL);
                DataSave.Data_type.Bright=DisOtur[cur->COL].index[DisOtur[cur->COL].cind] ;
                SaveAllPara();
            }
            else if(cur->COL==1)
            {
                ComBoxS(keytmp,&DisOtur[cur->COL],cur,SaverOnOffCOL);
                DataSave.Data_type.ScrSaver=DisOtur[cur->COL].index[DisOtur[cur->COL].cind] ;
                SaveAllPara();
                SaveStamp =  SetSaveStamp();
            }
            else if(cur->COL==2)
            {
                ComBoxS(keytmp,&DisOtur[cur->COL],cur,BeepOnCOL);
                DataSave.Data_type.BeepF=DisOtur[cur->COL].index[DisOtur[cur->COL].cind] ;
                SaveAllPara();

            }
            else if(cur->COL==3)
            {
                ComBoxS(keytmp,&DisOtur[cur->COL],cur,RoomCOL);
                DataSave.Data_type.RoomT=DisOtur[cur->COL].index[DisOtur[cur->COL].cind] ;
                SaveAllPara();
            }
            else if(cur->COL==4)
            {
                ComBoxS(keytmp,&DisOtur[cur->COL],cur,UnitCOL);
                DataSave.Data_type.TUnit=DisOtur[cur->COL].index[DisOtur[cur->COL].cind] ;
                FlashUnit();
                SaveAllPara();
            }
            else if(cur->COL==6)
            {

                ComBoxLang(keytmp,&DisOtur[cur->COL],cur,LangCOL);

            }
            else if(cur->COL==7)
            {
                ComBoxS(keytmp,&DisOtur[cur->COL],cur,ReturnCOL);
                if(DisOtur[cur->COL].index[DisOtur[cur->COL].cind] ==ON)  //¸ñÊ½»¯SD
                {
                    if(TiShiDia(keytmp,&DisOtur[cur->COL],cur,1)==0)
                    {
                        ReturnST_Init();
                    }
                }
                DisOtur[cur->COL].index[DisOtur[cur->COL].cind]  =OFF;
            }
            else if(cur->COL==8)
            {
                InForDia(keytmp,&DisOtur[cur->COL],cur);
            }
            else if(cur->COL==5)
            {
                DateTimeDia(keytmp,&DisOtur[cur->COL],cur);
            }
            break;
    }
    Stu_Otur_cycle(cur,curold);
    return keytmp;
}


//ÉèÖÃ²Ëµ¥ÈÎÎñ
void DoOtur_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //Ö÷Ò³
    Stu_Otur_Init();
    page_Otur();
    while(1)
    {
        //DoOturÈÎÎñ
//        BluidICOTask();
        keytmp = KeyTaskCreate(NoNull_ReTurn);
//        OSTaskDel(&ICOTaskTCB,&err);
        keytmp=ModifyOtur(keytmp,&CurOtur,&CurOturOld);     //´¦Àí¼üÖµ
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
            //É¾³ý×Ô¼ºÈÎÎñ
        }
        page_Otur_cycle();

    }
}

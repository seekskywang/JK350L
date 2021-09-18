/*
  * @file    File.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   �ļ����������߻ؿ�
 */

#include  "include.h"
#include "cpu.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
//������ƿ�
OS_TCB DoFileTaskTCB;
//�����ջ
CPU_STK DoFile_TASK_STK[DoFile_STK_SIZE];
//�α�  ���к���
struct CUR CurFile = {0};
struct CUR CurFileOld= {0};
//����bin�ļ����������
uint8 FileMaxCol = 0;
//�ļ��б���ҳ��
uint8 FilePage = 0;
//��ҳ�˵���������
struct RDispPara DisFile[FileCOL][FileCAM] = {0};

struct RDispPara DisTiShi[DisTiShiCAM] = {0};//�ӶԻ����ѹ�Ի���Ŀؼ�����
//0 ���ļ��� 1��ʱ�� 2���ļ���С
static struct FileStr FileEn[3] = {0};
static const struct HelpStr helpS[1]=
{
    {"��ʾ:�ļ�������̫�������Ҽ���ҳ��\0"," Name cannot be too long,L or R keys can turn pages.\0"},
};
static const struct HelpStr NoteS[1]=
{
    {"�����ļ��Ѿ����:\0","Export file completed:\0"},
};


char SaveTrig = TRIGReady;
u8 usbstatus = UNKNOWN;
extern uint8_t sndflag;
static char SDPath[4]= {0}; /* SD�߼�������·�� */
FATFS fs;                                                   /* FatFs�ļ�ϵͳ���� */
FIL fnew;                                                   /* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;                        /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]= {0};       /* �������� */
char FileRes = FILEReady;                /* ״̬ 0�ر� 1��tf�ļ� 2��usb�ļ�*/
extern FATFS flash_fs;
char filenameUSB[20]= {0};
extern Diskio_drvTypeDef  SD_Driver;
u8 FileHao=0;

//����
void Screen_shot_srand(const char* head,const char* type)
{
    FRESULT res_sd;
    char ALLbuf[12] = {0};
    if(FileRes ==FILEReady)
    {
        res_sd = f_mount(&fs,"0:",1);
        /*----------------------- ��ʽ������ ---------------------------*/
        /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
        if(res_sd == FR_NO_FILESYSTEM)
        {
            /* ��ʽ�� */
            res_sd=f_mkfs("0:",0,0);
            if(res_sd == FR_OK)
            {
                /* ��ʽ������ȡ������ */
                res_sd = f_mount(NULL,"0:",1);
                /* ���¹��� */
                res_sd = f_mount(&fs,"0:",1);
            }
        }
        GetNewName("0:\0",ALLbuf,head);
        strcat(ALLbuf,type);
        res_sd = f_open(&fnew, (TCHAR*)ALLbuf,FA_CREATE_ALWAYS | FA_WRITE|FA_READ );
        if(res_sd==FR_OK)
        {
            FileRes = FILETFOpen;
            Screen_shot(0,0,640,480,(uint8*)ALLbuf);
            FileRes = FILEReady;
        }
    }
}

//��ʼ��
uint8 FileInit(void)
{
    return FATFS_LinkDriver(&SD_Driver, SDPath);
}

u8 NewFile(const char* head,const char* type)
{
    FRESULT res_sd;
    u8 i=0;
    char ALLbuf[12] = {0};
    if(FileRes ==FILEReady)
    {
        res_sd = f_mount(&fs,"0:",1);
        /*----------------------- ��ʽ������ ---------------------------*/
        /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
        if(res_sd == FR_NO_FILESYSTEM)
        {
            /* ��ʽ�� */
            res_sd=f_mkfs("0:",0,0);

            if(res_sd == FR_OK)
            {
                /* ��ʽ������ȡ������ */
                res_sd = f_mount(NULL,"0:",1);
                /* ���¹��� */
                res_sd = f_mount(&fs,"0:",1);
            }
        }
        i=GetNewName("0:\0",ALLbuf,head);
        strcat(ALLbuf,type);
        res_sd = f_open(&fnew, (TCHAR*)ALLbuf,FA_CREATE_ALWAYS | FA_WRITE|FA_READ );
        if(res_sd==FR_OK)
            FileRes = FILETFOpen;


    }
    return i;
}

void TrigFile(void)
{
    u8 i=0;
    if(SaveTrig == TRIGStart)
    {
        if(usbstatus == CONNECTED)
        {
            InitUSBEXL();
            FileRes = FILEUSBOpen;
        }
        else
        {
            i=NewFile("NB",".BIN");
            FileHao= i;//����ļ���
        }
        FileSaveNum = 0;
        SaveTrig = TRIGReady ;
    }
    else if(SaveTrig == TRIGStopTF)
    {
        EndSD();
        SaveTrig = TRIGReady ;
    }
    else if(SaveTrig == TRIGStopUSB)
    {
        FileRes = FILEReady;
        SaveTrig = TRIGReady ;
    }
}


void IsReTrigFile(uint32 FileSaveNum)
{
    if(FileSaveNum>SDSaveMAX&&FileRes == FILETFOpen)
    {
	EndSD();
	FileRes = FILEReady;
	SaveTrig = TRIGStart;
    }
    else if(FileSaveNum>USBSaveMAX&&FileRes == FILEUSBOpen)
    {
       FileRes = FILEReady;
       SaveTrig = TRIGStart;
    }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

uint32 testcnt = 0;
void SaveSD(void)
{

    unsigned char uD[SDLEN]= {0};
    TestStatus sta=FAILED;
    uint8 rcnt=0;
    uint8 rcnt1=0;
    // uint32 m;
    // m = sizeof(Dsd);
    if(FileRes == FILETFOpen&&(usbstatus != CONNECTED))
    {
        rcnt=0;
        while(f_lseek(&fnew, f_size(&fnew))!=FR_OK)
        {
            if(rcnt++>100)return;
        };
        rcnt=0;
        while(f_sync(&fnew)!=FR_OK)
        {
            if(rcnt++>100)return;
        };
        do
        {
            rcnt=0;
            while(f_write(&fnew,Dsd.uD,sizeof(Dsd.uD),&fnum)!=FR_OK)
            {
                if(rcnt++>100)
                    return;
            };
            rcnt=0;
            while(f_lseek(&fnew, f_size(&fnew)-sizeof(Dsd.uD))!=FR_OK)
            {
                if(rcnt++>100)return;
            };
            rcnt=0;
            while(f_read(&fnew, uD, sizeof(uD), &fnum)!=FR_OK)
            {
                if(rcnt++>100)return;
            };
            sta=Buffercmp(Dsd.uD, uD, sizeof(Dsd.uD));
            if(rcnt1++>20)return;
        }
        while(sta==FAILED);
        if(sta!=FAILED)
            FileSaveNum++;
    }
}


void SaveSDFM(void)
{

    static unsigned char uD[SDLEN*5]= {0};
    static unsigned char uDOld[SDLEN*5]= {0};
//  static unsigned char uD[SDLEN]= {0};
    TestStatus sta=FAILED;
    uint8 rcnt=0;
    uint8 rcnt1=0;
    uint8 i,j;
    if(FileRes == FILETFOpen&&(usbstatus != CONNECTED))
    {
        rcnt=0;
        while(f_lseek(&fnew, f_size(&fnew))!=FR_OK)
        {
            if(rcnt++>100)return;
        };
        rcnt=0;
        while(f_sync(&fnew)!=FR_OK)
        {
            if(rcnt++>100)return;
        };
//	for(j=0; j<5; j++)
//	{
//        do
//        {
//            memset(uD,0, sizeof(uD));
//            memset(uDOld,0, sizeof(uDOld));
//            for(i=0; i<2; i++)
//            {
//                memcpy(uD+SDLEN*i,DsdFM[i+j*2].uD,sizeof(DsdFM[i+].uD));
//            }
//            rcnt=0;
//            while(f_write(&fnew,uD,sizeof(uD),&fnum)!=FR_OK)
//            {
//                if(rcnt++>100)break;
//            };
//            if(rcnt>100) continue;
//            rcnt=0;
//            while(f_lseek(&fnew, f_size(&fnew)-sizeof(uD))!=FR_OK)
//            {
//                if(rcnt++>100)break;
//            };
//            if(rcnt>100) continue;
//            rcnt=0;
//            while(f_read(&fnew, uDOld, sizeof(uDOld), &fnum)!=FR_OK)
//            {
//                if(rcnt++>100)break;
//            };
//            if(rcnt>100) continue;
//            sta=Buffercmp(uD, uDOld, sizeof(uDOld));
//            if(rcnt1++>50)
//                break;
//        }
//        while(sta==FAILED);
//        if(sta!=FAILED)
//            FileSaveNum+=2;
//	}
//        do
//        {
//            memset(uD,0, sizeof(uD));
//            memset(uDOld,0, sizeof(uDOld));
//            for(i=0; i<5; i++)
//            {
//                memcpy(uD+SDLEN*i,DsdFM[i+5].uD,sizeof(DsdFM[i+5].uD));
//            }
//            rcnt=0;
//            while(f_write(&fnew,uD,sizeof(uD),&fnum)!=FR_OK)
//            {
//                if(rcnt++>100)break;
//            };
//            if(rcnt>100) continue;
//            rcnt=0;
//            while(f_lseek(&fnew, f_size(&fnew)-sizeof(uD))!=FR_OK)
//            {
//                if(rcnt++>100)break;
//            };
//            if(rcnt>100) continue;
//            rcnt=0;
//            while(f_read(&fnew, uDOld, sizeof(uDOld), &fnum)!=FR_OK)
//            {
//                if(rcnt++>100)break;
//            };
//            if(rcnt>100) continue;
//            sta=Buffercmp(uD, uDOld, sizeof(uDOld));
//            if(rcnt1++>50)
//                break;
//        }
//        while(sta==FAILED);
//        if(sta!=FAILED)
//            FileSaveNum+=5;
        for(i=0; i<10; i++)
        {
            memset(uD,0, sizeof(uD));
            do
            {
                rcnt=0;
                while(f_write(&fnew,DsdFM[i].uD,sizeof(DsdFM[i].uD),&fnum)!=FR_OK)
                {
                    if(rcnt++>100)break;
                };
              if(rcnt>100) continue;
                rcnt=0;
                while(f_lseek(&fnew, f_size(&fnew)-sizeof(DsdFM[i].uD))!=FR_OK)
                {
                    if(rcnt++>100)break;
                };
              if(rcnt>100) continue;
                rcnt=0;
                while(f_read(&fnew, uD, sizeof(uD), &fnum)!=FR_OK)
                {
                    if(rcnt++>100)break;
                };
              if(rcnt>100) continue;
                sta=Buffercmp(DsdFM[i].uD, uD, sizeof(DsdFM[i].uD));
                if(rcnt1++>50)
                  break;
            }while(sta==FAILED);
          if(rcnt1>50) continue;
        }
    }
}

void EndSD(void)
{
    FRESULT res_sd;
    if(FileRes == FILETFOpen)
    {
        res_sd=f_close(&fnew);
        if(res_sd==FR_OK)
            FileRes = FILEReady;
    }
}
void FormatSD(void)
{
    FRESULT res_sd;
    EndSD();
    /* ��ʽ�� */
    res_sd=f_mkfs("0:",0,0);

    if(res_sd == FR_OK)
    {

        /* ��ʽ������ȡ������ */
        res_sd = f_mount(NULL,"0:",1);
        /* ���¹��� */
        res_sd = f_mount(&fs,"0:",1);
    }
    if(res_sd != FR_OK)  //sd����ȡʧ��
    {
        FileRes = SDERR;

    }
    else
    {
        if(FileRes == SDERR)//sd���ϴζ�ȡʧ��
        {
            FileRes = FILEReady;
        }

    }
}



//��������
uint8 FatReadAllDir(char *path,char *str)
{

    DIR DirInfo;
    static FILINFO fileinfo;
    char nametemp[13]= {0};
    uint8 col=0;

    CHAR* FileName = "\0";
    double dsize =0;
    FRESULT res_sd;

    res_sd = f_mount(&fs,"0:",1);
    /*----------------------- ��ʽ������ ---------------------------*/
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if(res_sd == FR_NO_FILESYSTEM)
    {
        /* ��ʽ�� */
        res_sd=f_mkfs("0:",0,0);

        if(res_sd == FR_OK)
        {
            /* ��ʽ������ȡ������ */
            res_sd = f_mount(NULL,"0:",1);
            /* ���¹��� */
            res_sd = f_mount(&fs,"0:",1);
        }
    }
    if(res_sd != FR_OK)  //sd����ȡʧ��
    {
        FileRes = SDERR;
        return col;
    }
    else
    {
        if(FileRes == SDERR)//sd���ϴζ�ȡʧ��
        {
            FileRes = FILEReady;
        }

    }
#if _USE_LFN//���ʹ��֧�ֳ��ļ��� �ȶ����������ʼ��
    fileinfo.lfsize=_MAX_LFN * 2 + 1;//
    fileinfo.lfname=(TCHAR*)FileName;//
#endif
    col=0;
    dsize =0;
    if(f_opendir(&DirInfo,(const TCHAR*)path) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir�ṹ���б��� */
    {

        while(f_readdir(&DirInfo, &fileinfo) == FR_OK)  /* ���ļ���Ϣ���ļ�״̬�ṹ���� */
        {
            if(!fileinfo.fname[0]) break; /* ����ļ���Ϊ��\0'��˵����ȡ��ɽ��� */
            //  printf("%s/",path);//��ӡ·��
#if _USE_LFN
            //        printf("�ļ�����%s\r\n",fileinfo.lfname );//��ӡ��Ϣ������
#else
            //       printf("�ļ�����%s\r\n", fileinfo.fname);//
#endif
            memset(nametemp,0,sizeof(fileinfo.fname));
            memcpy(nametemp,fileinfo.fname,sizeof(fileinfo.fname));
            if(strstr(nametemp,".BIN")!=NULL||strstr(nametemp,".bin")!=NULL
               ||strstr(nametemp,".BMP")!=NULL||strstr(nametemp,".bmp")!=NULL)
            {
                dsize += (double)(fileinfo.fsize)*0.001;
                col++;
                if(col>=98)   //�����99���ļ�
                    break;
            }
        }
    }
    memset(str,0,sizeof(str));
    sprintf(str,"%.3lfKB",dsize);
    return col;
}




//��������
uint8 FatReadDirTest(char *path,uint8 page)
{

    DIR DirInfo;
    static FILINFO fileinfo;
    char nametemp[13]= {0};
    uint8 col;
    uint8 stacol;
    uint8 endcol;
    char str[20]= {0};
    CHAR* FileName = "\0";
    double dsize =0;
    FRESULT res_sd;
    for(col=0; col<FileCOL; col++)
    {
        memset(FileEn[0].Ch[col], 0,sizeof(FileEn[0].Ch[col]));
        memset(FileEn[1].Ch[col], 0,sizeof(FileEn[0].Ch[col]));
    }
    col=0;
    res_sd = f_mount(&fs,"0:",1);
    /*----------------------- ��ʽ������ ---------------------------*/
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if(res_sd == FR_NO_FILESYSTEM)
    {
        /* ��ʽ�� */
        res_sd=f_mkfs("0:",0,0);

        if(res_sd == FR_OK)
        {
            /* ��ʽ������ȡ������ */
            res_sd = f_mount(NULL,"0:",1);
            /* ���¹��� */
            res_sd = f_mount(&fs,"0:",1);
        }
    }
    if(res_sd != FR_OK)  //sd����ȡʧ��
    {
        FileRes = SDERR;
        return col;
    }
    else
    {
        if(FileRes == SDERR)//sd���ϴζ�ȡʧ��
        {
            FileRes = FILEReady;
        }

    }
    stacol=page*FileCOL; //��ǰҳȡ��ʵ��
#if _USE_LFN//���ʹ��֧�ֳ��ļ��� �ȶ����������ʼ��
    fileinfo.lfsize=_MAX_LFN * 2 + 1;//
    fileinfo.lfname=(TCHAR*)FileName;//
#endif
    if(f_opendir(&DirInfo,(const TCHAR*)path) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir�ṹ���б��� */
    {
        col=0;
        while(f_readdir(&DirInfo, &fileinfo) == FR_OK)  /* ���ļ���Ϣ���ļ�״̬�ṹ���� */
        {
            if(!fileinfo.fname[0]) break; /* ����ļ���Ϊ��\0'��˵����ȡ��ɽ��� */
            //  printf("%s/",path);//��ӡ·��
#if _USE_LFN
            //        printf("�ļ�����%s\r\n",fileinfo.lfname );//��ӡ��Ϣ������
#else
            //       printf("�ļ�����%s\r\n", fileinfo.fname);//
#endif
            memset(nametemp,0,sizeof(fileinfo.fname));
            memcpy(nametemp,fileinfo.fname,sizeof(fileinfo.fname));
            if(strstr(nametemp,".BIN")!=NULL||strstr(nametemp,".bin")!=NULL
               ||strstr(nametemp,".BMP")!=NULL||strstr(nametemp,".bmp")!=NULL)
            {
                if(col>=stacol)
                {
                    strcpy(FileEn[0].Ch[col-stacol], nametemp);
                    dsize =0;
                    dsize = (double)(fileinfo.fsize)*0.001;
                    memset(str,0,sizeof(str));
                    sprintf(str,"%.3lfKB",dsize);
                    strcpy(FileEn[1].Ch[col-stacol], str);
                    col++;
                    if(col>=stacol+FileCOL)
                        break;
                }
                else
                {
                    col++;
                }
                if(col>=98)   //�����99���ļ�
                    break;
            }
        }
    }
    if(col>stacol)  //���ļ�
        endcol = col-stacol;
    else        //û���ļ�
        endcol =0;
    return endcol;
}


//����SD������û�и��ļ� �з���
uint8 GetNewName(char *path,char *name,const char* head)
{
    DIR DirInfo;
    static FILINFO fileinfo;
    char nametemp[13]= {0};

    CHAR* FileName = "\0";
    uint8 i;
    char buf[13] = {0};
    char ALLbuf[16] = {0};
    strcat(ALLbuf,path);
#if _USE_LFN//���ʹ��֧�ֳ��ļ��� �ȶ����������ʼ��
    fileinfo.lfsize=_MAX_LFN * 2 + 1;//
    fileinfo.lfname=(TCHAR*)FileName;//
#endif
    for(i=1; i<=99; i++)
    {
        sprintf(buf,"%s%02d\0",head,i);
        if(f_opendir(&DirInfo,(const TCHAR*)path) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir�ṹ���б��� */
        {

            while(f_readdir(&DirInfo, &fileinfo) == FR_OK)  /* ���ļ���Ϣ���ļ�״̬�ṹ���� */
            {

                if(!fileinfo.fname[0]) //������
                {
                    strcat(ALLbuf,buf);
                    strcpy(name,ALLbuf);
                    return i; /* ����ļ���Ϊ��\0'��˵����ȡ��ɽ��� */
                }
                memset(nametemp,0,sizeof(fileinfo.fname));
                memcpy(nametemp,fileinfo.fname,sizeof(fileinfo.fname));
                if(strstr(nametemp,buf)!=NULL)  //������ļ�
                {
                    break;
                }
            }
        }
    }
    return i;
}

void Flash_File_Init(void)
{
    uint8 m,n,z;

    //�õ��������
    FileMaxCol = FatReadDirTest("0:",FilePage);
    for(m=0; m<FileMaxCol; m++)
    {
        for(n=0; n<FileCAM; n++)
        {
            DisFile[m][n].index[n]=m;   //��˳������ȥ��
        }
    }
    for(m=0; m<FileCOL; m++)   //��ʾҳ
    {
        if(m<FileMaxCol)
        {
            DisFile[m][0].disp=1;
            DisFile[m][0].dispold=0;
            DisFile[m][1].disp=2;
            DisFile[m][1].dispold=0;
        }
        else
        {
            for(z=0; z<FileCAM; z++)
            {
                DisFile[m][z].disp=0;
                DisFile[m][z].dispold=1;
            }
        }
    }
}


void Stu_File_Init(void)
{
    uint8 m,n;
    for(m=0; m<FileCOL; m++)
        for(n=0; n<FileCAM; n++)
        {
            DisFile[m][n].col = m;//��
            DisFile[m][n].cam = n;//��
            DisFile[m][n].lang = ENG;     //��ҳȫӢ��
            DisFile[m][n].flag = 1;  //��ʼ����һ��ʾ��־
            DisFile[m][n].selold = 1;//��ǰȫѡ������ȫûѡ�� ˢ��
            DisFile[m][n].sel= 0;     //��ǰû��ѡ��
            DisFile[m][n].disp=1;  // �Ƿ���ʾ1 ��ʾ 0����ʾ
            DisFile[m][n].dispold=1; //֮ǰ�Ƿ���ʾ
            DisFile[m][n].Box.flag = 0;  //�ַ����Ƿ��п� 0 û�п�
            DisFile[m][n].FontCH.flag = 0;  //���ı�־
            DisFile[m][n].FontEN.flag = 0;  //Ӣ�ı�־
            DisFile[m][n].cind = n;  //0 ���ļ��� 1��ʱ�� 2���ļ���С
        }
    Flash_File_Init();
}


//�п����ݵĳ�ʼ��
void Stu_FileMid_Init(void)
{
    uint8 m,n;
    for(m=0; m<FileCOL; m++)
        for(n=0; n<FileCAM; n++)
        {
            DisFile[m][n].col = m;//��
            DisFile[m][n].cam = n;//��
            DisFile[m][n].lang = ENG;     //��ҳȫӢ��
            DisFile[m][n].flag = 1;  //��ʼ����һ��ʾ��־
            DisFile[m][n].selold = 1;//��ǰȫѡ������ȫûѡ�� ˢ��
            DisFile[m][n].sel= 0;     //��ǰû��ѡ��
            DisFile[m][n].disp=1;  // �Ƿ���ʾ1 ��ʾ 0����ʾ
            DisFile[m][n].dispold=1; //֮ǰ�Ƿ���ʾ
            DisFile[m][n].Box.flag = 0;  //�ַ����Ƿ��п� 0 û�п�
            DisFile[m][n].FontCH.flag = 0;  //���ı�־
            DisFile[m][n].FontEN.flag = 0;  //Ӣ�ı�־
            DisFile[m][n].cind = n;  //0 ���ļ��� 1��ʱ�� 2���ļ���С
        }
    Flash_File_Init();
}
//�������ý���ĳ�ʼ��
void Stu_file_cycle(struct CUR *sle,struct CUR *sleold)
{
    uint8 m,n;
    for(m=0; m<FileCOL; m++)
        for(n=0; n<FileCAM; n++)
        {
            DisFile[m][n].flag = 0;
            DisFile[m][n].selold = 0;
            DisFile[m][n].sel= 0;
        }
    if( sleold->CAM!= sle->CAM||sleold->COL!=sle->COL)
    {
        DisFile[sle->COL][sle->CAM].selold = 0;//ˢ��
        DisFile[sle->COL][sle->CAM].sel= 1;
        DisFile[sleold->COL][sleold->CAM].selold = 1;//ˢ��
        DisFile[sleold->COL][sleold->CAM].sel= 0;
    }
}

//��ʼ��
void page_file(u8 FilePage)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    char str[5];
    char str1[20]= {0};

    FatReadAllDir("0:",str1);

    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);
    LCD_DrawRect(0, 2, 639, 430);
    LCD_DrawHLine(0, 380, 639);
    if(CHI == DataSave.Data_type.LANG )
    {

        LCD_DisplayStringLineLJ(2,5, "�ļ�����",FONTTYPE24);
        SetmenuInitP(DataSave.Data_type.LANG,"��","��������","ɾ��","���񵼳�","�ĵ�����",
                     35,144,295,396,525);
        LCD_SetFont(&Font16);

        LCD_DisplayStringLineLJ(x,y, "���",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv+50,y, "�ļ���",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*4-32,y, "��������",FONTTYPE16);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*4+32,y, (uint8_t *)str1);


    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_DisplayStringLineLJ(3,5, "FILE SET",FONTTYPE24);
        SetmenuInit(DataSave.Data_type.LANG,"OPEN","RENAME","DEL","EXL","TXT");
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, "No:");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv+50,y, "Filename");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*4-32,y, "Used ");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*4+20,y, (uint8_t *)str1);

    }
    LCD_SetFont(&Font20);
    y = 60;
    Ydiv =28;
    for(i=FilePage*FileCOL; i<(FilePage+1)*FileCOL; i++)
    {
        if(i<9)
        {
            sprintf(str,"%d :",i+1);
            LCD_SetFont(&Font20);
            LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*(i-FilePage*FileCOL),(uint8_t* )str);
        }
        else
        {
            LCD_SetFont(&Font20);
            sprintf(str,"%d:",i+1);
            LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*(i-FilePage*FileCOL),(uint8_t* )str);
        }
    }
    page_file_cycle(CurFile);
}

void page_file_CHNum(const struct CUR cur)
{
    uint16 x=10;
    uint16 y;
    uint16 Ydiv;
    uint16 i=0;
    char str[5];
    y = 60;
    Ydiv =28;
    for(i=FilePage*FileCOL; i<(FilePage+1)*FileCOL; i++)
    {
        if(i<9)
        {
            sprintf(str,"%d :",i+1);
            LCD_SetFont(&Font20);
            if(cur.COL==i)
                LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*(i-FilePage*FileCOL),(uint8_t* )str,&Font20,LCD_COLOR_DARKGREEN);
            else
                LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*(i-FilePage*FileCOL),(uint8_t* )str,&Font20,LCD_COLOR_WHITE);
        }
        else
        {
            LCD_SetFont(&Font20);
            sprintf(str,"%d:",i+1);
            if(cur.COL==(i-FilePage*FileCOL))
                LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*(i-FilePage*FileCOL),(uint8_t* )str,&Font20,LCD_COLOR_DARKGREEN);
            else
                LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*(i-FilePage*FileCOL),(uint8_t* )str,&Font20,LCD_COLOR_WHITE);

        }
    }
}

void LCD_DisplayStringLine_EN_CH_LJ_File(struct RDispPara *sel,uint16_t x,uint16_t y)
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
        LCD_SetBackColor(LCD_COLOR_HUI);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )FileEn[sel->cind].Ch[sel->index[sel->cind]]);
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
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )FileEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )FileEn[sel->cind].Ch[sel->index[sel->cind]]);
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
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )FileEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )FileEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}

void page_file_cycle(const struct CUR cur)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=90;
    uint16 Ydiv=90;
    uint16 i;
    char str[10] = {0};
    y = 60;
    Ydiv =28;
    //ֻ��ʾ������һ��
    Display_Help(x-2,y+Ydiv*11+20,helpS[0].Ch[DataSave.Data_type.LANG]);
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(x-2, y+Ydiv*11+40, (uint8_t *)"���񵼳�    �ļ����ĵ�����    �ļ�",FONTTYPE16);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x-2+16*4, y+Ydiv*11+42, (uint8_t *)"CSV");
        LCD_DisplayStringLine_EN_CH_LJ(x-2+16*13, y+Ydiv*11+42, (uint8_t *)"TXT");
    }
    //��ʾҳ��
    sprintf(str,"PAGE:%d/9\0",FilePage+1);
    LCD_SetFont(&Font20);
    LCD_DisplayStringLine_EN_CH_LJ(x-15+Xdiv*5,8,(uint8_t* )str);
    page_file_CHNum(cur);//ͨ���Ÿ�����ʾ
    for(i=0; i<11; i++)
    {
        Disp_Box_Home(&DisFile[i][0],x-15+Xdiv,y+i*Ydiv+2,x-15+Xdiv+200,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        //���ȿ��ܺܶ೤Ҫ�����
        Disp_Box_Home(&DisFile[i][1],x-15+Xdiv*4,y+i*Ydiv+2,x-15+Xdiv*4+220,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
    }
    y = 64;
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
    for(i=0; i<11; i++)
    {

        LCD_DisplayStringLine_EN_CH_LJ_File(&DisFile[i][0],x-5+Xdiv,y+i*Ydiv);
        // ��Ϊ�п�ᱻ���Ҫ�Ӵ˾�
        if(DisFile[i][1].disp==2&&DisFile[i][1].dispold==1)
        {
            DisFile[i][1].dispold=0;
        }
        LCD_DisplayStringLine_EN_CH_LJ_File(&DisFile[i][1],x-5+Xdiv*4,y+i*Ydiv);

    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
}
//����
//�ֲ����¶���
void LCD_DisplayStringLineLJ_Home_File(struct RDispStr* sel,uint16_t x,uint16_t y,uint8_t FontType)
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
        LCD_SetBackColor(LCD_COLOR_HUI);

        LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
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
            LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}
void Disp_Box_Home_File(struct RDispStr *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color)
{
    char flag = sel->flag;
    if(sel->Box.flag ==0)
    {
        sel->Box.x1=x1;
        sel->Box.y1=y1;
        sel->Box.x2=x2;
        sel->Box.y2=y2;
        sel->Box.h=y2-y1;
        sel->Box.w=x2-x1;
        sel->Box.flag = 1;
    }
    if(sel->disp==1&&flag==1)
    {
        Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                if(sel->sel==1)
                {
                    Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_WHITE,LCD_COLOR_DARKGREEN);
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
        else if(sel->disp==2&&sel->dispold==0)
        {
            Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
        }
    }

}
//�ֲ����¶���
void LCD_DisplayStringLine_EN_CH_LJ_Home_File(struct RDispStr *sel,uint16_t x,uint16_t y)
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
        LCD_SetBackColor(LCD_COLOR_HUI);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
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
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
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
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&Font20);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;
        }
    }
}

void SoftKeyDiaInitFile(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
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
    DisSoftKey[0][0].cind =  RD->cind  ;//����

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

    x1y1.Box.x1 =  30;//ȡ��
    x1y1.Box.y1 =  65;//ȡ
    x2y2.Box.x2=  630;//ȡ��
    x2y2.Box.y2 = 420 ;//ȡ��
    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//����
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //�������ɫ
    LCD_SetTextColor(LCD_COLOR_WHITE);

    Ydiv=24;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Ӣ��������",FONTTYPE16);

        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv, (uint8_t *)"�ļ���",FONTTYPE16);

    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Soft Key");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+Ydiv,(uint8_t *)"Name");
    }
}

void LCD_DisplayNumLine_EN_CH_LJ_Home_File(struct RDispStr *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType)
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
        LCD_SetFont(&FontType);
        LCD_SetBackColor(LCD_COLOR_HUI);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
        LCD_SetBackColor(BackColor);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                LCD_SetFont(&FontType);
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_HUI);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
                    LCD_SetBackColor(BackColor);

                }
                sel->selold=sel->sel;
            }
        }
        else if(sel->disp==0&&sel->dispold==1)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=0;
        }
        else if(sel->disp==2&&sel->dispold==0)
        {
            LCD_SetFont(&FontType);
            TextColor=LCD_GetTextColor();
            BackColor=LCD_GetBackColor();
            LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )ptr);
            LCD_SetColors(TextColor,BackColor);
            sel->dispold=1;

        }
    }
}


//��������  �Ի���
static void SoftKeyDiaFile(uint8 keytmp,struct RDispStr* RD,struct CUR* cur)
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
    x1y1.Box.x1 =  30;//ȡ��
    x1y1.Box.y1 =  65;//
    x2y2.Box.x2=  630;//ȡ��
    x2y2.Box.y1 = 396;//ȡ��
    x2y2.Box.y2 = 420;//ȡ��
    SoftKeyDiaInitFile(keytmp,RD, cur) ;
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
    while(!(keytmp==KEY_ENTER&&Cur.COL==DiaSoftKeyCOL-1)&&keytmp!=KEY_ESC); //DC�ĶԻ����4����ȷ����ȡ����ť
    OSSchedLock(&err);
    Stu_FileMid_Init();
    ModifySoftKey(keytmp,RD,&DisLogOld,&Cur,DiaSoftKeyCOL,DiaSoftKeyCAM,20);
    page_file(FilePage);
    OSSchedUnlock(&err);
}

void RDispPara2RDispStr(struct RDispStr* RD,const struct RDispPara RP)
{
    uint8 n = 0;
    RD->col = RP.col;                   //��
    RD->cam = RP.cam;                  //��
    RD->disp = RP.disp ;                 //�Ƿ���ʾ
    RD->dispold = RP.dispold;                 //�Ƿ���ʾ
    RD->flag = RP.flag;                 //�Ƿ��ʼ��
    RD->sel = RP.sel;                 //�Ƿ�ѡ��
    RD->selold = RP.selold ;                 //�ϴ��Ƿ�ѡ��
    RD->lang = RP.lang;                //ʹ������
    RD->cind = RP.cind;                  // Ӣ�ļ�����  0     �������� 1
    memcpy(RD->index,RP.index,sizeof(RD->index));                //��ʾ�ڼ���
    RD->Box=RP.Box;       //��ʾ�ַ�������λ��
    RD->FontCH=RP.FontCH;    //�����ַ���
    RD->FontEN=RP.FontEN ;    // Ӣ���ַ���
    RD->Dxy = RP.Dxy;
    for(n=0; n<strlen(FileEn[RP.cind].Ch[RP.index[RP.cind]]); n++)
    {
        if(FileEn[RP.cind].Ch[RP.index[RP.cind]][n]=='.')
            break;
    }
    memset(RD->Str,0,sizeof(RD->Str));
    strncpy(RD->Str,FileEn[RP.cind].Ch[RP.index[RP.cind]],n);
}




void RDispStr2RDispPara(char* oldname,const struct RDispStr RP,struct RDispPara* RD)
{
    RD->col = RP.col;                   //��
    RD->cam = RP.cam;                  //��
    RD->disp = RP.disp ;                 //�Ƿ���ʾ
    RD->dispold = RP.dispold;                 //�Ƿ���ʾ
    RD->flag = RP.flag;                 //�Ƿ��ʼ��
    RD->sel = RP.sel;                 //�Ƿ�ѡ��
    RD->selold = RP.selold ;                 //�ϴ��Ƿ�ѡ��
    RD->lang = RP.lang;                //ʹ������
    RD->cind = RP.cind;                  // Ӣ�ļ�����  0     �������� 1
    memcpy(RD->index,RP.index,sizeof(RD->index));                //��ʾ�ڼ���
    RD->Box=RP.Box;       //��ʾ�ַ�������λ��
    RD->FontCH=RP.FontCH;    //�����ַ���
    RD->FontEN=RP.FontEN ;    // Ӣ���ַ���
    RD->Dxy = RP.Dxy;
    strncpy(oldname,FileEn[RD->cind].Ch[RD->index[RD->cind]],sizeof(FileEn[RD->cind].Ch[RD->index[RD->cind]]));
    memset(FileEn[RD->cind].Ch[RD->index[RD->cind]],0,sizeof(FileEn[RD->cind].Ch[RD->index[RD->cind]]));
    strcpy(FileEn[RD->cind].Ch[RD->index[RD->cind]],RP.Str);
    strcat(FileEn[RD->cind].Ch[RD->index[RD->cind]],".bin");
}
struct CUR SampleTimeCurF= {0};  //SampleTime����ѡ����α�
static struct RDispData  DDaF[DCOL][ DCAMR];
static struct RDispData  DFrqF[2];//+�Ƿ�����Ƶ��ֵ
RTC_TimeTypeDef RTC_TimeStrStartF;//ʱ��
RTC_DateTypeDef RTC_DateStrStartF;//����
union SD_U DsdF;//sd����������
struct CUR ZoneCurF= {0};//����ѡ����α�
static struct ChanalData ChanDataF;
uint32 RCNTALLF;
void JiSuanXYF(struct SLData *p,uint8 i,uint32 starttick,uint32 endtick,uint32 NumMs,uint32 ZoneD)
{


    if(DsdF.Data.CHD[i].SDData.vcind  == VT||DsdF.Data.CHD[i].SDData.vcind == VDC||DsdF.Data.CHD[i].SDData.vcind == VI||DsdF.Data.CHD[i].SDData.vcind == VG)  //ǰ�����Ǽ�ͷ��ALL
    {
        p->Dxy.X=GIXStartX+(endtick-starttick)/NumMs;
        p->Dxy.Y=GIXStartY-(i%ZoneD)*(PointsPiy/ ZoneD)-((p->DfStr.fval-DDaF[i][0].Low[DDa[i][0].vcind].fval)*(PointsPiy/ ZoneD)/(DDaF[i][0].Up[DDa[i][0].vcind].fval-DDaF[i][0].Low[DDa[i][0].vcind].fval));
    }

}
void UpDataDDaF(struct ChanalData ChanDataF,uint32 i)
{
    DDaF[i][0].Up[DDaF[i][0].vcind].fval=BYTE3toFloat(DsdF.Data.CHD[i].SDData.MaxB);
    DDaF[i][0].Low[DDaF[i][0].vcind].fval=BYTE3toFloat(DsdF.Data.CHD[i].SDData.MinB);
    if((DsdF.Data.DataType&0xf)==(CmdR_DATA&0xf))
        JiSuanXYF(&ChanDataF.chd[i],i,DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,SampletimeMs[DsdF.Data.SampleTimeCurCAM], ZoneData[ZoneCurF.CAM]);
    else if((DsdR.Data.DataType&0xf)==(CmdR_FDATA&0xf))
        JiSuanXYF(&ChanDataF.chd[i],i,DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,SampletimeMs[DsdF.Data.SampleTimeCurCAM]/10, ZoneData[ZoneCurF.CAM]);
    DDaF[i][0].DP.Dxy.X=0;
    if(ChanDataF.chd[i].Dxy.X<GIXStartX)
        ChanDataF.chd[i].Dxy.X=GIXStartX-1;
    else if(ChanDataF.chd[i].Dxy.X>GIXStartX+PointsPix)
        ChanDataF.chd[i].Dxy.X=GIXStartX+PointsPix+1;
    DDaF[i][0].DP.Dxy.X=ChanDataF.chd[i].Dxy.X;
    if(ChanDataF.chd[i].Dxy.Y<GIXStartY-PointsPiy)
        ChanDataF.chd[i].Dxy.Y=GIXStartY-PointsPiy;
    else if(ChanDataF.chd[i].Dxy.Y>GIXStartY)
        ChanDataF.chd[i].Dxy.Y=GIXStartY;
    DDaF[i][0].DP.Dxy.Y=ChanDataF.chd[i].Dxy.Y;
    if(ChanDataF.chd[i].DfStr.fval>=DDaF[i][0].Up[DDaF[i][0].vcind].fval)
        ChanDataF.chd[i].DfStr=DDaF[i][0].Up[DDaF[i][0].vcind];
    else if(ChanDataF.chd[i].DfStr.fval<=DDaF[i][0].Low[DDaF[i][0].vcind].fval)
        ChanDataF.chd[i].DfStr=DDaF[i][0].Low[DDaF[i][0].vcind];
    fStr2DfStr(&ChanDataF.chd[i]);//���պ����������ԭʼ����ת��Ϊ������ʾ
    DDaF[i][0].DP.Num = ChanDataF.chd[i].DfStr;//���յ����ݸ���
    DDaF[i][0].Uint[DDaF[i][0].vcind].Col = ChanDataF.chd[i].UintOne.Col;  // ��ʾ��λˢ���ǽ��յ�ָ����Զ��л���
    DDaF[i][0].DP.dispold = 0;   //ˢ�±�־ ��dispold ==0 �� disp ==2 ��������ʾ�Ż�ˢ��  ����Ҫ�� ���Ӳ�ˢ��
}


void Dsd2DsdF(const union SD_U Dsd)
{

    uint8 i = 0;
    UINT fnum=0;
    uint8 rcnt=0;
    static struct ChanalData ChanDataF;
    uint8 buf[36] = {0};
    memset((void *)&ChanDataF,0,sizeof(ChanDataF));
    memcpy(DsdF.uD,Dsd.uD,sizeof(DsdF.uD));
    rcnt=0;
    while(f_lseek(&fnew,(RCNTALLF)*sizeof(DsdF.uD))!=FR_OK)
    {
        if(rcnt++>100)break;
    };//ÿ�ζ�ȡ֮ǰ���ƶ���дָ��ﵽ��д��ͬ���ݵ�Ŀ��
    rcnt=0;
    while(f_sync(&fnew)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    rcnt=0;
    while(f_read(&fnew, DsdF.uD, sizeof(DsdF.uD), &fnum)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    stamp2Time( DsdF.Data.Timestamp,&RTC_DateStrStartF,&RTC_TimeStrStartF);
    SampleTimeCurF.CAM=DsdF.Data.SampleTimeCurCAM;
    for(i=0; i<2; i++)
    {
        memcpy(buf+i*3, DsdF.Data.FD[i].SDData.byte,sizeof(DsdF.Data.FD[i].SDData.byte));
        ChanDataF.frq[i].vcind = DsdF.Data.FD[i].SDData.vcind;
    }
    for(i=0; i<CHANNUM; i++)
    {
        memcpy(buf+6+i*3, DsdF.Data.CHD[i].SDData.byte,sizeof(DsdF.Data.CHD[i].SDData.byte));
        DDaF[i][0].vcind = DsdF.Data.CHD[i].SDData.vcind;
        ChanDataF.chd[i].vcind = DsdF.Data.CHD[i].SDData.vcind;
    }
    SerialRemoteHandleR(buf,&ChanDataF);
    for(i=0; i<2; i++)//Ƶ�ʲ���
    {
        DFrqF[i].DP.Num = ChanDataF.frq[i].DfStr;//���յ����ݸ���
        DFrqF[i].Uint[DFrqF[i].vcind].Col = ChanDataF.frq[i].UintOne.Col;  // ��ʾ��λˢ��
        DFrqF[i].DP.dispold = 0;   //ˢ�±�־ ��dispold ==0 �� disp ==2 ��������ʾ�Ż�ˢ��
        DFrqF[i].vcind = DsdF.Data.FD[i].SDData.vcind;
    }
    for(i=0; i<CHANNUM; i++)
    {
        UpDataDDaF(ChanDataF,i);
    }
}

// 0 �ǵ�ͷ��1 ����
uint8 RereadSDF(uint32 RCNTALL)
{

    uint8 i = 0;
    UINT fnum=0;
    uint8 rcnt=0;
    static struct ChanalData ChanDataF;
    uint8 buf[36] = {0};
    memset((void *)&ChanDataF,0,sizeof(ChanDataF));
    if((RCNTALL)*sizeof(DsdF.uD)>f_size(&fnew))
        return 0;
    rcnt=0;
    while(f_lseek(&fnew,(RCNTALL)*sizeof(DsdF.uD))!=FR_OK)
    {
        if(rcnt++>100)break;
    };//ÿ�ζ�ȡ֮ǰ���ƶ���дָ��ﵽ��д��ͬ���ݵ�Ŀ��
    rcnt=0;
    while(f_sync(&fnew)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    rcnt=0;
    while(f_read(&fnew, DsdF.uD, sizeof(DsdF.uD), &fnum)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    stamp2Time( DsdF.Data.Timestamp,&RTC_DateStrStartF,&RTC_TimeStrStartF);
    SampleTimeCurF.CAM=DsdF.Data.SampleTimeCurCAM;
    for(i=0; i<2; i++)
    {
        memcpy(buf+i*3, DsdF.Data.FD[i].SDData.byte,sizeof(DsdF.Data.FD[i].SDData.byte));
        ChanDataF.frq[i].vcind = DsdF.Data.FD[i].SDData.vcind;
    }
    for(i=0; i<CHANNUM; i++)
    {
        memcpy(buf+6+i*3, DsdF.Data.CHD[i].SDData.byte,sizeof(DsdF.Data.CHD[i].SDData.byte));
        DDaF[i][0].vcind = DsdF.Data.CHD[i].SDData.vcind;
        ChanDataF.chd[i].vcind = DsdF.Data.CHD[i].SDData.vcind;
    }
    SerialRemoteHandleR(buf,&ChanDataF);
    for(i=0; i<2; i++)//Ƶ�ʲ���
    {
        DFrqF[i].DP.Num = ChanDataF.frq[i].DfStr;//���յ����ݸ���
        DFrqF[i].Uint[DFrqF[i].vcind].Col = ChanDataF.frq[i].UintOne.Col;  // ��ʾ��λˢ��
        DFrqF[i].DP.dispold = 0;   //ˢ�±�־ ��dispold ==0 �� disp ==2 ��������ʾ�Ż�ˢ��
        DFrqF[i].vcind = DsdF.Data.FD[i].SDData.vcind;
    }
    for(i=0; i<CHANNUM; i++)
    {

        UpDataDDaF(ChanDataF,i);
    }
    return 1;
}


uint8 RereadSDFst(uint32 RCNTALL)
{

    UINT fnum=0;
    uint8 rcnt=0;


    memset((void *)&ChanDataF,0,sizeof(ChanDataF));
    if((RCNTALL)*sizeof(DsdF.uD)>f_size(&fnew))
        return 0;
    rcnt=0;
    while(f_lseek(&fnew,(RCNTALL)*sizeof(DsdF.uD))!=FR_OK)
    {
        if(rcnt++>100)break;
    };//ÿ�ζ�ȡ֮ǰ���ƶ���дָ��ﵽ��д��ͬ���ݵ�Ŀ��
    rcnt=0;
    while(f_sync(&fnew)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    rcnt=0;
    while(f_read(&fnew, DsdF.uD, sizeof(DsdF.uD), &fnum)!=FR_OK)
    {
        if(rcnt++>100)break;
    };
    return 1;
}
void udisk_scan(void)
{
    u8 res;

    if(usbstatus != UNCONNECTED)
    {
        res = CH376DiskConnect( );
        if( res != USB_INT_SUCCESS )/* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
        {
            usbstatus = UNCONNECTED;
        }
    }
    if(usbstatus != CONNECTED)
    {
        res = CH376DiskMount( );
        if(res == USB_INT_SUCCESS )
        {
            usbstatus = CONNECTED;
        }
    }
}
void Stu_DisplayDataFrq_InitF(void)
{
    uint8 m,n,i,j;
    for(m=0; m<2; m++)
    {
        DFrqF[m].DP.col = m;
        DFrqF[m].DP.cam = n;
        DFrqF[m].DP.lang = DataSave.Data_type.LANG;
        DFrqF[m].vcind = VFRQ ;//����
        for(i=0; i<VALCIND; i++)
            for(j=0; j<UintCOL; j++)
            {
                strncpy(DFrqF[m].Uint[i].Ch[j],ConstUintF[i].Ch[j],sizeof(ConstUintF[i].Ch[j]));   //��ѹ���� ����ĵ�λ

                DFrqF[m].Uint[i].Col =0;   // Ƶ������Ϊ0
            }

//        DFrqF[m].Up[DFrqF[m].vcind].fval=10000;
//        DFrqF[m].Up[DFrqF[m].vcind].dot=3;  //С������3λ֮��
//        DFrqF[m].Up[DFrqF[m].vcind].bit=3;//����λ����
//        DFrqF[m].Up[DFrqF[m].vcind].Decimalbit=1;//С��λ����
//        DFrqF[m].Low[DFrqF[m].vcind].fval=0;
//        DFrqF[m].Low[DFrqF[m].vcind].dot=1;  //С������3λ֮��
//        DFrqF[m].Low[DFrqF[m].vcind].bit=1;//����λ����
//        DFrqF[m].Low[DFrqF[m].vcind].Decimalbit=1;//С��λ����
        DFrqF[m].DP.Num.fval = 0.0;//�ȸ���Ĭ��ֵ1.1v ������� �¶�ʱĬ����1.1���϶�
        DFrqF[m].DP.Num.bit =1;//����λ1λ
        DFrqF[m].DP.Num.dot = 1;//С����һλ��
        DFrqF[m].DP.Num.Decimalbit = 1;//С��λ1λ

        DFrqF[m].DP.flag = 1;
        DFrqF[m].DP.disp=2;
        DFrqF[m].DP.dispold=0;

        DFrqF[m].DP.Box.flag = 0;
        DFrqF[m].DP.FontEN.flag = 0;
        ChanDataF.chd[m].vcind =DFrqF[m].vcind;
    }
}






void Stu_DisplayDataCha_InitF(void)
{
    uint8 m,n,i,j;
    for(m=0; m<DCOL; m++)
    {
        for(n=0; n<DCAMR; n++)
        {
            DDaF[m][n].DP.col = m;
            DDaF[m][n].DP.cam = n;
            DDaF[m][n].DP.lang = DataSave.Data_type.LANG;

            for(i=0; i<VALCIND; i++)
                for(j=0; j<UintCOL; j++)
                {
                    strncpy(DDaF[m][n].Uint[i].Ch[j],ConstUintF[i].Ch[j],sizeof(ConstUintF[i].Ch[j]));   //��ѹ���� ����ĵ�λ
                    if(DDaF[m][n].vcind == VDC)   //��ѹ����Ĭ��ѡΪ��λ��v �������
                        DDaF[m][n].Uint[i].Col = 2;  //0 MV 1KV 2 V 3 mV 4 uV
                    else  if(DDaF[m][n].vcind == VT)   //�¶ȵ�λ����
                        DDaF[m][n].Uint[i].Col = DataSave.Data_type.TUnit; //�������ϻ���
                    else  if(DDaF[m][n].vcind == VI)   //��������
                        DDaF[m][n].Uint[i].Col = 0; //mA
                    else  if(DDaF[m][n].vcind == VG)   //����
                        DDaF[m][n].Uint[i].Col = 0; //g
                    else
                        DDaF[m][n].Uint[i].Col =0;   // ��������ֻ��1�� ��Ϊ0
                }
            //��λ���ͳ�ʼ��
            if( DDaF[m][n].vcind ==VDC ) //��ѹ����
            {
                DDaF[m][n].Up[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaF[m][n].Up[DDaF[m][n].vcind].dot=1;  //С������1λ֮��
                DDaF[m][n].Up[DDaF[m][n].vcind].bit=1;//����λ����
                DDaF[m][n].Up[DDaF[m][n].vcind].Decimalbit=1;//С��λ����
                DDaF[m][n].Low[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaF[m][n].Low[DDaF[m][n].vcind].dot=1;  //С������1λ֮��
                DDaF[m][n].Low[DDaF[m][n].vcind].bit=1;//����λ����
                DDaF[m][n].Low[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

            }
            else if(DDaF[m][n].vcind ==VT)//�¶�����
            {
                DDaF[m][n].Up[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaF[m][n].Up[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Up[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Up[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

                DDaF[m][n].Low[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaF[m][n].Low[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Low[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Low[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

            }
            else if(DDaF[m][n].vcind ==VI)//��������
            {
                DDaF[m][n].Up[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaF[m][n].Up[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Up[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Up[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

                DDaF[m][n].Low[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaF[m][n].Low[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Low[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Low[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

            }
            else if(DDaF[m][n].vcind ==VG)//��������
            {
                DDaF[m][n].Up[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MaxB);
                DDaF[m][n].Up[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Up[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Up[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

                DDaF[m][n].Low[DDaF[m][n].vcind].fval=BYTE3toFloat(DsdR.Data.CHD[m].SDData.MinB);
                DDaF[m][n].Low[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Low[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Low[DDaF[m][n].vcind].Decimalbit=1;//С��λ����

            }
            else if(DDaF[m][n].vcind ==VR)//ʪ������
            {
                DDaF[m][n].Up[DDaF[m][n].vcind].fval=100;
                DDaF[m][n].Up[DDaF[m][n].vcind].dot=3;  //С������3λ֮��
                DDaF[m][n].Up[DDaF[m][n].vcind].bit=3;//����λ����
                DDaF[m][n].Up[DDaF[m][n].vcind].Decimalbit=1;//С��λ����
                DDaF[m][n].Low[DDaF[m][n].vcind].fval=0;
                DDaF[m][n].Up[DDaF[m][n].vcind].dot=1;  //С������3λ֮��
                DDaF[m][n].Up[DDaF[m][n].vcind].bit=1;//����λ����
                DDaF[m][n].Up[DDaF[m][n].vcind].Decimalbit=1;//С��λ����
            }

            DDaF[m][n].DP.Num.fval = 0.0;//�ȸ���Ĭ��ֵ1.1v ������� �¶�ʱĬ����1.1���϶�
            DDaF[m][n].DP.Num.bit =1;//����λ1λ
            DDaF[m][n].DP.Num.dot = 1;//С����һλ��
            DDaF[m][n].DP.Num.Decimalbit = 1;//С��λ1λ

            if(DDaF[m][n].vcind==0)//off ״̬�²���ʾ
            {
                DDaF[m][n].DP.flag = 0;
                DDaF[m][n].DP.disp=0;
                DDaF[m][n].DP.dispold=0;
            }
            else//���в���״̬�� 1 �������ֵ  2������Сֵ ��Ҫ���θ�ֵ
            {
                DDaF[m][n].DP.flag = 1;
                DDaF[m][n].DP.disp=2;
                DDaF[m][n].DP.dispold=0;
            }
            DDaF[m][n].DP.Box.flag = 0;
            DDaF[m][n].DP.FontEN.flag = 0;
            if(n==3||n==4)
            {
                DDaF[m][n].DP.flag = 0;
                DDaF[m][n].DP.disp=0;
                DDaF[m][n].DP.dispold=0;
            }
            DDaF[9][n].Up[VT].dot=3;  //С������3λ֮��
            DDaF[9][n].Up[VT].bit=3;//����λ����
            DDaF[9][n].Up[VT].Decimalbit=1;//С��λ����
            // DDaF[9][n].Low[VT].fval=0;
            DDaF[9][n].Low[VT].dot=3;  //С������3λ֮��
            DDaF[9][n].Low[VT].bit=3;//����λ����
            DDaF[9][n].Low[VT].Decimalbit=1;//С��λ����
        }
    }
}
void Stu_DisplayData_InitF(void)
{
    Stu_DisplayDataCha_InitF();
    Stu_DisplayDataFrq_InitF();
}

void USB_Str(char* str,char Feng)
{

    uint16 i;
    char ptr[10];
    char ptr1[11];
    for(i=1; i<11; i++)
    {
        memset(ptr,0,sizeof(ptr));
        if(DDaF[i-1][0].vcind==OFF)
            strcpy(ptr,"***\0");
        else
        {
            NumFloat2Char(&DDaF[i-1][0],&DDaF[i-1][0].DP.Num,ptr);
            AddUint2Char(DDaF[i-1][0],ptr,9);
        }
        memset(ptr1,0,sizeof(ptr1));
        ptr1[0]=Feng;
        strcat(ptr1,ptr);
        strcat(str,ptr1);
    }
    for(i=0; i<2; i++)
    {
        memset(ptr,0,sizeof(ptr));
        if(DFrqF[i].vcind==OFF)
        {
            strcpy(ptr,"***\0");
        }
        else
        {
            NumFloat2Char(&DFrqF[i],&DFrqF[i].DP.Num,ptr);
            AddUint2Char(DFrqF[i],ptr,9);
        }

        memset(ptr1,0,sizeof(ptr1));
        ptr1[0]=Feng;
        strcat(ptr1,ptr);
        strcat(str,ptr1);
    }


}
void SD2USB(char* TarName)
{
    u8 s;
    static uint32 RCNTALLF;
    char filename[20]= {0};
    u8 rcnt = 0;
    Stu_DisplayData_InitF();
    RCNTALLF = 0;
    memset(filename,0,sizeof(filename));
    sprintf((char *)filename, "/%s",TarName);
    rcnt = 0;
    do
    {
        s=CH376FileCreatePath((u8* )filename);
        if(rcnt++>100)break;

    }
    while(s != 0x14);
    while(1)
    {
        if(RereadSDFst(RCNTALLF++)!=0)
        {
            rcnt = 0;
            do
            {
                s=CH376ByteWrite( DsdF.uD, sizeof(DsdF.uD), NULL );
                if(rcnt++>100)break;

            }
            while(s != 0x14);
        }
        else
        {
            rcnt = 0;
            do
            {
                s = CH376FileClose(TRUE);
                if(rcnt++>100)break;
            }
            while(s != 0x14);
            break;
        }

    }


}

void str_replace(char * cp, int n, char * str)
{
    int lenofstr;

    char * tmp;
    lenofstr = strlen(str);
    //str3��str2�̣���ǰ�ƶ�
    if(lenofstr < n)
    {
        tmp = cp+n;
        while(*tmp)
        {
            *(tmp-(n-lenofstr)) = *tmp; //n-lenofstr���ƶ��ľ���
            tmp++;
        }
        *(tmp-(n-lenofstr)) = *tmp; //move '\0'
    }
    else
        //str3��str2���������ƶ�
        if(lenofstr > n)
        {
            tmp = cp;
            while(*tmp) tmp++;
            while(tmp>=cp+n)
            {
                *(tmp+(lenofstr-n)) = *tmp;
                tmp--;
            }
        }
    strncpy(cp,str,lenofstr);
}
char* uitoa(unsigned int n, char *s)
{
    int i, j;
    i = 0;
    char buf[20];
    memset(buf, 0, sizeof(buf));
    do
    {
        buf[i++] = n % 10 + '0';//ȡ��һ������
    }
    while ((n /= 10)>0);  //ɾ��������
    i -= 1;
    for (j = 0; i >= 0; j++, i--)//���ɵ�����������ģ�����Ҫ�������
        s[j] = buf[i];
    s[j] = '\0';
    return s;
}

void ShowDateTimeF(uint32 starttick,uint32 endtick,uint32 samp,char *str)
{
    static RTC_TimeTypeDef RTC_TimeStrTemp;
    static RTC_DateTypeDef RTC_DateStrTemp;
    uint32 s= samp+(endtick-starttick)/1000;
    stamp2Time(s,&RTC_DateStrTemp,&RTC_TimeStrTemp) ;
    //��ʾʱ��
    sprintf(str,"20%0.2d-%0.2d-%0.2d-%0.2d:%0.2d:%0.2d.%0.3d",
            RTC_DateStrTemp.Year,
            RTC_DateStrTemp.Month,
            RTC_DateStrTemp.Date,
            RTC_TimeStrTemp.Hours,
            RTC_TimeStrTemp.Minutes,
            RTC_TimeStrTemp.Seconds,
            (endtick-starttick)%1000);
}

void Display_Note(uint16_t x,uint16_t y,const char *ptr)
{

    LCD_FillRectLJ(x-2, y-2, 632,20,LCD_COLOR_BLACK); //����±߿�
    LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(x, y, (uint8_t *)ptr,FONTTYPE16);
    }
    else
    {
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x, y, (uint8_t *)ptr);
    }
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}

void InitUSBEXL(void)
{
    u8 s,rcnt,i;
    static char buf[200]= {0};
    if(usbstatus == CONNECTED)
    {
        memset(buf,0,sizeof(buf));
        memset(filenameUSB,0,sizeof(filenameUSB));
        sprintf((char *)filenameUSB,"/%0.2d%0.2d%0.2d%0.2d.CSV",
//            RTC_DateStr.Year,
//            RTC_DateStr.Month,
                RTC_DateStr.Date,
                RTC_TimeStr.Hours,
                RTC_TimeStr.Minutes,
                RTC_TimeStr.Seconds);
//      i=99;
//        sprintf((char *)filenameUSB,"/NB%0.2d.CSV",i);
        rcnt = 0;
        Stu_DisplayData_InitF();  //��ʼ��
        do
        {
            s=CH376FileCreatePath(( u8*)filenameUSB);
            if(rcnt++>100)break;
        }
        while(s != 0x14);
        RCNTALLF=0;

        sprintf((char *)buf,"No,Time,CH01,CH02,CH03,CH04,CH05,CH06,CH07,CH08,CH09,CH10,PLS1,PLS2,");
        rcnt = 0;
        do
        {
            s = CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
            if(rcnt++>100)break;
        }
        while(s != 0x14);
        rcnt = 0;
        do
        {
            s = CH376FileClose(TRUE);
            if(rcnt++>100)break;
        }
        while(s != 0x14);
    }
}

void SaveUSBEXL(char* TarName,uint8_t  UartCmd)
{
    u8 s,i,j,rcnt;
    static char buf[200]= {0};
    static char str[100]= {0};
    if(usbstatus == CONNECTED&&FileRes == FILEUSBOpen)
    {
        //CPU_SR_ALLOC();
        rcnt = 0;
        do
        {
            //CPU_CRITICAL_ENTER();
            s= CH376FileOpenPath(( u8*)TarName);
            //CPU_CRITICAL_EXIT() ;
            if(rcnt++>200)return;
        }
        while(s != 0x14);
        rcnt = 0;
        do
        {
            //CPU_CRITICAL_ENTER();
            s = CH376ByteLocate(0xFFFFFFFF);
            //CPU_CRITICAL_EXIT() ;
            if(rcnt++>200)return;
        }
        while(s != 0x14);
        if( UartCmd ==CmdR_DATA)
        {
            memset(buf,0,sizeof(buf));
            Dsd2DsdF(Dsd);
            buf[0]='\n';
            uitoa(++RCNTALLF,&buf[1]);
            strcat(buf,",");
            for(i=0; i<CHANNUM; i++)
            {
                if(DDaF[i][0].DP.Dxy.X!=0)
                {
                    memset(str,0,sizeof(str));
                    if((DsdF.Data.DataType&0xf)==(CmdR_DATA&0xf))
                        ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                    else if((DsdF.Data.DataType&0xf)==(CmdR_FDATA&0xf))
                        ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                    strcat(buf,str);
                    break;
                }
            }
            memset(str,0,sizeof(str));
            USB_Str(str,',');
            strcat(buf,str);
            rcnt = 0;
            do
            {
                //CPU_CRITICAL_ENTER();
                s=CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
                //CPU_CRITICAL_EXIT() ;
                if(rcnt++>200)return;
            }
            while(s != 0x14);
            FileSaveNum++;
        }
        else if(UartCmd ==CmdR_FDATA)
        {
            for(j=0; j<CHANNUM; j++)
            {
                memset(buf,0,sizeof(buf));
                Dsd2DsdF(DsdFM[j]);
                buf[0]='\n';
                uitoa(++RCNTALLF,&buf[1]);
                strcat(buf,",");
                for(i=0; i<CHANNUM; i++)
                {
                    if(DDaF[i][0].DP.Dxy.X!=0)
                    {
                        memset(str,0,sizeof(str));
                        if((DsdF.Data.DataType&0xf)==(CmdR_DATA&0xf))
                            ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                        else if((DsdF.Data.DataType&0xf)==(CmdR_FDATA&0xf))
                            ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                        strcat(buf,str);
                        break;
                    }
                }
                memset(str,0,sizeof(str));
                USB_Str(str,',');
                strcat(buf,str);
                rcnt = 0;
                do
                {
                    //CPU_CRITICAL_ENTER();
                    s=CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
                    //CPU_CRITICAL_EXIT() ;
                    if(rcnt++>200)break;
                }
                while(s != 0x14);
		if(s == 0x14)
		 FileSaveNum++;		
            }
        }
        rcnt = 0;
        do
        {
            //CPU_CRITICAL_ENTER();
            s = CH376FileClose(TRUE);
            //CPU_CRITICAL_EXIT() ;
            if(rcnt++>200)break;
        }
        while(s != 0x14);
    }
}

void SD2USBEXL(char* TarName)
{
    u8 s,i,rcnt=0;;
    uint16 x=10;
    uint16 y=39;
    uint16 Ydiv=90;
    static uint32 RCNTALLF=0;
    static char filename[20]= {0};
    char *p1 = NULL;
    char *p2 = NULL;
    static char buf[200]= {0};
    static char str[100]= {0};
    static u32 iapprocess = 0;
    u32 iapprocesstemp = 0;
    memset(buf,0,sizeof(buf));
    iapprocess = 0;
    y = 60;
    Ydiv =28;
    Stu_DisplayData_InitF();
    RCNTALLF = 0;
    memset(filename,0,sizeof(filename));
    sprintf((char *)filename, "/%s",TarName);
    p1 = strstr(filename,".BIN");
    p2 = strstr(filename,".bin");
    if(p1!=NULL)
    {
        str_replace(p1,4,".CSV");
    }
    else if(p2!=NULL)
    {
        str_replace(p2,4,".CSV");
    }
    rcnt=0;
    do
    {
        s=CH376FileCreatePath(( u8*)filename);
        if(rcnt++>100)break;
    }
    while(s != 0x14);

    sprintf((char *)buf,"No,Time,CH01,CH02,CH03,CH04,CH05,CH06,CH07,CH08,CH09,CH10,PLS1,PLS2,");
    rcnt=0;
    do
    {
        s = CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
        if(rcnt++>100)break;
    }
    while(s != 0x14);
    Display_Note(x-2,y+Ydiv*11+40,NoteS[0].Ch[DataSave.Data_type.LANG]);
    while(1)
    {
        if(RereadSDF(RCNTALLF++)!=0)
        {
            iapprocesstemp = (u32)(float)(((RCNTALLF)*sizeof(DsdF.uD))/(float)(f_size(&fnew))*(float)100.0);
            if(iapprocesstemp>iapprocess)
            {
                iapprocess=iapprocesstemp;
                if(iapprocess>100)
                    iapprocess=100;
                memset(str,0,sizeof(str));
                uitoa(iapprocess,str);
                strcat(str,"%");
                LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
                LCD_SetFont(&Font16);
                if(CHI == DataSave.Data_type.LANG )
                    LCD_DisplayStringLine_EN_CH_LJ(x-2+192, y+Ydiv*11+40, (uint8_t *)str);
                else
                    LCD_DisplayStringLine_EN_CH_LJ(x-2+192, y+Ydiv*11+40, (uint8_t *)str);
                LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            }
            memset(buf,0,sizeof(buf));
            buf[0]='\n';
            uitoa(RCNTALLF,&buf[1]);
            strcat(buf,",");
            for(i=0; i<CHANNUM; i++)
            {
                if(DDaF[i][0].DP.Dxy.X!=0)
                {
                    memset(str,0,sizeof(str));
                    if((DsdF.Data.DataType&0xf)==(CmdR_DATA&0xf))
                        ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                    else if((DsdF.Data.DataType&0xf)==(CmdR_FDATA&0xf))
                        ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                    strcat(buf,str);
                    break;
                }
            }
            memset(str,0,sizeof(str));
            USB_Str(str,',');
            strcat(buf,str);
            rcnt=0;
            do
            {
                s=CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
                if(rcnt++>100)break;
            }
            while(s != 0x14);
        }
        else
        {
            rcnt=0;
            do
            {
                s = CH376FileClose(TRUE);
                if(rcnt++>100)break;
            }
            while(s != 0x14);
            break;
        }
    }
}


void SD2USBTXT(char* TarName)
{
    u8 s,i,rcnt;
    uint16 x=10;
    uint16 y=39;
    uint16 Ydiv=90;
    static uint32 RCNTALLF=0;
    static char filename[20]= {0};
    char *p1 = NULL;
    char *p2 = NULL;
    static char buf[200]= {0};
    static char str[100]= {0};
    static u32 iapprocess = 0;
    u32 iapprocesstemp = 0;
    iapprocess = 0;
    y = 60;
    Ydiv =28;
    Stu_DisplayData_InitF();
    RCNTALLF = 0;
    memset(filename,0,sizeof(filename));
    sprintf((char *)filename, "/%s",TarName);
    p1 = strstr(filename,".BIN");
    p2 = strstr(filename,".bin");
    if(p1!=NULL)
    {
        str_replace(p1,4,".TXT");
    }
    else if(p2!=NULL)
    {
        str_replace(p2,4,".TXT");
    }
    rcnt=0;
    do
    {
        if(rcnt++>100)break;
        s=CH376FileCreatePath(( u8*)filename);
    }
    while(s != 0x14);
    sprintf((char *)buf,"No\tTime\tCH01\tCH02\tCH03\tCH04\tCH05\tCH06\tCH07\tCH08\tCH09\tCH10\tPLS1\tPLS2\t");
    rcnt=0;
    do
    {
        s = CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
        if(rcnt++>100)break;
    }
    while(s != 0x14);
    Display_Note(x-2,y+Ydiv*11+40,NoteS[0].Ch[DataSave.Data_type.LANG]);
    while(1)
    {
        if(RereadSDF(RCNTALLF++)!=0)
        {
            iapprocesstemp = (u32)(float)(((RCNTALLF)*sizeof(DsdF.uD))/(float)(f_size(&fnew))*(float)100.0);
            if(iapprocesstemp>iapprocess)
            {
                iapprocess=iapprocesstemp;
                if(iapprocess>100)
                    iapprocess=100;
                memset(str,0,sizeof(str));
                uitoa(iapprocess,str);
                strcat(str,"%");
                LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
                LCD_SetFont(&Font16);
                if(CHI == DataSave.Data_type.LANG )
                    LCD_DisplayStringLine_EN_CH_LJ(x-2+192, y+Ydiv*11+40, (uint8_t *)str);
                else
                    LCD_DisplayStringLine_EN_CH_LJ(x-2+192, y+Ydiv*11+40, (uint8_t *)str);
                LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
            }
            memset(buf,0,sizeof(buf));
            strcat(buf,"\r\n");
            uitoa(RCNTALLF,&buf[2]);
            strcat(buf,"\t");
            for(i=0; i<CHANNUM; i++)
            {
                if(DDaF[i][0].DP.Dxy.X!=0)
                {
                    memset(str,0,sizeof(str));
                    if((DsdF.Data.DataType&0xf)==(CmdR_DATA&0xf))
                        ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                    else if((DsdF.Data.DataType&0xf)==(CmdR_FDATA&0xf))
                        ShowDateTimeF(DsdF.Data.starttick,DsdF.Data.CHD[i].SDData.endtick,DsdF.Data.Timestamp,str);   //��ʾʱ��
                    strcat(buf,str);
                    break;
                }
            }
            memset(str,0,sizeof(str));
            USB_Str(str,'\t');
            strcat(buf,str);
            rcnt=0;
            do
            {

                s=CH376ByteWrite( ( u8*)buf, strlen((const char *)buf), NULL );
                if(rcnt++>100)break;
            }
            while(s != 0x14);
        }
        else
        {
            rcnt=0;
            do
            {
                s = CH376FileClose(TRUE);
                if(rcnt++>100)break;
            }
            while(s != 0x14);
            break;
        }
    }
}

// ��ʾ�Ի����ʼ��
void TiShiDiaInit(uint8 keytmp,struct RDispPara* RD,struct CUR* cur,uint8 type)
{
    uint8 n;
//    uint16 Xdiv=60;
    uint16 Ydiv=32;
    struct RDispPara x1y1;
    struct RDispPara x2y2;

    for(n=0; n<DisTiShiCAM; n++)
    {
        DisTiShi[n].flag = 1;
        DisTiShi[n].disp=0;
        DisTiShi[n].dispold=0;
        DisTiShi[n].lang =DataSave.Data_type.LANG;
        DisTiShi[n].Num.bit = 2;
        DisTiShi[n].Num.dot= 2;
        DisTiShi[n].Num.Decimalbit= 0;
    }

    DisTiShi[0].cind=16;
    DisTiShi[0].index[DisTiShi[0].cind] = 0;
    DisTiShi[0].disp=1;
    DisTiShi[1].cind=16;
    DisTiShi[1].index[DisTiShi[1].cind] = 1;
    DisTiShi[1].disp=1;
    DisTiShi[1].sel=1;
    DisTiShi[1].selold=0;
    DisTiShi[1].flag = 0;
    x1y1.Box.x1 =  100;//ȡ��
    x1y1.Box.y1 =  100;//ȡ
    x2y2.Box.x2=  500;//ȡ��
    x2y2.Box.y2 = 300 ;//ȡ��
//    Xdiv=80;
    Disp_Box_Zhi(x1y1.Box.x1, x1y1.Box.y1,x2y2.Box.x2, x2y2.Box.y2,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    LCD_DrawHLine(x1y1.Box.x1, x1y1.Box.y1+22, x2y2.Box.x2-x1y1.Box.x1);//����
    LCD_DrawHLine(x1y1.Box.x1, x2y2.Box.y2-Ydiv, x2y2.Box.x2-x1y1.Box.x1);
    LCD_SetTextColor(LCD_COLOR_BLUE);
    LCD_FillRect(x1y1.Box.x1+1, x1y1.Box.y1+1, x2y2.Box.x2-x1y1.Box.x1-2, 21); //�������ɫ
    LCD_SetTextColor(LCD_COLOR_WHITE);

    Ydiv=24;
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_DisplayStringLineLJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "��ʾ",FONTTYPE16);
        LCD_SetBackColor(LCD_COLOR_BLACK);
        if(type==0)
            LCD_DisplayStringLineLJ(x1y1.Box.x1+32, x1y1.Box.y1+60, "�Ƿ�ɾ�����ļ���",FONTTYPE16);
        else if(type==1)
            LCD_DisplayStringLineLJ(x1y1.Box.x1+32, x1y1.Box.y1+60, "�Ƿ�ָ��������ã�",FONTTYPE16);
        else if(type==2)
            LCD_DisplayStringLineLJ(x1y1.Box.x1+32, x1y1.Box.y1+60, "��ʽ��ʱ��ϳ����Ƿ��ʽ����",FONTTYPE16);

    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+2, x1y1.Box.y1+2, "Note");
        LCD_SetBackColor(LCD_COLOR_BLACK);
        if(type==0)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+32, x1y1.Box.y1+60, "Will the file be deleted?");
        else if(type==1)
            LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+22, x1y1.Box.y1+60, "Will factory settings be restore?");
        else if(type==2)
        {
            LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+32, x1y1.Box.y1+60, "Long time to format, do you");
            LCD_DisplayStringLine_EN_CH_LJ(x1y1.Box.x1+20, x1y1.Box.y1+80, "want to format?");
        }
    }
}
//��ʾ������ļ�ֵ��������
void ModifyTiShi(uint8 keytmp,struct RDispPara* RD,struct RDispPara* RDOld,struct CUR* cur,char MaxCam)
{
    OS_ERR err;
    switch(keytmp)
    {
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//��ʱ200ms
            }
            break;

        case KEY_LEFT://��
            DisTiShi[cur->CAM].selold=1;
            DisTiShi[cur->CAM].sel=0;
            do
            {
                if(cur->CAM==0)
                    cur->CAM= MaxCam-1;
                else
                    cur->CAM--;
            }
            while(DisTiShi[cur->CAM].disp!=1);  //����ʾ����
            DisTiShi[cur->CAM].selold=0;
            DisTiShi[cur->CAM].sel=1;
            break;
        case KEY_RIGHT:
            DisTiShi[cur->CAM].selold=1;
            DisTiShi[cur->CAM].sel=0;
            do
            {
                if( ++cur->CAM>MaxCam-1)
                    cur->CAM =0 ;
            }
            while(DisTiShi[cur->CAM].disp!=1);  //����ʾ����
            DisTiShi[cur->CAM].selold=0;
            DisTiShi[cur->CAM].sel=1;
            break;

        case KEY_ENTER:

            break;
        case KEY_ESC:
            RD->index[RD->cind] = RDOld->index[RDOld->cind];
            break;
    }


}
//��ʾ�Ի���
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
//    char ptr[10] = {0};
    Cur.CAM=1;
    Cur.COL=0;
    DisLogOld = *RD;
    x1y1.Box.x1 =  100;//ȡ��
    x1y1.Box.y1 =  100;//
    x2y2.Box.x2=  500;//ȡ��
    x2y2.Box.y1 = 276;//ȡ��
    x2y2.Box.y2 = 300;//ȡ��
    TiShiDiaInit(keytmp,RD, cur,type) ;
    Xdiv=80;
//    Ydiv=24;

    do
    {
        //  Ydiv=24;
        Disp_Box_Home(&DisTiShi[0],x1y1.Box.x1+Xdiv,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv+60,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        Disp_Box_Home(&DisTiShi[1],x1y1.Box.x1+Xdiv*3,x2y2.Box.y1-4,x1y1.Box.x1+Xdiv*3+120,x2y2.Box.y2-4,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
        //   Ydiv=32;
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
    Stu_FileMid_Init();
    ModifyTiShi(keytmp,RD,&DisLogOld,&Cur,DisTiShiCAM);
    OSSchedUnlock(&err);
    return Cur.CAM;
}



uint8 ModifyFile(uint8 keytmp,struct CUR* cur,struct CUR* curold)                 // ��ֵƥ�亯������while��ѭ���з���ƥ��
{
    FRESULT res_sd;
    OS_ERR err;
    static struct RDispStr RD;
    static char oldname[13] = {0};
    static char ALLname[16] ="0:/\0";
    *curold=*cur;
    cur->OPN=keytmp;                        //����ֵ����������־��������ǰ�Ĳ���״̬
    switch(cur->OPN)
    {
        case KEY_FUNC1://�ļ���
            if(strstr( FileEn[0].Ch[cur->COL],".BIN")!=NULL||strstr( FileEn[0].Ch[cur->COL],".bin")!=NULL)
            {
                EndSD();//��ǰ����������عر��ļ�
                res_sd = f_open(&fnew, FileEn[0].Ch[cur->COL],FA_OPEN_EXISTING | FA_READ );
                if(res_sd==FR_OK)
                    Record_Page();
            }
            break;
        case KEY_FUNC2://������
            RDispPara2RDispStr(&RD,DisFile[cur->COL][cur->CAM]);
            SoftKeyDiaFile(keytmp,&RD,cur);
            memset(oldname,0,sizeof(oldname));
            RDispStr2RDispPara(oldname,RD,&DisFile[cur->COL][cur->CAM]);
            memset(ALLname,0,sizeof(ALLname));
            strcpy(ALLname,"0:/\0");
            strcat(ALLname,oldname);
            EndSD();
            f_rename(ALLname,FileEn[DisFile[cur->COL][cur->CAM].cind].Ch[DisFile[cur->COL][cur->CAM].index[DisFile[cur->COL][cur->CAM].cind]]);
            break;
        case KEY_FUNC3://ɾ��
            if(TiShiDia(keytmp,&DisFile[cur->COL][cur->CAM],cur,0)==0)
            {
                f_unlink(FileEn[0].Ch[cur->COL]);
            }
            Stu_File_Init(); //����ˢ
            page_file(FilePage);
            break;
        case KEY_FUNC4://EXL
            if(usbstatus == CONNECTED)
            {
                if(strstr( FileEn[0].Ch[cur->COL],".BIN")!=NULL||strstr( FileEn[0].Ch[cur->COL],".bin")!=NULL)
                {
                    EndSD();//��ǰ����������عر��ļ�
                    res_sd = f_open(&fnew, FileEn[0].Ch[cur->COL],FA_OPEN_EXISTING | FA_READ );
                    if(res_sd==FR_OK)
                        SD2USBEXL(FileEn[0].Ch[cur->COL]);
                }
            }
            break;
        case KEY_FUNC5://TXT
            if(usbstatus == CONNECTED)
            {
                if(strstr( FileEn[0].Ch[cur->COL],".BIN")!=NULL||strstr( FileEn[0].Ch[cur->COL],".bin")!=NULL)
                {
                    EndSD();//��ǰ����������عر��ļ�
                    res_sd = f_open(&fnew, FileEn[0].Ch[cur->COL],FA_OPEN_EXISTING | FA_READ );
                    if(res_sd==FR_OK)
                        SD2USBTXT(FileEn[0].Ch[cur->COL]);
                }
            }
            break;
        case KEY_MANU://��������
            DataSave.Data_type.PageFlag =  FPageHome ;
            break;
        case KEY_ESC:
            DataSave.Data_type.PageFlag =  PageFlagSetRe;
            break;
        case KEY_0:
            if(ScreenFlag==OFF)
            {
                ScreenFlag=ON;
                OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//��ʱ200ms
            }
            break;
        case KEY_UP://��
            do
            {
                if(cur->COL>0)
                    cur->COL--;
                else
                    cur->COL=FileCOL;
            }
            while(DisFile[cur->COL][cur->CAM].disp!=1&&FileMaxCol > 0);  //����ʾ����
            break;
        case KEY_DOWN:
            do
            {
                if(cur->COL<FileCOL) //���һ���ǹ��ܼ�
                    cur->COL++;
                else
                    cur->COL=0;
            }
            while(DisFile[cur->COL][cur->CAM].disp!=1&&FileMaxCol > 0);  //����ʾ����
            break;
        case KEY_ACC:
            do
            {
                cur->COL+=2;
                if(cur->COL>FileCOL)
                    cur->COL=1;
            }
            while(DisFile[cur->COL][cur->CAM].disp!=1&&FileMaxCol > 0);  //����ʾ����
            break;
        case KEY_LEFT://
            if(FilePage>FilePageMin)
                FilePage--;
            Stu_File_Init(); //����ˢ��
            page_file(FilePage);
            break;
        case KEY_RIGHT:
            if(FilePage<FilePageMax)
                FilePage++;
            Stu_File_Init(); //����ˢ��
            page_file(FilePage);
            break;
        case KEY_ENTER:

            break;
    }
    Stu_file_cycle(cur,curold);
    return keytmp;
}

//�ļ��˵�
void DoFile_task(void *p_arg)
{
    OS_ERR err;
    static uint8 keytmp=0;
    //��ҳ
    Stu_File_Init();
    page_file(FilePage);
    while(1)
    {
        //DoWork����
//        BluidICOTask();
        keytmp = KeyTaskCreate(NoNull_ReTurn);
        //  OSTaskDel(&ICOTaskTCB,&err);
        keytmp=ModifyFile(keytmp,&CurFile,&CurFileOld);     //������ֵ
        if(PageFlagold!=DataSave.Data_type.PageFlag )
        {
            OSSemPost(&Page_SEM,OS_OPT_POST_1,&err);
        }
        page_file_cycle(CurFile);
    }
}

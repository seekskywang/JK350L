/*
  * @file    Home.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief   ���������ʾ�ؼ�
 */
//���ļ�Ϊ10��ͨ��������ҳ�� ��ӵ�һ��ҳ��
#include  "include.h"
//�α�  ���к���
struct CUR CurDisp = {0};
struct CUR CurDispOld= {0};
union Data_union DataSave;
//���ò˵�10��ͨ����������
struct RDispPara DisLog[HCOL+2][HCAM] = {0};
//10��ͨ���������
const struct RStr HCh[RStrNUM] =
{
    {"��"},   //���Ƿ���
    {"��","��ѹ","�¶�","ʪ��","����","����"},  //������ʽ
    {"2PTh","3PTh","4PTh","2PTt","3PTt","4PTt","TC-R","TC-S","TC-B","TC-K","TC-E","TC-T","TC-J","TC-N"},//�������¶��ǳ���
    {"0-1V","100V"},//�����ڵ�ѹ�ǳ���,//�����ڵ�ѹ�ǳ���
    {"Off","2","5","10","20","40"},
    {"Off >","On  >"},
    {">"},
    {"��ʾ"},  // �׿��ݼ���ť����
    {"���ݴ���"},// �׿��ݼ���ť����
    {"�ļ�"},// �׿��ݼ���ť����
    {"�ӿ�"},// �׿��ݼ���ť����
    {"����"},// �׿��ݼ���ť����
    {"1800$","1472&"},// �¶�ֵ��
    {"100%"},//ʪ��ֵ��
    {"Off" },
    {"��","��" },
    {"ȷ��","����ȡ��" },
    {"     " },//����  ������
    {"��  ","�ϳ�","�³�","����" },
    {"20mA"},   //����ֵ��
    {"999kg"},   //����ֵ��
    {"AM2305","HT800","AC3000"},   //������
    {"������","0-1V","100V"},   //������
    {"������","0-1V","100V"},   //������

};    //
const struct RStr HEn[RStrNUM]=
{
    {"��"},//���Ƿ���
    {"Off","DC","TEMP","RH","DI","GR"},
    {"2PTh","3PTh","4PTh","2PTt","3PTt","4PTt","TC-R","TC-S","TC-B","TC-K","TC-E","TC-T","TC-J","TC-N"},//�������¶��ǳ���
    {"0-1V","100V"},//�����ڵ�ѹ�ǳ���
    {"Off","2","5","10","20","40"},
    {"Off >","On  >"},
    {">"},
    {"AMP"},// �׿��ݼ���ť����
    {"DATA"},// �׿��ݼ���ť����
    {"FILE"},// �׿��ݼ���ť����
    {"I/F"},// �׿��ݼ���ť����
    {"OTUR"},// �׿��ݼ���ť����
    {"1800$","1472&"},// �¶�ֵ��
    {"100%"},//ʪ��ֵ��
    {"Off" },
    {"Off","On" },
    {"OK","Cancel" },
    {"     " },//����
    {"Off ","Hi","Low","Out" },
    {"20mA"},//����ֵ��
    {"999kg"},   //����ֵ��
    {"AM2305","HT800","AC3000"},   //������
    {"I-S","0-1V","100V"},   //������
    {"I-S","0-1V","100V"},   //������

};    //

//�����˵���ʾ
static const struct HelpStr helpS[7]=
{
    {"����:���Ҽ��л���ʾ���úͱ������á�\0","L or R key switch display settings and alarm settings.\0"},
    {"����:���趨���ʵ��������͡�\0","Please set the appropriate input type.\0"},
    {"����:��ѡ����ʵ��¶ȴ��������͡�\0","Please select the appropriate temperature sensor type.\0"},
    {"����:�趨����ֵ��Χ��","Set the input value range."},
    {"����:�趨�˲������ƶ�ƽ����������\0","Set filter, moving average number.\0"},
    {"����:�趨�������ݵ�λ��ϵ�л���\0","Set test data unit relation switch.\0"},
    {"����:������ʾ��Χ���������á�\0","Curve display range upper and lower limit settings.\0"},
};


void QFlashDataSave_home_Init(void)
{
    uint8 m,i;
    DataSave.Data_type.SW=0;
    for(m=0; m<CHANNUM+2; m++)
    {

        DataSave.Data_type.TempAdjMeas[m].bit=4;           //����λ����
        DataSave.Data_type.TempAdjMeas[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.TempAdjMeas[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.TempAdjEu[m].bit=4;
        DataSave.Data_type.TempAdjEu[m].Decimalbit=0;
        DataSave.Data_type.TempAdjEu[m].Symbol=0;

        DataSave.Data_type.DCUpMeas[m].bit=1;
        DataSave.Data_type.DCUpMeas[m].Decimalbit=0;
        DataSave.Data_type.DCUpMeas[m].Symbol=0;

        DataSave.Data_type.DCUpEu[m].bit=1;
        DataSave.Data_type.DCUpEu[m].Decimalbit=0;
        DataSave.Data_type.DCUpEu[m].Symbol=0;

        DataSave.Data_type.DCLowMeas[m].bit=1;
        DataSave.Data_type.DCLowMeas[m].Decimalbit=0;
        DataSave.Data_type.DCLowMeas[m].Symbol=0;

        DataSave.Data_type.DCLowEu[m].bit=1;
        DataSave.Data_type.DCLowEu[m].Decimalbit=0;
        DataSave.Data_type.DCLowEu[m].Symbol=0;

        for(i=0; i<VRVGSensorCOL; i++)
        {
            DataSave.Data_type.DIUpMeas[i][m].bit=3;
            DataSave.Data_type.DIUpMeas[i][m].Decimalbit=3;
            DataSave.Data_type.DIUpMeas[i][m].Symbol=0;

            DataSave.Data_type.DIUpEu[i][m].bit=6;
            DataSave.Data_type.DIUpEu[i][m].Decimalbit=0;
            DataSave.Data_type.DIUpEu[i][m].Symbol=0;

            DataSave.Data_type.DILowMeas[i][m].bit=3;
            DataSave.Data_type.DILowMeas[i][m].Decimalbit=3;
            DataSave.Data_type.DILowMeas[i][m].Symbol=0;

            DataSave.Data_type.DILowEu[i][m].bit=6;
            DataSave.Data_type.DILowEu[i][m].Decimalbit=0;
            DataSave.Data_type.DILowEu[i][m].Symbol=0;



            DataSave.Data_type.VGUpA[i][m].bit=3;
            DataSave.Data_type.VGUpA[i][m].Decimalbit=3;
            DataSave.Data_type.VGUpA[i][m].Symbol=0;


            DataSave.Data_type.VGUpB[i][m].bit=6;
            DataSave.Data_type.VGUpB[i][m].Decimalbit=0;
            DataSave.Data_type.VGUpB[i][m].Symbol=0;


            DataSave.Data_type.VGLowA[i][m].bit=3;
            DataSave.Data_type.VGLowA[i][m].Decimalbit=3;
            DataSave.Data_type.VGLowA[i][m].Symbol=0;


            DataSave.Data_type.VGLowB[i][m].bit=6;
            DataSave.Data_type.VGLowB[i][m].Decimalbit=0;
            DataSave.Data_type.VGLowB[i][m].Symbol=0;




            DataSave.Data_type.VRUpA[i][m].bit=3;
            DataSave.Data_type.VRUpA[i][m].Decimalbit=1;
            DataSave.Data_type.VRUpA[i][m].Symbol=0;


            DataSave.Data_type.VRUpB[i][m].bit=3;
            DataSave.Data_type.VRUpB[i][m].Decimalbit=1;
            DataSave.Data_type.VRUpB[i][m].Symbol=0;


            DataSave.Data_type.VRLowA[i][m].bit=3;
            DataSave.Data_type.VRLowA[i][m].Decimalbit=1;
            DataSave.Data_type.VRLowA[i][m].Symbol=0;


            DataSave.Data_type.VRLowB[i][m].bit=3;
            DataSave.Data_type.VRLowB[i][m].Decimalbit=1;
            DataSave.Data_type.VRLowB[i][m].Symbol=0;


        }



//  ����y��ķ�Χ
        DataSave.Data_type.TempMax[m].bit=4;           //����λ����
        DataSave.Data_type.TempMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.TempMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.TempMin[m].bit=4;
        DataSave.Data_type.TempMin[m].Decimalbit=0;
        DataSave.Data_type.TempMin[m].Symbol=0;

        if(DataSave.Data_type.TempMax[m].fval-DataSave.Data_type.TempMin[m].fval<=1.0F)
        {
            DataSave.Data_type.TempMax[m].fval =TEMPMAXVAL;
            DataSave.Data_type.TempMin[m].fval =TEMPMINVAL;
        }
        DataSave.Data_type.DCMax[m].bit=3;           //����λ����
        DataSave.Data_type.DCMax[m].Decimalbit=3;   //С��λ����
        DataSave.Data_type.DCMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.DCMin[m].bit=3;
        DataSave.Data_type.DCMin[m].Decimalbit=3;
        DataSave.Data_type.DCMin[m].Symbol=0;


        if(DataSave.Data_type.DCMax[m].fval-DataSave.Data_type.DCMin[m].fval<=0.001F)
        {
            DataSave.Data_type.DCMax[m].fval = 1;
            DataSave.Data_type.DCMin[m].fval =-1;
        }


        DataSave.Data_type.DIMax[m].bit=6;           //����λ����
        DataSave.Data_type.DIMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.DIMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.DIMin[m].bit=6;
        DataSave.Data_type.DIMin[m].Decimalbit=0;
        DataSave.Data_type.DIMin[m].Symbol=0;


        if(DataSave.Data_type.DIMax[m].fval-DataSave.Data_type.DIMin[m].fval<=0.1F)
        {
            DataSave.Data_type.DIMax[m].fval = 1;
            DataSave.Data_type.DIMin[m].fval =-1;
        }

        DataSave.Data_type.VGMax[m].bit=6;           //����λ����
        DataSave.Data_type.VGMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.VGMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.VGMin[m].bit=6;
        DataSave.Data_type.VGMin[m].Decimalbit=0;
        DataSave.Data_type.VGMin[m].Symbol=0;


        if(DataSave.Data_type.VGMax[m].fval-DataSave.Data_type.VGMin[m].fval<=0.1F)
        {
            DataSave.Data_type.VGMax[m].fval = 100;
            DataSave.Data_type.VGMin[m].fval = 0;
        }


        DataSave.Data_type.VRMax[m].bit=3;           //����λ����
        DataSave.Data_type.VRMax[m].Decimalbit=1;   //С��λ����
        DataSave.Data_type.VRMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.VRMin[m].bit=3;
        DataSave.Data_type.VRMin[m].Decimalbit=1;
        DataSave.Data_type.VRMin[m].Symbol=0;


        if(DataSave.Data_type.VRMax[m].fval-DataSave.Data_type.VRMin[m].fval<=0.1F)
        {
            DataSave.Data_type.VRMax[m].fval = 100;
            DataSave.Data_type.VRMin[m].fval = 0;
        }


        //������˸�������Сֵ
        DataSave.Data_type.TempLMax[m].bit=4;           //����λ����
        DataSave.Data_type.TempLMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.TempLMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.TempLMin[m].bit=4;
        DataSave.Data_type.TempLMin[m].Decimalbit=0;
        DataSave.Data_type.TempLMin[m].Symbol=0;

        if(DataSave.Data_type.TempLMax[m].fval-DataSave.Data_type.TempLMin[m].fval<=1.0F)
        {
            DataSave.Data_type.TempLMax[m].fval =TEMPMAXVAL;
            DataSave.Data_type.TempLMin[m].fval =TEMPMINVAL;
        }
        DataSave.Data_type.DILMax[m].bit=6;           //����λ����
        DataSave.Data_type.DILMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.DILMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.DILMin[m].bit=6;
        DataSave.Data_type.DILMin[m].Decimalbit=0;
        DataSave.Data_type.DILMin[m].Symbol=0;

        if(DataSave.Data_type.DILMax[m].fval-DataSave.Data_type.DILMin[m].fval<=0.1F)
        {
            DataSave.Data_type.DILMax[m].fval = 1;
            DataSave.Data_type.DILMin[m].fval =-1;
        }

        DataSave.Data_type.VGLMax[m].bit=6;           //����λ����
        DataSave.Data_type.VGLMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.VGLMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.VGLMin[m].bit=6;
        DataSave.Data_type.VGLMin[m].Decimalbit=0;
        DataSave.Data_type.VGLMin[m].Symbol=0;

        if(DataSave.Data_type.VGLMax[m].fval-DataSave.Data_type.VGLMin[m].fval<=0.1F)
        {
            DataSave.Data_type.VGLMax[m].fval = 100;
            DataSave.Data_type.VGLMin[m].fval = 0;
        }

        DataSave.Data_type.VRLMax[m].bit=4;           //����λ����
        DataSave.Data_type.VRLMax[m].Decimalbit=0;   //С��λ����
        DataSave.Data_type.VRLMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.VRLMin[m].bit=4;
        DataSave.Data_type.VRLMin[m].Decimalbit=0;
        DataSave.Data_type.VRLMin[m].Symbol=0;

        if(DataSave.Data_type.VRLMax[m].fval-DataSave.Data_type.VRLMin[m].fval<=0.1F)
        {
            DataSave.Data_type.VRLMax[m].fval = 100;
            DataSave.Data_type.VRLMin[m].fval = 0;
        }

        DataSave.Data_type.DCLMax[m].bit=3;           //����λ����
        DataSave.Data_type.DCLMax[m].Decimalbit=3;   //С��λ����
        DataSave.Data_type.DCLMax[m].Symbol=0;     //���� 0 Ϊ�� 1Ϊ����
        DataSave.Data_type.DCLMin[m].bit=3;
        DataSave.Data_type.DCLMin[m].Decimalbit=3;
        DataSave.Data_type.DCLMin[m].Symbol=0;

        if(DataSave.Data_type.DCLMax[m].fval-DataSave.Data_type.DCLMin[m].fval<=0.001F)
        {
            DataSave.Data_type.DCLMax[m].fval = 1;
            DataSave.Data_type.DCLMin[m].fval =-1;
        }
    }

}
void QFlashData_home_Init(void)
{
    uint8 m,n,z;
    //����ָ�
    for(m=0; m<HCOL+2; m++)
    {
        for(n=0; n<HCAM; n++)
        {
            if(n<HCAM-2&&m<HCOL-1&&m>0)
			{
                memcpy(DisLog[m][n].index,DataSave.Data_type.DisLog[m][n].index,sizeof(DisLog[m][n].index));
			}else if(n<HCAM-2&&m<HCOL+2&&m>HCOL-1){
				memcpy(DisLog[m][n].index,DataSave.Data_type.DisLog[m-1][n].index,sizeof(DisLog[m][n].index));
			}
        }
    }
	DisLog[1][0].index[DisLog[2][0].cind] = VT;
	DisLog[2][0].index[DisLog[2][0].cind] = VT;
	DisLog[3][0].index[DisLog[3][0].cind] = VT;
	DisLog[4][0].index[DisLog[4][0].cind] = VT;
	DisLog[5][0].index[DisLog[5][0].cind] = VT;
	DisLog[6][0].index[DisLog[6][0].cind] = VT;
	DisLog[7][0].index[DisLog[7][0].cind] = VT;
	DisLog[8][0].index[DisLog[8][0].cind] = VT;
	DisLog[9][0].index[DisLog[9][0].cind] = VT;
	DisLog[10][0].index[DisLog[10][0].cind] = VT;
	DisLog[11][0].index[DisLog[11][0].cind] = VR;
	DisLog[13][0].index[DisLog[13][0].cind] = VR;
	DisLog[14][0].index[DisLog[14][0].cind] = VR;

    for(m=0; m<HCOL+2; m++)
    {
        n=0;
        if(m==1)
        {
            if(DisLog[m][n].index[DisLog[m][n].cind]==OFF)//OFF
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=0;
                    DisLog[m][z].dispold=1;
                }
            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VDC)//��ѹ
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].disp=0;
                DisLog[m][1].dispold=1;
                DisLog[m][2].cind =3;
                DisLog[m][4].cind =5;
                if(m==1)
                    DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDC[CHANNUM];
                else
                    DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDC[m-2];
            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VT)//�¶�
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }
                DisLog[m][1].cind =2;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =12;
                DisLog[m][2].index[DisLog[m][2].cind]=DataSave.Data_type.TUnit;
                DisLog[m][4].cind =5;
                if(m==1)
                    DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeT[CHANNUM];
                else
                    DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeT[m-2];

            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VR)//ʪ��
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].cind =21;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =13;
                DisLog[m][2].index[DisLog[m][2].cind]=0;
                DisLog[m][4].cind =5;//OFF
                DisLog[m][4].index[DisLog[m][4].cind]=ON;
            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VI)//����
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].cind =22;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =19;
                DisLog[m][4].cind =5;//OFF
                if(m==1)
                    DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDI[DisLog[m][1].index[DisLog[m][1].cind]][CHANNUM];
                else
                {
                    if(DisLog[m][1].index[DisLog[m][1].cind]==SenI)
                        DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDI[DisLog[m][1].index[DisLog[m][1].cind]][m-2];
                    else
                        DisLog[m][4].index[DisLog[m][4].cind]=ON;
                }

            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VG)//����
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].cind =23;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =20;
                DisLog[m][4].cind =5;//OFF
                DisLog[m][4].index[DisLog[m][4].cind]=ON;
            }

        }
        else if(m>1&&m<HCOL+2 && m != 12)
        {
            if(DisLog[m][n].index[DisLog[m][n].cind]== OFF)
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=0;
                    DisLog[m][z].dispold=1;
                }
            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VDC)
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }
                DisLog[m][1].disp=0;
                DisLog[m][1].dispold=1;
                DisLog[m][2].cind =3;
                DisLog[m][4].cind =5;
                DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDC[m-2];

            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VT)
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }
                DisLog[m][1].cind =2;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =12;
                DisLog[m][2].index[DisLog[m][2].cind]=DataSave.Data_type.TUnit;
                DisLog[m][4].cind =5;
                DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeT[m-2];

            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VR)
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].cind =21;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =13;
                DisLog[m][2].index[DisLog[m][2].cind]=0;
                DisLog[m][4].cind =5;//OFF
                DisLog[m][4].index[DisLog[m][4].cind]=ON;

            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VI)//����
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].cind =22;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =19;
                DisLog[m][4].cind =5;//OFF
                if(m>1)
                    DisLog[m][4].index[DisLog[m][4].cind]=DataSave.Data_type.ComputeDI[DisLog[m][1].index[DisLog[m][1].cind]][m-2];
                else
                {
                    if(DisLog[m][1].index[DisLog[m][1].cind]==SenI)
                        DisLog[m][4].index[DisLog[m][4].cind]=OFF;
                    else
                        DisLog[m][4].index[DisLog[m][4].cind]=ON;
                }
            }
            else if(DisLog[m][n].index[DisLog[m][n].cind]==VG)//����
            {
                for(z=1; z<HCAM; z++)
                {
                    DisLog[m][z].disp=1;
                    DisLog[m][z].dispold=0;
                }

                DisLog[m][1].cind =23;
                DisLog[m][2].disp=2;
                DisLog[m][2].dispold=0;
                DisLog[m][2].cind =20;
                DisLog[m][4].cind =5;//OFF
                DisLog[m][4].index[DisLog[m][4].cind]=ON;
            }
        }
    }
}

void Stu_home_Init(void)
{
    uint8 m,n;
    QFlashDataSave_home_Init();    //Qflash�洢���ݵĽṹ���ʼ��
	CurDisp.PAGE = 0;
//	if(CurDisp.PAGE == 0)
//	{
		for(m=0; m<HCOL+2; m++)
			for(n=0; n<HCAM; n++)
			{
				DisLog[m][n].col = m;//��
				DisLog[m][n].cam = n;//��
				DisLog[m][n].lang = DataSave.Data_type.LANG;
				DisLog[m][n].flag = 1;  //��ʼ����һ��ʾ��־
				DisLog[m][n].selold = 1;//��ǰȫѡ������ȫûѡ�� ˢ��
				DisLog[m][n].sel= 0;     //��ǰû��ѡ��
				DisLog[m][n].disp=1;  // �Ƿ���ʾ1 ��ʾ 0����ʾ
				DisLog[m][n].dispold=1; //֮ǰ�Ƿ���ʾ
				DisLog[m][n].Box.flag = 0;  //�ַ����Ƿ��п� 0 û�п�
				DisLog[m][n].FontCH.flag = 0;  //���ı�־
				DisLog[m][n].FontEN.flag = 0;  //Ӣ�ı�־
				if(m==0&&n==0)
				{
					DisLog[m][n].sel= 1;//��ǰΪѡ��״̬
					DisLog[m][n].selold = 0;//��һ��Ϊѡ��״̬
					DisLog[m][n].cind=0;//����
				}

				else if(m==0&&n>0)
				{
					DisLog[m][n].disp=0;
					DisLog[m][n].dispold=0;
				}
				else if(m>0&&m!=HCOL-1)
					DisLog[m][n].cind=n+1;
				else if(m==HCOL-1)
				{
					DisLog[m][n].cind=n+7;
					if(n==0)
					{
						DisLog[m][n].sel= 1;
						DisLog[m][n].selold = 0;
					}
				}
				if(DisLog[m][n].cind==1)
				{
					DisLog[m][n].Ceng=VALCIND-1;
				}
				else if(DisLog[m][n].cind==2)
				{
					DisLog[m][n].Ceng=9; //���������м���ѡ�� 9��
				}
				else if(DisLog[m][n].cind==3)
				{
					DisLog[m][n].Ceng=13;//���������м���ѡ�� 13��
				}
				else if(DisLog[m][n].cind==4)
				{
					DisLog[m][n].Ceng=6;
				}
			}
//	}
//	else if(CurDisp.PAGE == 1){
//		for(m=13; m<15; m++)
//			for(n=0; n<HCAM; n++)
//			{
//				DisLog[m][n].col = m;//��
//				DisLog[m][n].cam = n;//��
//				DisLog[m][n].lang = DataSave.Data_type.LANG;
//				DisLog[m][n].flag = 1;  //��ʼ����һ��ʾ��־
//				DisLog[m][n].selold = 1;//��ǰȫѡ������ȫûѡ�� ˢ��
//				DisLog[m][n].sel= 0;     //��ǰû��ѡ��
//				DisLog[m][n].disp=1;  // �Ƿ���ʾ1 ��ʾ 0����ʾ
//				DisLog[m][n].dispold=1; //֮ǰ�Ƿ���ʾ
//				DisLog[m][n].Box.flag = 0;  //�ַ����Ƿ��п� 0 û�п�
//				DisLog[m][n].FontCH.flag = 0;  //���ı�־
//				DisLog[m][n].FontEN.flag = 0;  //Ӣ�ı�־
//				if(m==0&&n==0)
//				{
//					DisLog[m][n].sel= 1;//��ǰΪѡ��״̬
//					DisLog[m][n].selold = 0;//��һ��Ϊѡ��״̬
//					DisLog[m][n].cind=0;//����
//				}

//				else if(m==0&&n>0)
//				{
//					DisLog[m][n].disp=0;
//					DisLog[m][n].dispold=0;
//				}
//				else if(m>0&&m<HCOL-1)
//					DisLog[m][n].cind=n+1;
//				else if(m==HCOL-1)
//				{
//					DisLog[m][n].cind=n+7;
//					if(n==0)
//					{
//						DisLog[m][n].sel= 1;
//						DisLog[m][n].selold = 0;
//					}
//				}
//				if(DisLog[m][n].cind==1)
//				{
//					DisLog[m][n].Ceng=VALCIND-1;
//				}
//				else if(DisLog[m][n].cind==2)
//				{
//					DisLog[m][n].Ceng=9; //���������м���ѡ�� 9��
//				}
//				else if(DisLog[m][n].cind==3)
//				{
//					DisLog[m][n].Ceng=13;//���������м���ѡ�� 13��
//				}
//				else if(DisLog[m][n].cind==4)
//				{
//					DisLog[m][n].Ceng=6;
//				}
//			}
//	}
    for(n=0; n<HCAM; n++)
    {
        DisLog[12][n].sel= 0;
        DisLog[12][n].selold = 1;
        DisLog[12][n].lang = DataSave.Data_type.LANG;
    }
    DisLog[12][0].sel= 1;
    DisLog[12][0].selold = 2;
    QFlashData_home_Init();//���ý����п������ѡ�����ָ�
    SetChanFilter();//�˲���ȫ�ֱ�������
    CurDisp.COL = 0;
    CurDisp.CAM = 0;
}
//�п����ݵĳ�ʼ��
void Stu_Mid_Init(void)
{
    uint8 m,n;
    for(m=0; m<HCOL+2; m++)
        for(n=0; n<HCAM; n++)
        {

            DisLog[m][n].lang = DataSave.Data_type.LANG;
            DisLog[m][n].flag = 1;
            DisLog[m][n].selold = 1;//��ǰȫѡ������ȫûѡ�� ˢ��
            DisLog[m][n].sel= 0;
            if(m==0&&n==0)
            {
                DisLog[m][n].selold = 0;
            }
        }
    DisLog[CurDisp.COL+12][CurDisp.CAM].sel= 1;
    DisLog[CurDisp.COL+12][CurDisp.CAM].selold = 0;
    DisLog[CurDisp.COL+12][CurDisp.CAM].flag = 0;
	DisLog[CurDisp.COL][CurDisp.CAM].sel= 1;
    DisLog[CurDisp.COL][CurDisp.CAM].selold = 0;
    DisLog[CurDisp.COL][CurDisp.CAM].flag = 0;
    DisLog[HCOL-1][0].sel= 1;
    DisLog[HCOL-1][0].selold = 0;
}
//�������ý���ĳ�ʼ��
void Stu_home_cycle(struct CUR *sle,struct CUR *sleold)
{
    uint8 m,n;
    for(m=0; m<HCOL; m++)
        for(n=0; n<HCAM+2; n++)
        {
            DisLog[m][n].flag = 0;
            DisLog[m][n].selold = 0;
            DisLog[m][n].sel= 0;
        }
    if( sleold->CAM!= sle->CAM||sleold->COL!=sle->COL)
    {
		if(sle->PAGE == 0)
		{
			DisLog[sle->COL][sle->CAM].selold = 0;//ˢ��
			DisLog[sle->COL][sle->CAM].sel= 1;
			DisLog[sleold->COL][sleold->CAM].selold = 1;//ˢ��
			DisLog[sleold->COL][sleold->CAM].sel= 0;
		}else if(sle->PAGE == 1){
			DisLog[sle->COL+12][sle->CAM].selold = 0;//ˢ��
			DisLog[sle->COL+12][sle->CAM].sel= 1;
			DisLog[sleold->COL+12][sleold->CAM].selold = 1;//ˢ��
			DisLog[sleold->COL+12][sleold->CAM].sel= 0;
		}
    }
}
//��ʼ��
void page_home(void)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    char str[5];
    LCD_SelectLayer(0);
    LCD_Clear(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);
    LCD_DrawRect(0, 2, 639, 430);
    LCD_DrawHLine(0, 380, 639);
    if(CHI == DataSave.Data_type.LANG )
    {
        if(DataSave.Data_type.SW==0)
        {
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(2,5, (uint8_t *)"��ʾ����",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
            LCD_DisplayStringLineLJ(2+96,5, (uint8_t *)"�ͱ�����Χ",FONTTYPE24);
        }
        else
        {

            LCD_DisplayStringLineLJ(2,5, (uint8_t *)"��ʾ������",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(2+120,5, (uint8_t *)"������Χ",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
        }

        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLineLJ(x+Xdiv,y, (uint8_t *)"����",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*2,y, (uint8_t *)"������",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*3,y, (uint8_t *)"ֵ��",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*4,y, (uint8_t *)"�˲���",FONTTYPE16);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*5,y, (uint8_t *)"EU");
        LCD_DisplayStringLineLJ(x+Xdiv*6,y, (uint8_t *)"����",FONTTYPE16);
    }
    else if(ENG == DataSave.Data_type.LANG)
    {
        if(DataSave.Data_type.SW==0)
        {
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(3,5, (uint8_t *)"DISPLAY",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
            LCD_DisplayStringLineLJ(2+112,5, (uint8_t *)"��RANG",FONTTYPE24);
        }
        else
        {

            LCD_DisplayStringLineLJ(3,5, (uint8_t *)"DISPLAY��",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_DARKGREEN);
            LCD_DisplayStringLineLJ(2+136,5, (uint8_t *)"RANG",FONTTYPE24);
            LCD_SetTextColor(LCD_COLOR_WHITE);
        }

        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv,y, (uint8_t *)"InPut");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*2,y, (uint8_t *)"Sensor");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*3,y, (uint8_t *)"Range");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*4,y, (uint8_t *)"Filter");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*5,y, (uint8_t *)"EU");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*6,y, (uint8_t *)"Misc.");
    }
    LCD_SetFont(&Font20);
	y = 60;
	Ydiv =28;
    if(CurDisp.PAGE == 0)
	{
		
		LCD_DisplayStringLine_EN_CH_LJ(x-5,y,(uint8_t *)"AL:M");
		for(i=1; i<10; i++)
		{
			sprintf(str,"%d :M",i);
			LCD_SetFont(&Font20);
			LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )str);
		}
	}else if(CurDisp.PAGE == 1){
		for(i=0; i<1; i++)
		{
			sprintf(str,"%d:M",i+11);
			LCD_SetFont(&Font20);
			LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )str);
		}
	}
    LCD_SetFont(&Font20);
    if(CurDisp.PAGE == 0)
	{
		sprintf(str,"%d:M",i);
	}else if(CurDisp.PAGE == 1){
		sprintf(str,"%d:M",i+11);
	}
    LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )str);
    page_home_cycle(CurDisp);
}


void page_Mindle(void) //�п�
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=95;
    uint16 Ydiv=90;
    uint16 i;
    uint8 lang;
    char str[5];
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawRect(2, 38, 636, 20);  //����
    LCD_DrawRect(0, 2, 639, 430);
    LCD_DrawHLine(0, 380, 639);
    lang = DataSave.Data_type.LANG; //��Ӣ�Ķ���
    if(CHI == lang  )
    {
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLineLJ(x+Xdiv,y, (uint8_t *)"����",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*2,y, (uint8_t *)"������",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*3,y, (uint8_t *)"ֵ��",FONTTYPE16);
        LCD_DisplayStringLineLJ(x+Xdiv*4,y, (uint8_t *)"�˲���",FONTTYPE16);
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*5,y, (uint8_t *)"EU");
        if(DataSave.Data_type.SW==0)
            LCD_DisplayStringLineLJ(x+Xdiv*6,y, (uint8_t *)"����",FONTTYPE16);
        else
        {
            LCD_DisplayStringLineLJ(x+Xdiv*6,y, (uint8_t *)"����",FONTTYPE16);
        }
    }
    else if(ENG == lang )
    {
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x,y, (uint8_t *)"CH:");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv,y, (uint8_t *)"InPut");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*2,y, (uint8_t *)"Sensor");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*3,y, (uint8_t *)"Range");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*4,y, (uint8_t *)"Filter");
        LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*5,y, (uint8_t *)"EU");
        if(DataSave.Data_type.SW==0)
            LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*6,y, (uint8_t *)"Misc.");
        else
            LCD_DisplayStringLine_EN_CH_LJ(x+Xdiv*6,y, (uint8_t *)"ALarm");
    }
    LCD_SetFont(&Font20);
    y = 60;
    Ydiv =28;
    
	if(CurDisp.PAGE == 0)
	{
		LCD_DisplayStringLine_EN_CH_LJ(x-5,y,(uint8_t *)"AL:M");
		for(i=1; i<10; i++)
		{
			sprintf(str,"%d :M",i);
			LCD_SetFont(&Font20);
			LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )str);
		}
	}else if(CurDisp.PAGE == 1){
		for(i=0; i<1; i++)
		{
			sprintf(str,"%d:M",i+10);
			LCD_SetFont(&Font20);
			LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )str);
		}
	}
    LCD_SetFont(&Font20);
	if(CurDisp.PAGE == 0)
	{
		sprintf(str,"%d:M",i);
	}else if(CurDisp.PAGE == 1){
		sprintf(str,"%d:M",i+10);
	}
    LCD_DisplayStringLine_EN_CH_LJ(x-5,y+Ydiv*i,(uint8_t* )str);
    page_home_cycle(CurDisp);
}

void Disp_Box_Home(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t bevel_edge,uint32_t Biancolor,uint32_t color)
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
void Disp_Box_Zi(struct RDispPara *sel,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
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
                Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
            }
        }
    }
    else if(sel->disp==0&&sel->dispold==1)
    {
        Disp_Box(x1,y1,x2,y2,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
    }
}

void LCD_DisplayStringLineLJ_Zi(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontCH.flag ==0)
    {
        sel->FontCH.x1=x;
        sel->FontCH.y1=y;
        sel->FontCH.flag = 1;
    }
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                BackColor=LCD_GetBackColor();
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLineLJ(x,y,(uint8_t *)HCh[sel->cind].Ch[sel->index[sel->cind]],FontType);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_BLACK);
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
        }
    }
}
void LCD_DisplayStringLine_EN_CH_LJ_Zi(struct RDispPara *sel,uint16_t x,uint16_t y)
{
    uint32_t TextColor;
    uint32_t BackColor;
    if(sel->FontEN.flag ==0)
    {
        sel->FontEN.x1=x;
        sel->FontEN.y1=y;
        sel->FontEN.flag = 1;
    }
    if(sel->flag==1&&sel->disp==1)
    {
        LCD_SetFont(&Font20);
        LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
        sel->flag=0;
    }
    else
    {
        if(sel->disp==1)
        {
            if(sel->sel!=sel->selold)
            {
                BackColor=LCD_GetBackColor();
                LCD_SetFont(&Font20);
                if(sel->sel==1)
                {
                    LCD_SetBackColor(LCD_COLOR_DARKGREEN);
                    LCD_DisplayStringLine_EN_CH_LJ(x,y,(uint8_t* )HEn[sel->cind].Ch[sel->index[sel->cind]]);
                    LCD_SetBackColor(BackColor);
                }
                else
                {
                    LCD_SetBackColor(LCD_COLOR_BLACK);
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
        }
    }
}


//����
void LCD_DisplayStringLineLJ_Home(struct RDispPara *sel,uint16_t x,uint16_t y,uint8_t FontType)
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

void LCD_DisplayNumLine_EN_CH_LJ_Home(struct RDispPara *sel,uint16_t x,uint16_t y,char* ptr,sFONT FontType)
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
            sel->Num.fvalold = sel->Num.fval;
        }
    }

}

void LCD_DisplayStringLine_EN_CH_LJ_Home(struct RDispPara *sel,uint16_t x,uint16_t y)
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


void Display_Help(uint16_t x,uint16_t y,const char *ptr)
{

    LCD_FillRectLJ(x-2, y-2, 632,40,LCD_COLOR_BLACK); //����±߿�
    if(CHI == DataSave.Data_type.LANG )
    {
        LCD_DisplayStringLineLJ(x, y, (uint8_t *)ptr,FONTTYPE16);
    }
    else
    {
        LCD_SetFont(&Font16);
        LCD_DisplayStringLine_EN_CH_LJ(x, y, (uint8_t *)"Help:");
        LCD_DisplayStringLine_EN_CH_LJ(x, y+20, (uint8_t *)ptr);
    }
}
void page_home_CHNum(const struct CUR cur)
{
    uint16 x=10;
    uint16 y;
    uint16 Ydiv;
    uint16 i=0;
    char str[5];
    y = 60;
    Ydiv =28;
   
	if(cur.PAGE == 0)
	{
		 if(cur.COL==i+1)
			LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y,(uint8_t *)"AL:M",&Font20,LCD_COLOR_DARKGREEN);
		else
			LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y,(uint8_t *)"AL:M",&Font20,LCD_COLOR_WHITE);
		for(i=1; i<10; i++)
		{
			sprintf(str,"%d :M",i);
			LCD_SetFont(&Font20);
			if(cur.COL==i+1)
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*i,(uint8_t* )str,&Font20,LCD_COLOR_DARKGREEN);
			else
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*i,(uint8_t* )str,&Font20,LCD_COLOR_WHITE);
		}
	}else if(cur.PAGE == 1){
		for(i=0; i<1; i++)
		{
			sprintf(str,"%d:M",i+11);
			LCD_SetFont(&Font20);
			if(cur.COL==i+1)
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*i,(uint8_t* )str,&Font20,LCD_COLOR_DARKGREEN);
			else
				LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*i,(uint8_t* )str,&Font20,LCD_COLOR_WHITE);
		}
	}
    LCD_SetFont(&Font20);
    if(CurDisp.PAGE == 0)
	{
		sprintf(str,"%d:M",i);
	}else if(CurDisp.PAGE == 1){
		sprintf(str,"%d:M",i+11);
	}
    if(cur.COL==i+1)
        LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*i,(uint8_t* )str,&Font20,LCD_COLOR_DARKGREEN);
    else
        LCD_DisplayStringLine_EN_CH_LJ_WithFC(x-5,y+Ydiv*i,(uint8_t* )str,&Font20,LCD_COLOR_WHITE);
}


void page_home_cycle(const struct CUR cur)
{
    uint16 x=10;
    uint16 y=39;
    uint16 Xdiv=90;
    uint16 Ydiv=90;
    uint16 i;
    LCD_DisplayStringLineLJ_Home(&DisLog[0][0],400,5,FONTTYPE24);
    COMmenu(&DisLog[12][0],&DisLog[12][1],&DisLog[12][2],&DisLog[12][3],&DisLog[12][4]);
    y = 60;
    Ydiv =28;
    if(cur.CAM==0&&cur.COL==0)
        Display_Help(x-2,y+Ydiv*11+20,helpS[0].Ch[DataSave.Data_type.LANG]);
    else if(cur.COL>0)
        Display_Help(x-2,y+Ydiv*11+20,helpS[cur.CAM+1].Ch[DataSave.Data_type.LANG]);
    page_home_CHNum(cur);//ͨ���Ÿ�����ʾ

    LCD_DisplayStringLineLJ_Home(&DisLog[0][0],400,5,FONTTYPE24);
	if(cur.PAGE == 0)
	{
		for(i=0; i<11; i++)
		{
			Disp_Box_Home(&DisLog[i+1][0],x-15+Xdiv,y+i*Ydiv+2,x-15+Xdiv+80,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+1][1],x-15+Xdiv*2,y+i*Ydiv+2,x-15+Xdiv*2+80,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+1][2],x-15+Xdiv*3+10,y+i*Ydiv+2,x-15+Xdiv*3+80+20,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+1][3],x-15+Xdiv*4+30,y+i*Ydiv+2,x-15+Xdiv*4+70+20,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+1][4],x-15+Xdiv*5+20,y+i*Ydiv+2,x-15+Xdiv*5+70+30,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+1][5],x-15+Xdiv*6+60,y+i*Ydiv+2,x-15+Xdiv*6+60+30,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
		}
	}else if(cur.PAGE == 1){
		for(i=0; i<2; i++)
		{
			Disp_Box_Home(&DisLog[i+12+1][0],x-15+Xdiv,y+i*Ydiv+2,x-15+Xdiv+80,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+12+1][1],x-15+Xdiv*2,y+i*Ydiv+2,x-15+Xdiv*2+80,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+12+1][2],x-15+Xdiv*3+10,y+i*Ydiv+2,x-15+Xdiv*3+80+20,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+12+1][3],x-15+Xdiv*4+30,y+i*Ydiv+2,x-15+Xdiv*4+70+20,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+12+1][4],x-15+Xdiv*5+20,y+i*Ydiv+2,x-15+Xdiv*5+70+30,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
			Disp_Box_Home(&DisLog[i+12+1][5],x-15+Xdiv*6+60,y+i*Ydiv+2,x-15+Xdiv*6+60+30,y+i*Ydiv+24,0,LCD_COLOR_HUI,LCD_COLOR_HUI);
		}
	}
    y = 64;
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_HUI);
	if(cur.PAGE == 0)
	{
		for(i=0; i<11; i++)
		{
			if(CHI == DataSave.Data_type.LANG )
				LCD_DisplayStringLineLJ_Home(&DisLog[i+1][0],x-5+Xdiv,y+i*Ydiv,FONTTYPE16);
			else
				LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][0],x-5+Xdiv,y+i*Ydiv);
			if(i < 10)
			{
				if(CHI == DataSave.Data_type.LANG)
				{
					if(DisLog[1+i][1].index[DisLog[1+i][1].cind]==0
					   &&(DisLog[1+i][0].index[1] == VI||DisLog[1+i][0].index[1]==VR||DisLog[1+i][0].index[1] == VG))
					{
						LCD_DisplayStringLineLJ_Home(&DisLog[i+1][1],x-5+Xdiv*2,y+i*Ydiv,FONTTYPE16);
					}
					else
						LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][1],x-5+Xdiv*2,y+i*Ydiv);
				}
				else
					LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][1],x-5+Xdiv*2,y+i*Ydiv);
			}else{
				LCD_DisplayStringLineLJ_Home(&DisLog[i+1][1],x-5+Xdiv*2,y+i*Ydiv,FONTTYPE16);
			}
			//��Ϊ�п�ᱻ���Ҫ�Ӵ˾�
			if(DisLog[i+1][2].disp==2&&DisLog[i+1][2].dispold==1)
			{
				DisLog[i+1][2].dispold=0;
			}
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][2],x-5+Xdiv*3+20,y+i*Ydiv);
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][3],x-5+Xdiv*4+30,y+i*Ydiv);
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][4],x-5+Xdiv*5+20,y+i*Ydiv);
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1][5],x-5+Xdiv*6+60,y+i*Ydiv);
		}
	}else if(cur.PAGE == 1){
		for(i=0; i<2; i++)
		{
////			DisLog[i+1+11][0].cind = 1;
////			DisLog[i+1+11][1].cind = 21;
////			DisLog[i+1+11][1].cind = 13;
////			DisLog[i+1+11][1].cind = 13;
//			
//			DisLog[i+1+11][0].index[DisLog[i+1+11][0].cind] = 0x03;
//			DisLog[i+1+11][1].index[DisLog[i+1+11][1].cind] = 0x00;
//			DisLog[i+1+11][2].index[DisLog[i+1+11][2].cind] = 0x00;
//			DisLog[i+1+11][3].index[DisLog[i+1+11][3].cind] = 0x00;
//			DisLog[i+1+11][4].index[DisLog[i+1+11][4].cind] = 0x01;
//			DisLog[i+1+11][5].index[DisLog[i+1+11][5].cind] = 0x00;
			if(CHI == DataSave.Data_type.LANG )
				LCD_DisplayStringLineLJ_Home(&DisLog[i+1+12][0],x-5+Xdiv,y+i*Ydiv,FONTTYPE16);
			else
				LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][0],x-5+Xdiv,y+i*Ydiv);
			if(CHI == DataSave.Data_type.LANG)
			{
				if((DisLog[1+i+12][1].index[DisLog[1+i+12][1].cind]==0
				   &&(DisLog[1+i+12][0].index[1] == VI||DisLog[1+i+12][0].index[1] == VG)) || DisLog[1+i+12][0].index[1]==VR)
				{
					LCD_DisplayStringLineLJ_Home(&DisLog[i+1+12][1],x-5+Xdiv*2,y+i*Ydiv,FONTTYPE16);
		 		}
				else
					LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][1],x-5+Xdiv*2,y+i*Ydiv);
			}
			else
				LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][1],x-5+Xdiv*2,y+i*Ydiv);
			//��Ϊ�п�ᱻ���Ҫ�Ӵ˾�
			if(DisLog[i+1+12][2].disp==2&&DisLog[i+1+12][2].dispold==1)
			{
				DisLog[i+1+12][2].dispold=0;
			}
			
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][2],x-5+Xdiv*3+20,y+i*Ydiv);
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][3],x-5+Xdiv*4+30,y+i*Ydiv);
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][4],x-5+Xdiv*5+20,y+i*Ydiv);
			LCD_DisplayStringLine_EN_CH_LJ_Home(&DisLog[i+1+12][5],x-5+Xdiv*6+60,y+i*Ydiv);
		}
	}
    LCD_SetBackColor(LCD_COLOR_BLACK);
}




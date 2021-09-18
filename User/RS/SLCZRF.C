/*
  * @file  Slczrf.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief  指令解析
 */
#include "include.h"
uint8 IsUpChar(uint8 a)               //判断a的值是大写字母的返回1，否则返回0
{
    int b;

    b = (a<0x41 || a>0x5a)?0:1;
    return b;
}
uint8 IsDigitChar(uint8 a)             //如果a的值是数字的话返回1，否则返回0
{
    int b;

    b = (a<0x30 || a>0x39)?0:1;
    return b;
}
//求整数部分位数
int Count(int n)
{
    int i;

    if(n==0)
    {
        i=1;
    }

    for(i=0; n!=0; i++)
    {
        n/=10;
    }
    return i;
}

//数据的小数位 整数位 位数定义
void SwitchChanalData ( struct NumStru* p, uint8 decimalbit)
{
    p->bit= Count ((int)p->fval);//整数部分位数
    p->dot=p->bit;
    p->Decimalbit = decimalbit;  //小数点后默认两位

}

int func(float x)
{
    if(x >(float) 0.0)
        return (int)(x + (float)0.5);
    else
        return (int)(x - (float)0.5);
}
//解析浮点型转换4个字节
void FloattoBYTE4(float fval,uint8 *buf)
{

    uint16 u16val=0;
    if(fval>100||fval<-100)    //
    {
        buf[0] =8<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*10);
        }
        else
        {
            u16val = (uint16)(fval*10);
        }

    }
    else if(fval>10||fval<-10)
    {
        buf[0] = 7<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*100);
        }
        else
        {
            u16val = (uint16)(fval*100);
        }
    }
    else if(fval>1||fval<-1)
    {
        buf[0] = 6<<2;
        if(fval<0)
        {
            buf[0]|=0x40;
            fval=-fval;
            u16val = (uint16)(fval*1000);
        }
        else
        {
            u16val = (uint16)(fval*1000);
        }
    }
    buf[0]|=(uint8)((u16val>>14)&0x3);
    buf[1]|=(uint8)((u16val>>7)&0x7F);
    buf[2]|=(uint8)((u16val)&0x7F);

}





//解析3个字节转换为浮点型
float BYTE3toFloat(uint8 *buf)
{
    double fval=0;
    float Eval =1;
    uint16 u16val=0;
    uint8 tbuf[3] = {0};
    memcpy(tbuf,buf,sizeof(tbuf));
    u16val =  (uint16)(((uint16)tbuf[0]&0x3)<<14)|(((uint16)tbuf[1]&0x7F)<<7)|(((uint16)tbuf[2])&0x7F);
    switch((buf[0]&0x3C)>>2)
    {
        case     0:         //
            Eval = 0.000000001;
            break;
        case        1:         //
            Eval = 0.00000001;
            break;
        case        2:         //
            Eval = 0.0000001;
            break;
        case        3:         //
            Eval = 0.000001;
            break;
        case        4:         //
            Eval = 0.00001;
            break;
        case        5:         //
            Eval = 0.0001;
            break;
        case        6:         //
            Eval = 0.001;
            break;
        case        7:         //
            Eval = 0.01;
            break;
        case        8:         //
            Eval = 0.1;
            break;
        case        9:         //
            Eval = 1.0;;
            break;
        case        10:         //
            Eval = 10.0;;
            break;
        case        11:         //
            Eval = 100.0;;
            break;
        case        12:         //
            Eval = 1000.0;;
            break;
        case        13:         //
            Eval = 10000.0;;
            break;
        case        14:         //
            Eval = 100000.0;;
            break;
        case        15:         //
            Eval = 1000000.0;
            break;
    }
    fval = (float)(Eval *(u16val*(double)(1.0)));

    if((buf[0]&0x40)==0x40)
    {
        fval *=-1;
    }
    return fval;

}


void   fStr2DfStr(struct SLData *p)//接收后解析的数据原始数据转换为数据显示 例如1000000v会切换为1MV
{

    if( p->vcind==VDC)  //类型为电压 电压单位有5种
    {
        if(p->fStr.fval>=1000000||p->fStr.fval<=-1000000) //大于1000000v
        {
            p->DfStr.fval = p->fStr.fval*(float)0.000001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=0;//MV
        }
        else if(p->fStr.fval>=1000||p->fStr.fval<=-1000) //大于1000v
        {
            p->DfStr.fval = p->fStr.fval*(float)0.001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=1;//KV
        }
        else if(p->fStr.fval>=1||p->fStr.fval<=-1) //大于1v
        {
            p->DfStr.fval = p->fStr.fval;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=2;//V
        }
        else if(p->fStr.fval>=(float)0.001||p->fStr.fval<=(float)(-0.001)) //大于0.001v
        {
            p->DfStr.fval = p->fStr.fval*1000;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=3;//mV

        }
        else if(p->fStr.fval>=(float)0.000001||p->fStr.fval<=(float)(-0.000001)) //大于0.001v
        {
            p->DfStr.fval = p->fStr.fval*1000000;
            SwitchChanalData (&p->DfStr,0); //小数点后3位调试
            p->UintOne.Col=4;//uV

        }
        else
        {
            p->DfStr = p->fStr;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=2;//V
        }
    }
    else if( p->vcind==VT)  //温度两种类型
    {
        p->DfStr =p->fStr;
        p->UintOne.Col= DataSave.Data_type.TUnit; //温度两种类型
        SwitchChanalData (&p->DfStr,2); //小数点后2位调试
    }
    else if( p->vcind==  VFRQ)  //频率类型
    {
        if(p->fStr.fval>=1000000000||p->fStr.fval<=-1000000000) //大于1000000000
        {
            p->DfStr.fval = p->fStr.fval*(float)0.000000001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=3;//G
        }
        else if(p->fStr.fval>=1000000||p->fStr.fval<=-1000000) //大于1000000
        {
            p->DfStr.fval = p->fStr.fval*(float)0.000001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=2;//M
        }
        else if(p->fStr.fval>=1000||p->fStr.fval<=-1000) //大于1000
        {
            p->DfStr.fval = p->fStr.fval*(float)0.001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=1;//K
        }
        else if(p->fStr.fval>=1||p->fStr.fval<=-1) //大于1
        {
            p->DfStr.fval = p->fStr.fval;
            SwitchChanalData (&p->DfStr,0); //小数点后3位调试
            p->UintOne.Col=0;//
        }
    }
    else if( p->vcind==VI)  //电流
    {
		if(p->fStr.fval>=1000||p->fStr.fval<=-1000) //大于1000
        {
            p->DfStr.fval = p->fStr.fval*(float)0.001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=1;//A
        }
        else//
        {
            p->DfStr.fval = p->fStr.fval;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=0;//mA
        }
    }
	else if( p->vcind==VG)  //重力进来都是g
    {
		if(p->fStr.fval>=1000) //大于1000
        {
            p->DfStr.fval = p->fStr.fval*(float)0.001;
            SwitchChanalData (&p->DfStr,3); //小数点后3位调试
            p->UintOne.Col=1;//kg
        }
		else
		{
			p->DfStr.fval = p->fStr.fval;
			p->UintOne.Col= 0; //重力单位g
			SwitchChanalData (&p->DfStr,3); //小数点后3位调试
		}
        
    }
	else if( p->vcind==VR)  //湿度 lj 20201109改
    {
        p->DfStr =p->fStr;
        p->UintOne.Col= 0; //非电压时只有一种单位
        SwitchChanalData (&p->DfStr,1); //小数点后1位调试
    }
    else
    {
        p->DfStr =p->fStr;
        p->UintOne.Col= 0; //非电压时只有一种单位
        SwitchChanalData (&p->DfStr,3); //小数点后3位调试
    }

}





uint32_t wd2dat24(int32_t i)
{
    uint32_t dat = 0, n, dot;
    if (i < 0)
    {
        dat |= 1 << 22;
        n = -i;
    }
    else
    {
        n = i;
    }

    if (n > 655350)
    {
        n /= 100;
        dot = 8; // 10e-1
    }
    else if (n > 65535)
    {
        n /= 10;
        dot = 7; // 10e-2
    }
    else
    {
        dot = 6; // 10e-3
    }

    dat |= dot << 18;
    dat |= (n & 0xC000) << 2;
    dat |= (n & 0x3F80) << 1;
    dat |= (n & 0x007F) << 0;
    return dat;
}

uint32_t volt2dat24(int32_t i)
{
    uint32_t dat = 0, n, dot;
    if (i < 0)
    {
        dat |= 1 << 22;
        n = -i;
    }
    else
    {
        n = i;
    }

    if (n > 655350000)
    {
        n /= 100000;
        dot = 8; // 10e-1
    }
    else if (n > 65535000)
    {
        n /= 10000;
        dot = 7; // 10e-2
    }
    else if (n > 6553500)
    {
        n /= 1000;
        dot = 6; // 10e-3
    }
    else if (n > 655350)
    {
        n /= 100;
        dot = 5; // 10e-4
    }
    else if (n > 65535)
    {
        n /= 10;
        dot = 4; // 10e-5
    }
    else
    {
        dot = 3; // 10e-6
    }

    dat |= dot << 18;
    dat |= (n & 0xC000) << 2;
    dat |= (n & 0x3F80) << 1;
    dat |= (n & 0x007F) << 0;
    return dat;
}
void wd2buf(int32_t i,uint8 *buf)
{
    union u32To4_u8 dat;
    dat.Date=wd2dat24(i);
    buf[0] = dat.buf[3];
    buf[1] = dat.buf[2];
    buf[2] = dat.buf[1];
    buf[3] = dat.buf[0];

}
void wd2buf1(int32_t i,uint8 *buf)
{
    union u32To4_u8 dat;
    dat.Date=volt2dat24(i);
    buf[0] = dat.buf[3];
    buf[1] = dat.buf[2];
    buf[2] = dat.buf[1];
    buf[3] = dat.buf[0];
}

int Arry[10]=
{
    1,
    12,
    123,
    1234,
    12345,
    234567,
    3456789,
    45678901,
    567890123,
    1020304050,
};


void NumTest(void)
{
    static u8 buf[4];
    static struct ChanalData ChanDatat;
    int i;
//  static int j = 0;
    for(i=0; i<10; i++)
    {
        wd2buf1(Arry[i],buf);
        //wd2buf((++j+10000),buf);
        memcpy(ChanDatat.chd[i].byte,&buf[1],sizeof(ChanDatat.chd[i].byte));
        ChanDatat.chd[i].fStr.fval = BYTE3toFloat(&buf[1]);
        SwitchChanalData (&ChanDatat.chd[i].fStr,8);//小数点后8位调试
        ChanDatat.chd[i].vcind =ChanData.chd[i].vcind;
        fStr2DfStr(&ChanDatat.chd[i]);//接收后解析的数据原始数据转换为数据显示
        DDa[i][0].DP.Num = ChanDatat.chd[i].DfStr;//接收到数据更新
        DDa[i][0].DP.Dxy = ChanDatat.chd[i].Dxy;
        DDa[i][0].Uint[DDa[i][0].vcind].Col = ChanDatat.chd[i].UintOne.Col;  // 显示单位刷新是接收到指令后自动切换的
        DDa[i][0].DP.dispold = 0;   //刷新标志 在dispold ==0 且 disp ==2 是数据显示才会刷新  必须要加 不加不刷新

    }

}


//解析函数
uint8 SerialRemoteHandle(uint8 len,uint8 *buf,struct ChanalData *pCmd,struct ChanalMData *pMD)
{
    static char InitFalg  = 1;
    static float Ffvalold[2]= {0};
    float Ffvaltemp = 0;
    uint8 nCmd=0;
    uint16 currCharNum;

    uint8 i,val;
    uint8 Dnum;//收到数据个数
    if(InitFalg  == 1) //初始化
    {
        InitFalg  = 0 ;
        memset(Ffvalold,0,sizeof(Ffvalold));
    }
    val=0;
    for(i=1; i<len-1; i++)
    {
        val+=buf[i];
    }
    if(buf[0] != ChrStart||buf[2]<1||buf[1]>CmdR_TandH||((val&0x7F)!=buf[len-1]))
    {
        return Cmd_ERR;
    }
    nCmd=buf[1];  //指令
    currCharNum=1;
    switch(nCmd)
    {

        case CmdR_RST:         //
            if(buf[++currCharNum]!=0X01 )
            {
                return Cmd_ERR;
            }
            break;
        case CmdR_MOD:         //
            if(buf[++currCharNum]!=11 )
            {
                return Cmd_ERR;
            }

            break;
        case CmdR_TIME:
            if(buf[++currCharNum]!=12 )
            {
                return Cmd_ERR;
            }
            break;
        case CmdR_DATA:         //
            if(buf[++currCharNum]!=37 )
            {
                return Cmd_ERR;
            }
            ++currCharNum;
            for(i=0; i<2; i++)
            {
                memcpy(pCmd->frq[i].byte,&buf[currCharNum],sizeof(pCmd->frq[i].byte));
                pCmd->frq[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
                if(DataSave.Data_type.PLSF[i]==1)   //实时脉冲
                {
                    if(Ffvalold[i]<= pCmd->frq[i].fStr.fval)
                    {
                        Ffvaltemp = pCmd->frq[i].fStr.fval;
                        pCmd->frq[i].fStr.fval -= Ffvalold[i];
                        Ffvalold[i] = Ffvaltemp;
                    }
                }
                SwitchChanalData (&pCmd->frq[i].fStr,3);  //小数点后8位调试
                fStr2DfStr(&pCmd->frq[i]);//接收后解析的数据原始数据转换为数据显示
                currCharNum+=3;
            }
            for(i=0; i<9; i++)
            {
                memcpy(pCmd->chd[i].byte,&buf[currCharNum],sizeof(pCmd->chd[i].byte));
                pCmd->chd[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
		        pCmd->chd[i].fStr.fval = DoSW(pCmd->chd[i].fStr.fval,i);
                pCmd->chd[i].fStr.fval = DoFiler(pCmd->chd[i].fStr.fval, FilterVal[FilerIndex[i]],i);
                SwitchChanalData (&pCmd->chd[i].fStr,4);//小数点后8位调试
                fStr2DfStr(&pCmd->chd[i]);//接收后解析的数据原始数据转换为数据显示
                currCharNum+=3;
            }
			memcpy(cpyStr,chrStr,sizeof(chrStr));
            break;
        case CmdR_FDATA:         //
            ++currCharNum;
            Dnum=buf[++currCharNum];//数据个数
            ++currCharNum;
            for(i=0; i<10; i++) //10个数据
            {
                if(i<Dnum)
                {
                    memcpy(pMD->chmd[i].byte,&buf[currCharNum],sizeof(pMD->chmd[i].byte));
                    pMD->chmd[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
					pMD->chmd[i].fStr.fval = DoSW(pMD->chmd[i].fStr.fval, ChanMData.ch-1);		
                    pMD->chmd[i].fStr.fval = DoFiler(pMD->chmd[i].fStr.fval, FilterVal[FilerIndex[ChanMData.ch-1]],ChanMData.ch-1);
                    SwitchChanalData (&pMD->chmd[i].fStr,4);//小数点后8位调试
                    fStr2DfStr(&pMD->chmd[i]);//接收后解析的数据原始数据转换为数据显示
                    currCharNum+=3;
                }
                else
                {
                    pMD->chmd[i]=pMD->chmd[Dnum-1];
		            pMD->chmd[i].fStr.fval = DoSW(pMD->chmd[i].fStr.fval,ChanMData.ch-1);		
                    pMD->chmd[i].fStr.fval = DoFiler(pMD->chmd[i].fStr.fval, FilterVal[FilerIndex[ChanMData.ch-1]],ChanMData.ch-1);
                    SwitchChanalData (&pMD->chmd[i].fStr,4);//小数点后8位调试
                    fStr2DfStr(&pMD->chmd[i]);//接收后解析的数据原始数据转换为数据显示
                    currCharNum+=3;

                }

            }
            break;
        case CmdR_ALARM:
            if(buf[++currCharNum]!=3)
            {
                return Cmd_ERR;
            }
            break;
	case CmdR_TEMP:         //
            if(buf[++currCharNum]!=4 )
            {
                return Cmd_ERR;
            }
			++currCharNum;
             memcpy(chTemp.byte,&buf[currCharNum],sizeof(chTemp.byte));
             chTemp.fStr.fval = BYTE3toFloat(&buf[currCharNum]);
             SwitchChanalData (&chTemp.fStr,4);//小数点后4位调试
             fStr2DfStr(&chTemp);//接收后解析的数据原始数据转换为数据显示
            break;
	case CmdR_TandH:
			if(buf[++currCharNum]!=19 )
            {
                return Cmd_ERR;
            }
			currCharNum++;
//			currCharNum+=6;
//			for(i=0; i<6; i++)
//            {
//                memcpy(pCmd->hmd[i].byte,&buf[currCharNum],sizeof(pCmd->chd[i].byte));
//                pCmd->hmd[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
//		        pCmd->hmd[i].fStr.fval = DoSW(pCmd->hmd[i].fStr.fval,i);
//                pCmd->hmd[i].fStr.fval = DoFiler(pCmd->hmd[i].fStr.fval, FilterVal[FilerIndex[i]],i);
//                SwitchChanalData (&pCmd->hmd[i].fStr,4);//小数点后8位调试
//                fStr2DfStr(&pCmd->hmd[i]);//接收后解析的数据原始数据转换为数据显示
//                currCharNum+=3;
//            }
			for(i=0; i<6; i++)
            {
                memcpy(pCmd->hmd[i].byte,&buf[currCharNum],sizeof(pCmd->hmd[i].byte));
                pCmd->hmd[i].fStr.fval = BYTE3toFloat(&buf[currCharNum]);
		        pCmd->hmd[i].fStr.fval = DoSW(pCmd->hmd[i].fStr.fval,i);
                pCmd->hmd[i].fStr.fval = DoFiler(pCmd->hmd[i].fStr.fval, FilterVal[FilerIndex[i]],i);
                SwitchChanalData (&pCmd->hmd[i].fStr,4);//小数点后8位调试
                fStr2DfStr(&pCmd->hmd[i]);//接收后解析的数据原始数据转换为数据显示
                currCharNum+=3;
            }
		break;
        default:    //ERR
            return Cmd_ERR;
    }
    return nCmd;
}

uint32 JiSuanXY(struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD)
{
    uint32 endtick=0;
    endtick = HAL_GetTick();
    if(DataSave.Data_type.DisLog[i+2][0].index[1]  != OFF)  //前两行是箭头和ALL
    {
        p->Dxy.X=GIXStartX+(endtick-starttick)/NumMs;
        p->Dxy.Y=GIXStartY-(i%ZoneD)*(PointsPiy/ ZoneD)-((p->fStr.fval-DDa[i][0].Low[DDa[i][0].vcind].fval)*(PointsPix/ ZoneD)/(DDa[i][0].Up[DDa[i][0].vcind].fval-DDa[i][0].Low[DDa[i][0].vcind].fval));
    }
    return endtick;
}

uint32 JiSuanXY2(struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD)
{
    uint32 endtick=0;
    endtick = HAL_GetTick();
    if(DataSave.Data_type.DisLog[i+10+2][0].index[1]  != OFF)  //前两行是箭头和ALL
    {
        p->Dxy.X=GIXStartX+(endtick-starttick)/NumMs;
        p->Dxy.Y=GIXStartY-(i%ZoneD)*(PointsPiy/ ZoneD)-((p->fStr.fval-DDa[i+11][0].Low[DDa[i+11][0].vcind].fval)*(PointsPix/ ZoneD)/(DDa[i+11][0].Up[DDa[i+11][0].vcind].fval-DDa[i+11][0].Low[DDa[i+11][0].vcind].fval));
    }
    return endtick;
}

uint32 JiSuanXYsamp(struct SLData *p,uint8 i,uint32 starttick,uint32 NumMs,uint32 ZoneD,uint8 * pr1,uint16 XLen)
{
    uint32 endtick=0;
    endtick = HAL_GetTick();
    if(DataSave.Data_type.DisLog[i+2][0].index[1]  != OFF)  //前两行是箭头和ALL
    {
        p->Dxy.X=GIXStartX+(endtick-starttick)/NumMs;
		if((p->Dxy.X>GIXStartX+PointsPix)&&XLen>=400)
		{
			p->Dxy.X=GIXStartX+PointsPix;
			endtick=((p->Dxy.X-GIXStartX)*NumMs)+starttick;
			*pr1 = 1;
		}
		else
		{
			p->Dxy.X=GIXStartX+XLen;//强行校正
			endtick=((p->Dxy.X-GIXStartX)*NumMs)+starttick;
		}
        p->Dxy.Y=GIXStartY-(i%ZoneD)*(PointsPiy/ ZoneD)-((p->fStr.fval-DDa[i][0].Low[DDa[i][0].vcind].fval)*(PointsPix/ ZoneD)/(DDa[i][0].Up[DDa[i][0].vcind].fval-DDa[i][0].Low[DDa[i][0].vcind].fval));
    }
    return endtick;
}


//发送复位指令
void SendReSet(void)
{
    uint8 buf[4];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] = Cmd_RST;
    buf[i++] = 0x1;
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] = 0;
    UartCmd =Cmd_NULL;  //发之前标志清
    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}



//发送设置通道模式指令

void SendSetMod(const struct ChanalSet D)
{
    uint8 buf[15];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] =Cmd_MOD;
    buf[i++] = 11;
    memcpy(buf+i,D.Mod,sizeof(D.Mod));
    i+=sizeof(D.Mod);
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] ='\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}


//发送三湿度通道模式指令

void SendSetHMod(const struct ChanalSet D)
{
    uint8 buf[15];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] =Cmd_HMOD;
    buf[i++] = 4;
    memcpy(buf+i,D.Mod,sizeof(D.Mod));
    i+=sizeof(D.Mod);
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] ='\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}

//发送设置通道时间指令
void SendSetSYNCTime(const struct ChanalSet D)
{
    uint8 buf[16];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] =Cmd_TIME;
    buf[i++] = 12;
    buf[i++] = D.SYNC;
    memcpy(buf+i,D.Time,sizeof(D.Time));
    i+=sizeof(D.Time);
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i++] = '\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1; 
    }
}



//发送读数据指令
void SendReadData(const struct ChanalSet D)
{
    uint8 buf[7];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] = Cmd_DATA;
    buf[i++] = 0x1;
    // buf[i++] = D.alarm;
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] = '\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}

//发送读三通道温度湿度指令
void SendReadTandH(const struct ChanalSet D)
{
    uint8 buf[7];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] = Cmd_TandH;
    buf[i++] = 0x1;
    // buf[i++] = D.alarm;
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] = '\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}

// 快读指令
void SendReadFData(uint8 ch,uint8 Dnum)
{
    uint8 buf[7];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] = Cmd_FDATA;
    buf[i++] = 0x3;
    buf[i++] = ch;
    buf[i++] = Dnum;
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] = '\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}




// 报警输出命令
void SendSetALARM(uint8 TIMON,uint8 TIMOFF)
{
    uint8 buf[7];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] = Cmd_ALARM;
    buf[i++] = 0x3;
    buf[i++] = TIMON;
    buf[i++] = TIMOFF;
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] = '\0';
    UartCmd =Cmd_NULL;  //发之前标志清

    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}



// 读取端子温度
void SendReadTemp(void)
{
    uint8 buf[7];
    uint8 i=0;
    uint8 j=0;
    uint8 sum=0;
    memset(buf,0,sizeof(buf));
    buf[i++] = ChrStart;
    buf[i++] = Cmd_TEMP;
    buf[i++] = 0x1;
    for(j=1; j<i; j++)
        sum+=buf[j]  ;
    buf[i++] = sum&0x7F;
    buf[i] = '\0';
    UartCmd =Cmd_NULL;  //发之前标志清
    Usart_SendStringL(RS232_USART,buf,i);
    if(Rxflag==0)//清数据标志 新的指令发出后清串口接收数据
    {
        Rxflag=1;
    }
}



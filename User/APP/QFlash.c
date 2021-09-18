/*
  * @file    QFlash.c
  * @author  JK LJ
  * @version V1.0.0
  * @date    18-6-2020
  * @brief  ����spi�ĵ��籣��
 */

#include "include.h"
#define  FSAVE       0xA6
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;



#define  FLASH_WriteAddress     0x00000
#define  FLASH_WriteAddress3     (60*4096)
#define  FLASH_WriteAddress4     (710*4096)
//#define  FLASH_ReadAddress      FLASH_WriteAddress
//#define  FLASH_SectorToErase    FLASH_WriteAddress

#define  FLASH_PAGE_SIZE    256

//��ȡ��ID�洢λ��
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatusA2 = FAILED;
void Delay(__IO uint32_t nCount) ;
//union Data_union DataSaveOld;
//static unsigned char uDataOld[3416];

uint8 ReadAllPara(void)
{
    uint32 i;
	uint8 res = 0;
    uint32 m = sizeof(DataSave.Data_type);
    // SaveAllPara();
    for(i=0; i<(sizeof(DataSave.uData)/FLASH_PAGE_SIZE) ; i++)
    {
        if(i<16)
            BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress+FLASH_PAGE_SIZE*i, FLASH_PAGE_SIZE);
        else //if(i<32) 
        {
            BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress3+FLASH_PAGE_SIZE*(i-16), FLASH_PAGE_SIZE);
        }
//		else 
//        {
//            BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress4+FLASH_PAGE_SIZE*(i-32), FLASH_PAGE_SIZE);
//        }
    }
    i=(sizeof(DataSave.uData)/FLASH_PAGE_SIZE) ;
    if(i<16)
        BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress+FLASH_PAGE_SIZE*i, sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);
    else //if(i<32) 
    {
        BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress3+FLASH_PAGE_SIZE*(i-16), sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);
    }
//	else
//    {
//        BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress4+FLASH_PAGE_SIZE*(i-32), sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);
//    }
    if(DataSave.Data_type.BlockAddr != FSAVE )
    {
        res = 0xFE;
        ReturnST_Init();
        DataSave.Data_type.BlockAddr = FSAVE;
        SaveAllPara();
        for(i=0; i<(sizeof(DataSave.uData)/FLASH_PAGE_SIZE) ; i++)
        {
            if(i<16)
                BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress+FLASH_PAGE_SIZE*i, FLASH_PAGE_SIZE);
            else //if(i<32) 
            {
                BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress3+FLASH_PAGE_SIZE*(i-16), FLASH_PAGE_SIZE);
            }
//			else
//            {
//                BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress4+FLASH_PAGE_SIZE*(i-32), FLASH_PAGE_SIZE);
//            }
        }
        i=(sizeof(DataSave.uData)/FLASH_PAGE_SIZE) ;
        if(i<16)
            BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress+FLASH_PAGE_SIZE*i, sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);
        else //if(i<32)
        {
            BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress3+FLASH_PAGE_SIZE*(i-16), sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);
        }
//		else
//        {
//            BSP_QSPI_Read(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddress4+FLASH_PAGE_SIZE*(i-32), sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);
//        }
    }
	return res;
}

void SaveAllPara(void)
{
    uint32 i,tmepi;
    uint32 FLASH_WriteAddressTemp;
    BSP_QSPI_Erase_Block(FLASH_WriteAddress);
	for(i=0; i<(sizeof(DataSave.uData)/(FLASH_PAGE_SIZE*16)) ; i++)
		BSP_QSPI_Erase_Block(FLASH_WriteAddress3+4096*i);

	//BSP_QSPI_Erase_Block(FLASH_WriteAddress4);
    for(i=0; i<(sizeof(DataSave.uData)/FLASH_PAGE_SIZE) ; i++)
    {
        if(i<16)
        {
            FLASH_WriteAddressTemp = FLASH_WriteAddress;
            tmepi=i;
        }
        else //if(i<32)
        {
            FLASH_WriteAddressTemp = FLASH_WriteAddress3;
            tmepi=i-16;
        }
//		else
//        {
//            FLASH_WriteAddressTemp = FLASH_WriteAddress4;
//            tmepi=i-32;
//        }
        BSP_QSPI_Write(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddressTemp+FLASH_PAGE_SIZE*tmepi, FLASH_PAGE_SIZE);
    }
    i=(sizeof(DataSave.uData)/FLASH_PAGE_SIZE) ;
    if(i<16)
    {
        FLASH_WriteAddressTemp = FLASH_WriteAddress;
        tmepi=i;
    }
    else //if(i<32)
    {
        FLASH_WriteAddressTemp = FLASH_WriteAddress3;
        tmepi=i-16;
    }
//	else
//    {
//        FLASH_WriteAddressTemp = FLASH_WriteAddress4;
//        tmepi=i-32;
//    }
    BSP_QSPI_Write(&DataSave.uData[FLASH_PAGE_SIZE*i], FLASH_WriteAddressTemp+FLASH_PAGE_SIZE*tmepi, sizeof(DataSave.uData)-FLASH_PAGE_SIZE*i);


}

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
//uint8 Tx_Buffer[4000]={0};
//uint8 Rx_Buffer[4000]={0};
//uint16 BufferSize = 4000;
void QFlash(void)
{
//    uint16 i =0;
    /* 16M����flash W25Q256��ʼ�� */
    QSPI_FLASH_Init();
    /* ��ȡ Flash Device ID */
    DeviceID = QSPI_FLASH_ReadDeviceID();
    Delay( 200 );
    /* ��ȡ SPI Flash ID */
    FlashID = QSPI_FLASH_ReadID();
//  for(i=0; i<6000 ; i++)
//    {
//      Tx_Buffer[i] = 0xAC;
//      Rx_Buffer[i] = 0x0;
//  }
//
    /* ���� SPI Flash ID */
    //if (FlashID == sFLASH_ID)
    {
        printf("\r\n��⵽QSPI FLASH W25Q256 !\r\n");

        /* ������Ҫд��� QSPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
        //   BSP_QSPI_Erase_Block(FLASH_SectorToErase);
        BSP_QSPI_Erase_Block(FLASH_WriteAddress);
        BSP_QSPI_Erase_Block(FLASH_WriteAddress3);
        /* �����ͻ�����������д��flash�� */
//        BSP_QSPI_Write(Tx_Buffer, FLASH_WriteAddress+60*4096, BufferSize);

//        /* ���ո�д������ݶ������ŵ����ջ������� */
//          BSP_QSPI_Read(Rx_Buffer, FLASH_ReadAddress+60*4096, BufferSize);


        /* ���д�������������������Ƿ���� */
        //TransferStatusA2 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

        if( PASSED == TransferStatusA2 )
        {

            printf("\r\n32M����flash(W25Q256)���Գɹ�!\n\r");
        }
        else
        {

            printf("\r\n32M����flash(W25Q256)����ʧ��!\n\r");
        }
    }// if (FlashID == sFLASH_ID)
}

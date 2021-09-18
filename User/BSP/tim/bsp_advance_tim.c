/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �߼����ƶ�ʱ����ʱ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32F767 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "include.h"

TIM_HandleTypeDef TIM_TimeBaseStructureAD;
 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{	
	//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(ADVANCE_TIM_IRQn, 0, 3);
	// �����ж���Դ
	HAL_NVIC_EnableIRQ(ADVANCE_TIM_IRQn);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{

	// ����TIMx_CLK,x[1,8] 
	ADVANCE_TIM_CLK_ENABLE(); 
	
	TIM_TimeBaseStructureAD.Instance = ADVANCE_TIM;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructureAD.Init.Period = 16-1;       

	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=216MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100000Hz
	TIM_TimeBaseStructureAD.Init.Prescaler = 2160-1;	
	// ����ʱ�ӷ�Ƶ
	TIM_TimeBaseStructureAD.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	// ������ʽ
	TIM_TimeBaseStructureAD.Init.CounterMode=TIM_COUNTERMODE_UP;
	// �ظ�������:�ظ�1�Σ����������β�����һ���ж�
	TIM_TimeBaseStructureAD.Init.RepetitionCounter=1;

	// ��ʼ����ʱ��TIMx, x[1,8]
	HAL_TIM_Base_Init(&TIM_TimeBaseStructureAD);

	// ������ʱ�������ж�
	//HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructureAD);
	//�رն�ʱ��
	HAL_TIM_Base_Stop_IT(&TIM_TimeBaseStructureAD);


}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1s����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_ConfigurationAD(void)
{
	TIMx_NVIC_Configuration();	

	TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/

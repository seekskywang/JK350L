/**
  ******************************************************************************
  * @file    bsp_basic_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ������ʱ����ʱ����
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

TIM_HandleTypeDef TIM_TimeBaseStructure;
TIM_HandleTypeDef TIM_TimeBaseStructure7;
 /**
  * @brief  ������ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
	//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(BASIC_TIM_IRQn, 1, 3);
	// �����ж���Դ
	HAL_NVIC_EnableIRQ(BASIC_TIM_IRQn);
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
	// ����TIMx_CLK,x[6,7] 
	BASIC_TIM_CLK_ENABLE(); 

	TIM_TimeBaseStructure.Instance = BASIC_TIM;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
	//TIM_TimeBaseStructure.Init.Period = 5000-1; //      
	TIM_TimeBaseStructure.Init.Period = 35-1;
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=108MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure.Init.Prescaler = 10800-1;	

	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);

	// ������ʱ�������ж�
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  
	TIM_Mode_Config();
}





static void TIMx_NVIC_Configuration7(void)
{
	//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(BASIC_TIM7_IRQn, 1, 2);
	// �����ж���Դ
	HAL_NVIC_EnableIRQ(BASIC_TIM7_IRQn);
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
static void TIM_Mode_Config7(void)
{
	// ����TIMx_CLK,x[6,7] 
	BASIC_TIM7_CLK_ENABLE(); 

	TIM_TimeBaseStructure7.Instance = BASIC_TIM7;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
	//TIM_TimeBaseStructure.Init.Period = 5000-1; //      
	TIM_TimeBaseStructure7.Init.Period = 10-1;
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=108MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure7.Init.Prescaler = 10800-1;	

	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure7);

	// ������ʱ�������ж�
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure7);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration7(void)
{
	TIMx_NVIC_Configuration7();	
  
	TIM_Mode_Config7();
}
/*********************************************END OF FILE**********************/

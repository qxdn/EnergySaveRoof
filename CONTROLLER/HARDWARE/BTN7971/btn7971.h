#ifndef _BTN7971_H_
#define _BTN7971_H_

#include "sys.h"

/*********************
ȫ��ʹ�ú궨�壬�밴��ʵ�������
������һ����ʱ����
Ĭ�����Ƶ��10K
����ʹ��ǰ��ʾ�������
*********************/

#define TIM1ORTIM8 1   //ʹ�ø߼���ʱ��

//���ڵ������ڵĺ궨��
//�ǵð���ʵ�ʽӿ��޸�
#define BTN7971_EN1_IN1_RCC_GPIO (RCC_AHB1Periph_GPIOE)
#define BTN7971_EN1_IN2_RCC_GPIO (RCC_AHB1Periph_GPIOE)
#define BTN7971_EN2_IN1_RCC_GPIO (RCC_AHB1Periph_GPIOE)
#define BTN7971_EN2_IN2_RCC_GPIO (RCC_AHB1Periph_GPIOE)
//#define BTN7971_EN1_IN1 (PEout(9))  //TIM1_CH1
//#define BTN7971_EN1_IN2 (PEout(11)) //TIM1_CH2
//#define BTN7971_EN2_IN1 (PEout(13)) //TIM1_CH3
//#define BTN7971_EN2_IN2 (PEout(14)) //TIM1_CH4

//
#define BTN7971_TIM_ch1 GPIO_Pin_9
#define BTN7971_TIM_ch2 GPIO_Pin_11
#define BTN7971_TIM_ch3 GPIO_Pin_13
#define BTN7971_TIM_ch4 GPIO_Pin_14

//����GPIO
#define BTN7971_EN1_IN1_GPIO (GPIOE) //GPIOE
#define BTN7971_EN1_IN2_GPIO (GPIOE) //GPIOE
#define BTN7971_EN2_IN1_GPIO (GPIOE) //GPIOE
#define BTN7971_EN2_IN2_GPIO (GPIOE) //GPIOE

#define BTN7971_EN1_IN1_GPIOPINSOURCE (GPIO_PinSource9)
#define BTN7971_EN1_IN2_GPIOPINSOURCE (GPIO_PinSource11)
#define BTN7971_EN2_IN1_GPIOPINSOURCE (GPIO_PinSource13)
#define BTN7971_EN2_IN2_GPIOPINSOURCE (GPIO_PinSource14)

//����ʹ�ܿڵĶ���
//Ҳ����ֱ�ӽӸߵ�ƽ
//��ʵ�����Ÿ�
// #define	 BTN7971_EN1_RCC_GPIO   (RCC_AHB1Periph_GPIOA)
// #define  BTN7971_EN2_RCC_GPIO		(RCC_AHB1Periph_GPIOA)
// #define  BTN7971_EN1						(PAout(4))						//ʹ��1
// #define  BTN7971_EN2						(PAout(5))						//ʹ��2

// //ʹ�ܶ˵�GPIO����
// #define BTN7971_EN1_PIN   GPIO_Pin_4
// #define BTN7971_EN2_PIN   GPIO_Pin_5

//TIM_PWM
#define BTN7971_TIM_RCC (RCC_APB2Periph_TIM1)    //��Ҫ��APB2��
//
#define BTN7971_TIM (TIM1)
//TIM AF
#define BTN7971_TIM_AF (GPIO_AF_TIM1)

//168M/(PSC+1)/(ARR+1)   10K// 84M/(PSC+1)/(ARR+1)   10K
//Ԥ��Ƶϵ��
#define BTN7971_PSC (2 - 1)
//��װ��ֵ
#define BTN7971_ARR (8400 - 1)
//
#define MAX_SPEED BTN7971_ARR //����ٶ�

//void BTN7971_EN_Init(int channel);    //��ʼ��EN�ڣ�1��EN1��2��EN2��3��ȫ��   Ĭ��ʹ�ܿڸߵ�ƽ
//void BTN7971_EN1_PWM_Init(void);      //��ʼ��EN1������PWM����    Ĭ�����0
//void BTN7971_EN2_PWM_Init(void);      //��ʼ��EN2����������				Ĭ�����0
void BTN7971_Init(int ch);            //ֻ������3�������İ���
void contrl_speed(int speed, int ch); //speedΪ�ٶ�����,  chͨ��ѡ��1λEN1��2ΪEN2
void Usmart_7971speed(char* speed,uint8_t ch);
#endif

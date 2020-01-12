/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-02-28 14:32:01
 * @LastEditTime: 2019-09-21 14:26:50
 */

/*
 *��������ȡ
 *	Read_Encoder(&encoder_1);
 * 	ʹ��ǰ����encoder.h�е�һȦֵ
 */

#include "encoder.h"

My_Encoder Encoder_1; //������1
My_Encoder Encoder_2; //������2

/**
 * @description: ��ʼ��������
 * 				 ����ʹ�ö�ʱ���ı������ӿ�
 * 				 ʹ��ʱ�ǵø��жϵ����ȼ�
 * @param {type} void
 * @return: void
 */
void Encoder_TIM4_Init(void)
{
    //ѡTIM4�о��鷳Сһ��
    //��TIM4��ת�ɱ������ӿ�
    //1,2,3,4,5,8���б������ӿ�
    //ʱ���ṹ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //GPIO�ṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    //�жϽṹ��
    //	NVIC_InitTypeDef NVIC_InitStructure;
    //��ʱ������ṹ��
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //����TIM4ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    //D12,D13�˿ڸ���
    //D12 A
    //D13 B
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    //��Ҫ��©
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //��װ��ֵ
    //��Ҫ�����ԣ��Ͼ�ֱ��65535����NMDWSM
    TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF; //65535
        //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; //
    //���ϼ���
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //ʱ�ӷָ�
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //��ʼ��ʱ��
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    //TIM4,˫������������ģʽ�ӿ�
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    //���ö�ʱ������ṹ��ȱʡ����
    //�Ӻ����ڲ�����ͨ��1
    //��ʵ��ֻ��ͨ��1��ͨ��2�ſ���
    TIM_ICStructInit(&TIM_ICInitStructure);
    //�˲�ֵ,��ʱ��������ô�
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    //�����־λ
    // TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    // //�����жϸ���
    // TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    //��ռ���ֵ
    TIM4->CNT = 0x7FFF; //0;
#if 1
	//�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	//��Ӧ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//�����ж�
	NVIC_Init(&NVIC_InitStructure);
#endif
    Encoder_1.TIM = TIM4;                      //Encoder1ΪTIM4
    Encoder_1.MAX_PULSE = ENCODER_1_MAX_PULSE; //�������
    //������ʱ��
    TIM_Cmd(TIM4, ENABLE);
}

/**
 * @description:TIM5������ 
 * @param {type} void
 * @return: void
 */
void Encoder_TIM5_Init(void)
{
    //ʱ���ṹ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //GPIO�ṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    //�жϽṹ��
    //	NVIC_InitTypeDef NVIC_InitStructure;
    //��ʱ������ṹ��
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //����TIM5ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    //GPIOA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //A0 A1�˿ڸ���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //PA0
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //PA1
    //��Ҫ��©
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //��װ��ֵ
    //��Ҫ�����ԣ��Ͼ�ֱ��65535����NMDWSM
    TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF; //65535
        //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; //
    //���ϼ���
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //ʱ�ӷָ�
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //��ʼ��ʱ��
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);

    //TIM4,˫������������ģʽ�ӿ�
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    //���ö�ʱ������ṹ��ȱʡ����
    //�Ӻ����ڲ�����ͨ��1
    //��ʵ��ֻ��ͨ��1��ͨ��2�ſ���
    TIM_ICStructInit(&TIM_ICInitStructure);
    //�˲�ֵ,��ʱ��������ô�
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    //�����־λ
    // TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    // //�����жϸ���
    // TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    //��ռ���ֵ
    TIM5->CNT = 0x7FFF; //0;
#if 1
	//�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	//��Ӧ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//�����ж�
	NVIC_Init(&NVIC_InitStructure);
#endif
    Encoder_2.TIM = TIM5;                      //Encoder1ΪTIM4
    Encoder_2.MAX_PULSE = ENCODER_2_MAX_PULSE; //�������
    //������ʱ��
    TIM_Cmd(TIM5, ENABLE);
}

/**
 * @description: ��������ʼ��
 * @param {type} u8 choice ѡ�� 
 * 				1 ������1
 * 				2 ������2
 * 				0 ȫ��
 * @return: 
 */
void Encoder_Init(u8 choice)
{
    switch (choice)
    {
    case 0:
        Encoder_TIM4_Init();
        Encoder_TIM5_Init();
        break;
    case 1:
        Encoder_TIM4_Init();
        break;
    case 2:
        Encoder_TIM5_Init();
        break;
    default:
        break;
    }
}

//ת�Ƶ������������Ϊ��Ҫ����
// void TIM4_IRQHandler(void)
// {
// 	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
// 	{
// 		//�ж����
// 		//һ����˵û�м������͵���arr��ֵ��������,�˴���Ҫ��Ȧ��
// 	}
// 	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
// }
//��������ֵ
//���Ƕ�
//ʹ��ʱ ��ȡ���ھ�����Ҫ����һ��
/**
 * @description: ��encoder
 * @param {type} �洢�������Ľṹ��
 * @return: �Ƕ�
 */
double Read_Encoder(My_Encoder *e)
{
    //FIXME:�¸�û��
    int temp;
    int temp_circle = 0;
    temp = (e->TIM)->CNT - 0x7fff; //�����µ�ֵ
    e->speed = temp;               //�ٶȸ�ֵ
    if (temp > e->MAX_PULSE)       //һȦ
    {
        ++temp_circle;
        temp -= (e->MAX_PULSE);
    }
    else if (temp < -(e->MAX_PULSE))
    {
        --temp_circle;
        temp += (e->MAX_PULSE);
    }
    e->angle = e->angle + 1.0 * temp_circle * 360 + 1.0 * temp / (e->MAX_PULSE) * 360; //����Ƕ�
    e->circle = (int)(e->angle / 360);
    (e->TIM)->CNT = 0x7fff;
    return e->angle;
}

/**
 * @description: ����
 * @param {type} 
 * @return: 
 */
void Encoder_Clear(My_Encoder *e)
{
    e->angle = 0;
    e->speed = 0;
    e->circle = 0;
    (e->TIM)->CNT = 0x7fff;
}
/*********************
 * ���º��������µ�������ֵ 
 * ʵ�ʹ����в���
 ************************/

/**
 * @description:��ʼ������ 
 * @param {type} 
 * @return: 
 */
void EncoderMeasureInit(My_Encoder *e)
{
    e->TIM->CNT = 0;
}

u16 EncoderMeasureCNT(My_Encoder *e)
{
    return (e->TIM->CNT);
}

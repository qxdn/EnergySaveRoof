#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"


//TODO:��Ҫ�µĲ���
#define ENCODER_1_MAX_PULSE (30000) //��һ�������� һȦ��Ҫ
#define ENCODER_2_MAX_PULSE (30000) //�ڶ���������һȦ��Ҫ

typedef struct
{
	int speed;		  //���ж�����ٶ�
	double angle;	 //�Ƕ�
	int circle;		  //Ȧ��
	TIM_TypeDef *TIM; //��ʱ��
	u16 MAX_PULSE;	//�������
} My_Encoder;

extern My_Encoder Encoder_1; //������1
extern My_Encoder Encoder_2; //������2



void Encoder_Init(u8 choice);
double Read_Encoder(My_Encoder *e);
void Encoder_Clear(My_Encoder* e);

void EncoderMeasureInit(My_Encoder *e);
u16 EncoderMeasureCNT(My_Encoder *e);

#endif

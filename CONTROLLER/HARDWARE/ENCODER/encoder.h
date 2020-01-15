#ifndef _ENCODER_H_
#define _ENCODER_H
#include "sys.h"


//TODO:需要新的测量
#define ENCODER_1_MAX_PULSE (30000) //第一个编码器 一圈需要
#define ENCODER_2_MAX_PULSE (30000) //第二个编码器一圈需要

typedef struct
{
	int speed;		  //放中断里读速度
	double angle;	 //角度
	int circle;		  //圈数
	TIM_TypeDef *TIM; //定时器
	u16 MAX_PULSE;	//最大脉冲
} My_Encoder;

extern My_Encoder Encoder_1; //编码器1
extern My_Encoder Encoder_2; //编码器2



void Encoder_Init(u8 choice);
double Read_Encoder(My_Encoder *e);
void Encoder_Clear(My_Encoder* e);

void EncoderMeasureInit(My_Encoder *e);
u16 EncoderMeasureCNT(My_Encoder *e);

#endif

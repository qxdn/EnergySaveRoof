/*
 * @Author: QianXu
 * @LastEditors: QianXu
 * @Description: NONE
 * @Date: 2019-02-28 14:32:01
 * @LastEditTime: 2019-09-21 14:26:50
 */

/*
 *编码器读取
 *	Read_Encoder(&encoder_1);
 * 	使用前更改encoder.h中的一圈值
 */

#include "encoder.h"

My_Encoder Encoder_1; //编码器1
My_Encoder Encoder_2; //编码器2

/**
 * @description: 初始化编码器
 * 				 尝试使用定时器的编码器接口
 * 				 使用时记得改中断的优先级
 * @param {type} void
 * @return: void
 */
void Encoder_TIM4_Init(void)
{
    //选TIM4感觉麻烦小一点
    //将TIM4包转成编码器接口
    //1,2,3,4,5,8才有编码器接口
    //时基结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //GPIO结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    //中断结构体
    //	NVIC_InitTypeDef NVIC_InitStructure;
    //定时器输入结构体
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //开启TIM4时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    //D12,D13端口复用
    //D12 A
    //D13 B
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    //主要开漏
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //重装载值
    //不要都可以，毕竟直接65535走起，NMDWSM
    TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF; //65535
        //预分频系数
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; //
    //向上计数
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //时钟分割
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //初始化时基
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    //TIM4,双计数，编码器模式接口
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    //配置定时器输入结构体缺省部分
    //从函数内部看是通道1
    //事实上只有通道1和通道2才可以
    TIM_ICStructInit(&TIM_ICInitStructure);
    //滤波值,暂时还不清楚用处
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    //清除标志位
    // TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    // //允许中断更新
    // TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    //清空计数值
    TIM4->CNT = 0x7FFF; //0;
#if 1
	//配置中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	//相应优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置中断
	NVIC_Init(&NVIC_InitStructure);
#endif
    Encoder_1.TIM = TIM4;                      //Encoder1为TIM4
    Encoder_1.MAX_PULSE = ENCODER_1_MAX_PULSE; //最大脉冲
    //开启定时器
    TIM_Cmd(TIM4, ENABLE);
}

/**
 * @description:TIM5编码器 
 * @param {type} void
 * @return: void
 */
void Encoder_TIM5_Init(void)
{
    //时基结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //GPIO结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    //中断结构体
    //	NVIC_InitTypeDef NVIC_InitStructure;
    //定时器输入结构体
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //开启TIM5时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    //GPIOA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //A0 A1端口复用
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //PA0
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //PA1
    //主要开漏
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //重装载值
    //不要都可以，毕竟直接65535走起，NMDWSM
    TIM_TimeBaseInitStructure.TIM_Period = 0XFFFF; //65535
        //预分频系数
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; //
    //向上计数
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //时钟分割
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //初始化时基
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);

    //TIM4,双计数，编码器模式接口
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    //配置定时器输入结构体缺省部分
    //从函数内部看是通道1
    //事实上只有通道1和通道2才可以
    TIM_ICStructInit(&TIM_ICInitStructure);
    //滤波值,暂时还不清楚用处
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    //清除标志位
    // TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    // //允许中断更新
    // TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    //清空计数值
    TIM5->CNT = 0x7FFF; //0;
#if 1
	//配置中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	//相应优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置中断
	NVIC_Init(&NVIC_InitStructure);
#endif
    Encoder_2.TIM = TIM5;                      //Encoder1为TIM4
    Encoder_2.MAX_PULSE = ENCODER_2_MAX_PULSE; //最大脉冲
    //开启定时器
    TIM_Cmd(TIM5, ENABLE);
}

/**
 * @description: 编码器初始化
 * @param {type} u8 choice 选择 
 * 				1 编码器1
 * 				2 编码器2
 * 				0 全部
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

//转移到步进电机，因为需要两个
// void TIM4_IRQHandler(void)
// {
// 	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
// 	{
// 		//中断溢出
// 		//一般来说没有计数满就到了arr的值会产生溢出,此处需要计圈数
// 	}
// 	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
// }
//读编码器值
//读角度
//使用时 读取周期尽量不要超过一周
/**
 * @description: 读encoder
 * @param {type} 存储编码器的结构体
 * @return: 角度
 */
double Read_Encoder(My_Encoder *e)
{
    //FIXME:新改没测
    int temp;
    int temp_circle = 0;
    temp = (e->TIM)->CNT - 0x7fff; //计算新的值
    e->speed = temp;               //速度赋值
    if (temp > e->MAX_PULSE)       //一圈
    {
        ++temp_circle;
        temp -= (e->MAX_PULSE);
    }
    else if (temp < -(e->MAX_PULSE))
    {
        --temp_circle;
        temp += (e->MAX_PULSE);
    }
    e->angle = e->angle + 1.0 * temp_circle * 360 + 1.0 * temp / (e->MAX_PULSE) * 360; //计算角度
    e->circle = (int)(e->angle / 360);
    (e->TIM)->CNT = 0x7fff;
    return e->angle;
}

/**
 * @description: 归零
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
 * 以下函数用于新电机测编码值 
 * 实际工程中不用
 ************************/

/**
 * @description:初始化测量 
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

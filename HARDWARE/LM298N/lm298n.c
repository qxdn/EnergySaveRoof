#include "lm298n.h"
#include "stdlib.h"

void LM298N_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //TIM3

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); //PB0
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); //PB1

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上
    TIM_TimeBaseInitStructure.TIM_Period = LM298N_MAX_SPEED;        //arr 1.25us
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //psc
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //初始化定时器3

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //PWM模式1 低于CCR为有效极性
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较使能输出
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //不用影子寄存器输出互补
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效极性为高
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_Cmd(TIM3, ENABLE);
}

void LM298N_Dir_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //GPIOB

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LM298N_Init(void)
{
    LM298N_Dir_Init();
    LM298N_PWM_Init();
}

void LM298N_Speed_Control(int speed, uint8_t ch)
{
    //限幅
    if (speed > LM298N_MAX_SPEED)
    {
        speed = LM298N_MAX_SPEED;
    }
    else if (speed < -LM298N_MAX_SPEED)
    {
        speed = -LM298N_MAX_SPEED;
    }

    //选择通道
    switch (ch)
    {
    case 1:
        if (speed >= 0)
        {
            ChannelAIN1 = 1;
            ChannelAIN2 = 0;
        }
        else
        {
            ChannelAIN1 = 0;
            ChannelAIN2 = 1;
            speed=-speed;
        }
        TIM_SetCompare3(TIM3, speed);
        break;
    case 2:
        if (speed >= 0)
        {
            ChannelBIN1 = 1;
            ChannelBIN2 = 0;
        }
        else
        {
            ChannelBIN1 = 0;
            ChannelBIN2 = 1;
             speed=-speed;
        }
        TIM_SetCompare4(TIM3, speed);
        break;
    default:
        break;
    }
}

void Usmart_LM298N_Speed(char *speed, uint8_t ch)
{
    LM298N_Speed_Control(atoi(speed), ch);
}

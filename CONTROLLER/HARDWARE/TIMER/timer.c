#include "timer.h"
#include "encoder.h"
#include "led.h"
#include "pid.h"
#include "btn7971.h"
#include "oled.h"

DUTY System_Duty;

extern PID Motor1PID;
extern PID Motor2PID;

/**
 * @description: TIM7初始化 1ms
 * @param {type} arr=1000-1  psc=84-1
 * @return: 
 */
void TIM7_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStrucrure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseStrucrure.TIM_Period = arr;                     //自动重载
    TIM_TimeBaseStrucrure.TIM_Prescaler = psc;                  //预分频
    TIM_TimeBaseStrucrure.TIM_ClockDivision = TIM_CKD_DIV1;     //
    TIM_TimeBaseStrucrure.TIM_CounterMode = TIM_CounterMode_Up; //

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStrucrure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; //定时器7
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE); //使能TIM7外设
}

/**
 * @description: TIM7中断服务函数  1ms
 * @param {type} 
 * @return: 
 */
void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET) //溢出中断
    {

        if (System_Duty.ms == 1000)
        {
            System_Duty.ms = 0;
        }
        System_Duty.ms++;

        if (System_Duty.ms % 10 == 0)
            System_Duty.dutyflag_10ms = true;
        if (System_Duty.ms % 20 == 0)
            System_Duty.dutyflag_20ms = true;
        if (System_Duty.ms % 30 == 0)
            System_Duty.dutyflag_30ms = true;
        if (System_Duty.ms % 40 == 0)
            System_Duty.dutyflag_40ms = true;
        if (System_Duty.ms % 50 == 0)
            System_Duty.dutyflag_50ms = true;
        if (System_Duty.ms % 100 == 0)
            System_Duty.dutyflag_100ms = true;
        if (System_Duty.ms % 200 == 0)
            System_Duty.dutyflag_200ms = true;
        if (System_Duty.ms % 999 == 0)
            System_Duty.dutyflag_999ms = true;
    }
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //清除中断标志位
}

void TIM9_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStrucrure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

    TIM_TimeBaseStrucrure.TIM_Period = arr;                     //自动重载
    TIM_TimeBaseStrucrure.TIM_Prescaler = psc;                  //预分频
    TIM_TimeBaseStrucrure.TIM_ClockDivision = TIM_CKD_DIV1;     //
    TIM_TimeBaseStrucrure.TIM_CounterMode = TIM_CounterMode_Up; //

    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStrucrure);

    TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM9, ENABLE); //使能TIM9外设
}

void TIM10_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStrucrure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);

    TIM_TimeBaseStrucrure.TIM_Period = arr;                     //自动重载
    TIM_TimeBaseStrucrure.TIM_Prescaler = psc;                  //预分频
    TIM_TimeBaseStrucrure.TIM_ClockDivision = TIM_CKD_DIV1;     //
    TIM_TimeBaseStrucrure.TIM_CounterMode = TIM_CounterMode_Up; //

    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStrucrure);

    TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM10, ENABLE); //使能TIM9外设
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM9, TIM_IT_Update) == SET)
    {
    }
    TIM_ClearITPendingBit(TIM9, TIM_IT_Update); //清除中断标志位
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM10, TIM_IT_Update) == SET)
    {
    }
    TIM_ClearITPendingBit(TIM10, TIM_IT_Update); //清除中断标志位
}

//问题 当duty_loop() 放在while 循环中时 可能由于while循环运行时间较长 导致duty_50ms等的时间不是预期时间
void duty_10ms(void)
{
}

void duty_20ms(void)
{
}
void duty_30ms(void)
{
}

void duty_40ms(void)
{
    Read_Encoder(&Encoder_1);
    Read_Encoder(&Encoder_2);
    LOC_PIDCaculate(&Motor1PID, Encoder_1.angle);
    LOC_PIDCaculate(&Motor2PID, Encoder_2.angle);
    contrl_speed(Motor1PID.Pwmduty, 1);
    contrl_speed(Motor2PID.Pwmduty, 2);
}

void duty_50ms(void)
{
}

void duty_100ms(void)
{
}
void duty_200ms(void)
{
    OLED_ShowString(0, 0, "angle1:", 12);
    OLED_ShowString(0, 12, "angle2:", 12);
    OLED_ShowFloat(55, 0, Encoder_1.angle);
    OLED_ShowFloat(55, 12, Encoder_2.angle);
    OLED_Refresh_Gram();
}

void duty_999ms(void)
{
    LED = !LED;
}

/**
 * @description:循环 
 * @param {type} 
 * @return: 
 */
void duty_loop(void)
{
    if (System_Duty.dutyflag_10ms == true)
    {
        duty_10ms();
        System_Duty.dutyflag_10ms = false;
    }
    if (System_Duty.dutyflag_20ms == true)
    {
        duty_20ms();
        System_Duty.dutyflag_20ms = false;
    }
    if (System_Duty.dutyflag_30ms == true)
    {
        duty_30ms();
        System_Duty.dutyflag_30ms = false;
    }
    if (System_Duty.dutyflag_40ms == true)
    {
        duty_40ms();
        System_Duty.dutyflag_40ms = false;
    }
    if (System_Duty.dutyflag_50ms == true)
    {
        duty_50ms();
        System_Duty.dutyflag_50ms = false;
    }
    if (System_Duty.dutyflag_100ms == true)
    {
        duty_100ms();
        System_Duty.dutyflag_100ms = false;
    }
    if (System_Duty.dutyflag_200ms == true)
    {
        duty_200ms();
        System_Duty.dutyflag_200ms = false;
    }
    if (System_Duty.dutyflag_999ms == true)
    {
        duty_999ms();
        System_Duty.dutyflag_999ms = false;
    }
}

void ChangleMotorAngle(double angle, uint8_t ch)
{
    switch (ch)
    {
    case 1:
        Motor1PID.SetPoint = angle;
        break;
    case 2:
        Motor2PID.SetPoint = angle;
        break;
    default:
        break;
    }
}

/***********USMART***********/
void UsmartChangeMotorAngle(char *angle, uint8_t ch)
{
    ChangleMotorAngle(atof(angle), ch);
}

void UsmartAutoIncreaseMotor(char* angle)
{
    Motor1PID.SetPoint += atof(angle);
    Motor2PID.SetPoint += atof(angle);
    if (Motor1PID.SetPoint > 180)
    {
        Motor1PID.SetPoint = 180;
    }
    else if (Motor1PID.SetPoint < 0)
    {
        Motor1PID.SetPoint = 0;
    }
     if (Motor2PID.SetPoint > 180)
    {
        Motor2PID.SetPoint = 180;
    }
    else if (Motor2PID.SetPoint < 0)
    {
        Motor2PID.SetPoint = 0;
    }
}

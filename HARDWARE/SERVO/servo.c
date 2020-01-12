#include "servo.h"
#include "sys.h"
/**
 * @description: 初始化舵机
 * @param {type} void 
 * @return: 
 */
void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;               //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168 - 1;              //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出2使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** 比较输出2N失能 *******/
    TIM_OCInitStructure.TIM_Pulse = 0;                               //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //输出极性:TIM输出比较极性高

    TIM_OC1Init(TIM1, &TIM_OCInitStructure); //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH2预装载使能
    TIM_ARRPreloadConfig(TIM1, ENABLE);               //使能TIMx在ARR上的预装载寄存器



    //TODO:
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    TIM_CtrlPWMOutputs(TIM1, ENABLE); //高级定时器才有
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @description: 控制舵机转动角度
 *                 ！！！注意 现在还不知道是否线性关系和线性的值所以此时angle直接表示PWM的值
 * @param {type}  
 *                  angle 转动多少
 *                  ch 通道
 * @return: void
 */
void servo_control_angle(int angle, int ch)
{

    if (angle < SERVO_MIN)     //0.5ms
        angle = SERVO_MIN - 1; //0度
    if (angle > SERVO_MAX)     //2.5ms
        angle = SERVO_MAX - 1; //180度
    switch (ch)
    {
    case 1:
        TIM_SetCompare1(TIM1, angle);
        break;
    case 2:
        TIM_SetCompare2(TIM1, angle);
        break;
    }
}

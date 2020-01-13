#include "btn7971.h"
#include "usart.h"

#if 0 //不用了
//channel 选择通道
//1为EN1 2位EN2 3为全开
//可以硬件上接高电平不执行这个函数
/**
 * @description: 初始化使能端口
 * @param {type} 
 * @return: 
 */
void BTN7971_EN_Init(int channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启GPIOA
	RCC_AHB1PeriphClockCmd(BTN7971_EN1_RCC_GPIO | BTN7971_EN2_RCC_GPIO, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉

	if (1 == channel)
		GPIO_InitStructure.GPIO_Pin = BTN7971_EN1_PIN;
	else if (2 == channel)
		GPIO_InitStructure.GPIO_Pin = BTN7971_EN2_PIN;
	else if (3 == channel)
		GPIO_InitStructure.GPIO_Pin = BTN7971_EN1_PIN | BTN7971_EN2_PIN;

	GPIO_Init(GPIOF, &GPIO_InitStructure); //初始化

	//默认开启
	if (1 == channel)
		BTN7971_EN1 = 1;
	else if (2 == channel)
		BTN7971_EN2 = 1;
	else if (3 == channel)
	{
		BTN7971_EN1 = 1;
		BTN7971_EN2 = 1;
	}
}
#endif

/**
 * @description: 开启第一组电机的PWM输出
 * @param {type} void
 * @return: void
 */
void BTN7971_EN1_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(BTN7971_EN1_IN1_RCC_GPIO | BTN7971_EN1_IN2_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(BTN7971_TIM_RCC, ENABLE); //定时器时钟使能  因为换TIM1改APB2

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
	GPIO_InitStructure.GPIO_Pin = BTN7971_TIM_ch1;

	//复用
	GPIO_PinAFConfig(BTN7971_EN1_IN1_GPIO, BTN7971_EN1_IN1_GPIOPINSOURCE, BTN7971_TIM_AF);
	GPIO_PinAFConfig(BTN7971_EN1_IN2_GPIO, BTN7971_EN1_IN2_GPIOPINSOURCE, BTN7971_TIM_AF);

	GPIO_Init(BTN7971_EN1_IN1_GPIO, &GPIO_InitStructure); //初始化

	GPIO_InitStructure.GPIO_Pin = BTN7971_TIM_ch2;

	GPIO_Init(BTN7971_EN1_IN2_GPIO, &GPIO_InitStructure); //初始化

	//时基初始化
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = BTN7971_ARR;				//重装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = BTN7971_PSC;			//预分频系数

	TIM_TimeBaseInit(BTN7971_TIM, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				 //PWM模式1 低于CCR为有效极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较使能输出
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //不用影子寄存器输出互补
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效极性为高
	TIM_OCInitStructure.TIM_Pulse = 0;						  //初始CRR为0  即开始不转

	TIM_OC1Init(BTN7971_TIM, &TIM_OCInitStructure); //CH1
	TIM_OC2Init(BTN7971_TIM, &TIM_OCInitStructure); //CH2

	TIM_OC1PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值   更新到来时才能改CCR，否则随时改
	TIM_OC2PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值
	TIM_ARRPreloadConfig(BTN7971_TIM, ENABLE);				 //允许工作时改ARR  更新到来才能改

	//TIM_Cmd(BTN7971_TIM,ENABLE);

#if 0
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

	TIM_TimeBaseInitStructure.TIM_Period = BTN7971_ARR;				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = BTN7971_PSC;			//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				 //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出2使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** 比较输出2N失能 *******/
	TIM_OCInitStructure.TIM_Pulse =MAX_SPEED>>1;	 //TODO:					 //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		 //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);						 //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);		

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH2预装载使能
	TIM_ARRPreloadConfig(TIM1, ENABLE);				  //使能TIMx在ARR上的预装载寄存器

		//TODO:
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
	TIM_Cmd(TIM1,ENABLE);
#endif
}

/**
 * @description: 开启第二组电机的PWM输出
 * @param {type} 
 * @return: 
 */
void BTN7971_EN2_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(BTN7971_EN2_IN1_RCC_GPIO | BTN7971_EN2_IN2_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(BTN7971_TIM_RCC, ENABLE); //定时器时钟使能  因为换TIM1改APB2

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
	GPIO_InitStructure.GPIO_Pin = BTN7971_TIM_ch3;

	GPIO_Init(BTN7971_EN2_IN1_GPIO, &GPIO_InitStructure); //初始化

	GPIO_InitStructure.GPIO_Pin = BTN7971_TIM_ch4;

	GPIO_Init(BTN7971_EN2_IN2_GPIO, &GPIO_InitStructure); //初始化
	//复用
	GPIO_PinAFConfig(BTN7971_EN2_IN1_GPIO, BTN7971_EN2_IN1_GPIOPINSOURCE, BTN7971_TIM_AF);
	GPIO_PinAFConfig(BTN7971_EN2_IN2_GPIO, BTN7971_EN2_IN2_GPIOPINSOURCE, BTN7971_TIM_AF);

	//时基初始化
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = BTN7971_ARR;				//重装载初值
	TIM_TimeBaseInitStructure.TIM_Prescaler = BTN7971_PSC;			//预分频系数

	TIM_TimeBaseInit(BTN7971_TIM, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //PWM模式1 低于CCR为有效极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	 //有效极性为高
	TIM_OCInitStructure.TIM_Pulse = 0;							  //初始CRR为0  即开始不转

	TIM_OC3Init(BTN7971_TIM, &TIM_OCInitStructure); //CH3
	TIM_OC4Init(BTN7971_TIM, &TIM_OCInitStructure); //CH4

	TIM_OC3PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值   更新到来时才能改CCR，否则随时改
	TIM_OC4PreloadConfig(BTN7971_TIM, TIM_OCPreload_Enable); //使能预装载初值
	TIM_ARRPreloadConfig(BTN7971_TIM, ENABLE);				 //允许工作时改ARR  更新到来才能改
}

/**
 * @description: 初始化电机驱动
 * @param {type} ch开启通道 
 * 				1 第一组电机
 * 				2 第二组电机
 * 				3 全开			
 * @return: 
 */
void BTN7971_Init(int ch)
{
	BTN7971_EN1_PWM_Init();
	BTN7971_EN2_PWM_Init(); //使能2
#if TIM1ORTIM8
	TIM_CtrlPWMOutputs(BTN7971_TIM, ENABLE); //只有高级定时器才有
#endif
	TIM_Cmd(BTN7971_TIM, ENABLE); //开启时钟，输出PWM
}
//速度控制
//speed=CCR
//ch选择通道   1为EN1 ，2为EN2
void contrl_speed(int speed, int ch)
{
	if (speed > MAX_SPEED)
		speed = MAX_SPEED; //限幅
	else if (speed < (-MAX_SPEED))
		speed = (-MAX_SPEED);
	if (speed > 0) //正转
	{
		if (1 == ch)
		{
			TIM_SetCompare1(BTN7971_TIM, speed); //EN1  IN1 PWM
			TIM_SetCompare2(BTN7971_TIM, 0);	 //EN2   IN2 0
		}
		else if (2 == ch)
		{
			TIM_SetCompare3(BTN7971_TIM, speed); //EN2   IN1  PWM
			TIM_SetCompare4(BTN7971_TIM, 0);	 //EN2    IN2 0
		}
	}
	else //翻转
	{
		speed = -speed; //先换成正
		if (1 == ch)
		{
			TIM_SetCompare1(BTN7971_TIM, 0);	 //EN1  IN1 0
			TIM_SetCompare2(BTN7971_TIM, speed); //EN2   IN2 PWM
		}
		else if (2 == ch)
		{
			TIM_SetCompare3(BTN7971_TIM, 0);	 //EN2   IN1  0
			TIM_SetCompare4(BTN7971_TIM, speed); //EN2    IN2 0
		}
	}
}

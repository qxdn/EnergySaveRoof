#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "usmart.h"
#include "servo.h"
#include "encoder.h"
#include "btn7971.h"
#include "timer.h"
#include "led.h"
#include "pid.h"
#include "oled.h"

PID Motor1PID;
PID Motor2PID;

void Pid_Init(void){
	Motor1PID.Proportion=40;
	Motor1PID.Integral=1;
	Motor1PID.Derivative=25;

	Motor2PID.Proportion=-40;
	Motor2PID.Integral=-1;
	Motor2PID.Derivative=-25;
}


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	usmart_dev.init(84);
	LED_Init();
	OLED_Init();
	Encoder_Init(0);
	BTN7971_Init(3);
	Pid_Init();
	TIM7_Init(1000 - 1, 84 - 1);
	while (1)
	{
		duty_loop();
	}
}

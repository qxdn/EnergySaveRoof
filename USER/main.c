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

int main(void)
{
	uart_init(115200);
	delay_init(168);
	usmart_dev.init(84);
	LED_Init();
	Encoder_Init(0);
	BTN7971_Init(3);
	TIM7_Init(1000-1,84-1);
	while (1)
	{
		
	}
}

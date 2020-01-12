#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "usmart.h"
#include "servo.h"

int main(void)
{
	uart_init(115200);
	delay_init(168);
	usmart_dev.init(84);
	Servo_Init();
	while (1)
	{
	}
}

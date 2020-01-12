#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "usmart.h"
#include "servo.h"
#include "lm298n.h"
#include "encoder.h"

int main(void)
{
	uart_init(115200);
	delay_init(168);
	usmart_dev.init(84);
	LM298N_Init();
	Encoder_Init(0);
	while (1)
	{
		Read_Encoder(&Encoder_1);
		printf("angle:%lf\r\n",Encoder_1.angle);
		delay_ms(1000);
	}
}

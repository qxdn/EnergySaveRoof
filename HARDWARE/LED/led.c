#include "led.h"
#include "delay.h"

//初始化PD7为输出口.并使能这两个口的时钟
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOD时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PD7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO
    LED = 0; //初始为灭
    BEE = 1;
    delay_ms(100);
    BEE = 0;
    delay_ms(100);
    BEE = 1;
    delay_ms(100);
    BEE = 0;
}

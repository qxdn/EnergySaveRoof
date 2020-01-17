#include "led.h"
#include "delay.h"

//��ʼ��PD7Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC, ENABLE); //ʹ��GPIODʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PD7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ��GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��GPIO
    LED = 0; //��ʼΪ��
    BEE = 1;
    delay_ms(100);
    BEE = 0;
    delay_ms(100);
    BEE = 1;
    delay_ms(100);
    BEE = 0;
}
#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED�˿ڶ���
#define LED PDout(7) // PD7
#define BEE PCout(15)

void LED_Init(void); //��ʼ��
#endif

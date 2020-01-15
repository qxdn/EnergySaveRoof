#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口定义
#define LED PDout(7) // PD7
#define BEE PCout(15)

void LED_Init(void); //初始化
#endif

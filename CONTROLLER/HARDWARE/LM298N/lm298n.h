#ifndef _LM298N_H_
#define _LM298N_H_
#include "sys.h"


#define LM298N_MAX_SPEED  (8400-1)

#define ChannelAIN1   PBout(3)
#define ChannelAIN2   PBout(5)
#define ChannelBIN1   PBout(6)
#define ChannelBIN2   PBout(7)

void LM298N_Init(void);
void LM298N_Speed_Control(int speed, uint8_t ch);

void Usmart_LM298N_Speed(char* speed,uint8_t ch);

#endif

#ifndef _TIMER_H_
#define _TIMER_H_
//#include "stdbool.h"
#include "sys.h"


			
typedef struct
{
    u16 ms;
    bool dutyflag_10ms;
    bool dutyflag_20ms;
    bool dutyflag_30ms;
    bool dutyflag_40ms;
    bool dutyflag_50ms;
    bool dutyflag_100ms;
    bool dutyflag_200ms;
    bool dutyflag_999ms;
} DUTY;

extern DUTY SystemDuty;

void TIM7_Init(u16 arr, u16 psc);
void TIM9_Init(u16 arr, u16 psc);
void TIM10_Init(u16 arr, u16 psc);
void duty_loop(void);

void ChangleMotorAngle(double angle,uint8_t ch);


/*************USMART*************/
void UsmartChangeMotorAngle(char* angle,uint8_t ch);
void UsmartAutoIncreaseMotor1(char* angle);

#endif

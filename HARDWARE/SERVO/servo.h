#ifndef _SERVO_H_
#define _SERVO_H_

#include "sys.h"

#define SERVO_MAX (2500)
#define SERVO_MIN (500)

void Servo_Init(void);
void servo_control_angle(int angle, int ch);

#endif

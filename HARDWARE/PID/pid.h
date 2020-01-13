#ifndef _PID_H_
#define _PID_H_

#include "sys.h"

typedef struct PID
{
    double SetPoint;   // 设定目标 Desired Value
    double Proportion; // 比例常数 Proportional Const
    double Integral;   // 积分常数 Integral Const
    double Derivative; // 微分常数 Derivative Const
    double LastError;  // 上次误差
    double PrevError;  // 当前误差
    double SumError;   // 总误差,用来做积分


    double Pwmduty;
} PID;

void LOC_PIDCaculate(PID* pid, double measure);

#endif

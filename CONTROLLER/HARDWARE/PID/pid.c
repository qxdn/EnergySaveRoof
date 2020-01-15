#include "pid.h"
#include "math.h"

/**
 * @description: 位置式PID计算
 * @param {type} 
 * @return: 
 */
void LOC_PIDCaculate(PID *pid, double measure)
{

    pid->PrevError = pid->SetPoint - measure;
    // //误差在一个小范围内
    // if (fabs(pid->PrevError) < pid->AllowError) //允许误差内
    // {
    //     pid->PrevError = 0;
    // }

    // //误差小时允许积分
    // if (fabs(pid->PrevError) < (pid->AllowInergal)) {
    //     pid->SumError += pid->PrevError;
    // }

    // //积分限幅
    // if ((pid->SumError) > (pid->MaxSumError)) {
    //     pid->SumError = pid->MaxSumError;
    // }
    // if ((pid->SumError) < (-pid->MaxSumError)) {
    //     pid->SumError = -pid->MaxSumError;
    // }

    pid->SumError += pid->PrevError;
    //计算输出
    pid->Pwmduty = (pid->Proportion * pid->PrevError) + (pid->SumError * pid->Integral) + (pid->Derivative * (pid->PrevError - pid->LastError));
    pid->LastError = pid->PrevError;

}

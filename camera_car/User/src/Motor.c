#include "include.h"
#include "Motor.h"

#define    PWM_H           (980)
#define    PWM_L           (-980)          ////根据测速方式选择，如果是正交解码则可以到负值，现仅为正值
float Ratio_Encoder_Left = 1/(58*0.004);            //此计数为电磁三轮，待修改
float Ratio_Encoder_Righ = 1/(58*0.004);         //车行进1米编码器计到了5800个脉冲,得出的速度单位为cm



PID_Datatypedef MotorPID_Data;
Speedtypedef    Speed_Data;
bool IsMotorOn = false;

void MotorPWM_output(int M) //电机输出函数(占空比)
{
    if(M >= 0)
    {     
        ftm_pwm_duty(ftm0, ftm_ch5, M);
        ftm_pwm_duty(ftm0, ftm_ch4, 0);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch5, 0);
        ftm_pwm_duty(ftm0, ftm_ch4, abs(M));  
    }
}

int PWM_Limit(int outPWM)
{
    if(outPWM >= PWM_H)
        return PWM_H;
    else if(outPWM <= PWM_L)
        return PWM_L;
    else
        return outPWM;
}


void GetSpeed(void)
{
    Speed_Data.Mpulse = -FTM1_BASE_PTR->CNT;
    FTM1_BASE_PTR->CNT = 0;
}

//////电机PID
//增量式PI控制
int MotorPID_output(int ExpectSpeed, int NowSpeed)
{
    if(IsMotorOn == false) 
        return 0;
    static float LastSpeedErr_L = 0;
    static int PID_output_L = 0;
    float SpeedErr = ExpectSpeed - NowSpeed;
    float PID_POUT = MotorPID_Data.P * (SpeedErr - LastSpeedErr_L);
    PID_output_L += (int)(PID_POUT + MotorPID_Data.I * SpeedErr);
    LastSpeedErr_L = SpeedErr;
    return PWM_Limit(PID_output_L);
}
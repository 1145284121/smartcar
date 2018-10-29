#include "include.h"
#include "Motor.h"

#define    PWM_H           (980)
#define    PWM_L           (-980)          ////���ݲ��ٷ�ʽѡ�������������������Ե���ֵ���ֽ�Ϊ��ֵ
float Ratio_Encoder_Left = 1/(58*0.004);            //�˼���Ϊ������֣����޸�
float Ratio_Encoder_Righ = 1/(58*0.004);         //���н�1�ױ������Ƶ���5800������,�ó����ٶȵ�λΪcm



PID_Datatypedef MotorPID_Data;
Speedtypedef    Speed_Data;
bool IsMotorOn = false;

void MotorPWM_output(int M) //����������(ռ�ձ�)
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

//////���PID
//����ʽPI����
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
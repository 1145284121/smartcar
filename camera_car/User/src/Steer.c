#include "include.h"
#include "Motor.h"
#include "Steer.h"




PID_Datatypedef  SteerPIDdata[5];
float PicErr[5] = {0};
void SteerPID(float Offset)
{
    static float LastOffset = 0;
    int SteerPWM = 0; 
    float  SteerP, SteerD;
    float err = ABSF(Offset);
    if(err < PicErr[0])         //根据误差，进行分段控制
    {
        SteerP = SteerPIDdata[0].P;
        SteerD = SteerPIDdata[0].D;
    }
    else if(err < PicErr[1])
    {
        SteerP = SteerPIDdata[1].P;
        SteerD = SteerPIDdata[1].D;
    }
    else if(err < PicErr[2])
    {
        SteerP = SteerPIDdata[2].P;
        SteerD = SteerPIDdata[2].D;
    }
    else if(err < PicErr[3])
    {
        SteerP = SteerPIDdata[3].P;
        SteerD = SteerPIDdata[3].D;
    }
    else
    {
        SteerP = SteerPIDdata[4].P;
        SteerD = SteerPIDdata[4].D;
    }
  
    SteerPWM = (int)(SteerP * Offset + SteerD * (Offset - LastOffset));        //D实际实现I的效果，平滑控制
    LastOffset = Offset;    //保存本次误差
    if(SteerPWM >= SteerMAX)                //限幅
        SteerPWM = SteerMAX;
    else if(SteerPWM <= -SteerMAX)
        SteerPWM = -SteerMAX;
    ftm_pwm_duty(ftm2, ftm_ch0, (SteerCenter + SteerPWM));          //控制舵机转向
}
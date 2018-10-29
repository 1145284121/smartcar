#include "include.h"
#include "isr.h"
#include "Motor.h"
#include "Steer.h"
#include "Camera.h"
#include "KeyBoardDriver.h"
#include "Attitude_Calculation.h"


extern void DirectionControl(void);
/**************************************************/
/*将全部的控制及采集函数放于该中断*/
/*************************************************/
int setSpeed = 0;
void PIT0_IRQHandler(void)
{
    PIT_FlAG_CLR(PIT0);
  
    ///////////////至于磁力计只用于起跑线检测则无需进行浮点运算，只要判断数值大小即可
    GetSpeed();
    DirectionControl();
    //Get_Car_Angle(); 
    MotorPWM_output(MotorPID_output(setSpeed, Speed_Data.Mpulse));
    /*****************************************************************/
    /*我是分界线，下面是姿态控制*/
    /******************************************************************/
  
    /*****************************************************************/
    /*我是分界线，上面是姿态控制*/
    /******************************************************************/
}


void PIT1_IRQHandler(void)
{
    PIT_FlAG_CLR(PIT1);
    key_scan();
}





#include "include.h"
#include "isr.h"
#include "Motor.h"
#include "Steer.h"
#include "Camera.h"
#include "KeyBoardDriver.h"
#include "Attitude_Calculation.h"


extern void DirectionControl(void);
/**************************************************/
/*��ȫ���Ŀ��Ƽ��ɼ��������ڸ��ж�*/
/*************************************************/
int setSpeed = 0;
void PIT0_IRQHandler(void)
{
    PIT_FlAG_CLR(PIT0);
  
    ///////////////���ڴ�����ֻ���������߼����������и������㣬ֻҪ�ж���ֵ��С����
    GetSpeed();
    DirectionControl();
    //Get_Car_Angle(); 
    MotorPWM_output(MotorPID_output(setSpeed, Speed_Data.Mpulse));
    /*****************************************************************/
    /*���Ƿֽ��ߣ���������̬����*/
    /******************************************************************/
  
    /*****************************************************************/
    /*���Ƿֽ��ߣ���������̬����*/
    /******************************************************************/
}


void PIT1_IRQHandler(void)
{
    PIT_FlAG_CLR(PIT1);
    key_scan();
}





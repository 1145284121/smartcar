/*********************************************************************************************************************
 * 
 * @file       		direction.c
 *  				方向控制
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "include.h"
#include  "math.h"
#include  "BMX055.h"
#include  "Attitude_Calculation.h"
#include  "direction.h"

#define Yaw_Control_P_Max  2000
   
/**********全局变量定义********/ 
static bool  IsGyroOffsetReset = false;          /////如果需要进行陀螺仪零飘矫正则将改变量置为   1
                                                  /////置1的方式可以通过 按键或者别的操作
CarInfotypedef    CarInfo;
BMX055Datatypedef      BMX055_data;
/*PIDcontroltypedef      SpeedRadius={1,0,5};
PIDcontroltypedef      yawPIDcontrol={8.2,0,0};*/
AttitudeDatatypedef    GyroOffset;           //

float g_fDirectionControlOut;	//方向控制输出
//uint8 Flag_Round = OFF;			//进入环岛的标志（在环岛里为ON）

float AccZAngle = 0;

void GyroOffset_init(void)      /////////陀螺仪零飘初始化
{
    static uint16 Count = 0;
    if(Count == 0)
    {
        GyroOffset.Xdata = 0;
        GyroOffset.Ydata = 0;
        GyroOffset.Zdata = 0;
    }
  
    if(Count == 1000)
    {
        GyroOffset.Xdata /= 1000;
        GyroOffset.Ydata /= 1000;
        GyroOffset.Zdata /= 1000;
        IsGyroOffsetReset = false;
        Count = 0;
    }
    else
    {
        BMX055_DataRead(&BMX055_data, 0);
        GyroOffset.Xdata = BMX055_data.GYROXdata;
        GyroOffset.Ydata = BMX055_data.GYROYdata;
        GyroOffset.Zdata = BMX055_data.GYROZdata;
        Count++;
    }
}




void  Get_Car_Angle(void)   //获取陀螺仪角度
{
    float AccZ = 0;
    float LastAngle = 0;
    float AccZAdjust = 0;
    static uint8 MAGWaitCount = 0;
    MAGWaitCount++;
    if(MAGWaitCount < 15)
        BMX055_DataRead(&BMX055_data, 0);
    else
    {
        BMX055_DataRead(&BMX055_data, 1);    //////每30ms读取一次磁力计
        MAGWaitCount = 0;
    }

    BMX055_data.GYROXdata = (BMX055_data.GYROXdata - Gyro.Xoffset) * 0.030517578;   
    BMX055_data.GYROYdata = (BMX055_data.GYROYdata - Gyro.Yoffset) * 0.030517578;
    BMX055_data.GYROZdata = (BMX055_data.GYROZdata - Gyro.Zoffset) * 0.030517578;
    ///////1000 / 32768     //////BMX055本身零飘几乎可以忽略不计，但是安全起见还是矫正一下
    BMX055_data.ACCXdata *= 0.001953125;    ///////4 / 2048
    BMX055_data.ACCYdata *= 0.001953125;
    BMX055_data.ACCZdata *= 0.001953125;
  

    Acc.Xdata = BMX055_data.ACCXdata;
    Acc.Ydata = BMX055_data.ACCYdata;
    Acc.Zdata = BMX055_data.ACCZdata;
    Gyro.Xdata = BMX055_data.GYROXdata;
    Gyro.Ydata = BMX055_data.GYROYdata;
    Gyro.Zdata = BMX055_data.GYROZdata;
    
    if(CarInfo.Quaternion_init == 'F')
    {
        Quaternion_init();               //四元数初始化必要,此时最好静止
        CarInfo.Quaternion_init = 'T';
    }
    else
    {
        Attitude_UpdateGyro();////由陀螺仪解四元数          ///
        Attitude_UpdateAcc();////由加速度计解四元数              ///
  
        CarInfo.pitchrate = -EulerAngleRate.Roll/PI*180;   //俯仰角速度单位°
        CarInfo.yawrate = EulerAngleRate.Yaw / PI * 180;
        CarInfo.pitch = -EulerAngle.Roll/PI*180;            //俯仰角单位°
        CarInfo.yaw   = EulerAngle.Yaw/PI*180;
      
        AccZ = -Acc.Zdata;
        if(AccZ > 1)
            AccZ = 1;
        if(AccZ < -1)
            AccZ = -1;            /////////加速度真实值限幅
        AccZAngle = asinf(AccZ) * 180 / PI;
        LastAngle = CarInfo.pitch;
        AccZAdjust = (AccZAngle - LastAngle) * 0.002;
        CarInfo.pitch += (-Gyro.Xdata * 0.002 + AccZAdjust);
        CarInfo.MagX = BMX055_data.MAGXdata;
        CarInfo.MagY = BMX055_data.MAGYdata;
        CarInfo.MagZ = BMX055_data.MAGZdata;
    }
    return;
}

/*
void Get_Car_BAT()
{
  static float LastBAT = 0;
  uint16 buff = BAT_AD();
  if(LastBAT == 0)LastBAT = buff;
  LastBAT += (buff - LastBAT) * 0.04;
  CarInfo.BAT = (float)LastBAT * ADconvert;    ///一阶惯性滤波
}

*/

static float Set_Curvature(float Excurvature)/////半径闭环（完成）       使用到了CarInfo中的speed_Mtrue，MaxAcc，MinR，RadiusAdjustSpeed，yawrate
{                                                                               //一系列运算操作后，获得OUTPwm
  
    static float   ExYawRate = 0;
    static float   OutPwm = 0;
    //float SpeedMinRadius = 0;
    //float PathMinRadius = 0;
    float MinRadius = 0;
    float MaxCurvature = 0;
    float SpeedGainCur = 1;

    /*if(g_fRealSpeed > CarInfo.MinSpeed)                     //中真值速度
    {
        SpeedMinRadius = g_fRealSpeed * g_fRealSpeed * 0.01 / CarInfo.MaxAcc;//最大离心加速度
                                                                                //疑问：这个0.01是干什么用的？（单位换算？）
        if(SpeedMinRadius < CarInfo.MinR)
            MinRadius = CarInfo.MinR;
    }
    else
        MinRadius = CarInfo.MinR;
                                            //如果未超出最大速度，则之后无需用到最小半径（？？所以最小半径与最小速度匹配？？）
    
  
    if(CarInfo.speed_Mtrue  > CarInfo.RadiusAdjustSpeed)              //？？过弯时速度限制？？
    {
        SpeedGainCur = ((g_fRealSpeed - CarInfo.RadiusAdjustSpeed) * SpeedRadius.P);
        if(SpeedGainCur < 0.1)
            SpeedGainCur = 0.1;
        else if(SpeedGainCur > 10)
            SpeedGainCur = 10;
    }
                                                                    //如果未超出则使用默认值1
    SpeedGainCur = 1 / SpeedGainCur;*/
  
    if(Excurvature > 0)
        Excurvature = SpeedGainCur * Excurvature / (SpeedGainCur - Excurvature);
    else
        Excurvature = SpeedGainCur * Excurvature / (SpeedGainCur + Excurvature); 
  
    

    MaxCurvature = 1 / MinRadius;                            ///////最大曲率限幅
    
    if(Excurvature > MaxCurvature)
        Excurvature = MaxCurvature;
    else if(Excurvature < -MaxCurvature)
        Excurvature = -MaxCurvature; /////期望曲率限幅

    ExYawRate = Excurvature * CarInfo.speed_Mtrue * 180 / PI;  /////期望角速度=实际速度除以曲率半径

    float Yaw_Control_P = ExYawRate - CarInfo.yawrate;          //P_error

    if(Yaw_Control_P > Yaw_Control_P_Max)
        Yaw_Control_P = Yaw_Control_P_Max;
    else if(Yaw_Control_P < -Yaw_Control_P_Max)
        Yaw_Control_P = -Yaw_Control_P_Max;                     //积分限幅

    float SetP; 
    //float SetD;

    SetP = 666;
    //SetD = 666;                     //起到了一个积分的效果，平滑效果，

//    if(Yaw_Control_P * CarInfo.yawrate < 0)//////转向与角速度不同向，微分清零
//       SetD = 0;
      
    //OutPwm = (SetP * Yaw_Control_P + SetD * CarInfo.yawrate);           //？？PD控制的D直接乘以实际值？？
    OutPwm = SetP * Yaw_Control_P;
    
    return (OutPwm);
    
}


//预警：此函数完全被修改，上面的说明与实际不符
void DirectionControl(void)
{
    Get_Car_Angle();                                       //将BMX055的值读入CarInfo中，以供处理器做半径闭环
    g_fDirectionControlOut=Set_Curvature(CarInfo.ExpectCur);
}




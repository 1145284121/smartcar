/*********************************************************************************************************************
 * 
 * @file       		direction.c
 *  				�������
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "include.h"
#include  "math.h"
#include  "BMX055.h"
#include  "Attitude_Calculation.h"
#include  "direction.h"

#define Yaw_Control_P_Max  2000
   
/**********ȫ�ֱ�������********/ 
static bool  IsGyroOffsetReset = false;          /////�����Ҫ������������Ʈ�����򽫸ı�����Ϊ   1
                                                  /////��1�ķ�ʽ����ͨ�� �������߱�Ĳ���
CarInfotypedef    CarInfo;
BMX055Datatypedef      BMX055_data;
/*PIDcontroltypedef      SpeedRadius={1,0,5};
PIDcontroltypedef      yawPIDcontrol={8.2,0,0};*/
AttitudeDatatypedef    GyroOffset;           //

float g_fDirectionControlOut;	//����������
//uint8 Flag_Round = OFF;			//���뻷���ı�־���ڻ�����ΪON��

float AccZAngle = 0;

void GyroOffset_init(void)      /////////��������Ʈ��ʼ��
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




void  Get_Car_Angle(void)   //��ȡ�����ǽǶ�
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
        BMX055_DataRead(&BMX055_data, 1);    //////ÿ30ms��ȡһ�δ�����
        MAGWaitCount = 0;
    }

    BMX055_data.GYROXdata = (BMX055_data.GYROXdata - Gyro.Xoffset) * 0.030517578;   
    BMX055_data.GYROYdata = (BMX055_data.GYROYdata - Gyro.Yoffset) * 0.030517578;
    BMX055_data.GYROZdata = (BMX055_data.GYROZdata - Gyro.Zoffset) * 0.030517578;
    ///////1000 / 32768     //////BMX055������Ʈ�������Ժ��Բ��ƣ����ǰ�ȫ������ǽ���һ��
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
        Quaternion_init();               //��Ԫ����ʼ����Ҫ,��ʱ��þ�ֹ
        CarInfo.Quaternion_init = 'T';
    }
    else
    {
        Attitude_UpdateGyro();////�������ǽ���Ԫ��          ///
        Attitude_UpdateAcc();////�ɼ��ٶȼƽ���Ԫ��              ///
  
        CarInfo.pitchrate = -EulerAngleRate.Roll/PI*180;   //�������ٶȵ�λ��
        CarInfo.yawrate = EulerAngleRate.Yaw / PI * 180;
        CarInfo.pitch = -EulerAngle.Roll/PI*180;            //�����ǵ�λ��
        CarInfo.yaw   = EulerAngle.Yaw/PI*180;
      
        AccZ = -Acc.Zdata;
        if(AccZ > 1)
            AccZ = 1;
        if(AccZ < -1)
            AccZ = -1;            /////////���ٶ���ʵֵ�޷�
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
  CarInfo.BAT = (float)LastBAT * ADconvert;    ///һ�׹����˲�
}

*/

static float Set_Curvature(float Excurvature)/////�뾶�ջ�����ɣ�       ʹ�õ���CarInfo�е�speed_Mtrue��MaxAcc��MinR��RadiusAdjustSpeed��yawrate
{                                                                               //һϵ����������󣬻��OUTPwm
  
    static float   ExYawRate = 0;
    static float   OutPwm = 0;
    //float SpeedMinRadius = 0;
    //float PathMinRadius = 0;
    float MinRadius = 0;
    float MaxCurvature = 0;
    float SpeedGainCur = 1;

    /*if(g_fRealSpeed > CarInfo.MinSpeed)                     //����ֵ�ٶ�
    {
        SpeedMinRadius = g_fRealSpeed * g_fRealSpeed * 0.01 / CarInfo.MaxAcc;//������ļ��ٶ�
                                                                                //���ʣ����0.01�Ǹ�ʲô�õģ�����λ���㣿��
        if(SpeedMinRadius < CarInfo.MinR)
            MinRadius = CarInfo.MinR;
    }
    else
        MinRadius = CarInfo.MinR;
                                            //���δ��������ٶȣ���֮�������õ���С�뾶������������С�뾶����С�ٶ�ƥ�䣿����
    
  
    if(CarInfo.speed_Mtrue  > CarInfo.RadiusAdjustSpeed)              //��������ʱ�ٶ����ƣ���
    {
        SpeedGainCur = ((g_fRealSpeed - CarInfo.RadiusAdjustSpeed) * SpeedRadius.P);
        if(SpeedGainCur < 0.1)
            SpeedGainCur = 0.1;
        else if(SpeedGainCur > 10)
            SpeedGainCur = 10;
    }
                                                                    //���δ������ʹ��Ĭ��ֵ1
    SpeedGainCur = 1 / SpeedGainCur;*/
  
    if(Excurvature > 0)
        Excurvature = SpeedGainCur * Excurvature / (SpeedGainCur - Excurvature);
    else
        Excurvature = SpeedGainCur * Excurvature / (SpeedGainCur + Excurvature); 
  
    

    MaxCurvature = 1 / MinRadius;                            ///////��������޷�
    
    if(Excurvature > MaxCurvature)
        Excurvature = MaxCurvature;
    else if(Excurvature < -MaxCurvature)
        Excurvature = -MaxCurvature; /////���������޷�

    ExYawRate = Excurvature * CarInfo.speed_Mtrue * 180 / PI;  /////�������ٶ�=ʵ���ٶȳ������ʰ뾶

    float Yaw_Control_P = ExYawRate - CarInfo.yawrate;          //P_error

    if(Yaw_Control_P > Yaw_Control_P_Max)
        Yaw_Control_P = Yaw_Control_P_Max;
    else if(Yaw_Control_P < -Yaw_Control_P_Max)
        Yaw_Control_P = -Yaw_Control_P_Max;                     //�����޷�

    float SetP; 
    //float SetD;

    SetP = 666;
    //SetD = 666;                     //����һ�����ֵ�Ч����ƽ��Ч����

//    if(Yaw_Control_P * CarInfo.yawrate < 0)//////ת������ٶȲ�ͬ��΢������
//       SetD = 0;
      
    //OutPwm = (SetP * Yaw_Control_P + SetD * CarInfo.yawrate);           //����PD���Ƶ�Dֱ�ӳ���ʵ��ֵ����
    OutPwm = SetP * Yaw_Control_P;
    
    return (OutPwm);
    
}


//Ԥ�����˺�����ȫ���޸ģ������˵����ʵ�ʲ���
void DirectionControl(void)
{
    Get_Car_Angle();                                       //��BMX055��ֵ����CarInfo�У��Թ����������뾶�ջ�
    g_fDirectionControlOut=Set_Curvature(CarInfo.ExpectCur);
}




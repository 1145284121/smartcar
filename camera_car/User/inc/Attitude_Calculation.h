//#define  PI            3.1415926f
#define  PERIODHZ      (float)500       /////采样频率
#define  PERIODS       (float)0.002           ////采样周期


typedef struct{
  uint8  Quaternion_init;   ////四元数初始化
  uint8  Motor_ON;          ////电机使能
  
  float speed_R;          ///右编码器速度
  float speed_L;          //左编码器速度
  float speed_M;          //中编码器速度
  float speed_Mtrue;     //中真实速度
  float speed_Rtrue;      //右真实速度
  float speed_Ltrue;       //左真实速度
  float speed_RtrueEncoder;  //右真实编码器速度
  float speed_LtrueEncoder;  //左真实编码器速度
  float TureSpeedGrowLimit;    //真实速度增幅
  float TureSpeedReduceLimit;  //真实速度降幅
  float TrueSpeedMAXLimit;     //真实速度最大值
  float TrueSpeedYawRate;      //由真实速度计算的角速度
  float ExpectSpeed;           //期望速度
  float ExpectCur;              
  float ExpectAngleGrow;   ///期望角度增幅限制
  float ExpectAngleReduce; ///期望角度降幅限制
  
  
  float BalanceAngle;      ///平衡角度
  float AngleMax;          ///最大角度
  float AngleMin;          ///最小角度
  
  
  float pitchrate;   //俯仰角速度
  float pitch;       //俯仰角
  float yaw;         //偏航角
  float yawrate;     //偏航角速度
  float rollrate;    //翻滚角速度
  float roll;        //翻滚角
  float radius;       //半径
  float curvature;    //曲率
  
  float time_s;      //时间
  
  float length;        //距离
  int16 lengthcount;  //距离脉冲
  float lengthcarry;  //距离进位
  
  float BAT;         ///电池电压
  
  float NowSiteX;    ///当前坐标X
  float NowSiteY;    ///当前坐标Y
  
  float MinSpeed;     //最小速度
  float MinR;         //最小半径
  float MaxR;         //最大半径
  float MaxAcc;       //最大离心加速度
  float MaxSpeed;     //最大速度
  float BalanceSpeed; //均衡速度
  float SteerRmax;    //舵机右偏最大值
  float SteerLmax;    //舵机左偏最大值
  float BodyRadius;   //车体轴向半径
  float RadiusAdjustSpeed;      //
  float StraightTest;          //
  int   StraightCount;         //
  
  float MagX;
  float MagY;
  float MagZ;
    
  uint8 RoadType;            //直道为1，否则为0 
  uint8 IsStop;
}CarInfotypedef;



typedef struct{
  float W;
  float X;
  float Y;
  float Z;
}QuaternionTypedef;

typedef struct{ 
  float Pitch;  //俯仰角
  float Yaw;    //偏航角
  float Roll;   //翻滚角
  float Test;
}EulerAngleTypedef;


typedef struct{
  float Xdata;
  float Ydata;
  float Zdata;
  float Xoffset;
  float Yoffset;
  float Zoffset;
  uint8 GyrooffsetOK;
}AttitudeDatatypedef;

extern QuaternionTypedef    Quaternion;   //四元数
extern EulerAngleTypedef    EulerAngle;   //欧拉角
extern QuaternionTypedef    AxisAngle;    //轴角
extern EulerAngleTypedef    EulerAngleRate;//当前欧拉角速度

extern QuaternionTypedef    MeaQuaternion;
extern EulerAngleTypedef    MeaEulerAngle;
extern QuaternionTypedef    MeaAxisAngle;

extern QuaternionTypedef    ErrQuaternion;
extern EulerAngleTypedef    ErrEulerAngle;
extern QuaternionTypedef    ErrAxisAngle;
extern AttitudeDatatypedef         Acc;
extern AttitudeDatatypedef         Gyro;


extern void Quaternion_init();

extern void Attitude_UpdateGyro(void);

extern void Attitude_UpdateAcc(void);
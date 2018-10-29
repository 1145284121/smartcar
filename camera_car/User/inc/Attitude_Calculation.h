//#define  PI            3.1415926f
#define  PERIODHZ      (float)500       /////����Ƶ��
#define  PERIODS       (float)0.002           ////��������


typedef struct{
  uint8  Quaternion_init;   ////��Ԫ����ʼ��
  uint8  Motor_ON;          ////���ʹ��
  
  float speed_R;          ///�ұ������ٶ�
  float speed_L;          //��������ٶ�
  float speed_M;          //�б������ٶ�
  float speed_Mtrue;     //����ʵ�ٶ�
  float speed_Rtrue;      //����ʵ�ٶ�
  float speed_Ltrue;       //����ʵ�ٶ�
  float speed_RtrueEncoder;  //����ʵ�������ٶ�
  float speed_LtrueEncoder;  //����ʵ�������ٶ�
  float TureSpeedGrowLimit;    //��ʵ�ٶ�����
  float TureSpeedReduceLimit;  //��ʵ�ٶȽ���
  float TrueSpeedMAXLimit;     //��ʵ�ٶ����ֵ
  float TrueSpeedYawRate;      //����ʵ�ٶȼ���Ľ��ٶ�
  float ExpectSpeed;           //�����ٶ�
  float ExpectCur;              
  float ExpectAngleGrow;   ///�����Ƕ���������
  float ExpectAngleReduce; ///�����ǶȽ�������
  
  
  float BalanceAngle;      ///ƽ��Ƕ�
  float AngleMax;          ///���Ƕ�
  float AngleMin;          ///��С�Ƕ�
  
  
  float pitchrate;   //�������ٶ�
  float pitch;       //������
  float yaw;         //ƫ����
  float yawrate;     //ƫ�����ٶ�
  float rollrate;    //�������ٶ�
  float roll;        //������
  float radius;       //�뾶
  float curvature;    //����
  
  float time_s;      //ʱ��
  
  float length;        //����
  int16 lengthcount;  //��������
  float lengthcarry;  //�����λ
  
  float BAT;         ///��ص�ѹ
  
  float NowSiteX;    ///��ǰ����X
  float NowSiteY;    ///��ǰ����Y
  
  float MinSpeed;     //��С�ٶ�
  float MinR;         //��С�뾶
  float MaxR;         //���뾶
  float MaxAcc;       //������ļ��ٶ�
  float MaxSpeed;     //����ٶ�
  float BalanceSpeed; //�����ٶ�
  float SteerRmax;    //�����ƫ���ֵ
  float SteerLmax;    //�����ƫ���ֵ
  float BodyRadius;   //��������뾶
  float RadiusAdjustSpeed;      //
  float StraightTest;          //
  int   StraightCount;         //
  
  float MagX;
  float MagY;
  float MagZ;
    
  uint8 RoadType;            //ֱ��Ϊ1������Ϊ0 
  uint8 IsStop;
}CarInfotypedef;



typedef struct{
  float W;
  float X;
  float Y;
  float Z;
}QuaternionTypedef;

typedef struct{ 
  float Pitch;  //������
  float Yaw;    //ƫ����
  float Roll;   //������
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

extern QuaternionTypedef    Quaternion;   //��Ԫ��
extern EulerAngleTypedef    EulerAngle;   //ŷ����
extern QuaternionTypedef    AxisAngle;    //���
extern EulerAngleTypedef    EulerAngleRate;//��ǰŷ�����ٶ�

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
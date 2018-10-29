

#ifndef _MT9V032_h
#define _MT9V032_h



#define LimitL(L)     (L = ((L < 1) ? 1 : L))
#define LimitH(H)     (H =  ((H > 78) ? 78 : H))
#define ImageSensorMid       39
#define MaxJumpPoint         10
#define ImageScanInterval    5
#define ISColumn             80


/////////////////以下为MT9部分

#define COL     188//图像宽度       不要更改
#define ROW     120//图像高度		可选参数有120 240 480   分辨率切换后，最好断电一次系统


extern uint8 mt9v032_finish_flag;  //一场图像采集完成标志位
extern uint8 image[ROW][COL];      //内部使用


void   VSYNC(void);
void   camera_init(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);


//////////////////以上为MT9部分

typedef struct
{
  int   point;
  uint8 type;
}JumpPointtypedef;


typedef struct
{
  /*左右边边界标志    T为正常跳变边    W为无边   P为障碍类多跳边的内边*/
  uint8 IsRightFind;   
  uint8 IsLeftFind;
  
   
  int Wide;              ////边界宽度
  int LeftBorder;        ////左边界
  int RightBorder;       ////右边界
  int Center;            ////中线
  int RightTemp;         ////右边临时值
  int LeftTemp;          /////左边临时值
  int CenterTemp;        ////中线临时值
}ImageDealDatatypedef;


typedef struct
{
/*以下关于全局图像正常参数*/   
  uint8    OFFLine;          /////图像顶边
  float   OFFRoute;         /////图像顶边转换厘米
  float   PitchAngle;       ////当前图像俯仰角
  uint8    WhiteLine;        ////双边丢边数

  
  
  uint8   IsDanger; 
  float   ExpectCur;        /////图像期望曲率


/*以下关于障碍*/
  uint8   ObstacleType;
  float   ObstacleDistance;  /////检测到障碍的开始距离
  float   ObstacleCenter;    /////障碍延迟对中
  uint8    ObstacleDelay;     /////障碍延迟标志
  float   ObstacleOverDistance;
 
/*以下关于圆环*/   
  uint8    CirquePass;       /////通过圆环标志
  uint8    CirqueOut;        /////出圆环标志
  uint8    IsCinqueOutIn;
  int    CirquePassLine;
  uint8   CirqueType;
  
/*以下关于起跑线*/  
  uint8     IsStartPass;     ////起跑线通过标志
  float    StartDistance;   ////起跑线距离
  
  
  uint8    CenterGrow;
  uint8    CenterReduce;
  uint8    IsLittleS;
  
  uint8    RampWayFlag;
  float    RBorderK;
  float    LBorderK;
  float   RampStartDistance;
}ImageStatustypedef;

extern ImageDealDatatypedef    ImageDeal[60];
extern ImageStatustypedef      ImageStatus;
extern uint8 PicForNow[60][80];




#endif
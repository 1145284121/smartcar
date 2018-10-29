

#ifndef _MT9V032_h
#define _MT9V032_h



#define LimitL(L)     (L = ((L < 1) ? 1 : L))
#define LimitH(H)     (H =  ((H > 78) ? 78 : H))
#define ImageSensorMid       39
#define MaxJumpPoint         10
#define ImageScanInterval    5
#define ISColumn             80


/////////////////����ΪMT9����

#define COL     188//ͼ����       ��Ҫ����
#define ROW     120//ͼ��߶�		��ѡ������120 240 480   �ֱ����л�����öϵ�һ��ϵͳ


extern uint8 mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ
extern uint8 image[ROW][COL];      //�ڲ�ʹ��


void   VSYNC(void);
void   camera_init(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);


//////////////////����ΪMT9����

typedef struct
{
  int   point;
  uint8 type;
}JumpPointtypedef;


typedef struct
{
  /*���ұ߽߱��־    TΪ���������    WΪ�ޱ�   PΪ�ϰ�������ߵ��ڱ�*/
  uint8 IsRightFind;   
  uint8 IsLeftFind;
  
   
  int Wide;              ////�߽���
  int LeftBorder;        ////��߽�
  int RightBorder;       ////�ұ߽�
  int Center;            ////����
  int RightTemp;         ////�ұ���ʱֵ
  int LeftTemp;          /////�����ʱֵ
  int CenterTemp;        ////������ʱֵ
}ImageDealDatatypedef;


typedef struct
{
/*���¹���ȫ��ͼ����������*/   
  uint8    OFFLine;          /////ͼ�񶥱�
  float   OFFRoute;         /////ͼ�񶥱�ת������
  float   PitchAngle;       ////��ǰͼ������
  uint8    WhiteLine;        ////˫�߶�����

  
  
  uint8   IsDanger; 
  float   ExpectCur;        /////ͼ����������


/*���¹����ϰ�*/
  uint8   ObstacleType;
  float   ObstacleDistance;  /////��⵽�ϰ��Ŀ�ʼ����
  float   ObstacleCenter;    /////�ϰ��ӳٶ���
  uint8    ObstacleDelay;     /////�ϰ��ӳٱ�־
  float   ObstacleOverDistance;
 
/*���¹���Բ��*/   
  uint8    CirquePass;       /////ͨ��Բ����־
  uint8    CirqueOut;        /////��Բ����־
  uint8    IsCinqueOutIn;
  int    CirquePassLine;
  uint8   CirqueType;
  
/*���¹���������*/  
  uint8     IsStartPass;     ////������ͨ����־
  float    StartDistance;   ////�����߾���
  
  
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
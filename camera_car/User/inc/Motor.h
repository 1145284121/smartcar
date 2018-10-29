typedef struct{
  float P;
  float I;
  float D;
}PID_Datatypedef;

typedef struct{
  int16    Mpulse;
  float  Mspeed;
  float  Length;
}Speedtypedef;

extern PID_Datatypedef MotorPID_Data;
extern Speedtypedef    Speed_Data;

void MotorPWM_output(int M);
int MotorPID_output(int ExpectSpeed, int NowSpeed);
void GetSpeed(void);
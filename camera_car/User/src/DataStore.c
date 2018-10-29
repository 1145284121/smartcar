#include "include.h"
#include "DataStore.h"
#include "isr.h"
#include "Motor.h"
#include "Steer.h"

FLASH_WRITE_TYPE   FLASH_DATA[FLASHNUM];


bool My_FlashWrite(uint16 Boot)        /////写入
{
    FLASH_ERASE(BOOTSECTOR - Boot);
    for(int n = 0; n < FLASHNUM; n++)
    {
       FLASH_WRITE((BOOTSECTOR - Boot), (FLASH_ALIGN_ADDR * n), FLASH_DATA[n]);
       FLASH_DELAY(4000);
    }
    return true;
}

bool My_FlashRead(uint16 Boot)          ////读出
{
    for(int n = 0; n < FLASHNUM; n++)
    {
       FLASH_DATA[n] = FLASH_READ((BOOTSECTOR - Boot), (FLASH_ALIGN_ADDR * n), FLASH_WRITE_TYPE);
       FLASH_DELAY(4000);
    }
    return true;
}

extern int setSpeed;
extern  int TowPoint;
void DataSave(void)                       ///////数据保存
{
    FLASH_DATA[0] = Float2U32(MotorPID_Data.P);
    FLASH_DATA[1] = Float2U32(MotorPID_Data.I);
    FLASH_DATA[2] = Float2U32(setSpeed);
    FLASH_DATA[3] = Float2U32(TowPoint);
  
    for(uint8 i = 0; i < 5; i++)
    { 
        FLASH_DATA[10 + 3 * i] = Float2U32(PicErr[i]);
        FLASH_DATA[10 + 3 * i + 1] = Float2U32(SteerPIDdata[i].P);
        FLASH_DATA[10 + 3 * i + 2] = Float2U32(SteerPIDdata[i].D);
    }
    My_FlashWrite(0);
    return;
}

void DataLoad(void)                       ///////数据载入
{
    My_FlashRead(0);
    MotorPID_Data.P = U322Float(&FLASH_DATA[0]);
    MotorPID_Data.I = U322Float(&FLASH_DATA[1]);
    setSpeed = (int)U322Float(&FLASH_DATA[2]);
    TowPoint = (int)U322Float(&FLASH_DATA[3]);
  
    for(uint8 i = 0; i < 5; i++)
    {
        PicErr[i] = U322Float(&FLASH_DATA[10 + 3 * i]);
        SteerPIDdata[i].P = U322Float(&FLASH_DATA[10 + 3 * i + 1]);
        SteerPIDdata[i].D = U322Float(&FLASH_DATA[10 + 3 * i + 2]);
    }
}


void Data_init(void)
{
    FLASH_init();
    Common_delay(100);
    //DataLoad();
}
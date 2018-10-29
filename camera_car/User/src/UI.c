#include "common.h"
#include "include.h"
#include "KeyBoardDriver.h"
#include "Camera.h"
#include "Motor.h"
#include "Steer.h"
#include "UI.h"


#define  Main_menuL  12
#define  Main_menuH  4

Key_msg_t         KeyStatus;
int               Cursor = 0;
int               NowPage = 0;
int               Menustatus = 0;
uint8             uintForNow = 0;
float             DataInterval =0;
float             DataToChange[40] = {0};
double            uintArray[8] = {100, 10, 1, 0, 0.1, 0.01, 0.001, 0.0001};
int               DataToChangeNum = 0;
extern volatile uint8 IsStart;

uint8 Main_menu[Main_menuH][Main_menuL] = {
  "Pitch_Set  ",
  "Yaw_Set    ",
  "Camera_View",
  "Data_save",
};

void AutoMode(void);
void Key_Event(void)   //按键事件  
{
  if(IsStart == 'T')
  {
    AutoMode();
  }
  if(Get_KeyFifo(&KeyStatus))    //有未处理的按键消息
  { 
    switch(KeyStatus.key)        //选按键
    {
    case Key_U:       if(!Menustatus)            //Menustatus为0时为选择菜单 1数据菜单
                        {
                          if(KeyStatus.status == 0)
                          {                         
                            Cursor--;
                            if(Cursor < 0)
                              Cursor = 0;
                          }                      
                        }
                      else
                      {
                       if(KeyStatus.status == 0)
                       {
                         DataInterval++;
                       }
                      }
                      break;
    case Key_D:       if(!Menustatus)
                        {
                          if(KeyStatus.status == 0)
                          {
                            Cursor++;
                          }                  
                        }
                      else
                      {
                       if(KeyStatus.status == 0)
                       {
                         DataInterval--;
                       }
                      }
                      break;
    case Key_Y:       if(KeyStatus.status == 0)
                      {
                       if(!Menustatus)
                         NowPage++;
                       else
                       {
                         uintForNow++;
                         if(uintForNow == 8) uintForNow = 0;
                       }
                      }
                      break;
    case Key_N:       if(KeyStatus.status == 0)
                      {
                         NowPage--;
                         if(Menustatus == 1)
                           Menustatus = 0;
                      }
                       break;                  
    default:         break;
    }
  }
  return;
}



////////数据修改
void ChangeData(uint8 *Object, uint8 Num, uint8 LastPage)
{
  OLED_CLS();
  Menustatus = 1;
  uintForNow = 2;
  OLED_PrintString(0, 0, Object, 0);
  while(1)
  {
    Key_Event();
    if(NowPage == LastPage)
    {
      Menustatus = 0;
      OLED_CLS();
      break;
    }
    
    if(uintArray[uintForNow] != 0)
       DataToChange[Num] += DataInterval * uintArray[uintForNow];
    else if(uintArray[uintForNow] == 0 && DataInterval != 0)
       DataToChange[Num] = 0;
    
    DataInterval = 0;
    
    
      OLED_PrintVar(0, 1, DataToChange[Num], 11, 4);

    if(uintArray[uintForNow] == 0)
      OLED_PrintStr(0, 2, "Clear To  0", 14, 1);
    else
      OLED_PrintVar(0, 2, uintArray[uintForNow], 14, 5);
  }
}


////////摄像头图像预览
static uint8 OLED_IMG[60][80] = {0};
uint8 ImageSensorTestChoose = 0;
volatile uint8 ImageSensorOK   = 0;
#define ImageSensorH  4
#define ImageSensorL  7
uint8 ImageSensorMenu[ImageSensorH][ImageSensorL] = 
{
  "Camera",
  "Border",
  "Real  ",
  "Test  ",
};
void ImageSensorView(void)
{
  uint8 y = 0;
  uint8 x = 0;
  OLED_CLS();
  Cursor = 0;
  int YLine = 59;
  ImageSensorTestChoose = 1;
  ImageSensorOK = 0;
  while(1)
  {
    Key_Event();
    if(Cursor == ImageSensorH) Cursor = ImageSensorH - 1;
    if(NowPage == 4) 
    {
        Menustatus = 1;
    }
    
    if(NowPage == 4)
    {
      YLine += DataInterval;
      if(YLine >= 59)YLine = 59;
      else if(YLine <= ImageStatus.OFFLine) YLine = ImageStatus.OFFLine;
      DataInterval = 0;
    }
    
    ImageSensorTestChoose = Cursor + 1;
    
    if(NowPage == 2)
    {
      ImageSensorTestChoose = 0;
      ImageSensorOK = 0;
      OLED_CLS();
      break;
    }
    ImgVSY_port->ISFR = 1 << ImgVSY_pin;
    NVIC_EnableIRQ(PORTA_IRQn);     ////循环中开启场中断
    while(ImageSensorOK == 0);
    for(y = 0; y < 2; y++)
    {
      if(Cursor < 2)
      {
      if(Cursor == y)
        OLED_PrintStr(80, y, ImageSensorMenu[y], ImageSensorL, 0);
      else
        OLED_PrintStr(80, y, ImageSensorMenu[y], ImageSensorL, 1);
      }
      else
      {
        if((Cursor - 2) == y)
        OLED_PrintStr(80, y, ImageSensorMenu[y + 2], ImageSensorL, 0);
        else
        OLED_PrintStr(80, y, ImageSensorMenu[y + 2], ImageSensorL, 1);
      }
    }
    
    switch(ImageSensorTestChoose)
    {
    case 1:
      for(x = 0; x < 60; x++)
       for(y = 0; y < 80; y++)
       {
         OLED_IMG[x][y] = 0xff;
         
       if(PicForNow[x][y] == 0x00)
         OLED_IMG[x][y] = 0xff;
      else
         OLED_IMG[x][y] = 0x00;
      
      if(x >= ImageStatus.OFFLine)
      {
        if(y == ImageDeal[x].Center)
          OLED_IMG[x][y] = 0xff;
        if(ImageDeal[x].IsLeftFind == 'C' && ImageDeal[x].IsRightFind == 'C')
          OLED_IMG[x][y] = 0xff;
        
        if(ImageDeal[x].RightTemp == y)
          OLED_IMG[x][y] = 0xff;
        if(ImageDeal[x].LeftTemp == y)
          OLED_IMG[x][y] = 0xff;
      }
        OLED_IMG[x][39] = 0xff;
      }
      OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
      break;
    case 2:
      for(x = 0; x < 60; x++)
      {
        for(y = 0; y < 80; y++)
        {  
           OLED_IMG[x][y] = 0xff;
           if(ImageStatus.OFFLine <= x)
           {
             if(y == ImageDeal[x].LeftBorder)
               OLED_IMG[x][y] = 0x00;
             else if(y == ImageDeal[x].RightBorder)
               OLED_IMG[x][y] = 0x00;
             else if(y == ImageDeal[x].Center)
               OLED_IMG[x][y] = 0x00;
           }
             OLED_IMG[x][39] = 0x00;
        }
      }
      OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
      break;
    case 3:
       for(x = 0; x < 60; x++)
      {
        for(y = 0; y < 80; y++)
        {  
           OLED_IMG[x][y] = 0xff;
           OLED_IMG[x][39] = 0x00;
        }
      }
      OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
      break;
    case 4:
      for(x = 0; x < 60; x++)
       for(y = 0; y < 80; y++)
       {
       if(PicForNow[x][y] == 0x00)
         OLED_IMG[x][y] = 0xff;
       else
         OLED_IMG[x][y] = 0x00;
       if(x >= ImageStatus.OFFLine)
       {
        if(y == ImageDeal[x].Center)
          OLED_IMG[x][y] = 0xff;
        if(ImageDeal[x].IsLeftFind == 'Q' || ImageDeal[x].IsRightFind == 'Q')
        {
          OLED_IMG[x][y] = 0xff;
        }
       }
         OLED_IMG[x][39] = 0xff;
       if(x == YLine)
       OLED_IMG[x][y] = 0xff;
       }
      OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
      break;
    default:break;
    }
    ImageSensorOK = 0;
  }
}

extern int setSpeed;
extern  int TowPoint;
////////俯仰角控制系数修改
#define BalanceDataH  4
#define BalanceDataL  14
uint8 BalanceData[BalanceDataH][BalanceDataL] =
{
  "ExSpeed",
  "PID_P",
  "PID_I",
  "DetPoint"
};
void BalanceData_Set(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	DataToChangeNum = BalanceDataH;
	DataToChange[0] = setSpeed;
	DataToChange[1] = MotorPID_Data.P;
	DataToChange[2] = MotorPID_Data.I;
        DataToChange[3] = TowPoint;
	while (1)
	{
		Key_Event();
		if (Cursor == BalanceDataH) Cursor = BalanceDataH - 1;
		if (NowPage == 4)
		{
			ChangeData(BalanceData[Cursor], Cursor, 3);
		}
		else if (NowPage == 2)
		{
			setSpeed = (int)DataToChange[0];
			MotorPID_Data.P = DataToChange[1];
			MotorPID_Data.I = DataToChange[2];
                        TowPoint = (int)DataToChange[3];
			OLED_CLS();
			break;
		}
		for (y = 0; y < 8; y++)
		{
                  if(y == BalanceDataH)break;
			if (Cursor < 8)
			{
				if (y == Cursor)
					OLED_PrintStr(0, y, BalanceData[y], BalanceDataL, 0);
				else
					OLED_PrintStr(0, y, BalanceData[y], BalanceDataL, 1);

				OLED_PrintVar(90, y, DataToChange[y], 6, 1);
			}
			else
			{
				if (y == 7)
					OLED_PrintStr(0, y, BalanceData[Cursor - 7 + y], BalanceDataL, 0);
				else
					OLED_PrintStr(0, y, BalanceData[Cursor - 7 + y], BalanceDataL, 1);

				OLED_PrintVar(90, y, DataToChange[Cursor - 7 + y], 6, 1);
			}
		}
	}
}


////////偏航系数修改
#define YawDataH   15
#define YawDataL   14
uint8 YawData[YawDataH][YawDataL] =
{
  "DevErr[0]",
  "DevErr[1]",
  "DevErr[2]",
  "DevErr[3]",
  "DevErr[4]",
  "ExpP[0]",
  "ExpP[1]",
  "ExpP[2]",
  "ExpP[3]",
  "ExpP[4]",
  "ExpD[0]",
  "ExpD[1]",
  "ExpD[2]",
  "ExpD[3]",
  "ExpD[4]",
  
};
void YawData_Set(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	DataToChangeNum = YawDataH;
	DataToChange[0] = PicErr[0];
	DataToChange[1] = PicErr[1];
	DataToChange[2] = PicErr[2];
	DataToChange[3] = PicErr[3];
	DataToChange[4] = PicErr[4];
	DataToChange[5] = SteerPIDdata[0].P;
	DataToChange[6] = SteerPIDdata[1].P;
	DataToChange[7] = SteerPIDdata[2].P;
	DataToChange[8] = SteerPIDdata[3].P;
	DataToChange[9] = SteerPIDdata[4].P;
        DataToChange[10] = SteerPIDdata[0].D;
	DataToChange[11] = SteerPIDdata[1].D;
	DataToChange[12] = SteerPIDdata[2].D;
	DataToChange[13] = SteerPIDdata[3].D;
	DataToChange[14] = SteerPIDdata[4].D;
	while (1)
	{
		Key_Event();
		if (Cursor == YawDataH) Cursor = YawDataH - 1;
		if (NowPage == 4)
		{
			ChangeData(YawData[Cursor], Cursor, 3);
		}
		else if (NowPage == 2)
		{
			PicErr[0] = DataToChange[0];
			PicErr[1] = DataToChange[1];
			PicErr[2] = DataToChange[2];
			PicErr[3] = DataToChange[3];
			PicErr[4] = DataToChange[4];
			SteerPIDdata[0].P = DataToChange[5];
			SteerPIDdata[1].P = DataToChange[6];
			SteerPIDdata[2].P = DataToChange[7];
			SteerPIDdata[3].P = DataToChange[8];
			SteerPIDdata[4].P = DataToChange[9];
                        SteerPIDdata[0].D = DataToChange[10];
			SteerPIDdata[1].D = DataToChange[11];
			SteerPIDdata[2].D = DataToChange[12];
			SteerPIDdata[3].D = DataToChange[13];
			SteerPIDdata[4].D = DataToChange[14];
			OLED_CLS();
			break;
		}
		for (y = 0; y < 8; y++)
		{
                  if(y == YawDataH)break;
			if (Cursor < 8)
			{
				if (y == Cursor)
					OLED_PrintStr(0, y, YawData[y], YawDataL, 0);
				else
					OLED_PrintStr(0, y, YawData[y], YawDataL, 1);

				OLED_PrintVar(90, y, DataToChange[y], 6, 1);
			}
			else
			{
				if (y == 7)
					OLED_PrintStr(0, y, YawData[Cursor - 7 + y], YawDataL, 0);
				else
					OLED_PrintStr(0, y, YawData[Cursor - 7 + y], YawDataL, 1);

				OLED_PrintVar(90, y, DataToChange[Cursor - 7 + y], 6, 1);
			}
		}
	}
}


#include "DataStore.h"
#define  FlashL 15
#define  FlashH 1
uint8 Flashch[FlashH][FlashL] = {
  "CH_1_ToSet",
};
void Data_WR(void)
{
	OLED_CLS();
	uint8 y = 0;
	Cursor = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == FlashH)Cursor = FlashH - 1;
		if (NowPage == 5) NowPage = 5;
		if (NowPage == 2)
		{
			OLED_CLS();
			break;
		}
		else if (NowPage == 4)
		{
			OLED_PrintString(70, 1, "U_ForLOAD", 1);
			OLED_PrintString(70, 2, "D_ForSAVE", 1);
			Menustatus = 1;
			while (1)
			{
				DataInterval = 0;
				Key_Event();
				if (NowPage == 5) NowPage = 4;
				if (DataInterval >= 1)
				{
					DataLoad();
					OLED_CLS();
					OLED_PrintString(20, 3, "Load Success", 1);
					CommonDelay(500);
					OLED_CLS();
					NowPage--;
					break;
				}
				else if (DataInterval <= -1)
				{
                                        DataSave();
					if (1)
					{
						OLED_CLS();
						OLED_PrintString(20, 3, "Save Success", 1);
						CommonDelay(500);
						OLED_CLS();
						NowPage--;
					}
					else
					{
						OLED_CLS();
						OLED_PrintString(20, 3, "Save Failed", 1);
						CommonDelay(500);
						OLED_CLS();
						NowPage--;
					}
					break;
				}
				else if (NowPage == 3)
				{
					OLED_CLS();
					break;
				}
			}
			Menustatus = 0;
		}
		else if (NowPage == 3)
		{
			OLED_PrintString(70, 1, "      ", 1);
			OLED_PrintString(70, 2, "      ", 1);
		}

		for (y = 0; y < FlashH; y++)
		{
			if (y == Cursor)
				OLED_PrintString(0, y, Flashch[y], 0);
			else
				OLED_PrintString(0, y, Flashch[y], 1);
		}
	}
}

extern bool IsMotorOn;
void AutoMode(void)    /////自动驾驶模式打开
{
  OLED_CLS();
  NVIC_DisableIRQ(PIT1_IRQn);
  NVIC_DisableIRQ(PIT0_IRQn);
  
  
  BUZZDOWN;  
  BUZZUP;
  CommonDelay(100);
  BUZZDOWN;
  CommonDelay(100);
  BUZZUP;
  CommonDelay(100);
  BUZZDOWN;
  CommonDelay(1000);
  IsMotorOn = true;
  NVIC_EnableIRQ(PIT0_IRQn);
  ImgVSY_port->ISFR = 1 << ImgVSY_pin;/////清除场中断标志位
  NVIC_EnableIRQ(PORTA_IRQn);
   while(1)
  {
  }
}


void Nextmenu(void)
{
  OLED_CLS();
  if(Cursor == 0)
  { 
     BalanceData_Set();
  }
  else if(Cursor == 1)
  {
     YawData_Set();
  }
  else if(Cursor == 2)
  {
    ImageSensorView();
  }
  else if(Cursor == 3)
  {
    Data_WR();
    OLED_CLS();
  }
  else
  {
    NowPage--;
  }
}


void OS_menu(void)
{
  NowPage = 2;     //当前页为2号页
  uint8 y = 0;
  Cursor = 0;    //当前光标为0
  Menustatus = 0;
  OLED_CLS();     //清屏
  
  while(1)
  {
    Key_Event();
    if(Cursor == Main_menuH)Cursor = 0;
    
    if(Cursor == Main_menuH) Cursor = Main_menuH - 1;
    switch(NowPage)
    {
    case 1:NowPage = 2;
           break;
    case 2:
      
           for(y = 0; y < 8; y++)
           {
             if(y == Main_menuH)break;
             if(Cursor < 8)
             {
              if(y == Cursor)
               {
                 OLED_PrintStr(0 , y , Main_menu[y] , Main_menuL, 0);     //光标选中反码输出
               }
              else
              OLED_PrintStr(0 , y , Main_menu[y] , Main_menuL, 1);
             }
             else
             {
               if(y == 7)
               {
                 OLED_PrintStr(0 , y , Main_menu[Cursor - 7 + y] , Main_menuL, 0);     //光标选中反码输出
               }
               else
               OLED_PrintStr(0 , y , Main_menu[Cursor - 7 + y] , Main_menuL, 1);
             }
           }
           //OLED_PrintBat(90, 0, CarInfo.Battery);
           break;
    case 3:Nextmenu();
           Cursor = 0;
           break;
    }
  }
}

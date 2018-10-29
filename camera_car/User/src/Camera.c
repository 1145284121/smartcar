#include "include.h"
#include "Camera.h"
#include "Motor.h"
#include "Steer.h"


/////////图像处理//0黑1白
uint8  ImageBuff[600] = {0};
uint8  PicForNow[60][80] = {0};
int   TowPoint = 29;
uint8   image[ROW][COL];      //图像数组


//-------------------------------------------------------------------------------------------------------------------
//*****************************************我是分界线**************************************************
//----------------------------------------以下为MT9部分
//-------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头初始化
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
/*
void camera_init(void)
{
    //设置参数    具体请参看使用手册
    uint16 light;
    //摄像头配置数组
    uint8 MT9V032_CFG[8];
	
    Common_delay(30);			//延时以保证摄像头上面的51完成上电初始化。此处被修改过
	
    uart_init (uart3, 115200);	//初始换串口 配置摄像头                     
    
    MT9V032_CFG[0] = 0xFF;     	//帧头         

    //命令位
	//具体请参看使用手册
    MT9V032_CFG[1] = 0x00;   
    
    //分辨率选择位   分辨率切换后，最好断电一次系统
	//其他分辨率具体看说明书，不过列超过188后K60无法采集，提供的更大的分辨率是给STM32具有摄像头接口的用户
	switch(ROW)
	{
		case 480:MT9V032_CFG[2] = 8;
                        break;
		case 240:MT9V032_CFG[2] = 9;
                        break;
		case 120:MT9V032_CFG[2] = 10;
                        break;
		default :MT9V032_CFG[2] = 10;
                        break;
	}
            
    
    //设置图像帧率        行数不同可设置的范围也不同		范围限制  120行的时候是1-200		240行的时候是1-132		480行的时候是1-66
    MT9V032_CFG[3] = 50;                    
    
    //曝光时间越大图像越亮     由于最大曝光时间受到fps与分辨率的共同影响，这里不给出具体范围，可以直接把曝光设置为1000，摄像头上的51收到之后会根据分辨率及FPS计算最大曝光时间，然后把
	//曝光时间设置为最大曝光时间，并且会返回当前设置的最大曝光时间，这样就可以知道最大可以设置的曝光时间是多少了，然后觉得图像过亮，就可以在这个最大曝光值的基础上相应减少。
    light = 600;
    MT9V032_CFG[4] = light>>8;          //曝光时间高八位   
    MT9V032_CFG[5] = (uint8)light;      //曝光时间低八位  
    
    //设置为0表示关闭自动曝光，设置1-63则启用自动曝光，设置的越大图像就越亮  建议开启这个功能，可以有效适应各种场地
    MT9V032_CFG[6] = 30; 
    
    //帧尾  
    MT9V032_CFG[7] = 0x5A;                
    //通过串口发送配置参数
    uart_putbuff(uart3,MT9V032_CFG,8);
	
	//延时以保障上个配置数据51已经成功写入到摄像头
    Common_delay(30);  //此处修改过
		
	
	//以下用于设置摄像头亮度   与上面的曝光时间是不一样的
    MT9V032_CFG[0] = 0xFF;     	//帧头
    MT9V032_CFG[1] = 0x02; 		//命令位  
    MT9V032_CFG[2] = 0; 		//无用，需设为0
    MT9V032_CFG[3] = 0;     	//无用，需设为0
    MT9V032_CFG[4] = 0;			//无用，需设为0
    MT9V032_CFG[5] = 32;      	//亮度等级选择            亮度等级 1 - 64
    MT9V032_CFG[6] = 0x35; 
    MT9V032_CFG[7] = 0x5A; 		//帧尾 
    uart_putbuff(uart3,MT9V032_CFG,8);


    //摄像头采集初始化
    DisableInterrupts;
    //DMA通道0初始化，PTE0触发源(默认上升沿)，源地址为C_IN_DATA(1)(PTC8-PTC15)，目的地址为：image，每次传输1Byte 每次传输完毕恢复目的地址
    dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(1), (void *)image, C18, DMA_BYTE1, COL, DADDR_KEEPON);
    port_init(C18, ALT1 | DMA_FALLING | PULLDOWN);  			//PCLK  触发源设置
    DMA_DIS(DMA_CH0);                                     		//禁用DMA通道
    DMA_IRQ_CLEAN(DMA_CH0);                               		//清除通道传输中断标志位
    DMA_IRQ_EN(DMA_CH0);                                  		//允许DMA通道中断
    DMA_EN(DMA_CH0);  											//使能DMA
	
    disable_irq(PORTC_IRQn);                             		//关闭PTC的中断
    //port_init(C7, ALT1 | IRQ_FALLING | PULLDOWN);      			//行中断
    port_init(C6, ALT1 | IRQ_FALLING | PULLDOWN);        		//场中断，上拉，下降沿触发中断，带滤波
    set_irq_priority(PORTC_IRQn,1);                             // 中断优先级
    enable_irq (PORTC_IRQn);
    EnableInterrupts;
}


uint16  now_row = 0;		  //当前正在采集行数
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头场中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void VSYNC(void)
{
    now_row = 0;
    dma_repeat(DMA_CH0,(void *)&C_IN_DATA(1),(void *)image[0],COL);
}



uint8   mt9v032_finish_flag = 0;      //一场图像采集完成标志位
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头DMA完成中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void row_finished(void)
{
    
    now_row++;
    if(now_row<ROW) 
        dma_repeat(DMA_CH0,(void *)&C_IN_DATA(1),(void *)image[now_row],COL);
    if(now_row >= ROW)	
        mt9v032_finish_flag = 1;
	
	//本例程对黑边不做处理，大家在使用数据的时候不使用image数组最左边与最右边即可，建议大家还是对数组做一次转存，因为避免在你使用设个数组的时候下一副来的时候，DMA也在操作这个数组
	//如果分辨率过大，就没办法转存了，因为K60的RAM会不够
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(void)
{
    uart_putchar(uart2,0x00);
    uart_putchar(uart2,0xff);
    uart_putchar(uart2,0x01);
    uart_putchar(uart2,0x01);//发送命令
    uart_putbuff(uart2, (uint8_t *)image, ROW*COL);  //发送图像
}
*/

//-------------------------------------------------------------------------------------------------------------------
//*****************************************我是分界线**************************************************
//----------------------------------------以上为MT9部分
//-------------------------------------------------------------------------------------------------------------------








///////寻找跳变点   区间查找
void GetJumpPointFromDet(uint8 *p, uint8 type, int L, int H, JumpPointtypedef *Q)
{
	int i = 0;
	if (type == 'L')
	{
		for (i = L; i <= H; i++)
		{
			if (*(p + i) == 0 && *(p + i + 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';         /////正确跳变
				break;
			}
			else if (i == (H - 1))
			{
				if (*(p + (L + H) / 2) != 0)
				{
					Q->point = (L + H) / 2;
					Q->type = 'W';      /////非正确跳变且中间为白，
					break;
				}
				else                 /////非正确跳变且中间为黑
				{
					Q->point = H;
					Q->type = 'H';
					break;
				}
			}
		}
	}
	else if (type == 'R')
	{
		for (i = H; i >= L; i--)
		{
			if (*(p + i) == 0 && *(p + i - 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';
				break;
			}
			else if (i == (L + 1))
			{
				if (*(p + (L + H) / 2) != 0)
				{
					Q->point = (L + H) / 2;
					Q->type = 'W';
					break;
				}
				else
				{
					Q->point = L;
					Q->type = 'H';
					break;
				}
			}
		}
	}
}


///////寻找跳变点   中心向两边查找
void GetJumpPointFromCenter(uint8 *p, JumpPointtypedef *L, JumpPointtypedef *R, int Center)
{
	int n = 0;
	for (n = Center; n <= 79; n++)
	{
		if (*(p + n) != 0 && *(p + n + 1) == 0)
		{
			R->point = n;
			R->type = 'T';
			break;
		}
		else if (n == 78)
		{
			R->point = n + 1;
			R->type = 'W';
			break;
		}
	}
	for (n = Center; n >= 1; n--)
	{
		if (*(p + n) != 0 && *(p + n - 1) == 0)
		{
			L->point = n - 1;
			L->type = 'T';
			break;
		}
		else if (n == 1)
		{
			L->point = n - 1;
			L->type = 'W';
			break;
		}
	}
}


//////单向
void GetJumpPointFromOther(uint8 *p, JumpPointtypedef *Q, uint8 type, int Start)
{
	int i = 0;
	if (type == 0)
	{
		for (i = (Start - 1); i >= 1; i--)
		{
			if (*(p + i) == 0 && *(p + i + 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';
				break;
			}
			else if (i == 1)
			{
				Q->point = i;
				Q->type = 'W';
				break;
			}
		}
	}
	else if (type == 1)
	{
		for (i = (Start + 1); i <= 78; i++)
		{
			if (*(p + i) == 0 && *(p + i - 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';
				break;
			}
			else if (i == 78)
			{
				Q->point = i;
				Q->type = 'W';
				break;
			}
		}
	}
}



ImageDealDatatypedef    ImageDeal[60];
ImageStatustypedef      ImageStatus;
static int       Ysite = 0, Xsite = 0;    ///////Y,X坐标
static uint8     *PicTemp;
static int       IntervalLow = 0, IntervalHigh = 0;   /////定义高低扫描区间
static int       ytemp = 0;
static int       TFSite = 0, FTSite = 0;
static float     DetR = 0, DetL = 0;
/************************/
/*从底边开始向远处5行寻找边线以及中线，采取最简单的方法*/
static     int      BottomBorderRight = 79, BottomBorderLeft = 0, BottomCenter = 0;

static uint8 DrawLinesFirst(void)
{
	PicTemp = PicForNow[59];        //图像最左下角的像素点
	if (*(PicTemp + ImageSensorMid) == 0)                           //如果底边图像中点为黑
	{
		for (Xsite = 0; Xsite < ImageSensorMid; Xsite++)
		{
			if (*(PicTemp + ImageSensorMid - Xsite) != 0)
				break;                                           //从中间分别向两边寻找跳变点，找到就退出for循环,退出后的（中点+-Xsite即为第一个跳变点横坐标
			if (*(PicTemp + ImageSensorMid + Xsite) != 0)
				break;
		}
		if (Xsite > (ImageSensorMid - 15))
            return 'F';
		if (*(PicTemp + ImageSensorMid - Xsite) != 0)
		{
			BottomBorderRight = ImageSensorMid - Xsite + 1;
			for (Xsite = BottomBorderRight; Xsite > 0; Xsite--)
			{
				if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) == 0)
				{
					BottomBorderLeft = Xsite;
					break;
				}
				else if (Xsite == 1)
				{
					BottomBorderLeft = 0;
					break;
				}
			}
		}
		else if (*(PicTemp + ImageSensorMid + Xsite) != 0)
		{
			BottomBorderLeft = ImageSensorMid + Xsite - 1;
			for (Xsite = BottomBorderLeft; Xsite < 79; Xsite++)
			{
				if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) == 0)
				{
					BottomBorderLeft = Xsite;
					break;
				}
				else if (Xsite == 78)
				{
					BottomBorderLeft = 79;
					break;
				}
			}
		}
	}
	else
	{
		for (Xsite = ImageSensorMid; Xsite < 79; Xsite++)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) == 0)
			{
				BottomBorderRight = Xsite;
				break;
			}
			else if (Xsite == 78)
			{
				BottomBorderRight = 79;
				break;
			}
		}
		for (Xsite = ImageSensorMid; Xsite > 0; Xsite--)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) == 0)
			{
				BottomBorderLeft = Xsite;
				break;
			}
			else if (Xsite == 1)
			{
				BottomBorderLeft = 0;
				break;
			}
		}
	}
	BottomCenter = (BottomBorderLeft + BottomBorderRight) / 2;
	ImageDeal[59].LeftBorder = BottomBorderLeft;
	ImageDeal[59].RightBorder = BottomBorderRight;
	ImageDeal[59].Center = BottomCenter;                          ////确定最底边
	ImageDeal[59].Wide = BottomBorderRight - BottomBorderLeft;   /////存储宽度信息
	ImageDeal[59].IsLeftFind = 'T';
	ImageDeal[59].IsRightFind = 'T';
	for (Ysite = 58; Ysite > 54; Ysite--)                         ////由中间向两边确定底边五行
	{
		PicTemp = PicForNow[Ysite];
		for (Xsite = ImageDeal[Ysite + 1].Center; Xsite < 79; Xsite++)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) == 0)
			{
				ImageDeal[Ysite].RightBorder = Xsite;
				break;
			}
			else if (Xsite == 78)
			{
				ImageDeal[Ysite].RightBorder = 79;
				break;
			}
		}
		for (Xsite = ImageDeal[Ysite + 1].Center; Xsite > 0; Xsite--)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) == 0)
			{
				ImageDeal[Ysite].LeftBorder = Xsite;
				break;
			}
			else if (Xsite == 1)
			{
				ImageDeal[Ysite].LeftBorder = 0;
				break;
			}
		}
		ImageDeal[Ysite].IsLeftFind = 'T';
		ImageDeal[Ysite].IsRightFind = 'T';
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;   //存储中点
		ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;         //存储宽度
	}
	return 'T';
}  /////最基本的要求，最近的五行首先不会受到干扰，这需要在安装的时候调整摄像头的视角


/*边线追逐大致得到全部边线*/
static void DrawLinesProcess(void)//////不用更改
{
	for (Ysite = 55; Ysite > ImageStatus.OFFLine; Ysite--)
	{
		PicTemp = PicForNow[Ysite];
		JumpPointtypedef JumpPoint[2];

		IntervalLow = ImageDeal[Ysite + 1].LeftBorder - ImageScanInterval;
		IntervalHigh = ImageDeal[Ysite + 1].LeftBorder + ImageScanInterval;

		LimitL(IntervalLow);
		LimitH(IntervalHigh);                       /////确定右扫描区间并进行限制
		GetJumpPointFromDet(PicTemp, 'L', IntervalLow, IntervalHigh, &JumpPoint[0]);



		IntervalLow = ImageDeal[Ysite + 1].RightBorder - ImageScanInterval;
		IntervalHigh = ImageDeal[Ysite + 1].RightBorder + ImageScanInterval;

		LimitL(IntervalLow);
		LimitH(IntervalHigh);                       /////确定右扫描区间并进行限制
		GetJumpPointFromDet(PicTemp, 'R', IntervalLow, IntervalHigh, &JumpPoint[1]);

		if (JumpPoint[0].type == 'W')
		{
            ImageDeal[Ysite].LeftBorder = ImageDeal[Ysite + 1].LeftBorder;
		}
		else
		{
			ImageDeal[Ysite].LeftBorder = JumpPoint[0].point;
		}

		if (JumpPoint[1].type == 'W')
		{
			ImageDeal[Ysite].RightBorder = ImageDeal[Ysite + 1].RightBorder;
		}
		else
		{
			ImageDeal[Ysite].RightBorder = JumpPoint[1].point;
		}

		ImageDeal[Ysite].IsLeftFind = JumpPoint[0].type;
		ImageDeal[Ysite].IsRightFind = JumpPoint[1].type;


		///////重新确定那些大跳变的边缘
		if ((ImageDeal[Ysite].IsLeftFind == 'H' || ImageDeal[Ysite].IsRightFind == 'H'))
		{
			if (ImageDeal[Ysite].IsLeftFind == 'H')
				for (Xsite = (ImageDeal[Ysite].LeftBorder + 1); Xsite <= (ImageDeal[Ysite].RightBorder - 1); Xsite++)
				{
					if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite + 1) != 0))
					{
						ImageDeal[Ysite].LeftBorder = Xsite;////如果左边的左边还有黑白跳变则为绝对边线直接取出
						ImageDeal[Ysite].IsLeftFind = 'T';
						break;
					}
					else if (*(PicTemp + Xsite) != 0)   ///一旦出现白点则直接跳出
						break;
					else if (Xsite == (ImageDeal[Ysite].RightBorder - 1))
					{
						ImageDeal[Ysite].LeftBorder = Xsite;////如果左边的左边还有黑白跳变则为绝对边线直接取出
						ImageDeal[Ysite].IsLeftFind = 'T';
						break;
					}
				}
			if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder) <= 5)///////图像宽度限定
			{
				ImageStatus.OFFLine = Ysite + 1;
				break;
			}
			if (ImageDeal[Ysite].IsRightFind == 'H')
				for (Xsite = (ImageDeal[Ysite].RightBorder - 1); Xsite >= (ImageDeal[Ysite].LeftBorder + 1); Xsite--)
				{
					if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite - 1) != 0))
					{
						ImageDeal[Ysite].RightBorder = Xsite;
						ImageDeal[Ysite].IsRightFind = 'T';
						break;
					}
					else if (*(PicTemp + Xsite) != 0)
						break;
					else if (Xsite == (ImageDeal[Ysite].LeftBorder + 1))
					{
						ImageDeal[Ysite].RightBorder = Xsite;
						ImageDeal[Ysite].IsRightFind = 'T';
						break;
					}
				}
		}

		/////此段用于将十字等折点消除，首先要满足不是圆环  
		if (Ysite <= 50)//////虽然不是很科学，但是挺好用的
		{
			if (ImageDeal[Ysite].IsLeftFind == 'T')
				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 4].LeftBorder) <= -4 && (ImageDeal[Ysite + 4].LeftBorder - ImageDeal[Ysite + 8].LeftBorder) >= 1)
				{
					for (ytemp = Ysite; ytemp <= (Ysite + 5); ytemp++)
					{
						if (ImageDeal[ytemp].IsRightFind == 'W')
						{
							for (ytemp = (Ysite + 4); ytemp >= Ysite; ytemp--)
							{
								ImageDeal[ytemp].LeftBorder = 2 * ImageDeal[ytemp + 1].LeftBorder - ImageDeal[ytemp + 2].LeftBorder;
								ImageDeal[ytemp].IsLeftFind = 'W';
							}
							ImageStatus.WhiteLine += 4;
							break;
						}
					}
				}

			if (ImageDeal[Ysite].IsRightFind == 'T')
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 4].RightBorder) >= 4 && (ImageDeal[Ysite + 4].RightBorder - ImageDeal[Ysite + 8].RightBorder) <= 1
					)
				{
					for (ytemp = Ysite; ytemp <= (Ysite + 5); ytemp++)
					{
						if (ImageDeal[ytemp].IsLeftFind == 'W')
						{
							for (ytemp = (Ysite + 4); ytemp >= Ysite; ytemp--)
							{
								ImageDeal[ytemp].RightBorder = 2 * ImageDeal[ytemp + 1].RightBorder - ImageDeal[ytemp + 2].RightBorder;
								ImageDeal[ytemp].IsRightFind = 'W';
							}
							ImageStatus.WhiteLine += 4;
							break;
						}
					}
				}
		}

		if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite].IsRightFind == 'W')
			ImageStatus.WhiteLine++;

		LimitL(ImageDeal[Ysite].LeftBorder);
		LimitH(ImageDeal[Ysite].LeftBorder);
		LimitL(ImageDeal[Ysite].RightBorder);
		LimitH(ImageDeal[Ysite].RightBorder);

		ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

		if (ImageDeal[Ysite].Wide <= 0)
		{
			ImageStatus.OFFLine = Ysite + 1;
			break;
		}
		else if (ImageDeal[Ysite].RightBorder <= 5 || ImageDeal[Ysite].LeftBorder >= 75)
		{
			ImageStatus.OFFLine = Ysite + 1;
			break;
		}//////当图像宽度小于0或者左右边达到一定的限制时，则终止巡边
	}



	if (ImageStatus.OFFLine <= 40 && ImageStatus.IsCinqueOutIn == 'F' && ImageStatus.CirquePass == 'F')
		for (Ysite = ImageStatus.OFFLine; Ysite <= (ImageStatus.OFFLine + 5); Ysite++)
		{/////////用于把弯道远端对边的误点删除，实用但是不科学
			if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite + 1].IsLeftFind == 'W'
				&& ImageDeal[Ysite].IsRightFind == 'T' && ImageDeal[Ysite + 1].IsRightFind == 'T')
			{
				while (Ysite >= (ImageStatus.OFFLine + 1))
				{
					Ysite--;
					if (ImageDeal[Ysite].IsRightFind != 'T')break;
					if (ImageDeal[Ysite].IsLeftFind == 'T' && (ImageDeal[Ysite].Wide <= 10 || Ysite <= (ImageStatus.OFFLine + 5)))
					{
						ImageDeal[Ysite].LeftBorder = ImageDeal[Ysite + 2].LeftBorder;
						ImageDeal[Ysite].IsLeftFind = 'W';
						ImageDeal[Ysite].RightBorder = 2 * ImageDeal[Ysite + 1].RightBorder - ImageDeal[Ysite + 2].RightBorder;
						LimitL(ImageDeal[Ysite].LeftBorder);
						LimitH(ImageDeal[Ysite].LeftBorder);
						LimitL(ImageDeal[Ysite].RightBorder);
						LimitH(ImageDeal[Ysite].RightBorder);

						ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;
						ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

						if (ImageDeal[Ysite].Wide <= 0)
						{
							ImageStatus.OFFLine = Ysite + 1;
							break;
						}
					}
				}
				break;
			}
			else if (ImageDeal[Ysite].IsRightFind == 'W' && ImageDeal[Ysite + 1].IsRightFind == 'W'
				&& ImageDeal[Ysite].IsLeftFind == 'T' && ImageDeal[Ysite + 1].IsLeftFind == 'T')
			{
				while (Ysite >= (ImageStatus.OFFLine + 1))
				{
					Ysite--;
					if (ImageDeal[Ysite].IsLeftFind != 'T')break;
					if (ImageDeal[Ysite].IsRightFind == 'T' && (ImageDeal[Ysite].Wide <= 10 || Ysite <= (ImageStatus.OFFLine + 5)))
					{
						ImageDeal[Ysite].RightBorder = ImageDeal[Ysite + 2].LeftBorder;
						ImageDeal[Ysite].IsRightFind = 'W';
						ImageDeal[Ysite].LeftBorder = 2 * ImageDeal[Ysite + 1].RightBorder - ImageDeal[Ysite + 2].RightBorder;
						LimitL(ImageDeal[Ysite].LeftBorder);
						LimitH(ImageDeal[Ysite].LeftBorder);
						LimitL(ImageDeal[Ysite].RightBorder);
						LimitH(ImageDeal[Ysite].RightBorder);

						ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;
						ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

						if (ImageDeal[Ysite].Wide <= 0)
						{
							ImageStatus.OFFLine = Ysite + 1;
							break;
						}
					}
				}
				break;
			}
		}
	return;
}

///////延长线绘制，理论上来说是很准确的
static void DrawExtensionLine(void)/////绘制延长线并重新确定中线 //////不用更改
{
    for (Ysite = 55; Ysite >= (ImageStatus.OFFLine + 2); Ysite--)
    {
        PicTemp = PicForNow[Ysite];
        if (ImageDeal[Ysite].IsLeftFind == 'W')
        {
            if (ImageDeal[Ysite + 1].LeftBorder >= 70)
            {
                ImageStatus.OFFLine = Ysite + 1;
                break;
			}
			while (Ysite >= (ImageStatus.OFFLine + 2))
			{
				Ysite--;
				if (ImageDeal[Ysite].IsLeftFind == 'T' && ImageDeal[Ysite - 1].IsLeftFind == 'T' && ImageDeal[Ysite - 2].IsLeftFind == 'T')
				{
					FTSite = Ysite - 2;
					break;
				}
			}
			DetL = ((float)(ImageDeal[FTSite].LeftBorder - ImageDeal[TFSite].LeftBorder)) / ((float)(FTSite - TFSite));
			for (ytemp = TFSite; ytemp >= FTSite; ytemp--)
			{
				ImageDeal[ytemp].LeftBorder = (int)(DetL * ((float)(ytemp - TFSite))) + ImageDeal[TFSite].LeftBorder;
			}
		}
		else
			TFSite = Ysite + 2;
	}
	for (Ysite = 55; Ysite >= (ImageStatus.OFFLine + 2); Ysite--)
	{
		if (ImageDeal[Ysite].IsRightFind == 'W')
		{
			if (ImageDeal[Ysite + 1].RightBorder <= 10)
			{
				ImageStatus.OFFLine = Ysite + 1;
				break;
			}
			while (Ysite >= (ImageStatus.OFFLine + 2))
			{
				Ysite--;
				if (ImageDeal[Ysite].IsRightFind == 'T' && ImageDeal[Ysite - 1].IsRightFind == 'T' && ImageDeal[Ysite - 2].IsRightFind == 'T')
				{
					FTSite = Ysite - 2;
					break;
				}
			}
			DetR = ((float)(ImageDeal[FTSite].RightBorder - ImageDeal[TFSite].RightBorder)) / ((float)(FTSite - TFSite));
			for (ytemp = TFSite; ytemp >= FTSite; ytemp--)
			{
				ImageDeal[ytemp].RightBorder = (int)(DetR * ((float)(ytemp - TFSite))) + ImageDeal[TFSite].RightBorder;
			}
		}
		else
			TFSite = Ysite + 2;
	}
	for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
	{
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder) / 2;
		ImageDeal[Ysite].Wide = -ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder;
	}
	return;
}


static void RouteFilter(void)
{
	for (Ysite = 58; Ysite >= (ImageStatus.OFFLine + 1); Ysite--)
    {
        if(ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite].IsRightFind == 'W' && Ysite <= 45)
        {
            ytemp = Ysite;
            while(ytemp >= (ImageStatus.OFFLine + 1))
            {
                ytemp--;
                if(ImageDeal[ytemp].IsLeftFind == 'T' && ImageDeal[ytemp].IsRightFind == 'T')
                {
                    DetR = (float)(ImageDeal[ytemp - 1].Center - ImageDeal[Ysite + 2].Center) / (float)(ytemp - 1 - Ysite - 2);
                    int CenterTemp = ImageDeal[Ysite + 2].Center;
                    int LineTemp = Ysite + 2;
                    while(Ysite >= ytemp)
                    {
                        ImageDeal[Ysite].Center = (int)(CenterTemp + DetR * (float)(Ysite - LineTemp));
                        Ysite--;
                    }
                    break;
                }
            }
        }
        ImageDeal[Ysite].Center = (ImageDeal[Ysite - 1].Center + ImageDeal[Ysite].Center + ImageDeal[Ysite + 1].Center) / 3;
	}
}




void ImageProcess(void)
{
    ImageStatus.OFFLine = 5;   ///////这个值根据真实距离得到，必须进行限制 
    ImageStatus.WhiteLine = 0;
    for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
    {
		ImageDeal[Ysite].IsLeftFind = 'F';
		ImageDeal[Ysite].IsRightFind = 'F';
		ImageDeal[Ysite].LeftBorder = 0;
		ImageDeal[Ysite].RightBorder = 79;
		ImageDeal[Ysite].LeftTemp = 0;
		ImageDeal[Ysite].RightTemp = 79;
    }/////////边界与标志位初始化
    UnZip((uint8 *)PicForNow, ImageBuff, sizeof(ImageBuff));   ///图像解压
    DrawLinesFirst();       ////绘制底边
    DrawLinesProcess();     ///得到基本边线
    DrawExtensionLine();    ///得到延长线
    RouteFilter();          ///路径滤波
    float Det = 0;
    if(ImageStatus.OFFLine > (TowPoint - 1))
    {
        Det = (ImageDeal[ImageStatus.OFFLine + 1].Center - 39) + (ImageDeal[ImageStatus.OFFLine + 2].Center - 39) + (ImageDeal[ImageStatus.OFFLine + 3].Center - 39);
        Det /= 3;
    }
    else
    {
        Det = (ImageDeal[TowPoint + 1].Center - 39) + (ImageDeal[TowPoint].Center - 39) + (ImageDeal[TowPoint - 1].Center - 39);
        Det /= 3;           //偏差？
    }
    SteerPID(Det);
}



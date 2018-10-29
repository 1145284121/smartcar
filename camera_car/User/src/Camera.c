#include "include.h"
#include "Camera.h"
#include "Motor.h"
#include "Steer.h"


/////////ͼ����//0��1��
uint8  ImageBuff[600] = {0};
uint8  PicForNow[60][80] = {0};
int   TowPoint = 29;
uint8   image[ROW][COL];      //ͼ������


//-------------------------------------------------------------------------------------------------------------------
//*****************************************���Ƿֽ���**************************************************
//----------------------------------------����ΪMT9����
//-------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ��ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
/*
void camera_init(void)
{
    //���ò���    ������ο�ʹ���ֲ�
    uint16 light;
    //����ͷ��������
    uint8 MT9V032_CFG[8];
	
    Common_delay(30);			//��ʱ�Ա�֤����ͷ�����51����ϵ��ʼ�����˴����޸Ĺ�
	
    uart_init (uart3, 115200);	//��ʼ������ ��������ͷ                     
    
    MT9V032_CFG[0] = 0xFF;     	//֡ͷ         

    //����λ
	//������ο�ʹ���ֲ�
    MT9V032_CFG[1] = 0x00;   
    
    //�ֱ���ѡ��λ   �ֱ����л�����öϵ�һ��ϵͳ
	//�����ֱ��ʾ��忴˵���飬�����г���188��K60�޷��ɼ����ṩ�ĸ���ķֱ����Ǹ�STM32��������ͷ�ӿڵ��û�
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
            
    
    //����ͼ��֡��        ������ͬ�����õķ�ΧҲ��ͬ		��Χ����  120�е�ʱ����1-200		240�е�ʱ����1-132		480�е�ʱ����1-66
    MT9V032_CFG[3] = 50;                    
    
    //�ع�ʱ��Խ��ͼ��Խ��     ��������ع�ʱ���ܵ�fps��ֱ��ʵĹ�ͬӰ�죬���ﲻ�������巶Χ������ֱ�Ӱ��ع�����Ϊ1000������ͷ�ϵ�51�յ�֮�����ݷֱ��ʼ�FPS��������ع�ʱ�䣬Ȼ���
	//�ع�ʱ������Ϊ����ع�ʱ�䣬���һ᷵�ص�ǰ���õ�����ع�ʱ�䣬�����Ϳ���֪�����������õ��ع�ʱ���Ƕ����ˣ�Ȼ�����ͼ��������Ϳ������������ع�ֵ�Ļ�������Ӧ���١�
    light = 600;
    MT9V032_CFG[4] = light>>8;          //�ع�ʱ��߰�λ   
    MT9V032_CFG[5] = (uint8)light;      //�ع�ʱ��Ͱ�λ  
    
    //����Ϊ0��ʾ�ر��Զ��ع⣬����1-63�������Զ��ع⣬���õ�Խ��ͼ���Խ��  ���鿪��������ܣ�������Ч��Ӧ���ֳ���
    MT9V032_CFG[6] = 30; 
    
    //֡β  
    MT9V032_CFG[7] = 0x5A;                
    //ͨ�����ڷ������ò���
    uart_putbuff(uart3,MT9V032_CFG,8);
	
	//��ʱ�Ա����ϸ���������51�Ѿ��ɹ�д�뵽����ͷ
    Common_delay(30);  //�˴��޸Ĺ�
		
	
	//����������������ͷ����   ��������ع�ʱ���ǲ�һ����
    MT9V032_CFG[0] = 0xFF;     	//֡ͷ
    MT9V032_CFG[1] = 0x02; 		//����λ  
    MT9V032_CFG[2] = 0; 		//���ã�����Ϊ0
    MT9V032_CFG[3] = 0;     	//���ã�����Ϊ0
    MT9V032_CFG[4] = 0;			//���ã�����Ϊ0
    MT9V032_CFG[5] = 32;      	//���ȵȼ�ѡ��            ���ȵȼ� 1 - 64
    MT9V032_CFG[6] = 0x35; 
    MT9V032_CFG[7] = 0x5A; 		//֡β 
    uart_putbuff(uart3,MT9V032_CFG,8);


    //����ͷ�ɼ���ʼ��
    DisableInterrupts;
    //DMAͨ��0��ʼ����PTE0����Դ(Ĭ��������)��Դ��ַΪC_IN_DATA(1)(PTC8-PTC15)��Ŀ�ĵ�ַΪ��image��ÿ�δ���1Byte ÿ�δ�����ϻָ�Ŀ�ĵ�ַ
    dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(1), (void *)image, C18, DMA_BYTE1, COL, DADDR_KEEPON);
    port_init(C18, ALT1 | DMA_FALLING | PULLDOWN);  			//PCLK  ����Դ����
    DMA_DIS(DMA_CH0);                                     		//����DMAͨ��
    DMA_IRQ_CLEAN(DMA_CH0);                               		//���ͨ�������жϱ�־λ
    DMA_IRQ_EN(DMA_CH0);                                  		//����DMAͨ���ж�
    DMA_EN(DMA_CH0);  											//ʹ��DMA
	
    disable_irq(PORTC_IRQn);                             		//�ر�PTC���ж�
    //port_init(C7, ALT1 | IRQ_FALLING | PULLDOWN);      			//���ж�
    port_init(C6, ALT1 | IRQ_FALLING | PULLDOWN);        		//���жϣ��������½��ش����жϣ����˲�
    set_irq_priority(PORTC_IRQn,1);                             // �ж����ȼ�
    enable_irq (PORTC_IRQn);
    EnableInterrupts;
}


uint16  now_row = 0;		  //��ǰ���ڲɼ�����
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ���ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void VSYNC(void)
{
    now_row = 0;
    dma_repeat(DMA_CH0,(void *)&C_IN_DATA(1),(void *)image[0],COL);
}



uint8   mt9v032_finish_flag = 0;      //һ��ͼ��ɼ���ɱ�־λ
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷDMA����ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void row_finished(void)
{
    
    now_row++;
    if(now_row<ROW) 
        dma_repeat(DMA_CH0,(void *)&C_IN_DATA(1),(void *)image[now_row],COL);
    if(now_row >= ROW)	
        mt9v032_finish_flag = 1;
	
	//�����̶Ժڱ߲������������ʹ�����ݵ�ʱ��ʹ��image��������������ұ߼��ɣ������һ��Ƕ�������һ��ת�棬��Ϊ��������ʹ����������ʱ����һ������ʱ��DMAҲ�ڲ����������
	//����ֱ��ʹ��󣬾�û�취ת���ˣ���ΪK60��RAM�᲻��
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ͷͼ��������λ���鿴ͼ��
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(void)
{
    uart_putchar(uart2,0x00);
    uart_putchar(uart2,0xff);
    uart_putchar(uart2,0x01);
    uart_putchar(uart2,0x01);//��������
    uart_putbuff(uart2, (uint8_t *)image, ROW*COL);  //����ͼ��
}
*/

//-------------------------------------------------------------------------------------------------------------------
//*****************************************���Ƿֽ���**************************************************
//----------------------------------------����ΪMT9����
//-------------------------------------------------------------------------------------------------------------------








///////Ѱ�������   �������
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
				Q->type = 'T';         /////��ȷ����
				break;
			}
			else if (i == (H - 1))
			{
				if (*(p + (L + H) / 2) != 0)
				{
					Q->point = (L + H) / 2;
					Q->type = 'W';      /////����ȷ�������м�Ϊ�ף�
					break;
				}
				else                 /////����ȷ�������м�Ϊ��
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


///////Ѱ�������   ���������߲���
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


//////����
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
static int       Ysite = 0, Xsite = 0;    ///////Y,X����
static uint8     *PicTemp;
static int       IntervalLow = 0, IntervalHigh = 0;   /////����ߵ�ɨ������
static int       ytemp = 0;
static int       TFSite = 0, FTSite = 0;
static float     DetR = 0, DetL = 0;
/************************/
/*�ӵױ߿�ʼ��Զ��5��Ѱ�ұ����Լ����ߣ���ȡ��򵥵ķ���*/
static     int      BottomBorderRight = 79, BottomBorderLeft = 0, BottomCenter = 0;

static uint8 DrawLinesFirst(void)
{
	PicTemp = PicForNow[59];        //ͼ�������½ǵ����ص�
	if (*(PicTemp + ImageSensorMid) == 0)                           //����ױ�ͼ���е�Ϊ��
	{
		for (Xsite = 0; Xsite < ImageSensorMid; Xsite++)
		{
			if (*(PicTemp + ImageSensorMid - Xsite) != 0)
				break;                                           //���м�ֱ�������Ѱ������㣬�ҵ����˳�forѭ��,�˳���ģ��е�+-Xsite��Ϊ��һ������������
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
	ImageDeal[59].Center = BottomCenter;                          ////ȷ����ױ�
	ImageDeal[59].Wide = BottomBorderRight - BottomBorderLeft;   /////�洢�����Ϣ
	ImageDeal[59].IsLeftFind = 'T';
	ImageDeal[59].IsRightFind = 'T';
	for (Ysite = 58; Ysite > 54; Ysite--)                         ////���м�������ȷ���ױ�����
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
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;   //�洢�е�
		ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;         //�洢���
	}
	return 'T';
}  /////�������Ҫ��������������Ȳ����ܵ����ţ�����Ҫ�ڰ�װ��ʱ���������ͷ���ӽ�


/*����׷����µõ�ȫ������*/
static void DrawLinesProcess(void)//////���ø���
{
	for (Ysite = 55; Ysite > ImageStatus.OFFLine; Ysite--)
	{
		PicTemp = PicForNow[Ysite];
		JumpPointtypedef JumpPoint[2];

		IntervalLow = ImageDeal[Ysite + 1].LeftBorder - ImageScanInterval;
		IntervalHigh = ImageDeal[Ysite + 1].LeftBorder + ImageScanInterval;

		LimitL(IntervalLow);
		LimitH(IntervalHigh);                       /////ȷ����ɨ�����䲢��������
		GetJumpPointFromDet(PicTemp, 'L', IntervalLow, IntervalHigh, &JumpPoint[0]);



		IntervalLow = ImageDeal[Ysite + 1].RightBorder - ImageScanInterval;
		IntervalHigh = ImageDeal[Ysite + 1].RightBorder + ImageScanInterval;

		LimitL(IntervalLow);
		LimitH(IntervalHigh);                       /////ȷ����ɨ�����䲢��������
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


		///////����ȷ����Щ������ı�Ե
		if ((ImageDeal[Ysite].IsLeftFind == 'H' || ImageDeal[Ysite].IsRightFind == 'H'))
		{
			if (ImageDeal[Ysite].IsLeftFind == 'H')
				for (Xsite = (ImageDeal[Ysite].LeftBorder + 1); Xsite <= (ImageDeal[Ysite].RightBorder - 1); Xsite++)
				{
					if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite + 1) != 0))
					{
						ImageDeal[Ysite].LeftBorder = Xsite;////�����ߵ���߻��кڰ�������Ϊ���Ա���ֱ��ȡ��
						ImageDeal[Ysite].IsLeftFind = 'T';
						break;
					}
					else if (*(PicTemp + Xsite) != 0)   ///һ�����ְ׵���ֱ������
						break;
					else if (Xsite == (ImageDeal[Ysite].RightBorder - 1))
					{
						ImageDeal[Ysite].LeftBorder = Xsite;////�����ߵ���߻��кڰ�������Ϊ���Ա���ֱ��ȡ��
						ImageDeal[Ysite].IsLeftFind = 'T';
						break;
					}
				}
			if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder) <= 5)///////ͼ�����޶�
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

		/////�˶����ڽ�ʮ�ֵ��۵�����������Ҫ���㲻��Բ��  
		if (Ysite <= 50)//////��Ȼ���Ǻܿ�ѧ������ͦ���õ�
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
		}//////��ͼ����С��0�������ұߴﵽһ��������ʱ������ֹѲ��
	}



	if (ImageStatus.OFFLine <= 40 && ImageStatus.IsCinqueOutIn == 'F' && ImageStatus.CirquePass == 'F')
		for (Ysite = ImageStatus.OFFLine; Ysite <= (ImageStatus.OFFLine + 5); Ysite++)
		{/////////���ڰ����Զ�˶Աߵ����ɾ����ʵ�õ��ǲ���ѧ
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

///////�ӳ��߻��ƣ���������˵�Ǻ�׼ȷ��
static void DrawExtensionLine(void)/////�����ӳ��߲�����ȷ������ //////���ø���
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
    ImageStatus.OFFLine = 5;   ///////���ֵ������ʵ����õ�������������� 
    ImageStatus.WhiteLine = 0;
    for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
    {
		ImageDeal[Ysite].IsLeftFind = 'F';
		ImageDeal[Ysite].IsRightFind = 'F';
		ImageDeal[Ysite].LeftBorder = 0;
		ImageDeal[Ysite].RightBorder = 79;
		ImageDeal[Ysite].LeftTemp = 0;
		ImageDeal[Ysite].RightTemp = 79;
    }/////////�߽����־λ��ʼ��
    UnZip((uint8 *)PicForNow, ImageBuff, sizeof(ImageBuff));   ///ͼ���ѹ
    DrawLinesFirst();       ////���Ƶױ�
    DrawLinesProcess();     ///�õ���������
    DrawExtensionLine();    ///�õ��ӳ���
    RouteFilter();          ///·���˲�
    float Det = 0;
    if(ImageStatus.OFFLine > (TowPoint - 1))
    {
        Det = (ImageDeal[ImageStatus.OFFLine + 1].Center - 39) + (ImageDeal[ImageStatus.OFFLine + 2].Center - 39) + (ImageDeal[ImageStatus.OFFLine + 3].Center - 39);
        Det /= 3;
    }
    else
    {
        Det = (ImageDeal[TowPoint + 1].Center - 39) + (ImageDeal[TowPoint].Center - 39) + (ImageDeal[TowPoint - 1].Center - 39);
        Det /= 3;           //ƫ�
    }
    SteerPID(Det);
}



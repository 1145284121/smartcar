#include "ImageSensorDriver.h"



Registermaptypedef OV7725Registermap[] = {
	//寄存器，寄存器值
   {OV7725_COM4         , 0xC1},               // 1100 0001        6x  全窗曝光最高帧率150HZ
   {OV7725_CLKRC        , 0x00},
   {OV7725_COM2         , 0x03},
   {OV7725_COM3         , 0xD0},
   {OV7725_COM7         , 0x40},
   {OV7725_COM8         , 0xCF},               //自适应曝光控制   1100 1111  详细见DATASHEET
   {OV7725_COM5         , 0x79},                //16:2 自动曝光参数控制
   //{OV7725_AEC         , 0xD0},               //定曝光值设定15:0
   {OV7725_HSTART       , 0x3F},
   {OV7725_HSIZE        , 0x50},
   {OV7725_VSTRT        , 0x03},
   {OV7725_VSIZE        , 0x78},
   {OV7725_HREF         , 0x00},
   {OV7725_SCAL0        , 0x0A},
   {OV7725_AWB_Ctrl0    , 0xE0},
   {OV7725_DSPAuto      , 0xff},
   {OV7725_DSP_Ctrl2    , 0x0C},
   {OV7725_DSP_Ctrl3    , 0x00},
   {OV7725_DSP_Ctrl4    , 0x00},
   {OV7725_HOutSize     , 0x14},
   {OV7725_VOutSize     , 0x1E},
   {OV7725_EXHCH        , 0x00},
   {OV7725_GAM1         , 0x0c},
   {OV7725_GAM2         , 0x16},
   {OV7725_GAM3         , 0x2a},
   {OV7725_GAM4         , 0x4e},
   {OV7725_GAM5         , 0x61},
   {OV7725_GAM6         , 0x6f},
   {OV7725_GAM7         , 0x7b},
   {OV7725_GAM8         , 0x86},
   {OV7725_GAM9         , 0x8e},
   {OV7725_GAM10        , 0x97},
   {OV7725_GAM11        , 0xa4},
   {OV7725_GAM12        , 0xaf},
   {OV7725_GAM13        , 0xc5},
   {OV7725_GAM14        , 0xd7},
   {OV7725_GAM15        , 0xe8},
   {OV7725_SLOP         , 0x20},
   {OV7725_LC_RADI      , 0x00},
   {OV7725_LC_COEF      , 0x13},
   {OV7725_LC_XC        , 0x08},
   {OV7725_LC_COEFB     , 0x14},
   {OV7725_LC_COEFR     , 0x17},
   {OV7725_LC_CTR       , 0x05},
   {OV7725_BDBase       , 0x99},
   {OV7725_BDMStep      , 0x03},
   {OV7725_SDE          , 0x04},
   {OV7725_BRIGHT       , 0x00},
   {OV7725_CNST         , 0x49},     //阈值
   {OV7725_SIGN         , 0x06},
   {OV7725_UVADJ0       , 0x11},
   {OV7725_UVADJ1       , 0x02},
};



static uint8 ImageSensorRegister_init()
{
    int wait = 0;
    SCCB_init();

	wait = 10;
#if (USINGCAMERA == 0)
	while ('F' == SCCB_sendbyteU8(OV7725ADDR, OV7725_COM7, 0x80) && wait > 0)  //Restart
	{
		wait--;
	}
	if (wait == 0)
        return 'F';

	for (uint8 n = 0; n < ARRAY_SIZE(OV7725Registermap); n++)
	{
		if ('F' == SCCB_sendbyteU8(OV7725ADDR, OV7725Registermap[n].addr, OV7725Registermap[n].val))
		{
			return 'F';
		}
	}
#elif (USINGCAMERA == 1)
	return 'F';
#else
	return 'F';
#endif

	return 'T';
}



uint8 *DMAaddr;
void ImagePCLK_IRQHandler(void);
void ImageVSY_IRQHandler(void);
void ImageSensor_init(uint8 *imgaddr, uint16 imgsize)
{
    int wait = 10;
    DMAaddr = imgaddr;
    while ('F' == ImageSensorRegister_init() && wait > 0)
    {
        wait--;
    }
    if (wait == 0)                  //重启吧，摄像头都启动不了还玩什么呢<<<<<<<<
    {
        NVIC_SystemReset();  //System Restart
    }
    ////////摄像头初始化
    /**********************************************/
    /*端口初始化*/
    /**********************************************/
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;            //开启B端口时钟
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;            //开启A端口时钟
    for (uint8 i = 0; i < 8; i++)         //使用大端口的首位8个管脚，其实可以改为任意的连续八个
    {
        PORT_ISFR_REG(ImgDATA_port) = (1 << i);
        ImgDATA_port->PCR[i] = PORT_PCR_MUX(1) | (PULLDOWN);       //数据端口下拉，这样默认就为低电平 
    
        ImgDATA_GPIO_port->PDIR &= ~(1 << i);
    }
    PORT_ISFR_REG(ImgPCLK_port) = (1 << ImgPCLK_pin);
    ImgPCLK_port->PCR[ImgPCLK_pin] = PORT_PCR_MUX(1) | (DMA_FALLING) | (PULLUP);//下拉
    ImgPCLK_GPIO_port->PDIR &= ~(1 << ImgPCLK_pin);                      //设置为输入
    PORT_ISFR_REG(ImgVSY_port) = (1 << ImgVSY_pin);
    ImgVSY_port->PCR[ImgVSY_pin] = PORT_PCR_MUX(1) | (IRQ_RISING) | (PULLDOWN);//下拉
    ImgVSY_GPIO_port->PDIR &= ~(1 << ImgVSY_pin);         //下降沿上升沿都可以，频率150HZ   (0x0A << PORT_PCR_IRQC_SHIFT)  //下降沿
														 //(0x09 << PORT_PCR_IRQC_SHIFT)    上升沿
  /**********************************************/

    //共16路DMA通道使用0号通道，DMA0通道的优先级最高

    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;              //开启DMA时钟
#if (defined(MK60D10) || defined(MK60DZ10) || defined(MK66F18))
        SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟
#elif defined(MK60F15)
        SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;                    //打开DMA多路复用器时钟
#endif
    DMAMUX_CHCFG0 = 0x00;
    DMAMUX_CHCFG0 &= ~DMAMUX_CHCFG_ENBL_MASK;
    DMAMUX_CHCFG0 &= ~DMAMUX_CHCFG_TRIG_MASK;



    DMA_BASE_PTR->TCD[0].SADDR = (uint32)((void *)&(ImgDATA_GPIO_port->PDIR));  //选择源地址
    DMA_BASE_PTR->TCD[0].SOFF = 0x00u;
    DMA_BASE_PTR->TCD[0].DADDR = (uint32)((void *)DMAaddr); //设置目标地址
    DMA_BASE_PTR->TCD[0].DOFF = 1;              //目的地址偏移1个字节
    DMA_BASE_PTR->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(imgsize);
    DMA_BASE_PTR->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(imgsize);
    DMA_BASE_PTR->TCD[0].CSR = DMA_CSR_BWC(3) | DMA_CSR_DREQ_MASK | DMA_CSR_INTMAJOR_MASK;   //延时 传送后产生中断并停止DMA传输
    DMA_BASE_PTR->TCD[0].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(1);          //每次传输1字节
    DMA_BASE_PTR->TCD[0].ATTR = DMA_ATTR_SMOD(0x0) | DMA_ATTR_SMOD(0x0) | DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);  //数据格式声明
    DMA_BASE_PTR->TCD[0].SLAST = 0;              //传输后源头无偏移
    DMA_BASE_PTR->TCD[0].DLAST_SGA = 0;          //传输后目的地址偏移至数首指针
    DMA_CR &= ~DMA_CR_EMLM_MASK;


        
#if (defined(MK60D10) || defined(MK60DZ10) || defined(MK66F18))
        DMAMUX_BASE_PTR->CHCFG[0] = (0
#elif (defined(MK60F15))
        DMAMUX0_BASE_PTR->CHCFG[0] = (0
#endif
                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                            | DMAMUX_CHCFG_SOURCE(DMA_PORTA)  // 通道触发传输源:    
                          );

	DMA_ERQ &= ~(DMA_ERQ_ERQ0_MASK << 0);
	DMA_INT |= (DMA_INT_INT0_MASK << 0);       //清中断标志位

	set_vector_handler(DMA0_VECTORn, ImagePCLK_IRQHandler);
	set_vector_handler(PORTA_VECTORn, ImageVSY_IRQHandler);
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_SetPriority(PORTA_IRQn, 1);
	NVIC_EnableIRQ(DMA0_IRQn);
	NVIC_DisableIRQ(PORTA_IRQn);               //以上代码无BUG，采集时序待修改
}

                                     
                                
//////场中断
void ImageVSY_IRQHandler(void)
{
	if (ImgVSY_port->ISFR &  (1 << ImgVSY_pin))
	{
		ImgVSY_port->ISFR = 1 << ImgVSY_pin;
		if (1 == ((ImgVSY_GPIO_port->PDIR >> ImgVSY_pin) & 1))
		{
			NVIC_DisableIRQ(PORTA_IRQn);   ///////       
			NVIC_SetPriority(DMA0_IRQn, 1);   ////中断升为最高级
			DMA_INT |= (DMA_INT_INT0_MASK << 0);
			ImgPCLK_port->ISFR |= 1 << ImgPCLK_pin;
			DMA_ERQ |= (DMA_ERQ_ERQ0_MASK << 0);
			DMA_BASE_PTR->TCD[0].DADDR = (uint32)DMAaddr;
			ImgPCLK_port->ISFR |= 1 << ImgPCLK_pin;
		}
	}
}


///////像素中断

extern uint8 ImageSensorTestChoose;
extern volatile uint8 ImageSensorOK;
extern void ImageProcess();
void ImagePCLK_IRQHandler(void)
{  //////采集到此需要6.33ms                  ////DMA中断次级
	DMA_INT |= (DMA_INT_INT0_MASK << 0);

	NVIC_SetPriority(DMA0_IRQn, 2);            ////中断降级

    ImageProcess();
        
	ImgVSY_port->ISFR = 1 << ImgVSY_pin;

	if(ImageSensorTestChoose == 0)
    {
        NVIC_EnableIRQ(PORTA_IRQn);
    }
    else
    {
        ImageSensorOK = 1;
    }
}



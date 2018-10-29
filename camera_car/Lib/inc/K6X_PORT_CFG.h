#ifndef K6X_PORT_CFG_H
#define K6X_PORT_CFG_H

typedef enum
{
    //中断方式和DMA请求方式，两者只能选其中一种（可以不选）
    //中断方式选择
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //低电平触发
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //跳变沿触发
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //高电平触发

    //DMA请求选择
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //跳变沿触发


    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //输出高驱动能力
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //漏极输出
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //带无源滤波器
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //输出慢变化率          Slow slew rate

    //下拉上拉选择
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉

    //功能复用选择(如果不需要改变功能复用选择，保留原先的功能复用，直接选择ALT0 )
    //需要查 K60 Signal Multiplexing and Pin Assignments
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
}PORT_CFG;


typedef enum // 枚举管脚编号 
{
    //  A端口     //0~31
    A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,  A8,  A9,  A10, A11, A12, A13, A14, A15,
    A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31,

    //  B端口     //32~63
    B0,  B1,  B2,  B3,  B4,  B5,  B6,  B7,  B8,  B9,  B10, B11, B12, B13, B14, B15,
    B16, B17, B18, B19, B20, B21, B22, B23, B24, B25, B26, B27, B28, B29, B30, B31,

    //  C端口     //64~95
    C0,  C1,  C2,  C3,  C4,  C5,  C6,  C7,  C8,  C9,  C10, C11, C12, C13, C14, C15,
    C16, C17, C18, C19, C20, C21, C22, C23, C24, C25, C26, C27, C28, C29, C30, C31,

    //  D端口     //96~127
    D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,  D8,  D9,  D10, D11, D12, D13, D14, D15,
    D16, D17, D18, D19, D20, D21, D22, D23, D24, D25, D26, D27, D28, D29, D30, D31,

    //  E端口     //128~159
    E0,  E1,  E2,  E3,  E4,  E5,  E6,  E7,  E8,  E9,  E10, E11, E12, E13, E14, E15,
    E16, E17, E18, E19, E20, E21, E22, E23, E24, E25, E26, E27, E28, E29, E30, E31,
} PTX_n;


typedef enum //枚举端口状态
{
    GPI = 0, //定义管脚输入方向      
    GPO = 1, //定义管脚输出方向
}GPIO_DDR;

#define PTX(PTX_n)  (PTX_n>>5)  //获取模块号 PTX_n/32
#define PTn(PTX_n)  (PTX_n&0x1f)//获取引脚号 PTX_n%32


/**********************************  UART   ***************************************/

//      模块通道    端口          可选范围                          建议
#define UART0_RX_PIN    A15        //PTA1、PTA15、PTB16、PTD6          PTA1不要用（与Jtag冲突）
#define UART0_TX_PIN    A14        //PTA2、PTA14、PTB17、PTD7          PTA2不要用（与Jtag冲突）

#define UART1_RX_PIN    C3        //PTC3、PTE1
#define UART1_TX_PIN    C4        //PTC4、PTE0

#define UART2_RX_PIN    D2        //PTD2
#define UART2_TX_PIN    D3        //PTD3

#define UART3_RX_PIN    B10       //PTB10、PTC16、PTE5
#define UART3_TX_PIN    B11      //PTB11、PTC17、PTE4

#define UART4_RX_PIN    C14       //PTC14、PTE25
#define UART4_TX_PIN    C15       //PTC15、PTE24

#define UART5_RX_PIN    E9        //PTD8、PTE9
#define UART5_TX_PIN    E8        //PTD9、PTE8


//-----------------------------------  FTM  -----------------------------------

//      模块通道        端口      可选范围                建议
#define FTM0_CH0_PIN    C1        //C1、A3                A3不要用（与Jtag、SWD冲突）
#define FTM0_CH1_PIN    C2        //C2、A4    
#define FTM0_CH2_PIN    C3        //C3、A5    
#define FTM0_CH3_PIN    C4        //C4、A6    
#define FTM0_CH4_PIN    D4        //D4、A7    
#define FTM0_CH5_PIN    D5        //D5、A0                A0不要用（与Jtag、SWD冲突）
#define FTM0_CH6_PIN    D6        //D6、A1                A1不要用（与Jtag冲突）
#define FTM0_CH7_PIN    D7        //D7、A2                A2不要用（与Jtag冲突）


//      模块通道        端口      可选范围                建议
#define FTM1_CH0_PIN    A12       //A8、A12、B0
#define FTM1_CH1_PIN    A13       //A9、A13、B1

//      模块通道        端口      可选范围                建议
#define FTM2_CH0_PIN    B18       //A10、B18
#define FTM2_CH1_PIN    B19       //A11、B19


#if (defined(MK60F15) || defined(MK66F18))

#define FTM3_CH0_PIN    E5        //PTE5                   PTD0
#define FTM3_CH1_PIN    E6        //PTE6                   PTD1
#define FTM3_CH2_PIN    E7        //PTE7                   PTD2
#define FTM3_CH3_PIN    E8        //PTE8                   PTD3
#define FTM3_CH4_PIN    E9        //PTE9       PTC8
#define FTM3_CH5_PIN    E10       //PTE10      PTC9
#define FTM3_CH6_PIN    E11       //PTE11      PTC10
#define FTM3_CH7_PIN    E12       //PTE12      PTC11

#endif

//正交解码模块通道      端口      可选范围                建议
#define FTM1_QDPHA_PIN  A12       //A8、A12、B0
#define FTM1_QDPHB_PIN  A13       //A9、A13、B1

#define FTM2_QDPHA_PIN  B18       //A10、B18
#define FTM2_QDPHB_PIN  B19       //A11、B19


#endif
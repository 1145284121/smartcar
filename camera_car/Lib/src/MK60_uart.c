/***************************************/
/********内容原引自逐飞科技**************/
/***************************************/
/*更改于2017.11.8  By Alex*/


#include "MK60_uart.h"



UART_MemMapPtr UARTN[5] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR}; //定义五个指针数组保存 UARTN 的地址

void uart_init(UARTn uartn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint32 uartsclk;     //时钟
    //进行管脚复用设置
    switch(uartn)
    {
        case uart0:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UART0 时钟
            if(UART0_RX_PIN == A1)                                                             
            {
              PORT_init( UART0_RX_PIN, ALT2);
            }
            else if((UART0_RX_PIN == A15) || (UART0_RX_PIN == B16) || (UART0_RX_PIN == D6) )    
            {
              PORT_init( UART0_RX_PIN, ALT3);
            }

            if(UART0_TX_PIN == A2)    
            {
              PORT_init( UART0_TX_PIN, ALT2);
            }
            else if((UART0_TX_PIN == A14) || (UART0_TX_PIN == B17) || (UART0_TX_PIN == D7) )   
            {
              PORT_init( UART0_TX_PIN, ALT3);
            }
        }break;
        
        case uart1:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;      //使能 UART1 时钟
            if((UART1_RX_PIN == C3) || (UART1_RX_PIN == E1))       
            {
              PORT_init( UART1_RX_PIN, ALT3);
            }
            if((UART1_TX_PIN == C4) || (UART1_TX_PIN == E0))           
            {
              PORT_init( UART1_TX_PIN, ALT3);
            }
        }break;
        
        case uart2:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;      //使能 UART2 时钟
            if(UART2_TX_PIN == D3)           
            {
              PORT_init( UART2_TX_PIN, ALT3);
            }
            if(UART2_RX_PIN == D2)      
            {
              PORT_init( UART2_RX_PIN, ALT3);
            }
        }break;
        
        case uart3:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;      //使能 UART3 时钟
            if((UART3_RX_PIN == B10) || (UART3_RX_PIN == C16) || (UART3_RX_PIN == E5) )   
            {
              PORT_init( UART3_RX_PIN, ALT3);
            }
            if((UART3_TX_PIN == B11) || (UART3_TX_PIN == C17) || (UART3_TX_PIN == E4) ) 
            {
              PORT_init( UART3_TX_PIN, ALT3);
            }
        }break;
        
        case uart4:
        {
            SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;      //使能 UART4 时钟
            if((UART4_RX_PIN == C14) || (UART4_RX_PIN == E25)  )        
            {
              PORT_init( UART4_RX_PIN, ALT3);
            }
            if((UART4_TX_PIN == C15) || (UART4_TX_PIN == E24)  )       
            {
              PORT_init( UART4_TX_PIN, ALT3);
            }
        }break;
        default:
        return;
    }
    
    //设置的时候，应该禁止发送和接收
    UARTN[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);
    
    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    UARTN[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                        //| UART_C1_PE_MASK                     //奇偶校验使能（注释了表示禁用）
                        //| UART_C1_PT_MASK                     //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                       );
    
    //计算波特率，串口0、1使用内核时钟，其它串口使用bus时钟
    if ((uartn == uart0) || (uartn == uart1)) uartsclk = SysClock.core_khz * 1000;  //内核时钟
    else                                            uartsclk = SysClock.bus_khz * 1000;   //bus时钟

    //UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] + BRFA/32))
    //不考虑 BRFA 的情况下， SBR = UART 模块时钟 / (16 * UART 波特率)
    sbr = (uint16)(uartsclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR 是 13bit，最大为 0x1FFF

    //已知 SBR ，则 BRFA = UART 模块时钟 * 2/ UART 波特率 - 32 ×SBR[12:0]
    brfa = (uartsclk * 2 / baud)  - (sbr * 32);
    if(brfa>0x1F)   brfa = 0x1F;
    
    //写 SBR
    UARTN[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //清除原来波特率
    UARTN[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //先写入SBR高位
    UARTN[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //再写入SBR低位

    //写 BRFD
    UARTN[uartn]->C4 &= ~UART_C4_BRFA_MASK;
    UARTN[uartn]->C4 |= UART_C4_BRFA(brfa);
    
    //设置FIFO(FIFO的深度是由硬件决定的，软件不能设置)
    UARTN[uartn]->PFIFO |= (0
                           | UART_PFIFO_TXFE_MASK               //使能TX FIFO(注释表示禁止)
                           //| UART_PFIFO_TXFIFOSIZE(0)         //（只读）TX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                           | UART_PFIFO_RXFE_MASK               //使能RX FIFO(注释表示禁止)
                           //| UART_PFIFO_RXFIFOSIZE(0)         //（只读）RX FIFO 大小，0为1字节，1~6为 2^(n+1)字节
                           );
    UARTN[uartn]->CFIFO |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);
    
    UARTN[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //发送使能
                        | UART_C2_RE_MASK                       //接收使能
                        //| UART_C2_TIE_MASK                    //发送中断或DMA传输请求使能（注释了表示禁用）
                        //| UART_C2_TCIE_MASK                   //发送完成中断使能（注释了表示禁用）
                        //| UART_C2_RIE_MASK                    //接收满中断或DMA传输请求使能（注释了表示禁用）
                        );
}

void uart_putchar(UARTn uartn, uint8 ch)
{
  while(!((UARTN[uartn]->S1) & UART_S1_TDRE_MASK) );  //等待发送缓冲区空
  UARTN[uartn]->D = (uint8)ch;                         //发送数
}

void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uartn, *buff);
        buff++;
    }
}

void uart_putstr (UARTn uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

void uart_getchar (UARTn uartn, uint8 *ch) 
{
    while(!(UARTN[uartn]->S1 & UART_S1_RDRF_MASK));  //等待接收满了   
    *ch =  UARTN[uartn]->D;                          // 获取接收到的8位数据
}

char uart_querychar (UARTn uartn, uint8 *ch)
{
    if( UARTN[uartn]->RCFIFO )         //查询是否接受到数据
    {
        *ch  =   UARTN[uartn]->D;      //接受到8位的数据
        return  1;                    //返回 1 表示接收成功
    }
    return 0;                         //返回0表示接收失败
}

int fputc(int ch, FILE *f)            //printf重定向
{
  uart_putchar(uart0, (uint8)ch);
  return ch;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_en(UARTn uartn)
{
    UARTN[uartn]->C2 |= UART_C2_RIE_MASK;                                        //使能UART接收中断
    NVIC_EnableIRQ((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));                   //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_en(UARTn uartn)
{
    UARTN[uartn]->C2 |= UART_C2_TCIE_MASK;                                       //使能UART发送完成中断
    NVIC_EnableIRQ((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));                   //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_dis(UARTn uartn)
{
    UARTN[uartn]->C2 &= ~UART_C2_RIE_MASK;                                       //禁止UART接收中断

    //如果发送中断还没有关，则不关闭IRQ
    if(!(UARTN[uartn]->C2 & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        NVIC_DisableIRQ((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));              //关IRQ中断
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_dis(UARTn uartn)
{
    UARTN[uartn]->C2 &= ~UART_C2_TCIE_MASK;                                      //禁止UART发送完成中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(UARTN[uartn]->C2 & UART_C2_RIE_MASK) )
    {
        NVIC_DisableIRQ((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));              //关IRQ中断
    }
}
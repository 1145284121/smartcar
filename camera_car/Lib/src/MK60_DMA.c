/***************************************/
/********内容原引自逐飞科技**************/
/***************************************/
/*更改于2017.11.8  By Alex*/

#include "MK60_DMA.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA初始化，由IO口请求传输输入端口的数据到内存
//  @param      DMA_CHn         通道号（DMA_CH0 ~ DMA_CH15）
//  @param      SADDR           源地址( (void * )&X_IN_DATA
//  @param      DADDR           目的地址
//  @param      PTxn            触发端口
//  @param      DMA_BYTEn       每次DMA传输字节数
//  @param      count           一个主循环传输字节数
//  @param      cfg             DMA传输配置，从DMA_cfg里选择
//  @return     void
//  @since      v1.0
//  Sample usage:   uint8 dat[10];
//                  dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(0), dat, A7, DMA_BYTE1, 10, DADDR_RECOVER);
//                  												//DMA初始化，源地址：C_IN_DATA(即C0-C7)，目的地址：dat,A7触发(默认上升沿)，每次传输1字节，共传输 10次 ，传输结束后恢复地址
//                  port_init(A7,ALT1 | DMA_FALLING);               //默认触发源是上升沿，此处改为 下降沿触发
//                  DMA_EN(DMA_CH0);                				//需要使能 DMA 后才能传输数据
//-------------------------------------------------------------------------------------------------------------------
void DMA_DDR_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTX_n ptxn, DMA_BYTEn byten, uint32 count, uint32 cfg)
{
    uint8 BYTEs = 1;
    uint8 i = DMA_BYTE1;
    
    while(i)//计算传输字节数
    {
        BYTEs *= 2;
        i--;
    }
    //DMA 寄存器 配置

    // 开启时钟 
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟
    
#if (defined(MK60D10) || defined(MK60DZ10) || defined(MK66F18))
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟
#elif defined(MK60F15)
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;                    //打开DMA多路复用器时钟
#endif

    
    
    // 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) 
    DMA_BASE_PTR->TCD[CHn].SADDR = 0;
    DMA_BASE_PTR->TCD[CHn].SADDR = (uint32)SADDR;                   // 设置  源地址
    DMA_BASE_PTR->TCD[CHn].DADDR = 0;                               
    DMA_BASE_PTR->TCD[CHn].DADDR = (uint32)DADDR;                   // 设置目的地址
    DMA_BASE_PTR->TCD[CHn].SOFF = 0;                                
    DMA_BASE_PTR->TCD[CHn].SOFF = 0x00u;                            // 设置源地址偏移 = 0x0, 即不变
    DMA_BASE_PTR->TCD[CHn].DOFF = 0;                                
    DMA_BASE_PTR->TCD[CHn].DOFF = BYTEs;                            // 每次传输后，目的地址加 BYTEs
    DMA_BASE_PTR->TCD[CHn].ATTR = 0;
    DMA_BASE_PTR->TCD[CHn].ATTR = (0
                         | DMA_ATTR_SMOD(0x0)               // 源地址模数禁止  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)            // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)               // 目标地址模数禁止
                         | DMA_ATTR_DSIZE(byten)            // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
                        );
    DMA_BASE_PTR->TCD[CHn].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(count);   //当前主循环次数 当前循环次数耗尽后再次加载起始主循环次数
    DMA_BASE_PTR->TCD[CHn].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(count);   //起始主循环次数
    DMA_BASE_PTR->CR &= ~DMA_CR_EMLM_MASK;                                // CR[EMLM] = 0
    DMA_BASE_PTR->TCD[CHn].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(BYTEs); // 通道每次传输字节数，这里设置为BYTEs个字节。注：值为0表示传输4GB 
    
    
    // 配置 DMA 传输结束后的操作 
    DMA_BASE_PTR->TCD[CHn].SLAST = 0;//调整  源地址的附加值,主循环结束后恢复  源地址
    DMA_BASE_PTR->TCD[CHn].DLAST_SGA = (uint32)( (cfg & DADDR_KEEPON ) == 0 ? (-count)  : 0 ); //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址;
    DMA_BASE_PTR->TCD[CHn].CSR = 0;
    DMA_BASE_PTR->TCD[CHn].CSR = (0
                             | DMA_CSR_BWC(3)               //带宽控制,每读一次，eDMA 引擎停止 8 个周期（0不停止；1保留；2停止4周期；3停止8周期）
                             | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                             | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                            );
    DMA_BASE_PTR->TCD[CHn].SLAST = 0;
    
    
    // 配置 DMA 触发源 

#if (defined(MK60D10) || defined(MK60DZ10) || defined(MK66F18))
    DMAMUX_BASE_PTR->CHCFG[CHn] = (0
#elif (defined(MK60F15))
    DMAMUX0_BASE_PTR->CHCFG[CHn] = (0
#endif
                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                            //| DMAMUX_CHCFG_TRIG_MASK                      // Trigger Mode: Periodic   PIT周期触发传输模式   通道1对应PIT1，必须使能PIT1，且配置相应的PIT定时触发 
                            | DMAMUX_CHCFG_SOURCE(PTX(ptxn) + DMA_PORTA)  // 通道触发传输源:    
                          );
    
    DMA_DIS(CHn);                                    //使能通道CHn 硬件请求
    DMA_IRQ_CLEAN(CHn);
}
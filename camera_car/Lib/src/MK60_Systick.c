#include "MK60_Systick.h"

vuint32 SysTicks = 0;

void SysTick_IRQHandler(void)
{
  SysTicks++;
}

void Systick_init(uint32 time)
{
    SYST_RVR = time;                        //设置延时时间
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */
    SYST_CVR = 0x00;                        //清空计数器
    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //使能 systick
                 | SysTick_CSR_TICKINT_MASK      //使能中断 (注释了表示关闭中断)
                 | SysTick_CSR_CLKSOURCE_MASK    //时钟源选择 ( core clk)
               );
    set_vector_handler(SysTick_VECTORn, SysTick_IRQHandler);
}


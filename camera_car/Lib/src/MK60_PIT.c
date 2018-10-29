#include "MK60_PIT.h"

//PIT_init(PIT1,50);
void PIT_init(PITn pitn, float ms)
{
    float cnt = 0;
    cnt = (float)SysClock.bus_khz * ms;
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;        			//使能PIT时钟
    PIT_BASE_PTR->MCR         = 0;									//使能PIT定时器时钟 ，调试模式下继续运行
    PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = (uint32)cnt - 1;              			//设置溢出中断时间
    PIT_FlAG_CLR(pitn);										//清除中断标志位
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
				| PIT_TCTRL_TEN_MASK        //使能 PITn定时器
				| PIT_TCTRL_TIE_MASK        //开PITn中断
			        );
}

//例如：PIT_IRQ_init(PIT0, 5, PIT0_IRQHandler);
void PIT_IRQ_init(PITn pitn, float ms, void pfunc_handler(void))
{
  set_vector_handler((VECTORn_t)((int)pitn + PIT0_VECTORn), pfunc_handler);
  PIT_init(pitn, ms);
}

//example :PIT_delay(PIT2,30);
void PIT_delay(PITn pitn, float ms)
{
    float cnt = 0;
    cnt = (float)SysClock.bus_khz * ms;
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;        			//使能PIT时钟
    PIT_BASE_PTR->MCR         = 0;									//使能PIT定时器时钟 ，调试模式下继续运行
    PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = (uint32)cnt;              			//设置溢出中断时间
    PIT_FlAG_CLR(pitn);										//清除中断标志位
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
				| PIT_TCTRL_TEN_MASK        //使能 PITn定时器
				//| PIT_TCTRL_TIE_MASK        //开PITn中断
				);
    while( !((PIT_BASE_PTR->CHANNEL[pitn].TFLG) & PIT_TFLG_TIF_MASK) );//等待时间到
    PIT_FlAG_CLR(pitn);                                     //清中断标志位
}

//example:PIT_time_start(PIT2);
void PIT_time_start(PITn pitn)
{
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;        			//使能PIT时钟
    PIT_BASE_PTR->MCR         = 0;									//使能PIT定时器时钟 ，调试模式下继续运行
    PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = ~0;              			//设置溢出中断时间
    PIT_FlAG_CLR(pitn);										//清除中断标志位
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
				| PIT_TCTRL_TEN_MASK        //使能 PITn定时器
			        //| PIT_TCTRL_TIE_MASK        //开PITn中断
				);
}

//////返回单位为毫秒
float PIT_time_get(PITn pitn)
{
    uint32 val;
    float temp;
    val = (uint32)(~0) - PIT_BASE_PTR->CHANNEL[pitn].CVAL;
    if(PIT_BASE_PTR->CHANNEL[pitn].TFLG &  PIT_TFLG_TIF_MASK)		//判断时间是否超时
    {
        PIT_FlAG_CLR(pitn);									//清除中断标志位
        PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;   //禁止PITn定时器（用于清空计数值）
        return 0XFFFFFFFF;									//超时返回最大值
    }
    temp = val / (SysClock.bus_khz / 1000);
    temp /= 1000;
    return temp;
}

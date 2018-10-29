#ifndef MK60_PIT_H
#define MK60_PIT_H


#include "common.h"

//¶¨ÒåPITÄ£¿éºÅ
typedef enum
{
    PIT0,
    PIT1,
    PIT2,
    PIT3,
}PITn;

#define     PIT_FlAG_CLR(PITn)          PIT_BASE_PTR->CHANNEL[PITn].TFLG |= PIT_TFLG_TIF_MASK        //
#define     Common_delay(ms)           PIT_delay(PIT3,ms)

void PIT_init(PITn pitn, float ms);
void PIT_delay(PITn pitn, float ms);
void PIT_time_start(PITn pitn);
float PIT_time_get(PITn pitn);
void PIT_IRQ_init(PITn pitn, float ms, void pfunc_handler(void));
#endif
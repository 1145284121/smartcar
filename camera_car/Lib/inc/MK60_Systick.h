#ifndef MK60_SYSTICK_H
#define MK60_SYSTICK_H

#include "common.h"

extern vuint32 SysTicks;

void Systick_init(uint32 time);

#endif
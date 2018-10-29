#ifndef MK60_GPIO_H
#define MK60_GPIO_H

#include "common.h"
void PORT_init(PTX_n ptx_n, PORT_CFG cfg);
void GPIO_init(PTX_n ptx_n, GPIO_DDR ddr, PORT_CFG cfg, uint8 dat);
void GPIO_ddrset(PTX_n ptx_n, GPIO_DDR ddr);
uint8 GPIO_get(PTX_n ptx_n);
void GPIO_set(PTX_n ptx_n, uint8 dat);
#endif
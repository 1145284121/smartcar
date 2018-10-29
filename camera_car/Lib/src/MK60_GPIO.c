#include "MK60_GPIO.h"

static PORT_MemMapPtr PORTX[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};
static GPIO_MemMapPtr GPIOX[5] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR};


void PORT_init(PTX_n ptx_n, PORT_CFG cfg)
{
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptx_n));  //使能端口时钟
  PORTX[PTX(ptx_n)]->PCR[PTn(ptx_n)] &= ~(uint32)PORT_PCR_MUX_MASK;
  PORTX[PTX(ptx_n)]->PCR[PTn(ptx_n)] = cfg;
}

void GPIO_init(PTX_n ptx_n, GPIO_DDR ddr, PORT_CFG cfg, uint8 dat)
{
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptx_n));  //使能端口时钟
  PORTX[PTX(ptx_n)]->PCR[PTn(ptx_n)] &= ~(uint32)PORT_PCR_MUX_MASK;
  PORTX[PTX(ptx_n)]->PCR[PTn(ptx_n)] = ALT1 | cfg;
  if(ddr == GPO) 
  {
    GPIOX[PTX(ptx_n)]->PDDR |= (uint32)(1 << PTn(ptx_n));
    if(dat == 1) 
    {
      GPIOX[PTX(ptx_n)]->PDOR |=  (uint32)(1 << PTn(ptx_n));
    }
    else    
    {
      GPIOX[PTX(ptx_n)]->PDOR &= ~(uint32)(1 << PTn(ptx_n)); 
    }
  }
  else   
  {
    GPIOX[PTX(ptx_n)]->PDDR &= ~(uint32)(1 << PTn(ptx_n));
  }
}

void GPIO_ddrset(PTX_n ptx_n, GPIO_DDR ddr)
{
  if(ddr == GPO) 
  {
    GPIOX[PTX(ptx_n)]->PDDR |= (uint32)(1 << PTn(ptx_n));
  }
  else   
  {
    GPIOX[PTX(ptx_n)]->PDDR &= ~(uint32)(1 << PTn(ptx_n));
  }
}

uint8 GPIO_get(PTX_n ptx_n)
{
  return ((GPIOX[PTX(ptx_n)]->PDIR >> PTn(ptx_n)) & 0x01);
}

void GPIO_set(PTX_n ptx_n, uint8 dat)
{
  if(dat == 1)
  {
    GPIOX[PTX(ptx_n)]->PDDR |= (1 << PTn(ptx_n));
  }
  else
  {
    GPIOX[PTX(ptx_n)]->PDDR &= ~(1 << PTn(ptx_n));
  }
}
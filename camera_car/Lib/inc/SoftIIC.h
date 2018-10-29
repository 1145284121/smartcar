#ifndef SOFTIIC_H
#define SOFTIIC_H

#include "common.h"


#if (defined(MK60F15) || defined(MK66F18) || defined(MK60D10) || defined(MK60DZ10))
#include "MK60_PIT.h"
/*软件I2C引脚设置*/
#define IICDelay      16              
/**************************************/
/*  K66 210M时该值设为3-15    大概150ns完成采集
*/
/*************************************/
#define IICCLOCKON     (SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK)
#define IIC_PORT       PORTE_BASE_PTR
#define IIC_GPIO       PTE_BASE_PTR

#define IIC_SCLpin     0
#define IIC_SDApin     1
#define IIC_SDA_OUT()  {(IIC_GPIO->PDDR |= (1 << IIC_SDApin));}
#define IIC_SDA_IN()   {(IIC_GPIO->PDDR &= ~(1 << IIC_SDApin));}
#define IIC_DATA       ((IIC_GPIO->PDIR >> IIC_SDApin) & 0x01)
#define IIC_SDA_H      (IIC_GPIO->PDOR |= (1 << IIC_SDApin))
#define IIC_SCL_H      (IIC_GPIO->PDOR |= (1 << IIC_SCLpin))
#define IIC_SDA_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SDApin))
#define IIC_SCL_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SCLpin))



#elif (defined(S9KEAZ1284))
#include "KEA128_Systick.h"
#include "KEA128_GPIO.h"



#define IICDelay       10 
#define IIC_GPIO       GPIOB_BASE_PTR
#define IIC_SCLpin     7
#define IIC_SDApin     6



#define IIC_SDA_OUT()  {(IIC_GPIO->PDDR |= (1 << IIC_SDApin));}
#define IIC_SDA_IN()   {(IIC_GPIO->PDDR &= ~(1 << IIC_SDApin));}
#define IIC_DATA       ((IIC_GPIO->PDIR >> IIC_SDApin) & 0x01)
#define IIC_SDA_H      (IIC_GPIO->PDOR |= (1 << IIC_SDApin))
#define IIC_SCL_H      (IIC_GPIO->PDOR |= (1 << IIC_SCLpin))
#define IIC_SDA_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SDApin))
#define IIC_SCL_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SCLpin))


#endif



uint8 IIC_Read_Reg(uint8 addr, uint8 offset);
bool IIC_Write_Reg(uint8 addr, uint8 offset, uint8 data);
bool IIC_Read_Buff(uint8 addr, uint8 offset, uint8* buff, uint8 size);
void IIC_init(void);
#endif
#ifndef SCCB_H
#define SCCB_H

#include "common.h"


#define SCCB_CLKON              (SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK)
#define SCCB_PORT               PORTB_BASE_PTR
#define SCCB_SDA_port           PTB_BASE_PTR           
#define SCCB_SDA_pin            11
#define SCCB_SCL_port           PTB_BASE_PTR           
#define SCCB_SCL_pin            10


#define SCCB_SDA_IN             ((SCCB_SDA_port->PDIR >> SCCB_SDA_pin) & 0x01)
#define SCCB_SDA_L              (SCCB_SDA_port->PDOR &=~(1<< SCCB_SDA_pin ))
#define SCCB_SDA_H              (SCCB_SDA_port->PDOR |= (1<< SCCB_SDA_pin ))
#define SCCB_SCL_L              (SCCB_SCL_port->PDOR &=~(1<< SCCB_SCL_pin ))
#define SCCB_SCL_H              (SCCB_SCL_port->PDOR |= (1<< SCCB_SCL_pin ))
#define SCCB_SCL_DDR_OUT        (SCCB_SCL_port->PDDR |= (1<< SCCB_SCL_pin ))
#define SCCB_SDA_DDR_OUT        (SCCB_SDA_port->PDDR |= (1<< SCCB_SDA_pin ))
#define SCCB_SDA_DDR_IN         (SCCB_SDA_port->PDDR &=~(1<< SCCB_SDA_pin ))
#define SCCB_DELAY              (SCCB_delay(900))              /////SCCBÑÓ³Ù


void SCCB_init(void);
uint8 SCCB_sendbyteU8(uint8 ADDR, uint8 OFFSET, uint8 data);
uint8 SCCB_sendbyteU16(uint8 ADDR, uint8 OFFSET, uint16 data);
uint8 SCCB_receivedataU8(uint8 ADDR, uint8 OFFSET, uint8 *data, uint16 length);
uint8 SCCB_receivedataU16(uint8 ADDR, uint8 OFFSET, uint16 * data);
#endif

/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * All rights reserved.
 *中南大学黄竞辉
 *
 * @file       		sccb
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @Target core		MK60DNZ  MK60DN MK60FX/FN MK66FX/MK66FN
 * @date       		2017-11-9
 ********************************************************************************************************************/
 

#include "SCCB.h"

////////
void SCCB_init(void)
{
	SCCB_CLKON;
	PORT_ISFR_REG(SCCB_PORT) = (1 << SCCB_SDA_pin);
	PORT_ISFR_REG(SCCB_PORT) = (1 << SCCB_SCL_pin);
	SCCB_PORT->PCR[SCCB_SDA_pin] = ALT1 | ODO | PULLUP;//上拉开漏复用GPIO
	SCCB_PORT->PCR[SCCB_SCL_pin] = ALT1 | ODO | PULLUP;
	SCCB_SDA_DDR_OUT;                                  //输出
	SCCB_SCL_DDR_OUT;
}

static void SCCB_delay(volatile uint16 t)
{
	while (t--);
}

static uint8 SCCB_start()    //开始信号
{
	SCCB_SDA_H;
	SCCB_SCL_H;
	SCCB_SDA_DDR_IN;
	if (!SCCB_SDA_IN)         //忙检测
	{
		SCCB_SDA_DDR_OUT;
		return 'F';
	}
	SCCB_SDA_DDR_OUT;
	SCCB_SDA_L;
	SCCB_DELAY;
	SCCB_SCL_L;

	if (SCCB_SDA_IN)         //忙检测
	{
		SCCB_SDA_DDR_OUT;
		return 'F';
	}
	return 'T';
}

static uint8 SCCB_stop()   //停止信号
{
	SCCB_SCL_L;
	SCCB_SDA_L;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	SCCB_SDA_H;
	SCCB_DELAY;
	return 'T';
}

static uint8 SCCB_waitack() //等待应答
{
	SCCB_SCL_L;
	SCCB_SDA_DDR_IN;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	if (SCCB_SDA_IN) // 检测外部
	{
		SCCB_SDA_DDR_OUT;
		SCCB_SCL_L;
		return 'F';
	}
	SCCB_SDA_DDR_OUT;
	SCCB_SCL_L;
	return 'T';
}

static void SCCB_ack()     //应答
{
	SCCB_SCL_L;
	SCCB_DELAY;
	SCCB_SDA_L;
	//SCCB_DELAY;
	SCCB_SCL_H;
	//SCCB_DELAY;
	SCCB_SCL_L;
	//SCCB_DELAY;
}

static void SCCB_noack()    //无应答
{
	SCCB_SCL_L;
	SCCB_DELAY;
	SCCB_SDA_H;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	SCCB_SCL_L;
	SCCB_DELAY;
}

static void SCCB_writebyte(uint8 data)
{
	uint8 i = 8;
	while (i--)
	{
		if (data & 0x80)          //SDA 输出数据
		{
			SCCB_SDA_H;
		}
		else
		{
			SCCB_SDA_L;
		}
		data <<= 1;
		SCCB_DELAY;
		SCCB_SCL_H;
		SCCB_DELAY;
		SCCB_SCL_L;
	}
}

static uint8 SCCB_readbyte()
{
	uint8 data = 0;
	uint8 i = 8;
        SCCB_SDA_DDR_IN;
	while (i--)
	{
		data <<= 1;
		SCCB_SCL_L;
		SCCB_DELAY;
		SCCB_SCL_H;
		SCCB_DELAY;
		if (SCCB_SDA_IN)
		{
			data |= 0x01;
		}
		else
		{
			data &= ~0x01;
		}
	}
	SCCB_SDA_DDR_OUT;
	SCCB_SCL_L;
	return data;
}

uint8 SCCB_sendbyteU8(uint8 ADDR, uint8 OFFSET, uint8 data)
{
	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 0);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_writebyte(OFFSET);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_writebyte(data);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_stop();
	return 'T';
}

uint8 SCCB_sendbyteU16(uint8 ADDR, uint8 OFFSET, uint16 data)
{
	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 0);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_writebyte(OFFSET);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_writebyte((data >> 8) & 0x00ff);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
        SCCB_writebyte(data & 0x00ff);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_stop();
	return 'T';
}

uint8 SCCB_receivedataU8(uint8 ADDR, uint8 OFFSET, uint8 *data, uint16 length)
{
	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 0);   //for write
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}
	SCCB_writebyte(OFFSET);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}
	SCCB_stop();              //指向目标寄存器

	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 1);  //for read

	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}

	while (length)
	{
		*data = SCCB_readbyte();
		if (length == 1)
		{
			SCCB_noack();
		}
		else
		{
			SCCB_ack();
		}
		data++;
		length--;
	}
	SCCB_stop();
	return 'T';
}

uint8 SCCB_receivedataU16(uint8 ADDR, uint8 OFFSET, uint16 * data)
{
        uint16 temp[2] = {0};
	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 0);   //for write
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}
	SCCB_writebyte(OFFSET);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}
	SCCB_stop();              //指向目标寄存器

	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 1);  //for read

	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}

	temp[0] = SCCB_readbyte();
        SCCB_ack();
        temp[1] = SCCB_readbyte();
        *data = ((temp[0] << 8) & 0xff00) | temp[1];
	SCCB_stop();
	return 'T';
}

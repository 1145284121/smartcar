#ifndef MK60_UART_H
#define MK60_UART_H


#include "common.h"
#include "MK60_GPIO.h"
typedef enum
{
    uart0,
    uart1,
    uart2,
    uart3,
    uart4,
}UARTn;

void uart_init(UARTn uartn, uint32 baud);
void uart_putchar(UARTn uartn, uint8 ch);
void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len);
void uart_putstr (UARTn uartn, const uint8 *str);
void uart_getchar (UARTn uartn, uint8 *ch);
char uart_querychar (UARTn uartn, uint8 *ch);
void uart_rx_irq_en(UARTn uartn);
#define  SendChar(ch)              uart_putchar(uart0, ch)
#define  SendBuff(str, size)       uart_putbuff(uart0, str, size)

#endif
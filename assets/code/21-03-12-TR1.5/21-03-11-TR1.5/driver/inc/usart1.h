#ifndef _USART1_H
#define _USART1_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"
#include "queue.h"
#include "filter.h"

//typedef struct _UART_SEND
//{
//	wait_event_t 	wait;
//	u32_t 		send_num;
//	u32_t 		cnt;
//	u32_t 		size;
//	atomic		sending;
//	u8_t 		err;
//	u8_t*		buff; 
//}UART_SEND;

void usart1_init(void);
void USART1_printf(const char *fmt, ...);
void USART1_sendData(u8 *a,u8 count);

//void usart1_sendU16(u16 data);
void usart1_sendFloatX10(float data);

#endif


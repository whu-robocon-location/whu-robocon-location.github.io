#ifndef _USART1_H
#define _USART1_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"
#include "queue.h"


void usart1_init(void);
void USART1_printf(const char *fmt, ...);
void USART1_sendData(u8 *a,u8 count);
void USART2_MyInit(void);
void USART2_IRQHandler(void);

//void usart1_sendU16(u16 data);
void usart1_sendFloatX10(float data);
extern int flag1;

#endif


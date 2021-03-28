#ifndef _LED_H
#define _LED_H

#include "stm32f4xx.h"

#define LED_GREEN_ON()				GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define LED_GREEN_OFF()			GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define LED_GREEN_TOGGLE()		GPIO_ToggleBits(GPIOB, GPIO_Pin_9)

#define LED_RED_ON()				GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define LED_RED_OFF()			GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LED_RED_TOGGLE()		GPIO_ToggleBits(GPIOB, GPIO_Pin_8)

void led_config(void);

#endif

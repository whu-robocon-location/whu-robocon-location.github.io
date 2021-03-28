#include "led.h"

void led_config(void)
{
	GPIO_InitTypeDef gpio;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&gpio);
  
	LED_RED_OFF();
    LED_GREEN_OFF();
}

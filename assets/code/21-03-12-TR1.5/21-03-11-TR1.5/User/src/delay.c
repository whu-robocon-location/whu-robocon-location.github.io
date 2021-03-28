#include "delay.h"

/****************** —” ±1us ****************/
void delay_1us()
{
	int i = 40;
	while (i--)
		;
}
/****************** —” ± us ****************/
void delay_us(uint32_t us)
{
	while (us--)
		delay_1us();
}

void delay_ms(u16 nms)
{
	u16 i,j = 42000;
	for(i=0;i<nms;i++)
	{
		while(j--);
	}
}

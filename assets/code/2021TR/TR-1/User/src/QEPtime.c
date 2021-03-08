#include "QEPtime.h"//QEP（Quadrature Encoder Pulse）正交编码脉冲电路

int DeltaCntEncoder1=0;
int DeltaCntEncoder2=0;

void QEPtime_config(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM5,ENABLE);
	
	//time3 for QEP1;
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); 
	
	gpio.GPIO_Pin 	= GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode 	= GPIO_Mode_AF;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_Speed = GPIO_Speed_100MHz;
  gpio.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);

	TIM_EncoderInterfaceConfig(TIM3, 
														 TIM_EncoderMode_TI12,
														 TIM_ICPolarity_Falling,
														 TIM_ICPolarity_Falling);
	TIM_Cmd(TIM3,ENABLE);
	
	//time5 for QEP2;
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); 
	
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AF;
  gpio.GPIO_OType = GPIO_OType_PP;  
  gpio.GPIO_Speed = GPIO_Speed_100MHz;
  gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
	
	TIM_EncoderInterfaceConfig(TIM5, 
														 TIM_EncoderMode_TI12,
							 							 TIM_ICPolarity_Falling,
							 							 TIM_ICPolarity_Falling);
	TIM_Cmd(TIM5,ENABLE);
	//开始正交信号计数
	Encoder_Start();
}


void Encoder_Start(void)
{
     TIM3->CNT = 0x7fff;
	   TIM5->CNT = 0x7fff;
}


void Encoder_Get_CNT(void)
{	
     DeltaCntEncoder1 = 0x7fff-(TIM5->CNT);//CNT最大65535即0xffff
     TIM5->CNT = 0x7fff;
	
     DeltaCntEncoder2 = -(0x7fff-(TIM3->CNT));
     TIM3->CNT = 0x7fff;
}

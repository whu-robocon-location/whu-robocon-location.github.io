#include "stm32f4xx.h"
#include "spi2.h"
#include "delay.h"
#include "ADS1255.h"

/*把SPI2初始化为主模式
* SPI_I2S_SendData(),中的参数是16位的，所以SPI_DataSize_16b;
* 如果想使用8b,可以直接操作寄存器，SPI1->DR
*/	

void spi2_config(void)
{
 	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable SPI2 and GPIOB clocks */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 , ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//CS 引脚初始化    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;           
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_6); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI2 configuration */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	                    //设置SPI1为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                   //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	 		                //串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 		                //第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			                //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //定义波特率预分频的值:波特率预分频值为32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				    //数据传输从MSB(最高)位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;						    //CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);
	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE); 

}

u8 SPI2_ReadByte(u8 spi_addr)
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,spi_addr);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);

	return SPI_I2S_ReceiveData(SPI2);
}

void SPI2_WriteByte(u8 data)
{
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2,data);
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
}

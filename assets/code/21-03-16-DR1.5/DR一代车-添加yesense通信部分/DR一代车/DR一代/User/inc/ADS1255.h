#ifndef __ADS1255_H__
#define __ADS1255_H__

#ifndef _ADS1255_CONFIG_H
#define _ADS1255_CONFIG_H

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
	
/*ADS1255 Register Address*/
#define  STATUS  0x00            	//状态寄存器
#define  MUX     0x01             //模拟多路选择寄存器
#define  ADCON   0x02            //AD控制寄存器  
#define  DRATE   0x03             //AD数据速度寄存器
#define  IO   	 0x04            	//I/O控制寄存器
#define  OFC0    0x05           	//
#define  OFC1    0x06
#define  OFC2    0x07
#define  FSC0    0x08
#define  FSC1    0x09
#define  FSC2    0x0A


/*Command Definition*/

#define WAKEUP    0x00
#define RDATA    	0x01            	//单次读取数据
#define RDATAC   	0x03            	//连续读取数据
#define SDATAC   	0x0f            	//停止连续读取数据
#define RREG     	0x10            	//从寄存器中读取数据  0x1x
#define WREG     	0x50             	//向寄存器中写入数据  0x5x
#define SELFCAL   0xf0             	//补偿和增益自我校准
#define SELFOCAL  0xf1             	//补偿自我校准
#define SELFGCAL  0xf2             	//增益自我校准
#define SYSOCAL   0xf3             	//系统补偿校准
#define SYSGCAL   0xf4             	//系统增益校准
#define SYNC      0xfc             	//使AD转换同步
#define STANDBY   0xfd             	//启动待机模式
#define Reset     0xfe             	//重置升高值
#define WAKEDOWN    0xff            //结束，退出待机模式

#define ClrAD_CS        GPIO_ResetBits(GPIOC,GPIO_Pin_6)             	//0 PC6复位
#define ClrAD_DIN 			GPIO_ResetBits(GPIOB,GPIO_Pin_15)             //             
#define ClrAD_DOUT			GPIO_ResetBits(GPIOB,GPIO_Pin_14)             //
#define Read_DRDY			  GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)             //读DRDY管叫状态，为0时数据转换结束，可以读数据


#define SetAD_DIN 		  GPIO_SetBits(GPIOB,GPIO_Pin_15)             	//1             
#define SetAD_CS        GPIO_SetBits(GPIOC,GPIO_Pin_6)             		//                  
#define SetAD_DOUT			GPIO_SetBits(GPIOB,GPIO_Pin_14)            		//

#endif

void ADS1255_config(void);
u32 Read_AD_Value(void);
#endif

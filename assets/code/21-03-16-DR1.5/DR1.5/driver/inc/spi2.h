#ifndef _SPI2_H
#define _SPI2_H

#define u8 unsigned char
#define u16 unsigned short
	
void spi2_config(void);
u8 SPI2_ReadByte(u8 spi_addr);
void SPI2_WriteByte(u8 data);
#endif























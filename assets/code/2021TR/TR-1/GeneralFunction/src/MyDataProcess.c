#include "MyDataProcess.h"


//Name:	EncodeFloatData()
//Describe:float型数据无精度丢失地截成4个字节，放在buff中
//Input:float* f, unsigned char* buff
//Output:None
//Return:None
void EncodeFloatData(float* f, unsigned char* buff)
{
	u16 f_c[2];
	*(float*)f_c = *f;
//	buff[0] = (f_c[0]&0x00ff);
//	buff[1] = ((f_c[0]>>8)&0x00ff);
//	buff[2] = (f_c[1]&0x00ff);
//	buff[3] = ((f_c[1]>>8)&0x00ff);
	buff[3] = (f_c[0]&0x00ff);
	buff[2] = ((f_c[0]>>8)&0x00ff);
	buff[1] = (f_c[1]&0x00ff);
	buff[0] = ((f_c[1]>>8)&0x00ff);
}


//Name:	DecodeFloatData()
//Describe:将从buff开始的4个字节位拼接转换成浮点数
//Input:float* f, unsigned char* buff
//Output:None
//Return:None
void DecodeFloatData(float* f, unsigned char* buff)
{
	u16 f_c[2];
	f_c[0]=((buff[1]&0x00ff)<<8)|buff[0];
	f_c[1]=((buff[3]&0x00ff)<<8)|buff[2];
	*f=*(float *)f_c;
}

//Name:	EncodeFloatData()
//Describe:float型数据无精度丢失地截成4个字节，放在buff中
//Input:float* f, unsigned char* buff
//Output:None
//Return:None
void EncodeS32Data(s32* f, unsigned char* buff)
{
	u16 f_c[2];
	*(s32*)f_c = *f;
	buff[0] = (f_c[0]&0x00ff);
	buff[1] = ((f_c[0]>>8)&0x00ff);
	buff[2] = (f_c[1]&0x00ff);
	buff[3] = ((f_c[1]>>8)&0x00ff);
}


//Name:	DecodeFloatData()
//Describe:将从buff开始的4个字节位拼接转换成浮点数
//Input:float* f, unsigned char* buff
//Output:None
//Return:None
void DecodeS32Data(s32* f, unsigned char* buff)
{
	u16 f_c[2];
	f_c[0]=((buff[1]&0x00ff)<<8)|buff[0];
	f_c[1]=((buff[3]&0x00ff)<<8)|buff[2];
	*f=*(s32 *)f_c;
}

//Name:	EecodeS16Data()
//Describe:将16位有符号数据转换成2个字节放在buff中
//Input:s16* f, unsigned char* buff
//Output:None
//Return:None
void EncodeS16Data(s16* f, unsigned char* buff)
{
	buff[0] = (uint8_t)(*f);
	buff[1] = (uint8_t)((*f)>>8);
}

//Name:	DecodeS16Data()
//Describe:将buff中的2个字节拼接为s16
//Input:s16* f, unsigned char* buff
//Output:None
//Return:None
void DecodeS16Data(s16* f, unsigned char* buff)
{
	*f = (s16)(((u16)(buff[1])<<8)|buff[0]);
}


//Name:	EecodeU16Data()
//Describe:将16位无符号数据转换成2个字节放在buff中
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void EncodeU16Data(u16* f, unsigned char* buff)
{
	buff[0] = (uint8_t)(*f);
	buff[1] = (uint8_t)((*f)>>8);
}

//Name:	DecodeU16Data()
//Describe:将buff中的2个字节拼接为u16
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void DecodeU16Data(u16* f, unsigned char* buff)
{
	*f = ((u16)(buff[1])<<8)|buff[0];
}

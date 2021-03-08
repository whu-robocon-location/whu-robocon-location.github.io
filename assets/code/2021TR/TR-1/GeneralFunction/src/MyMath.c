#include "MyMath.h"


/*
 * 函数名: fsign
 * 描  述: 取出浮点数的符号位
 * 输  入: 浮点数a
 * 输  出: int型的符号
 * 调  用: 外部调用
 */
int fsign(float a)
{
	if(a>0)
		return 1;
	else if(a<0)
		return -1;
	else return 0;
}

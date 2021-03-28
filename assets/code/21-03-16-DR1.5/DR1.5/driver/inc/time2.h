#ifndef _TIME2_H
#define _TIME2_H

#include "stm32f4xx.h"
#include "filter.h"

#define P ((float)(1))							   //陀螺安装倾斜的标定系数

extern u8 init_flag;


void time2_config(void);
//void exti_config(void);
#endif

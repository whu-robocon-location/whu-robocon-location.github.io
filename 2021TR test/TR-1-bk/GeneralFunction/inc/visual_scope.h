#ifndef _VISUAL_SCOPE_H_
#define _VISUAL_SCOPE_H_

#include "stm32f4xx.h"
#include "usart1.h"
#include "includes.h"
#include "MyDataProcess.h"
void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4);
void Wheel_diameter(int32_t n_data1,int32_t n_data2,float n_data3) ;
//void Filtering_lag(float n_data1,float n_data2,float n_data3 );
void Filtering_lag(float n_data1,float n_data2);
//	void Filtering_lag(float n_data1);   
#endif

#ifndef QEPTIME_H
#define QEPTIME_H

#include "stm32f4xx.h"

extern int DeltaCntEncoder1;
extern int DeltaCntEncoder2;

void QEPtime_config(void);
void Encoder_Start(void);
void Encoder_Get_CNT(void);

#endif








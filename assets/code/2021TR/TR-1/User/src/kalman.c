#include "kalman.h"
#include "main.h"

KALMAN_filter   kalman_Encoder1;
KALMAN_filter   kalman_Encoder2;
KALMAN_filter   kalman_Vx;
KALMAN_filter   kalman_Vy;
KALMAN_filter   kalman_Vw;
KALMAN_filter   kalman_Accelerate;
KALMAN_filter   kalmanVx;
KALMAN_filter   kalmanVy;
KALMAN_filter   kalmanVw;
KALMAN_filter   kalmanAccelerate;

void kalman_init(KALMAN_filter* kalman)
{
    kalman->x_last=1;
    kalman->p_last=5;     //这两个数值初始化无所谓，但P不要为0
    
    kalman->Q=0.0001;     //过程噪声
    kalman->R=0.01; 	  //观测噪声,即原始速度数据的方差？？？？
    kalman->A=1;          //简化单模型
    kalman->H=1;
}

double KAL_filter(KALMAN_filter *kalman,double input)
{
    kalman->x_last=kalman->A*kalman->x_last;                //根据上一次的值预测本次的值    
    kalman->p_last=kalman->p_last+kalman->Q;                //根据上一次的协方差计算本次的协方差    
    kalman->kg=kalman->p_last/(kalman->p_last+kalman->R);   //根据本次的协方差计算kalman增益    
    kalman->x_last=kalman->x_last+kalman->kg*(input-kalman->H*kalman->x_last);//求出本次的最优值    
    kalman->p_last=(1-kalman->kg)*kalman->p_last;           //求出本次的协方差	
	kalman->best=kalman->x_last;    
    return kalman->best;
}


#ifndef FILTER_H_
#define FILTER_H_

#include "stm32f4xx.h"

#define SAMP_RATE 1000  // 1200 //采样频率(Hz)
#define CUTOFF_RATE \
    50                     // 50
                           // //截止频率,实际截止频率为100Hz
#define pi 3.14159265359   //圆周率
#define SAMP_TIME 0.0001005 /*采样周期(s)*/
#define SCALE 0.0127       /*(1000*5)/(65535*6)*(N-ZERO_POinT)*/
#define MAX_NUM 5 /*冒泡排序处理数据的数目*/
#define MAX_ZP_SAMPLE 2000
#define ZERO_THREDHOLD  (float)(44.788452)
//(float)(315.406) /*如果前后两次采样的绝对值小于阈值，则用此数据更新零点*/
//#define			THETA			0.65
///*一阶低通滤波系数*/

/*复合滤波结构体定义,去掉极值求平均*/

/*复合滤波结构体定义*/
typedef struct tag_comb_filter4 {
    u32 max;
    u32 min;
    double average;
    double sum;
    unsigned char comb_filter_flag;
    u32 data[MAX_NUM];
} comb_filter;

/*递归滤波器*/
typedef struct IIR {
    float last;  //上一时刻滤波后的值
    float now;   //这一时刻滤波后的值
    float a;     //低通滤波系数
} IIR_struct;

typedef struct tag_k_filter {
    int32_t A; /*系统状态转移矩阵*/
               //	float B;						/*噪声输入矩阵，列矩阵*/
    int32_t H; /*观测矩阵，行矩阵*/
    /*前三个矩阵为系统的参数矩阵，计算过程已经将矩阵直接代入，程序运算过程不用此矩阵，仅作记录用*/

    float C;      /*预测过程协方差矩阵     C(k|k)  */
    float C_last; /*上次预测过程协方差矩阵 C(k|k-1)*/
    float Q;      /*过程噪声*/
    float R;      /*观测噪声*/

    float X;          /*系统状态滤波输出矩阵，列矩阵*/
    float X_last;     /*系统状态预测矩阵，列矩阵*/
    float kg;         /*卡尔曼增益，列矩阵*/
    float zero_point; /*kalman filter zero_point*/
    float input;      /*量测值，即Z(k)*/
} k_filter;

/*二阶低通滤波器结构体*/
typedef struct tag_second_order {
    unsigned int fs;  //采样频率(Hz)
    unsigned int f0;  //截止频率,注意要乘2才是实际的截止频(Hz)
    float den;
    float den_ac;
    float den_b;
    float a;                 // x[n]系数
    float b;                 // y[n-1]系数
    float c;                 // y[n-2]系数
    float current_output;    // y[n]
    float last_output;       // y[n-1]
    float last_last_output;  // y[n-2]
} second_order_struct;

extern float kal_input;
extern float IIR_input;
extern float kal_output;
extern float sec_output;
extern float second_order_exkalman_output;
extern float cha;
extern float point;  ///////////////仅测试，以后好了要去掉

extern k_filter kal_filter;
extern k_filter EXkal_filter;
extern second_order_struct second_order_flt;
extern comb_filter comb_flt;
extern IIR_struct IIR_flt;
static double exkalman_filte_b;

void IIR_init(IIR_struct* IIR_flt);
double IIR_filter(IIR_struct* IIR_flt, double input);
/*
 * 		kalman滤波器初始化
 *		k1_flt: 一阶kalman指针
 */
extern void kal_flt_init(k_filter* k_flt);
/*
 * 		kalman滤波器初始化
 *		k1_flt: 一阶kalman指针
 */
extern float kalman_filter(k_filter* k_flt, float input);
void second_order_init(second_order_struct* second_struct);
double second_order_filter(second_order_struct* second_struct, double input);
double second_order_exkalman_filter(k_filter* k_flt,
                                    second_order_struct* second_struct,
                                    double input);
void EXkal_flt_init(k_filter* EXk_flt);
/*
 * 		组合滤波器
 *		comb_flt: comb_filter 指针
 */
double combination_filter(comb_filter* comb_flt);

#endif

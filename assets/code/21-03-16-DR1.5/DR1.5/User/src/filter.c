#include "main.h"

k_filter kal_filter;
k_filter EXkal_filter;
second_order_struct second_order_flt;
comb_filter comb_flt;
IIR_struct IIR_flt;
static double exkalman_filte_b;
float kal_input;
float IIR_input;
float kal_output;
float sec_output;
float second_order_exkalman_output;
float cha = 0;
float point = 0;  ///////////////仅测试，以后好了要去掉

int cmp(const void *a, const void *b) { return *(int *)a - *(int *)b; }
/*
 *
 * 		先去除趋势项，然后复合滤波的实现：融合低通滤波和限幅滤波，剔除最大值和最小值，剩余值求平均
 *
 */
// double combination_filter(comb_filter* comb_flt)
//{
//	int i=0;
//	long sum=0;
//	double n=MAX_NUM/2;
////趋势项随时间变化斜率,MAX_NUM/2,去除由温度引起的趋势项 	double k=0;
////时间短温漂引起的趋势项不明显，这样做没有意义
//
//	comb_flt->sum = 0;
//
//	for (i=0 ;i< MAX_NUM/2; i++)
//		sum += (comb_flt->data[i+(int)n]-comb_flt->data[i]);
//	k=(double)sum/(n*n);
//
//	for(i=0;i<MAX_NUM;i++)
//	{
//		comb_flt->data[i]=comb_flt->data[i]-k*(i+1)*0.0001;
//		comb_flt->sum+=comb_flt->data[i];
//	}
//
//	qsort(comb_flt->data,MAX_NUM,sizeof(int),cmp);
//	comb_flt->sum=comb_flt->sum-comb_flt->data[0]-comb_flt->data[MAX_NUM-1];
//	comb_flt->average = (float)(comb_flt->sum/(MAX_NUM-2));

//	comb_flt->comb_filter_flag = 1;
//	return 	comb_flt->average;
//}

double combination_filter(comb_filter *comb_flt)  //运行 44.67us（组合滤波）
{
    int i = 0;

    comb_flt->sum = 0;
    comb_flt->max = comb_flt->data[0];
    comb_flt->min = comb_flt->data[0];
    for (i = 1; i < MAX_NUM; i++) {
        if (comb_flt->max < comb_flt->data[i]) {
            comb_flt->max = comb_flt->data[i];
        }
        if (comb_flt->min > comb_flt->data[i]) {
            comb_flt->min = comb_flt->data[i];
        }
    }
    for (i = 0; i < MAX_NUM; i++) {
        comb_flt->sum += comb_flt->data[i];
    }

    comb_flt->sum = comb_flt->sum - comb_flt->max - comb_flt->min;
    comb_flt->average = (float)(comb_flt->sum / (MAX_NUM - 2));
    comb_flt->sum = 0;
    comb_flt->comb_filter_flag = 1;

    return comb_flt->average;
}

/**
 * @brief 卡尔曼滤波器初始化
 */
void kal_flt_init(k_filter *k_flt) {
    int i = 0;
    /* 系统状态转移矩阵 */
    k_flt->A = 1;
    /* 观测矩阵，行矩阵 */
    k_flt->H = 1;
    /* 上次预测过程协方差矩阵 C(k|k-1) */
    k_flt->C_last = 1;
    /* 读取 AD 的值 */
    for (i = 0; i < MAX_NUM; i++) comb_flt.data[i] = Read_AD_Value();
    /* 系统状态预测矩阵，列矩阵 */
    k_flt->X_last = combination_filter(&comb_flt);

    /* 观测噪声 */
    k_flt->R = 0.1;
    /* 过程噪声 */
    k_flt->Q = 0.00001;
    /* 初始量测值, 即Z(k)为0 */
    k_flt->input = 0;
}

/**
 * @brief 扩展卡尔曼滤波器初始化
 */
void EXkal_flt_init(k_filter *EXk_flt) {
    int i = 0;
    /* 系统状态转移矩阵 */
    EXk_flt->A = 1;
    /* 观测矩阵，行矩阵 */
    EXk_flt->H = 1;
    /* 上次预测过程协方差矩阵 C(k|k-1) */
    EXk_flt->C_last = 1;
    /* 读取 AD 的值 */
    for (i = 0; i < MAX_NUM; i++) comb_flt.data[i] = Read_AD_Value();
	
    /* 系统状态预测矩阵，列矩阵 */
    EXk_flt->X_last = combination_filter(&comb_flt);
    /* 观测噪声 */
    EXk_flt->R = 0.011;
    /* 过程噪声 */
    EXk_flt->Q = 0.00001;
    /* 初始量测值, 即Z(k)为0 */
    EXk_flt->input = 0;
}

/**
 * @brief kalman filter 的实现
 * 卡尔曼滤波, 加了一阶滞后滤波 -- 运行 46.26us
 */
float kalman_filter(k_filter *k_flt, float input) {
    /* 一阶滞后滤波法获得测量值, 如果滞后很强就用下面的 */
    // k_flt->input=IIR_filter(&IIR_flt,input);
    /* 量测方程的更新，3组方程 */
    k_flt->input = input;
    /* 计算卡尔曼增益 kg(k) = C(k|k-1)*H(转置)/[H*C(k|k-1)*H(转置)+R] */
    k_flt->kg = (k_flt->C_last) / (k_flt->C_last + k_flt->R);
    /* 得到现在状态的最优化估算值 X(k|k) = X(k|k-1)+kg(k)*[Z(k)-H*X(k|k-1)] */
    k_flt->X = k_flt->X_last + k_flt->kg * (k_flt->input - k_flt->X_last);
    /* 更新 k 状态下 X(k|k) 的协方差 C(k|k) = [I-kg(k)*H]*C(k|k-1) */
    k_flt->C = (1 - k_flt->kg) * (k_flt->C_last);
    /* 积分得角度 */
    // k_flt->output = k_flt->output + k_flt->X*SAMP_TIME;
    /*时间更新方程，2组方程*/
    /* 根据上一状态预测下一状态的结果 X(k+1|k) = A*X(k|k) 没有控制量 */
    k_flt->X_last = k_flt->X;
    /* 更新 X(k|k-1) 的协方差 C(k+1|k) = A*C(k|k)*A(转置)+B*Q*B(转置) */
    k_flt->C_last = k_flt->C + k_flt->Q;

    /* 返回卡尔曼滤波后的值 */
    return k_flt->X;
}

/**
 * @brief 二阶低通滤波器初始化
 */
void second_order_init(second_order_struct *second_struct) {
    second_struct->fs = SAMP_RATE;
    second_struct->f0 = CUTOFF_RATE;
    second_struct->den =
        (second_struct->fs) * (second_struct->fs) +
        6 * pi * (second_struct->f0) * (second_struct->fs) +
        4 * pi * pi * (second_struct->f0) * (second_struct->f0);
    second_struct->a = 4 * pi * pi * (second_struct->f0) * (second_struct->f0) /
                       (second_struct->den);
    second_struct->b = (2 * (second_struct->fs) * (second_struct->fs) +
                        6 * pi * (second_struct->fs) * (second_struct->f0)) /
                       (second_struct->den);
    second_struct->c =
        (second_struct->fs) * (second_struct->fs) / (second_struct->den);

    //	second_struct->den_ac = (second_struct->fs)*(second_struct->fs) +
    // 6*pi*(second_struct->f0) +
    // 4*(second_struct->fs)*(second_struct->fs)*(second_struct->f0)*(second_struct->f0);
    //	second_struct->den_b = (second_struct->fs)*(second_struct->fs) +
    // 6*pi*(second_struct->f0)*(second_struct->fs) +
    // 4*(second_struct->fs)*(second_struct->fs)*(second_struct->f0)*(second_struct->f0);
    //	second_struct->a 	=
    // 4*pi*pi*(second_struct->f0)*(second_struct->f0)/(second_struct->den_ac);
    //	second_struct->b 	= (2*(second_struct->fs)*(second_struct->fs) +
    // 6*pi*(second_struct->fs)*(second_struct->f0))/(second_struct->den_b);
    //	second_struct->c 	=
    //(second_struct->fs)*(second_struct->fs)/(second_struct->den_ac);
}

/**
 * @brief 二阶低通滤波器初始化
 * 二阶低通滤波 -- 运行 44.58us
 */
double second_order_filter(second_order_struct *second_struct, double input) {
    second_struct->current_output =
        (second_struct->a) * input +
        (second_struct->b) * (second_struct->last_output) -
        (second_struct->c) *
            (second_struct->last_last_output);  //计算出本次滤波输出
    /* 更新上上次输出 */
    second_struct->last_last_output = second_struct->last_output;
    /* 更新上次输出 */
    second_struct->last_output = second_struct->current_output;
    /* 返回本次滤波输出 */
    return second_struct->current_output;
}

/**
 * @brief 低通滤波初始化
 */
void IIR_init(IIR_struct *IIR_flt) {
    IIR_flt->a = 0.99999;               //低通滤波系数
    IIR_flt->last = kal_filter.X_last;  //上一时刻滤波后的值
}
double IIR_filter(IIR_struct *IIR_flt,
                  double input)  //一阶滞后低通滤波 -- 运行 45.21us
{
    IIR_flt->now = (1 - IIR_flt->a) * IIR_flt->last +
                   IIR_flt->a * input;  //这一时刻滤波后的值
    IIR_flt->last = IIR_flt->now;       //上一时刻滤波后的值

    return IIR_flt->now;
}

double second_order_exkalman_filter(k_filter *EXk_flt,
                                    second_order_struct *second_struct,
                                    double input) {
    second_struct->current_output =
        (second_struct->a) * input +
        (second_struct->b) * (second_struct->last_output) -
        (second_struct->c) *
            (second_struct->last_last_output);  //计算出本次滤波输出
    second_struct->last_last_output =
        second_struct->last_output;  //更新上上次输出
    exkalman_filte_b =
        second_struct->current_output - second_struct->last_output;
    second_struct->last_output = second_struct->current_output;  //更新上次输出

    //	k_flt->input=IIR_filter(&IIR_flt,input); //一阶滞后滤波法获得测量值,
    //如果滞后很强就用下面的
    /*量测方程的更新，3组方程*/
    EXk_flt->input = input;
    EXk_flt->kg = (EXk_flt->C_last) /
                  (EXk_flt->C_last +
                   EXk_flt->R); /*计算卡尔曼增益 kg(k) =
                                   C(k|k-1)*H(转置)/[H*C(k|k-1)*H(转置)+R] */
    EXk_flt->X =
        EXk_flt->X_last + exkalman_filte_b +
        EXk_flt->kg * (EXk_flt->input - EXk_flt->X_last -
                       exkalman_filte_b); /*得到现在状态的最优化估算值 X(k|k) =
                                             X(k|k-1)+kg(k)*[Z(k)-H*X(k|k-1)]*/
    EXk_flt->C = (1 - EXk_flt->kg) *
                 (EXk_flt->C_last); /*更新 k 状态下 X(k|k) 的协方差 C(k|k) =
                                       [I-kg(k)*H]*C(k|k-1) */
    /*积分得角度*/
    //	k_flt->output = k_flt->output + k_flt->X*SAMP_TIME;
    /*时间更新方程，2组方程*/
    EXk_flt->X_last = EXk_flt->X; /*根据上一状态预测下一状态的结果 X(k+1|k) =
                                     A*X(k|k) 没有控制量*/
    EXk_flt->C_last =
        EXk_flt->C + EXk_flt->Q; /*更新 X(k|k-1) 的协方差 C(k+1|k) =
                                    A*C(k|k)*A(转置)+B*Q*B(转置)*/

    return EXk_flt->X;  //返回卡尔曼滤波后的值
}

#include "time2.h"

/*动态更新零点的全局变量*/ 
extern double Angular_Velocity;

void time2_config(void) {
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 84 - 1;  //分频数  84   84/84=1M,1us
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 66;  //重载值 67us进行一次定时器中断
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void) {
    static int i = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        if (i < MAX_NUM) {
            comb_flt.data[i] = Angular_Velocity;
            i++;
        }
        if (i == MAX_NUM) {
            i = 0;
            comb_flt.comb_filter_flag = 2;
        }
    }
}


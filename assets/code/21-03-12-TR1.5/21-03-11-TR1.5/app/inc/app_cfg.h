#ifndef  _APP_CFG_H
#define  _APP_CFG_H

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "type.h"

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
* UCOS每个线程优先级不能相同，放在这里统一管理
*********************************************************************************************************
*/
enum TASK_PRIO{
TASK_MAIN_PRIO = 10,
TASK_POSI_PRIO,
TASK_TRANS_PRIO,			
TASK_LED_PRIO,

TASK_SCOPE_PRIO,	
	
OS_TASK_TMR_PRIO
};

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*单位WORD
*********************************************************************************************************
*/
#define TASK_MAIN_STACK_SIZE				512
#define TASK_POSI_STACK_SIZE				512
#define TASK_TRANS_STACK_SIZE				512
#define TASK_LED_STACK_SIZE					128

#define TASK_SCOPE_STACK_SIZE				512

/*
*********************************************************************************************************
*                               		IRQ_PRIORITY
*如果中断函数里需要打印信息，则此中断优先级要低于uart中断
*********************************************************************************************************
*/

enum IRQ_PRIORITY{
	CAN1_IRQn_Priority = 1,
	CAN2_IRQn_Priority,
	USART1_IRQn_Priority,				//2
	USART2_IRQn_Priority,				//2
	USART4_IRQn_Priority,				//2
	TIM2_IRQn_Priority,					//3
	DMA1_Stream5_IRQn_Priority,	//4
	DMA1_Stream6_IRQn_Priority,	//5
};

/*
*********************************************************************************************************
*                                      kernel 里面常用资源值定义
*********************************************************************************************************
*/
#define EVENTS_MAX			40		//EVENTS_MAX是所有事件的数量之和，包括邮箱、消息队列、信号量、互斥量等
#define TIMER_MAX      	40		//定时器
#define MSG_QUEUE_MAX		16		//消息队列
#define TASKS_MAX				14		//任务


/*
*********************************************************************************************************
*                                      定义中断向量表地址
* 因低16k地址用于bootloader，我们的代码需要从16k开始，所以需要在启动时将中断向量重新映射到16k地址
*********************************************************************************************************
*/
//#define IVECTOR_ADDR 					(16*1024)
#define IVECTOR_ADDR 					(0)

#endif

#include "main.h"


/************************************UCOSII任务设置************************************/

/************************************START任务************************************/
#define START_TASK_PRIO      			14 //开始任务的优先级设置为最低
#define START_STK_SIZE  				1024
OS_STK START_TASK_STK[START_STK_SIZE];                                                                 
void start_task(void *pdata);		   
/************************************Angle任务************************************/
#define ANGLE_TASK_PRIO       			9
#define ANGLE_STK_SIZE  		    		1024
OS_STK ANGLE_TASK_STK[ANGLE_STK_SIZE];
void ANGLE_task(void *pdata);
unsigned int ANGLE_FLAG = 0;
/************************************POSI任务************************************/
#define POSI_TASK_PRIO       			10 
#define POSI_STK_SIZE  		    		1024
OS_STK POSI_TASK_STK[POSI_STK_SIZE];
void POSI_task(void *pdata);
u16 RST_flag = 0;		//陀螺仪复位信号
/************************************TRANS任务************************************/
#define TRANS_TASK_PRIO       			11 
#define TRANS_STK_SIZE  		    		1024
OS_STK TRANS_TASK_STK[TRANS_STK_SIZE];
void TRANS_task(void *pdata);
unsigned short TRANS_FLAG = 0;
/************************************LED任务************************************/
#define LED_TASK_PRIO       			12
#define LED_STK_SIZE  		    		1024
OS_STK LED_TASK_STK[LED_STK_SIZE];
void LED_task(void *pdata);
unsigned short LED_FLAG = 0;
u8 CANTransSuccess_flag = 0;	//数据发送成功信号

int main(void) 
{
	RCC_ClocksTypeDef RCC_ClocksStatus;

	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	led_config();										//初始化 LED 引脚
	usart1_init();
	spi2_config();
	can_init();											//初始化 CAN
	time2_config();
	//OSTimeDly(1000);
	kal_flt_init(&kal_filter);
  //kal_flt_init_acc_x(&kal_filter_acc[0]);	//卡尔曼滤波器结构体初始化
	//kal_flt_init_acc_y(&kal_filter_acc[1]);	//卡尔曼滤波器结构体初始化
	EXkal_flt_init(&EXkal_filter);
	IIR_init(&IIR_flt);									//低通滤波初始化
	second_order_init(&second_order_flt);				//二阶低通滤波器结构体初始化
	QEPtime_config();									//tim3&tim5 for encoder（为编码用的两个定时器）
	kalman_init(&kalman_Vx);
	kalman_init(&kalman_Vy);
	kalman_init(&kalman_Vw);
	kalman_init(&kalman_Accelerate);

	//Position_Now.X = OO_distance * cos(Angle_OO);
	//Position_Now.Y = OO_distance * sin(Angle_OO);
	
	OSInit();
	//OSTimeDly(10000);
	//offset[0]=g_output_info.yaw;//程序开始的时候给一个原始数据的偏移量，消除掉误差	


 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	
	OSStart();	
    return 0;
}


void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	  pdata = pdata; 
	  OS_CPU_SysTickInit();
	
  	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)  
  
    OSTaskCreate(ANGLE_task, NULL, (OS_STK*)&ANGLE_TASK_STK[ANGLE_STK_SIZE-1], ANGLE_TASK_PRIO);
	  //offset[0]=g_output_info.yaw;//程序开始的时候给一个原始数据的偏移量，消除掉误差	
    OSTaskCreate(POSI_task, NULL, (OS_STK*)&POSI_TASK_STK[POSI_STK_SIZE-1], POSI_TASK_PRIO);
		OSTaskCreate(TRANS_task, NULL, (OS_STK*)&TRANS_TASK_STK[TRANS_STK_SIZE-1],TRANS_TASK_PRIO);
		OSTaskCreate(LED_task, NULL, (OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1], LED_TASK_PRIO); 				
		OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.


	 OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//int isAnim = 0;
void ANGLE_task(void *pdata){
	while(1)	
	{	
			if(!offset[0])
		{
			offset[0]=g_output_info.yaw;//程序开始的时候给一个原始数据的偏移量，消除掉误差	                    
		}
		CountAngel();
		AccelerateCac();
		OSTimeDly(1);
	}

}

void POSI_task(void *pdata){
    while(1)
    {
        if(ANGLE_FLAG >= 1)
        {
            ANGLE_FLAG--;
					
            if(init_flag == 1)													//复位的时候init_flag==0
            {
                CountPosition();				//1ms更新一次码盘坐标
				CountSpeed();
								TRANS_FLAG++;
                LED_FLAG++;
            }
            else																//复位状态(复位时保持陀螺静止2.25s左右)
            {
                if(RST_flag == 1)												//复位陀螺仪信号
                {
                    can_init();													//重新配置 CAN
                    RST_flag = 0;
                }
                TIM3->CNT = 0x7fff;
                TIM5->CNT = 0x7fff;
            }
        }
    	OSTimeDly(1);
	}
}

void LED_task(void *pdata){
    while(1)
    {
        if(LED_FLAG>= 500)						//LED闪烁周期(阈值) X 5ms
        {
            LED_FLAG = 0;
            if(CANTransSuccess_flag == 0)		   		 //CAN发送坐标成功，绿灯闪烁
            {
                LED_GREEN_TOGGLE();
                LED_RED_OFF();
            }
            else										//数据发送失败，红灯闪烁
            {
                LED_RED_TOGGLE();
                LED_GREEN_OFF();
				CAN_DeInit(CAN1);
				can_init();		
            }
        }
		OSTimeDly(1);
	}
}

void TRANS_task(void *pdata){
    while(1)
    {
        if(TRANS_FLAG >= 1)	//运行 229.76us
        {
            TRANS_FLAG--;					//对此段时间计算平均速度
			CANTransSuccess_flag = Position_Send(&Position);			//发送坐标数据
            OSTimeDly(9);
            Speed_Send(&Speed_Now);			                     		//发送速度数据
		}
    	OSTimeDly(1);
	}
}

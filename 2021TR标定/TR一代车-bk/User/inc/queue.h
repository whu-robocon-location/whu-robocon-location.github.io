#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "stm32f4xx.h"
#include "string.h"
#define QUEUE_SIZE    200  
#define ARRAY_SIZE    (QUEUE_SIZE + 1) 

typedef struct  
{
    int     front;   
    int     rear; 
    uint8_t Data[ARRAY_SIZE];    
}QueueTypeDef;

void SetEmpty(QueueTypeDef * queue);
void EnQueue(QueueTypeDef * queue,uint8_t Data);
void DeQueue(QueueTypeDef * queue,uint8_t * Data);
uint8_t GetQueueLength(QueueTypeDef * queue);

#endif /*_QUEUE_H_*/



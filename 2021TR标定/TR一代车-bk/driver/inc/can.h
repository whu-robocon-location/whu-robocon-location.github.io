#ifndef _CAN_H
#define _CAN_H
#include "PositionCount.h"
#include "stm32f4xx.h"


#define ID_MASTER_TEST 0X00010000
#define ID_CHOOSE 0x00010200
#define ID_Respond_Mater 0x00020101
#define FuncID_MASTER_TEST 0X40
#define Sub_Err 0X7F

/********* u8  ErrFlag[5] **********/
#define FuncIDErr 0x00
#define SubIDtan90 0x01
#define DataLengthErr 0x02
#define SubIDUseErr 0x03
#define DataErr 0x04

extern CORRECT Position_Correct;  //实际修正后的坐标

u8 Position_Send(POSITION* position);
u8 Speed_Send(SPEED* speed);
u8 Diatance_Send(POSITION* position);
u8 RePosition_Send(void);

void can_init(void);
void anglesend(void);
void Answer_Master(int length, char FuncID, char SubID, char Data1, char Data2,
                   char Data3);
void SelfTest(void);
void Reset_RS(void);


/***********************************/
#endif

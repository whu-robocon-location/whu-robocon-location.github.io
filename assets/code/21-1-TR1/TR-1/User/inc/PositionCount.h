#ifndef _POSITIONCOUNT_H
#define _POSITIONCOUNT_H

#include "MyMath.h"
#include "filter.h"
#include "stm32f4xx.h"
#include "math.h"
#include "kalman.h"
#include "time2.h"
#include "QEPtime.h"
#include "led.h"
#include "ucos_ii.h"

extern float DeltaCntEncoder10, DeltaCntEncoder20;
extern float DeltaCntEncoder11, DeltaCntEncoder22;

//定义结构体数据类型
typedef struct pot {
    float X;  //机器人自身坐标系原点在场地坐标系中的X坐标，cm
    float Y;  //机器人自身坐标系原点在场地坐标系中的Y坐标，cm
    float
        AngleDeg;  //机器人的姿态角(场地坐标系X轴转向自身坐标系X轴)，角度制，顺负逆正
    float Sum_Encoder1;  //码盘1走过的路程
    float Sum_Encoder2;  //码盘2走过的路程
    float Sum_Distance;  //机器人走过的总路程

} POSITION;

typedef struct spd {
    float Vx;  //机器人在场地坐标系X轴方向的分速度，cm/s
    float Vy;  //机器人在场地坐标系Y轴方向的分速度，cm/s
    float Vw;  //机器人角速度，度/s,顺负逆正
    float V;   //机器人的合速度
} SPEED;

typedef struct correct {
    float real_x;
    float dt_del_x;
    float real_y;
    float dt_del_y;
} CORRECT;

extern float CM_PER_CNT_1;  // 2*pi*Radius_Wheel_1/(4*编码器线数) //定位系统X轴
extern float CM_PER_CNT_2;  // 2*pi*Radius_Wheel_2/(4*编码器线数) //定位系统Y轴

// extern float CM_PER_CNT_11;		//2*pi*Radius_Wheel_1/(4*编码器线数)
// //定位系统X轴 extern float CM_PER_CNT_22;
// //2*pi*Radius_Wheel_2/(4*编码器线数) //定位系统Y轴

//下面的参数取决于码盘的安装位置
extern float OO_distance;  //自身坐标系原点与定位系统坐标系原点的距离，单位cm
extern float
    Angle_XX;  //自身坐标系x轴转向定位系统坐标系x轴的角度，弧度制，顺负逆正
extern float
    Angle_OO;  //自身坐标系X轴转向自身坐标系原点指向定位系统坐标系原点连线的角度，弧度制，顺负逆正

//机器人位姿与速度数据
extern POSITION Position_Now;
extern POSITION Position;
extern SPEED Speed_Now;

extern float Gyro_angle_delta;  //陀螺角度增量，角度制
extern float speed_V, speedv;
extern float time;
extern float Accelerated_Speed;

void Position_init(void);
void CountPosition(void);
void CountSpeed(void);
void CountAngel(void);
void GYRO_Reset(void);


#endif

#include "main.h"
#include "can.h"
volatile u8  ErrFlag[5] = {0};        
volatile u8  Flag_Reset=0;
volatile float SetSpeed;

float positionx;
float positiony;
float speedx;               //积分系数相关变量
float speedy;
#define X_coeff 1
#define Y_coeff 1

void can_init(void)     //can的初始化
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//打开GPIOA的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//打开CAN1的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//打开CAN2的时钟

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);//PA11的CAN1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);//PA12的CAN1

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);

    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    CAN_DeInit(CAN1);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);

    can_filter.CAN_FilterNumber = 1;
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh = (u16)(((ID_MASTER_TEST<<3))>>16);         //
    can_filter.CAN_FilterIdLow = (u16)(((ID_MASTER_TEST<<3))>>0);           //
    can_filter.CAN_FilterMaskIdHigh = (uint16_t)((uint32_t)(0X1FFFFFFFu<<3)>>16);      //1111 1111 1111 1111
    can_filter.CAN_FilterMaskIdLow = (uint16_t)((uint32_t)(0X1FFFFFFFu<<3)>>0);        //1111 1111 1111 1000
    can_filter.CAN_FilterFIFOAssignment = 0;
    can_filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&can_filter);

    can_filter.CAN_FilterNumber=0;
		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;//标识符屏蔽模式
		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;//宽度
		can_filter.CAN_FilterIdHigh=(u16)(((ID_CHOOSE<<3))>>16);                  //0000 0000 0000 1000
		can_filter.CAN_FilterIdLow=(u16)(((ID_CHOOSE<<3))>>0);                    //0001 0000 0000 0000
		can_filter.CAN_FilterMaskIdHigh=(uint16_t)((uint32_t)(0X1FFFFFF0u<<3)>>16);          //1111 1111 1111 1111
		can_filter.CAN_FilterMaskIdLow=(uint16_t)((uint32_t)(0X1FFFFFF0u<<3)>>0);            //1111 1111 1000 0000
		//以上4句，限制只接受ID为0x0100的报文
		can_filter.CAN_FilterFIFOAssignment=0;
		can_filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&can_filter);

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

u8 Position_Send(POSITION* position)						   //给主控发送位置数据
{
    CanTxMsg tx_message;
		positionx=position->X*X_coeff;
		positiony=position->Y*Y_coeff;
    tx_message.ExtId = ID_Respond_Mater;							//报文ID号
    tx_message.DLC = 0x08;								//数据字节数
    tx_message.RTR = CAN_RTR_Data;				//数据帧
    tx_message.IDE = CAN_Id_Extended;			

    tx_message.Data[0] =  0x41;
    tx_message.Data[1] =  'P';
    tx_message.Data[2] = (uint8_t)((int16_t)(positionx * 10));             //精确到mm
    tx_message.Data[3] = (uint8_t)((int16_t)(positionx * 10) >> 8);
    tx_message.Data[4] = (uint8_t)((int16_t)(positiony * 10));
    tx_message.Data[5] = (uint8_t)((int16_t)(positiony * 10) >> 8);
    tx_message.Data[6] = (uint8_t)((int16_t)(position->AngleDeg * 10));
    tx_message.Data[7] = (uint8_t)((int16_t)(position->AngleDeg * 10) >> 8);
    if(CAN_Transmit(CAN1, &tx_message) == CAN_TxStatus_NoMailBox)
        return 1;
    else
        return 0;
}


u8 Speed_Send(SPEED* speed)		//给主控发送速度数据 次/5ms
{
    CanTxMsg tx_message;
    tx_message.ExtId = ID_Respond_Mater;
    tx_message.DLC = 0x08;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;

    tx_message.Data[0] =  0x41;
    tx_message.Data[1] =  'S';
    tx_message.Data[2] = (uint8_t)((int16_t)(speed->Vx * 10));
    tx_message.Data[3] = (uint8_t)((int16_t)(speed->Vx * 10) >> 8);
	
    tx_message.Data[4] = (uint8_t)((int16_t)(speed->Vy * 10));
    tx_message.Data[5] = (uint8_t)((int16_t)(speed->Vy * 10) >> 8);
	
    tx_message.Data[6] = (uint8_t)((int16_t)(speed->Vw * 10));
    tx_message.Data[7] = (uint8_t)((int16_t)(speed->Vw * 10) >> 8);
    if(CAN_Transmit(CAN1, &tx_message) == CAN_TxStatus_NoMailBox)
        return 1;
    else
        return 0;
}

//void Accelerated_Send(float A)
//{
//    CanTxMsg tx_message;
//    tx_message.ExtId = (ID_Respond_Mater);
//    tx_message.DLC = 0x04;
//    tx_message.RTR = CAN_RTR_Data;
//    tx_message.IDE = CAN_Id_Extended;

//    tx_message.Data[0] =  0x41;
//    tx_message.Data[1] =  'A';
//    tx_message.Data[2] = (uint8_t)((int16_t)(A * 10));
//    tx_message.Data[3] = (uint8_t)((int16_t)(A * 10) >> 8);
//   	CAN_Transmit(CAN1, &tx_message);
//}


void SelfTest(void)
{
    CanTxMsg tx_message;
    tx_message.ExtId = (ID_Respond_Mater);
    tx_message.DLC = 0x05;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;

    tx_message.Data[0] =  0x40;
    tx_message.Data[1] =  'C';
    tx_message.Data[2] = (uint8_t)'W';
    tx_message.Data[3] = (uint8_t)'H';
    tx_message.Data[4] = (uint8_t)'U';
    CAN_Transmit(CAN1, &tx_message);
}


void Reset_RS(void)     //复位响应
{
    CanTxMsg tx_message;
    tx_message.ExtId = ID_Respond_Mater;
    tx_message.DLC = 0x04;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;

    tx_message.Data[0] =  0x40;
    tx_message.Data[1] =  'R';
    tx_message.Data[2] = (uint8_t)'R';
    tx_message.Data[3] = (uint8_t)'S';
    CAN_Transmit(CAN1, &tx_message);
}



u8 RePosition_Send(void)						   //响应主控发送位置
{
    CanTxMsg tx_message;
    tx_message.ExtId = ID_Respond_Mater;
    tx_message.DLC = 0x04;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;

    tx_message.Data[0] =  0x41;
    tx_message.Data[1] =  'R';
    tx_message.Data[2] = (uint8_t)'R';
    tx_message.Data[3] = (uint8_t)'S';
    CAN_Transmit(CAN1, &tx_message);
	if(CAN_Transmit(CAN1, &tx_message) == CAN_TxStatus_NoMailBox)
        return 1;
    else
        return 0;
}





void Answer_Master(int length ,char FuncID,char SubID,char Data1,char Data2,char Data3)  //
{
	CanTxMsg tx_message;
	tx_message.ExtId = ID_Respond_Mater;							
	tx_message.RTR = CAN_RTR_Data;				
	tx_message.IDE = CAN_Id_Extended;				
	tx_message.DLC = length;								            
	tx_message.Data[0] = FuncID;		
	tx_message.Data[1] = SubID;
	tx_message.Data[2] = Data1;
	tx_message.Data[3] = Data2;
	tx_message.Data[4] = Data3;
	CAN_Transmit(CAN1,&tx_message);
}

void Accelerated_Send(SPEED *Accel)
{
    CanTxMsg tx_message;
    tx_message.ExtId = (ID_Respond_Mater);
    tx_message.DLC = 0x04;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;
	
	tx_message.Data[0] =  0x41;
    tx_message.Data[1] =  'G';
    tx_message.Data[2] = (uint8_t)((int16_t)(Accel->Vx * 10));
    tx_message.Data[3] = (uint8_t)((int16_t)(Accel->Vx * 10) >> 8);
	
    tx_message.Data[4] = (uint8_t)((int16_t)(Accel->Vy * 10));
    tx_message.Data[5] = (uint8_t)((int16_t)(Accel->Vy * 10) >> 8);
	
    tx_message.Data[6] = (uint8_t)((int16_t)(Accel->Vw * 10));
    tx_message.Data[7] = (uint8_t)((int16_t)(Accel->Vw * 10) >> 8);
	CAN_Transmit(CAN1, &tx_message);
}

void Yesense_Accelerated_Send(float *yesenseAcc)
{
    CanTxMsg tx_message;
    tx_message.ExtId = (ID_Respond_Mater);
    tx_message.DLC = 0x04;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;

	tx_message.Data[0] =  0x41;
    tx_message.Data[1] =  'Y';
    tx_message.Data[2] = (uint8_t)((int16_t)(yesenseAcc[0] * 10));
    tx_message.Data[3] = (uint8_t)((int16_t)(yesenseAcc[0] * 10) >> 8);
	
    tx_message.Data[4] = (uint8_t)((int16_t)(yesenseAcc[1]* 10));
    tx_message.Data[5] = (uint8_t)((int16_t)(yesenseAcc[1] * 10) >> 8);
	
    tx_message.Data[6] = (uint8_t)((int16_t)(yesenseAcc[2]* 10));
    tx_message.Data[7] = (uint8_t)((int16_t)(yesenseAcc[2] * 10) >> 8);

	CAN_Transmit(CAN1, &tx_message);
}


void CAN1_RX0_IRQHandler(void)
{	
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		if(rx_message.ExtId == ID_MASTER_TEST)
		{			
				if(rx_message.Data[0]==0x00)
					{
						switch(rx_message.Data[1])
						{
							case 'C':   //自检
								if(rx_message.DLC==5)
								{
										if(rx_message.Data[2] == 'W')
										{		
										Answer_Master(0x05,0x40,'C','W','H','U');
										}
										else
										Answer_Master(0x02 ,0x7F,DataErr,0,0,0);
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
								break;
								
							case 'R':   //复位
								if(rx_message.DLC==4)
								{
										if(rx_message.Data[2] == 'R'&&rx_message.Data[3] == 'S')
										{
											Position_Now.X = 0;
											Position_Now.Y = 0;
											Position_Now.AngleDeg = 0;
											Position.X = 0;
											Position.Y = 0;
											Position.AngleDeg = 0;
											RST_flag = 1;
											GYRO_Reset();
											Reset_RS();
										}
										else
											Answer_Master(0x02 ,0x7F,DataErr,0,0,0);
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
								break;
							default:
								Answer_Master(0x02 ,0x7F,SubIDtan90,0,0,0);
								break;
						}
					}
				 else
					 Answer_Master(0x02 ,0x7F,FuncIDErr,0,0,0);
		}
		
		if(rx_message.ExtId == ID_CHOOSE)
		{
		
			if(rx_message.Data[0]==0x01)
			{
				switch(rx_message.Data[1])
						{
							case 'R':   //复位
								if(rx_message.DLC==8)
								{
									
								float temp, temp_sin, temp_cos;	//用于寄存公用的三角函数运算值，减少重复计算
                Position_Now.AngleDeg = (double)((s16)(((u16)(rx_message.Data[7]) << 8) | rx_message.Data[6])) * 0.1;
                temp = (Position_Now.AngleDeg) * PI / 180 + Angle_OO;
								temp_cos = cos(temp);
                temp_sin = sin(temp);
                Position_Now.X = (double)((s16)(((u16)(rx_message.Data[3]) << 8) | rx_message.Data[2])) * 0.1/X_coeff + OO_distance * temp_cos;
                Position_Now.Y = (double)((s16)(((u16)(rx_message.Data[5]) << 8) | rx_message.Data[4])) * 0.1/Y_coeff + OO_distance * temp_sin;
                RePosition_Send();
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
							  break;
							case 'X':                              //只修改X坐标
								if(rx_message.DLC==4)
									{
								float temp, temp_cos;
                temp = (Position_Now.AngleDeg) * PI /180 + Angle_OO;
                temp_cos = cos(temp);
								Position_Now.X = (double)((s16)(((u16)(rx_message.Data[3]) << 8) | rx_message.Data[2])) * 0.1/X_coeff + OO_distance * temp_cos;
								Answer_Master(0x04 ,0x41,'X',rx_message.Data[2],rx_message.Data[3],0);
								} 
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
								break;
																	
							case 'Y':                               //只修改Y坐标
								if(rx_message.DLC==4)
								{
								float temp, temp_sin;	
                temp = (Position_Now.AngleDeg) * PI / 180 + Angle_OO;
                temp_sin = sin(temp);
                Position_Now.Y = (double)((s16)(((u16)(rx_message.Data[3]) << 8) | rx_message.Data[2])) * 0.1/Y_coeff + OO_distance * temp_sin;
								Answer_Master(0x04 ,0x41,'Y',rx_message.Data[2],rx_message.Data[3],0);
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
							  break;
							case 'L':                               //修改x和角度
								if(rx_message.DLC==6)
								{
								float temp,  temp_cos;	//用于寄存公用的三角函数运算值，减少重复计算
                Position_Now.AngleDeg = (double)((s16)(((u16)(rx_message.Data[5]) << 8) | rx_message.Data[4])) ;
                temp = (Position_Now.AngleDeg + Angle_XX) * PI / 180;
                temp_cos = cos(temp);
                Position_Now.X = (double)((s16)(((u16)(rx_message.Data[3]) << 8) | rx_message.Data[2])) * 0.1/X_coeff + OO_distance * temp_cos;
                Answer_Master(0x04 ,0x41,'R','S','S',0);
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
							  break;
																									
							case 'V':                               //
								if(rx_message.DLC==4)
								{
                SetSpeed = (double)((s16)(((u16)(rx_message.Data[3]) << 8) | rx_message.Data[2])) * 0.1;
								Answer_Master(0x04 ,0x41,'V',rx_message.Data[2],rx_message.Data[3],0);
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
							  break;
								
							case 'A':                               //加速度查询
								if(rx_message.DLC==4)
								{
									if(rx_message.Data[2] == 'C'&&rx_message.Data[3] == 'G')
										{
											Accelerated_Send(&Accel);
							     	}
										else if(rx_message.Data[2] == 'C'&&rx_message.Data[3] == 'Y')
										{
											Yesense_Accelerated_Send(accel);
										}
									else
											Answer_Master(0x02 ,0x7F,DataErr,0,0,0);
								}
								else
									Answer_Master(0x02 ,0x7F,DataLengthErr,0,0,0);
							  break;
							default:
								Answer_Master(0x02 ,0x7F,SubIDtan90,0,0,0);
								break;
					   }
			}
							 else
					 Answer_Master(0x02 ,0x7F,FuncIDErr,0,0,0);
		}
	}
}


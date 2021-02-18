#include "usart1.h"
#include "stdbool.h"


/*���ں������ڵ�����ʱʹ�ã��ڳ�����can��ͨ��*/



u8 dbg_show_enable = 1;
uint8_t aTxBuffer[50] = {0};
QueueTypeDef TxBuffer;

u8 HEAD_FLAG = 0;
u8 RX_FLAG = 0;
short Data_len = 2;
u8 Data_Yesense[98] = {0x59, 0x53};
int res = 0;

void usart1_init(void)     //����1��ʼ������
{
	DMA_InitTypeDef   DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //����PA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //����PA10����ΪUSART1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);         // ��ʼ��PA9, PA10
	
	USART_InitStructure.USART_BaudRate = 460800;       //������115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;       //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;          //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //�շ�
	USART_Init(USART1, &USART_InitStructure);              //��ʼ������1
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//���������ж�
	//BSP_IntVectSet(BSP_INT_ID_USART1,BSP_INTHandlerUSART1);
	
	DMA_DeInit(DMA2_Stream7);
	//* Configure DMA Initialization Structure 
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                         //DMA_MODE_NORMAL
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART1->DR)) ;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	//Configure Tx DMA
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer;
	DMA_Init(DMA2_Stream7,&DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);  
	//����USARTΪDMA����
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	//����NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure);                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           
	NVIC_Init(&NVIC_InitStructure);   
	
	USART_Cmd(USART1, ENABLE);   //ʹ��USART
}


/**
 * @function    : 
 * @Description : ���ڷ��ʹ�������,�����������Ƿ���δ��������
**/
void USART_TxHandler(void)
{
	uint8_t i = 0,j = 0,data;
	j = GetQueueLength(&TxBuffer);
	if (j > 20) j = 20;
	else if (j == 0) return;
	for (i = 0; i < j; i++)
	{
			DeQueue(&TxBuffer,&data);
			aTxBuffer[i] = data;
	}
	DMA_SetCurrDataCounter(DMA2_Stream7,j);
	DMA_Cmd(DMA2_Stream7, ENABLE);  
}

void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
		USART_TxHandler();
	}
}


//���ڸĽ���printf()
void USART1_printf(const char *fmt, ...)
{	
	va_list ap;
	uint16_t len;
	uint8_t buf[128];
	va_start(ap, fmt);
	len = vsprintf((char *)buf, (char *)fmt, ap);
	va_end(ap);	/*�ÿɱ�����б���vsprintf������Ҫ������ַ�����ʽ����������ͻ�����*/
	while (len >= (50 - GetQueueLength(&TxBuffer)));/*���������������̫��Ļ���Ҫ��һ��*/
	USART1_sendData(buf,len);
}

void USART1_sendData(u8 *a,u8 count)
{
	uint8_t i = 0,j = 0,data;
	for (i = 0; i < count; i++)
	{
		EnQueue(&TxBuffer,a[i]);
	} 	
	if (DMA_GetCurrDataCounter(DMA2_Stream7) == 0)
	{
		j = GetQueueLength(&TxBuffer);
		if (j > 50) j = 50;
		for (i = 0; i < j; i++)
		{
			DeQueue(&TxBuffer,&data);
			aTxBuffer[i] = data;
		}
		DMA_SetCurrDataCounter(DMA2_Stream7,j);
		DMA_Cmd(DMA2_Stream7, ENABLE);  
	}
}

//��������
u8 USART_RX_BUF[12] = {0};
bool flag_rx_start = false;
short rx_num = 0;
double Angular_Velocity = 0.0;		//���ٶ�
int j = 0;
u8 temp;	
unsigned short sum = 0;
int Cal_temp = 0;
short s ;
char *ps ;      
char *pret ;
int num_loss = 0;

void USART1_IRQHandler(void)
{
	int i = 0;	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	
	{
		USART_ClearFlag (USART1,USART_FLAG_RXNE);			
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
		
		temp = USART_ReceiveData(USART1);
	//	USART_SendData(USART2,temp);
		if(!flag_rx_start)		
		{
			if(temp == 0xDD)
				flag_rx_start = true;
		}
		else			//��ʼ����
		{
			USART_RX_BUF[rx_num] = temp;
			rx_num++;
			if(rx_num >= 7)
			{
				flag_rx_start = false;
				rx_num = 0;
				sum = 0;
				for(i = 0; i < 3; i++)
					sum+=USART_RX_BUF[i];
				//if(((sum & 0xFF00)>>8)==USART_RX_BUF[5] && (sum & 0x00FF)==USART_RX_BUF[6])		//У��ͼ�����ȷ
				if(sum == ((USART_RX_BUF[5] << 8) | USART_RX_BUF[6]))
				{
					
					//���ٶ� USART_RX_BUF[1]�Ǹ�λ�ֽڣ�USART_RX_BUF[0]�ǵ�λ�ֽڣ�USART_RX_BUF[2]����λ�ֽ�
					//��USART_RX_BUF[1]������չΪ�����ֽڣ���ps[1],ps[0]

					signed char *cc = (signed char*)(&USART_RX_BUF[1]);
					s = (short)cc[0];
					ps = (char *)&s;
					pret = (char *)&Cal_temp;
										
					pret[0] = USART_RX_BUF[0];
					pret[1] = USART_RX_BUF[2];
					pret[2] = ps[0];      
					pret[3] = ps[1];
					
					Angular_Velocity = Cal_temp;
				
					if(j<MAX_NUM)	
					{
						comb_flt.data[j]= Angular_Velocity;
						j++;
					}
					 if(j == MAX_NUM)
					{
						j = 0;
				          comb_flt.comb_filter_flag = 2;
					}
			
				}
				else num_loss++;
			}
		}
	}
}

//u8 USART_RX_BUF[32];     //���ջ���,���32���ֽ�.
////����״̬
////bit7��������ɱ�־
////bit6�����տ�ʼ��־
////bit5�����յ�0x0d
////bit4~0�����յ�����Ч�ֽ���Ŀ
//u8 USART_RX_STA=0;       //����״̬���	
//void USART1_IRQHandler(void)
//{
//	u8 temp;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	//���ռĴ����ǿ�
//	{
//		USART_ClearFlag (USART1,USART_IT_RXNE);			//����жϱ�־
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
//				
//		temp = USART_ReceiveData(USART1);
//		
//		if(RX_FLAG == 1)
//		{
//			Data_len++;
//			Data_Yesense[Data_len-1] = temp;
//			if(Data_len>97)					//������Χ
//			{
//				Data_len = 2;
//				RX_FLAG = 0;
//			}
//		}
//					
//		if(HEAD_FLAG == 1)
//		{
//			if(temp == 0x53)				//֡ͷ2
//			{
//				Data_len-=2;
//				res = analysis_data(Data_Yesense, Data_len);
//				Data_len = 2;
//				RX_FLAG = 1;
//				HEAD_FLAG = 0;   
//			}
//			else if(temp != 0x59)			
//				HEAD_FLAG = 0;
//		}
//		if(temp == 0x59)					//֡ͷ1
//			HEAD_FLAG = 1;
//			
//			if((USART_RX_STA&0x40)!=0)//�����Ѿ���ʼ
//			{			
//				if((USART_RX_STA&0x80)==0)//����δ���
//				{
//					if(USART_RX_STA&0x20)//�ѽ��յ���0x0d
//					{
//						if(temp!=0x0a)
//							USART_RX_STA=0;//���մ���,���¿�ʼ
//						else 				
//								USART_RX_STA|=0x80;	//���������														
//					}
//					else //��û�յ�0X0D
//					{	
//						if(temp==0x0d)
//							USART_RX_STA|=0x20;
//						else
//						{
//							USART_RX_BUF[USART_RX_STA&0X1F]=temp;
//							USART_RX_STA++;
//							if((USART_RX_STA&0X1F)>31)USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//						}		 
//					}
//				}  		
//			}
//			else if(temp=='#')		//���յ���ͷ
//				USART_RX_STA|=0x40;	
//			
//			if((USART_RX_STA&0x80)!=0)			//�����Ѿ���ɣ���������ָ��
//			{
//				
//			}
//	}
//}








///******�����ø�������************/
//void usart1_sendU16(u16 data)		
//{
//	u16 temp =  data;
//	int i;
//	u8 temp1[7]={'\n','\r',0,0,0,0,0};
//	for(i=0;i<5;i++)
//	{
//		temp1[i+2]=temp%10+0x30;
//		temp = temp/10;
//	}
//	for(i=6;i>=0;i--)
//	{
//		
//	}
//}

////��ȷ��С�����1λ���������ֲ�����4λ
void usart1_sendFloatX10(float data)		
{
//	s16 temp =  data*10;
	s16 temp =  data;
	int i;
	u8 temp1[9]={0,0,0,0,0,0,0,' ',' '};

	for(i=6;i>=0;i--)
	{
		temp1[i]=temp%10+0x30;
		temp = temp/10;
	}
	USART1_sendData(temp1,9);		
}

int fputc(int ch,FILE*f)
{
	USART_SendData(USART1,(unsigned char) ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	return(ch);
}
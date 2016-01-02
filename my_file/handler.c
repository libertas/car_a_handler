#include "handler.h"
#include "global.h"

uint8_t data[10] = {0};
uint8_t data_old[10] = {0};
uint8_t PSX_CONN[5] = {0x01,0x43,0x00,0x01,0x00};                       //��������ģʽ
uint8_t PSX_MODE[9] = {0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};  //����ΪAnalog Mode
uint8_t PSX_POLL[9] = {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //��ѯ
uint8_t PSX_EXIT[9] = {0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};  //�˳��������Motor�汾�ﱻɾ����
char buff[10] = {0};
uint8_t PSX_Ana = 0x00;	   //����Analog Mode�ı�־
int v1=0,v2=0,v3=0,v4=0;
int ctr_v0 = 0,ctr_v1 = 0,ctr_v2 = 0;
int v_x = 0,v_y = 0;
float motor_v[3] = {0};
double X=0,Y=0,dX=0,dY=0;
float degree = 0;  //��ǰ������	 
double vx=0,vy=0,w=0;	   //Ŀ���ٶ�
double Dx=0,Dy=0,De=0;    //Ŀ�������

void lunxun()
{
  	int i;
	GPIO_ResetBits(GPIOB, ATT); 
	delay_us(5);
			 
	for (i = 0; i < 9; i++)					   //������ѯ���ݰ�
	{
		SPI_I2S_SendData(SPI2, PSX_POLL[i]);
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
		data[i] = SPI_I2S_ReceiveData(SPI2);
	}
	GPIO_SetBits(GPIOB, ATT);
	delay_us(5);
	//myprintf(USART3,"%d*%d*%d*%d*%d*%d",data[4],data[5],data[6],data[7],data[8],data[9]);
}


int handle_init(void)
{
	int i,try_times = 0;
	uint8_t PSX_Ana = 0x00;	   //����Analog Mode�ı�־
	uint8_t PSX_Ana_check = 0;
	while(PSX_Ana_check != 255)	//0xF3��Analog Mode��Dataֵ   255
	{
	    GPIO_ResetBits(GPIOB, ATT);	   //����ATT
			delay_us(5);	
			for (i = 0; i <= 4; i++)	   //��������ģʽ
			{ 
			//	myprintf(USART3,"5\r\n");
				SPI_I2S_SendData(SPI2, PSX_CONN[i]); 					//����   //0x01,0x43,0x00,0x01,0x00
				while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	//�ȴ��������
 				SPI_I2S_ReceiveData(SPI2);				 //������
			}			 
		   	GPIO_SetBits(GPIOB, ATT);					 //����ATT
			delay_us(5);									 //��ʱ
			GPIO_ResetBits(GPIOB, ATT); 				 //������	
			delay_us(5);
			for (i = 0; i <= 8; i++)					 //����ΪAnalog Mode
			{ 
			//	myprintf(USART3,"9\r\n");
		      	SPI_I2S_SendData(SPI2, PSX_MODE[i]);   //0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00
				while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
			    //SPI_I2S_ReceiveData(SPI2);			
				if (i == 1){
					PSX_Ana = SPI_I2S_ReceiveData(SPI2);
					if(PSX_Ana == 255){
						uprintf(USART3,"PSX_Ana = 0x%x\n",PSX_Ana);
					}
				}
				else
					SPI_I2S_ReceiveData(SPI2);
			}	
	 	   	GPIO_SetBits(GPIOB, ATT);
			delay_us(5);				  
			GPIO_ResetBits(GPIOB, ATT);	
			delay_us(5);
			for (i = 0; i <= 8; i++)					 //����Ƿ����óɹ�
			{ 	
			//	myprintf(USART3,"9\r\n");	
				SPI_I2S_SendData(SPI2, PSX_EXIT[i]); 
				while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  //0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A
				if (i == 1){
					PSX_Ana_check = SPI_I2S_ReceiveData(SPI2);
					uprintf(USART3,"PSX_Ana = 0x%x\n",PSX_Ana);
					uprintf(USART3,"PSX_Ana_check = 0x%x\n",PSX_Ana_check);
				}
				else	 
					SPI_I2S_ReceiveData(SPI2);
			}
			GPIO_SetBits(GPIOB, ATT);
			delay_us(5);//2
			if(PSX_Ana != 255){
				try_times++;
				if(try_times == 100){  //��������100�β��ɹ�
					return -1;
				}
				delay_ms(100);
			}
			printf("PSX_Ana = 0x%x\n",PSX_Ana);
		}		
	return 0;
}


void handler_test(void){
	uprintf(USART3,"%x*%x*%x*%x*%x*%x\n",data[3],data[4],data[5],data[6],data[7],data[8]);
}


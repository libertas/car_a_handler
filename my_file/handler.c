#include "handler.h"
#include "global.h"

uint8_t data[10] = {0};
uint8_t data_old[10] = {0};
uint8_t PSX_CONN[5] = {0x01,0x43,0x00,0x01,0x00};                       //进入设置模式
uint8_t PSX_MODE[9] = {0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};  //设置为Analog Mode
uint8_t PSX_POLL[9] = {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //轮询
uint8_t PSX_EXIT[9] = {0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};  //退出，这个在Motor版本里被删掉了
char buff[10] = {0};
uint8_t PSX_Ana = 0x00;	   //进入Analog Mode的标志
int v1=0,v2=0,v3=0,v4=0;
int ctr_v0 = 0,ctr_v1 = 0,ctr_v2 = 0;
int v_x = 0,v_y = 0;
float motor_v[3] = {0};
double X=0,Y=0,dX=0,dY=0;
float degree = 0;  //当前点坐标	 
double vx=0,vy=0,w=0;	   //目标速度
double Dx=0,Dy=0,De=0;    //目标点坐标

void lunxun()
{
  	int i;
	GPIO_ResetBits(GPIOB, ATT); 
	delay_us(5);
			 
	for (i = 0; i < 9; i++)					   //发送轮询数据包
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
	uint8_t PSX_Ana = 0x00;	   //进入Analog Mode的标志
	uint8_t PSX_Ana_check = 0;
	while(PSX_Ana_check != 255)	//0xF3是Analog Mode的Data值   255
	{
	    GPIO_ResetBits(GPIOB, ATT);	   //拉低ATT
			delay_us(5);	
			for (i = 0; i <= 4; i++)	   //进入设置模式
			{ 
			//	myprintf(USART3,"5\r\n");
				SPI_I2S_SendData(SPI2, PSX_CONN[i]); 					//发送   //0x01,0x43,0x00,0x01,0x00
				while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 	//等待接收完成
 				SPI_I2S_ReceiveData(SPI2);				 //读数据
			}			 
		   	GPIO_SetBits(GPIOB, ATT);					 //拉高ATT
			delay_us(5);									 //延时
			GPIO_ResetBits(GPIOB, ATT); 				 //再拉低	
			delay_us(5);
			for (i = 0; i <= 8; i++)					 //设置为Analog Mode
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
			for (i = 0; i <= 8; i++)					 //检查是否设置成功
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
				if(try_times == 100){  //尝试连接100次不成功
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


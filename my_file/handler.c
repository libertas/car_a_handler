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
	//myprintf(USART1,"%d*%d*%d*%d*%d*%d",data[4],data[5],data[6],data[7],data[8],data[9]);
}

void shouDongMove(double a,double b,double c)
{
	v1 = 1.41421*(a+b)/2+c;
	v2 = 1.41421*(a-b)/2+c;
	v3 = -1.41421*(a-b)/2+c;
	v4 = -1.41421*(a+b)/2+c;
	if(v1>MAXv)  {v1=MAXv;}
  	else if(v1<-MAXv) {v1=-MAXv;}
    if(v2>MAXv)	{v2=MAXv;}
    else if (v2<-MAXv) {v2=-MAXv;}
    if(v3>MAXv)	{v3=MAXv;}
    else if(v3<-MAXv)  {v3=-MAXv;}
	if(v4>MAXv)  {v4=MAXv;}
  	else if(v4<-MAXv) {v4=-MAXv;}
	myprintf(USART1, "1v%d\r2v%d\r3v%d\r4v%d\r", v1, v2, v3,v4);
//	LCDMainPrintf("%d %d %d %d",v1, v2, v3,v4);
}
/*L1 L2 R1 R2 ��ϼ�  data[5]����ֵ
	  ȫ�� 240
			 L1	  L2	 R1	 R2          
	L1  251  250  243  249
	
	L2  250  254  246  252
	
	R1  243  246  247  245
	
	R2  249  252  245  253
	*/
void press()
{   
//	int i;
/*	for(i=0;i<=9;i++)
	{
		myprintf(USART1,"[%d]=%d    ",i,data[i]);		
	}  */
	
	myprintf(USART1,"a\r");
	myprintf(USART1, "1ac%d\r2ac%d\r3ac%d\r4ac%d\r", 400, 1000, 400, 1000);
	myprintf(USART1, "1dec%d\r2dec%d\r3dec%d\r4dec%d\r", 500, 500, 500, 500);
//	myprintf(USART1,"1ac300\r");myprintf(USART1,"1dec300\r");
//	myprintf(USART1,"2ac300\r");myprintf(USART1,"2dec300\r");
//	myprintf(USART1,"3ac170\r");myprintf(USART1,"3dec500\r");
//	myprintf(USART1,"4ac800\r");myprintf(USART1,"4dec500\r");
/*	 if(((data[6]&&data[7]&&data[8]&&data[9])==255))		//||((data[6]||data[7]||data[8]||data[9])==0)
	{
		myprintf(USART1,"�ֱ��ѱ�!!!!!!!\r\n");
	}
	else if(((data[6]&&data[7]&&data[8]&&data[9])!=255))	//&&((data[6]||data[7]||data[8]||data[9])!=0)
	{
		myprintf(USART1,"�ֱ��ָ�����~~~~~\r\n");
	}									*/
	if(data[5] == 251)  //L1
	{

		  w = -2500;										//������ʱ����ת
		
	}
	else if(data[5] == 254)  // L2
	{
		myprintf(USART3,"a ");      //a����
	}
		
	else if(data[5] == 247)   //R1   
	{
      w = 2500;										//����˳ʱ����ת	
	
	}
	else if(data[5] == 253) // R2
	{
		myprintf(USART3,"b ");      //b����
	}
	else
	{
		w = 0;
	}
	/*
	if(data[4]==239)	//    up
	{   
		
	}
		
	else if(data[4]==191)	//  down
	{
		 
	}	
		
	else if(data[4]==223)	 //  right	
	{
			
		
	}
	else if(data[4]==127)     //  left
	{		
	
	}
	else
	{
	
	}
	*/
	if(data[5]==191)	//��汻����
	{	
			myprintf(USART3,"x ");
	}	
	if(data[5]==223)	 // ȦȦ
	{
		myprintf(USART3,"o ");
	}		
	if(data[5]==127)	 //���
	{
		myprintf(USART3,"s ");          //����װ�� �̵��������
	}  
	if(data[5]==239)       //����
	{
     myprintf(USART3,"o ");
	}
	if(data[5]==255)	 //����
	{
		myprintf(USART3,"t ");              //ֹͣ
	}
	
	if(data[5]!=239)
	{		
		if((data[9]>=180))  //��ҡ����   	   	
			vy=(160-data[9]);						
		if(data[9]<=90)	    //��ҡ����
		{
			vy=(90-data[9])*shouDongMaxv;			    
		}
		
		if((data[8]>=180))	  //��ҡ����
			vx=-(160-data[8])*shouDongMaxv;
		if(data[8]<=90)	      //��ҡ����
			vx=-(90-data[8]);
												
		 
		/*
		if(data[6]>=150)	    	//��ҡ����
			w=-(140-data[6])*shouDongMaxv;								
		if(data[6]<=90)	       //��ҡ����
			w=-(90-data[6])*shouDongMaxv;					
		if(data[7]<=90)	       //��ҡ����
		{}
		if(data[7]>=150)	   //��ҡ����
		{}
		*/
	
	
		if((data[7]>=160))  //��ҡ����   	   	
			vy=(90-data[9])*shouDongMaxv;
		if(data[7]<=90)	    //��ҡ����
			vy=(160-data[9])*shouDongMaxv;
		if((data[6]>=160))	  //��ҡ����
			vx=-(90-data[8])*shouDongMaxv;		
		if(data[6]<=90)	      //��ҡ����
			vx=-(160-data[8])*shouDongMaxv;
												
		if(data[9]>90&&data[9]<180)
		{
					if(vy > 1000)
						vy -= 900;
					else if (vy < -1000)
						vy += 900;
					else
						vy = 0;
		}

		if((data[8]>90)&&(data[8]<180))
		{
				if(vx > 1000)
					vx -= 900;
				else if(vx < -1000)
					vx += 900;
				else
					vx = 0; 
		}			
		/*
		if(data[6]>=150)	    	//��ҡ����
			w=-(140-data[6])*shouDongMaxv;								
		if(data[6]<=90)	       //��ҡ����
			w=-(90-data[6])*shouDongMaxv;					
		if(data[7]<=90)	       //��ҡ����
		{}
		if(data[7]>=150)	   //��ҡ����
		{}
		*/
	
	}
	shouDongMove(vx,vy,w);
} 


int control(){
//	if(data[4] == ){
//		if((data[8]>=160)){  //��ҡ����   	  
//			ctr_v0=(130-data[8])*shouDongMaxv;	
//			ctr_v2=-(130-data[8])*shouDongMaxv;	
//		}			
//		else if(data[8]<=90)	    //��ҡ����
//		{
//			ctr_v0=(130-data[8])*shouDongMaxv;	
//			ctr_v2=-(130-data[8])*shouDongMaxv;		    
//		}
//		else{
//			ctr_v0 = 0;	
//			ctr_v2 = 0;		  
//		}
//		
		if((data[7]>=160)){	  //��ҡ����
			w=(110-data[7])*shouDongMaxv;
		}
		else if(data[7]<=90){	      //��ҡ����
			w=(110-data[7])*shouDongMaxv;
		}
		else{
			w = 0;
		}
	
//	else{
		if((data[6]>=130)){  //��ҡ����   	  
			v_y=(130-data[6])*shouDongMaxv;	
		}			
		else if(data[6]<=100)	    //��ҡ����
		{
			v_y=(130-data[6])*shouDongMaxv;	  
		}
		else{
			v_y = 0;		  
		}
		
		if((data[5]>=130)){	  //��ҡ����
			v_x=(110-data[5])*shouDongMaxv;
		}
		else if(data[5]<=100){	      //��ҡ����
			v_x=(110-data[5])*shouDongMaxv;
		}
		else{
			v_x = 0;
		}
		myprintf(USART2, "2v%d\r5v%d\r3v%d\r", (int)motor_v[0],(int)motor_v[1], (int)motor_v[2]);
	  //printf(USART2, "v3000\r");
		myprintf(USART1, "5v%d  3v%d  2v%d\n", (int)motor_v[0],(int)motor_v[1], (int)motor_v[2]);
		//	myprintf(USART2, "0v%d\rv%d\r2v%d\r", motor_v[0],motor_v[1], motor_v[2]);
	//}
//	myprintf(USART2, "0v%d\r1v%d\r2v%d\r", ctr_v0,ctr_v1, ctr_v2);
	//myprintf(USART1, "0v%d  1v%d  2v%d\n", ctr_v0,ctr_v1, ctr_v2);
	return 1;
}


int  handle_init(void)
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
						myprintf(USART1,"PSX_Ana = 0x%x\n",PSX_Ana);
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
					myprintf(USART1,"PSX_Ana = 0x%x\n",PSX_Ana);
					myprintf(USART1,"PSX_Ana_check = 0x%x\n",PSX_Ana_check);
				}
				else	 
					SPI_I2S_ReceiveData(SPI2);
			}
			GPIO_SetBits(GPIOB, ATT);
			delay_us(5);//2
			if(PSX_Ana != 255){
				try_times++;
				if(try_times == 100){  //��������30�β��ɹ�
					return -1;
				}
				delay_ms(100);
			}
			printf("PSX_Ana = 0x%x\n",PSX_Ana);
		}		
	return 0;
}


void handler_test(void){
	myprintf(USART1,"%d*%d*%d*%d*%d*%d\n",data[3],data[4],data[5],data[6],data[7],data[8]);
}


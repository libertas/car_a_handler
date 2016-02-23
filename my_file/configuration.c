#include "configuration.h"

GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
USART_InitTypeDef USART_InitStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
DMA_InitTypeDef DMA_Structure; 
EXTI_InitTypeDef EXTI_InitStructure;

TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
TIM_OCInitTypeDef  TIM1_OCInitStructure;//ͨ�������ʼ���ṹ
TIM_ICInitTypeDef   TIM1_ICInitStructure;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;//ͨ�������ʼ���ṹ
TIM_ICInitTypeDef   TIM_ICInitStructure;




void system_clk_set(void)
{ 
	ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
 
  RCC_HSEConfig(RCC_HSE_ON );
 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
  if(HSEStartUpStatus == SUCCESS)     
  {
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
    RCC_PCLK2Config(RCC_HCLK_Div1);   
		RCC_PCLK1Config(RCC_HCLK_Div2);  
 
   // FLASH_SetLatency(FLASH_Latency_2);   
   // FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);      
 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //PLL=8*9=72M
   
    RCC_PLLCmd(ENABLE); 
 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
       {
       }
 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
 
    while(RCC_GetSYSCLKSource() != 0x08)       
       { 
       }
     }
    

}



void rcc_config()
{
	RCC_DeInit();			//��ʼ��Ϊȱʡֵ
	system_clk_set();   //����ϵͳʱ��
	//SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC������.
	//RCC_GetClocksFreq(&RCC_ClockFreq);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
                                                            //�����¼����ƼĴ���/�ⲿ�жϿ��ƼĴ���/��ӳ��ʱ���뿪��AFIOʱ�ӣ������ܽŵ�Ĭ�����蹦�ܲ�����Ҫ��AFIOʱ�ӣ�û����ӳ�䣩
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	 //ʹ��USART1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//ʹ��USART3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 		
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   

}


void GPIO_Configuration(uint16_t GPIO_Pin,GPIOMode_TypeDef  GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed,GPIO_TypeDef* GPIOx)
{
 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;                       //�ܽź� 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;                     //���ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  	GPIO_Init(GPIOx, &GPIO_InitStructure);                        //�ܽ���� 
}

void gpio_config(void)
{
	//δʹ�� ���ô���1  PB6 TX  PB7 RX
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	GPIO_Configuration(GPIO_Pin_7,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOB); //USART1 RX
	GPIO_Configuration(GPIO_Pin_6,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOB);   //USART1 TX
	
	//δʹ�� ���ô���2   PD5 TX PD6 RX
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	GPIO_Configuration(GPIO_Pin_5,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOD);   //USART2 TX
	GPIO_Configuration(GPIO_Pin_6,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOD); //USART2 RX
	//����3    �жϽ�����������
	GPIO_Configuration(GPIO_Pin_10,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOB);   //USART3 TX
	GPIO_Configuration(GPIO_Pin_11,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOB); //USART3 RX
	
	//�ֱ�    PB15=>orange:command PB12=>yellow:attention PB13=>blue:clock PB14=>brown:data
	GPIO_Configuration(GPIO_Pin_14,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOB);
	GPIO_Configuration(GPIO_Pin_12,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOB);
	GPIO_Configuration(GPIO_Pin_13|GPIO_Pin_15,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOB);
		/************************************************************************************
	                 		��ʾ�� 
#define OLED_D0_PORT	GPIOE
#define OLED_D0_Pin		GPIO_Pin_8
#define OLED_D1_PORT	GPIOE
#define OLED_D1_Pin		GPIO_Pin_7

#define OLED_CS_PORT	GPIOG
#define OLED_CS_Pin		GPIO_Pin_0
#define OLED_DC_PORT	GPIOG
#define OLED_DC_Pin		GPIO_Pin_1
	*************************************************************************************/
	GPIO_Configuration(GPIO_Pin_0,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_1,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_8,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOE);
	GPIO_Configuration(GPIO_Pin_7,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOE);
	
	//����ֵ����  DO:PG5  CLK:PG4 CS:PG3
	GPIO_Configuration(GPIO_Pin_5,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOG);  //DO����
	GPIO_Configuration(GPIO_Pin_4,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_3,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	
	//��ʱ�� ��������   ����TIM4 PD12 PD13   TIM5 PA1 PA0
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	GPIO_Configuration(GPIO_Pin_12|GPIO_Pin_13,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOD);  //TIM4��remap
	GPIO_Configuration(GPIO_Pin_0|GPIO_Pin_1,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOA); 
	
	//�����ŷ�  (�̵���)   PG11 PG10
	GPIO_Configuration(GPIO_Pin_11,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_10,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);

	//��դ(�������)   PF1
  GPIO_Configuration(GPIO_Pin_1,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOF);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource1);/*�����ּ����Ӧ�Ĺܽ����ӵ��ڲ��ж��� */ 

	//�̵����ϵ�ϵͳ  PG12
	GPIO_Configuration(GPIO_Pin_12,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_13,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	
}
void tim2_config()        //0.005ms ��ʱ
{
	/*TIM2*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 499;  
  TIM_TimeBaseStructure.TIM_Period = 719;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //������ʱ�ӷָ�
    //��ʱ����ʱ������Ϊ (4+1)*(71+1)/(72*10^6)=0.005ms
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// ����  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void TIM4_Configuration()   //��������
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_DeInit(TIM4);
   
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 29999;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //������ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// ����  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);

  TIM4->CNT=4000;

  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4, ENABLE);	
}

void usart_config(void)
{
	//����1
	USART_InitTypeDef USART_InitStructure; 
	USART_InitStructure.USART_BaudRate = 115200; 	   //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //һ��֡�д��������λ�����ֳ�Ϊ8λ���ݸ�ʽ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; 	//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���ͺͽ���ģʽ
	USART_Init(USART1, &USART_InitStructure);	 //��ʼ������	
	USART_Cmd(USART1, ENABLE);	  //ʹ�ܴ���
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	
	//����2
	USART_InitStructure.USART_BaudRate = 115200; 	   //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //һ��֡�д��������λ�����ֳ�Ϊ8λ���ݸ�ʽ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; 	//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���ͺͽ���ģʽ
	USART_Init(USART2, &USART_InitStructure);	 //��ʼ������	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//����DMA
	USART_Cmd(USART2, ENABLE);	  //ʹ�ܴ���
	
	//����3 ����
	USART_InitStructure.USART_BaudRate = 115200; 	   //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //һ��֡�д��������λ�����ֳ�Ϊ8λ���ݸ�ʽ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; 	//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���ͺͽ���ģʽ
	USART_Init(USART3, &USART_InitStructure);	 //��ʼ������	
	//USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);//����DMA
	USART_Cmd(USART3, ENABLE);	  //ʹ�ܴ���
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);
}

void nvic_config()
{
    //USART3 �ж�
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
	//USART1 �ж�
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
		
		//�����ⲿ�ж� 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	 //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);				 //����NVIC_InitStructure��ָ���Ĳ�����ʼ������NVIC
	
	//TIM2�ж�
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void spi_config(void)  //�ֱ�
{
	SPI_InitTypeDef SPI_Structure;
	SPI_Structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//��Ƶ������ΪCPUʱ��Ƶ��
	SPI_Structure.SPI_CPHA = SPI_CPHA_2Edge;							//�ڶ������ؿ�ʼ����
	SPI_Structure.SPI_CPOL = SPI_CPOL_High;								//ʱ�ӿ���ʱΪ�ߵ�ƽ
	SPI_Structure.SPI_CRCPolynomial = 7;								//CRCУ�����ʽ
	SPI_Structure.SPI_DataSize = SPI_DataSize_8b;						//8λһ����
	SPI_Structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//ȫ˫��
	SPI_Structure.SPI_FirstBit = SPI_FirstBit_LSB;						//С��ģʽ
	SPI_Structure.SPI_Mode = SPI_Mode_Master;							//���豸
	SPI_Structure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_Structure);
	//SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
}





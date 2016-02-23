#include "configuration.h"

GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
USART_InitTypeDef USART_InitStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
DMA_InitTypeDef DMA_Structure; 
EXTI_InitTypeDef EXTI_InitStructure;

TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
TIM_OCInitTypeDef  TIM1_OCInitStructure;//通道输出初始化结构
TIM_ICInitTypeDef   TIM1_ICInitStructure;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;//通道输出初始化结构
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
	RCC_DeInit();			//初始化为缺省值
	system_clk_set();   //配置系统时钟
	//SystemInit();//源自system_stm32f10x.c文件,只需要调用此函数,则可完成RCC的配置.
	//RCC_GetClocksFreq(&RCC_ClockFreq);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
                                                            //配置事件控制寄存器/外部中断控制寄存器/重映射时必须开启AFIO时钟，而开管脚的默认外设功能并不需要开AFIO时钟（没有重映射）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	 //使能USART1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//使能USART3时钟
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
 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;                       //管脚号 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;                     //输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  	GPIO_Init(GPIOx, &GPIO_InitStructure);                        //管脚组别 
}

void gpio_config(void)
{
	//未使用 复用串口1  PB6 TX  PB7 RX
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	GPIO_Configuration(GPIO_Pin_7,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOB); //USART1 RX
	GPIO_Configuration(GPIO_Pin_6,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOB);   //USART1 TX
	
	//未使用 复用串口2   PD5 TX PD6 RX
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	GPIO_Configuration(GPIO_Pin_5,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOD);   //USART2 TX
	GPIO_Configuration(GPIO_Pin_6,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOD); //USART2 RX
	//串口3    中断接受蓝牙数据
	GPIO_Configuration(GPIO_Pin_10,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOB);   //USART3 TX
	GPIO_Configuration(GPIO_Pin_11,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOB); //USART3 RX
	
	//手柄    PB15=>orange:command PB12=>yellow:attention PB13=>blue:clock PB14=>brown:data
	GPIO_Configuration(GPIO_Pin_14,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOB);
	GPIO_Configuration(GPIO_Pin_12,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOB);
	GPIO_Configuration(GPIO_Pin_13|GPIO_Pin_15,GPIO_Mode_AF_PP, GPIO_Speed_50MHz,GPIOB);
		/************************************************************************************
	                 		显示屏 
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
	
	//绝对值码盘  DO:PG5  CLK:PG4 CS:PG3
	GPIO_Configuration(GPIO_Pin_5,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOG);  //DO浮空
	GPIO_Configuration(GPIO_Pin_4,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_3,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	
	//定时器 增量码盘   复用TIM4 PD12 PD13   TIM5 PA1 PA0
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	GPIO_Configuration(GPIO_Pin_12|GPIO_Pin_13,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOD);  //TIM4的remap
	GPIO_Configuration(GPIO_Pin_0|GPIO_Pin_1,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOA); 
	
	//击球电磁阀  (继电器)   PG11 PG10
	GPIO_Configuration(GPIO_Pin_11,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_10,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);

	//光栅(检测落球)   PF1
  GPIO_Configuration(GPIO_Pin_1,GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz,GPIOF);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource1);/*将数字激光对应的管脚连接到内部中断线 */ 

	//继电器断电系统  PG12
	GPIO_Configuration(GPIO_Pin_12,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	GPIO_Configuration(GPIO_Pin_13,GPIO_Mode_Out_PP, GPIO_Speed_50MHz,GPIOG);
	
}
void tim2_config()        //0.005ms 定时
{
	/*TIM2*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 499;  
  TIM_TimeBaseStructure.TIM_Period = 719;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置了时钟分割
    //计时器的时钟周期为 (4+1)*(71+1)/(72*10^6)=0.005ms
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void TIM4_Configuration()   //增量码盘
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_DeInit(TIM4);
   
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 29999;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置了时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上  
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
	//串口1
	USART_InitTypeDef USART_InitStructure; 
	USART_InitStructure.USART_BaudRate = 115200; 	   //设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //一个帧中传输的数据位数（字长为8位数据格式）
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; 	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送和接受模式
	USART_Init(USART1, &USART_InitStructure);	 //初始化串口	
	USART_Cmd(USART1, ENABLE);	  //使能串口
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	
	//串口2
	USART_InitStructure.USART_BaudRate = 115200; 	   //设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //一个帧中传输的数据位数（字长为8位数据格式）
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; 	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送和接受模式
	USART_Init(USART2, &USART_InitStructure);	 //初始化串口	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//允许DMA
	USART_Cmd(USART2, ENABLE);	  //使能串口
	
	//串口3 蓝牙
	USART_InitStructure.USART_BaudRate = 115200; 	   //设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    //一个帧中传输的数据位数（字长为8位数据格式）
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; 	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送和接受模式
	USART_Init(USART3, &USART_InitStructure);	 //初始化串口	
	//USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);//允许DMA
	USART_Cmd(USART3, ENABLE);	  //使能串口
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);
}

void nvic_config()
{
    //USART3 中断
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
	//USART1 中断
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
		
		//配置外部中断 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	 //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);				 //根据NVIC_InitStructure中指定的参数初始化外设NVIC
	
	//TIM2中断
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void spi_config(void)  //手柄
{
	SPI_InitTypeDef SPI_Structure;
	SPI_Structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//分频，分子为CPU时钟频率
	SPI_Structure.SPI_CPHA = SPI_CPHA_2Edge;							//第二个边沿开始采样
	SPI_Structure.SPI_CPOL = SPI_CPOL_High;								//时钟空闲时为高电平
	SPI_Structure.SPI_CRCPolynomial = 7;								//CRC校验多项式
	SPI_Structure.SPI_DataSize = SPI_DataSize_8b;						//8位一个包
	SPI_Structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//全双工
	SPI_Structure.SPI_FirstBit = SPI_FirstBit_LSB;						//小端模式
	SPI_Structure.SPI_Mode = SPI_Mode_Master;							//主设备
	SPI_Structure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_Structure);
	//SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
}





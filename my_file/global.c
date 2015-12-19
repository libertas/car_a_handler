#include "global.h"


int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    return ch;
}

int fgetc(FILE *fp)
{
	int ch = 0;
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)USART1->DR & 0xFF;
	
    //putchar(ch); //����
	
	return ch;
}

void myprintf(USART_TypeDef* USARTx, char *fmt, ...)
{

	char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN�����Լ������
	u8 i = 0;
	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
	USART_ClearFlag(USARTx,USART_FLAG_TXE);
	while ((i < CMD_BUFFER_LEN) && buffer[i])
	{
		if(buffer[i] == '\n'){
			USART_SendData(USARTx,(u8)buffer[i++]);
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
			USART_SendData(USARTx,(u8)'\r');
		}else{
	    USART_SendData(USARTx, (u8) buffer[i++]);
		}
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
	}
	va_end(arg_ptr);
} 



void delay_us(uint32_t us)	   //1us���ӳ٣�����ƫС
{
	char i = 0;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	if (us > 1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
	
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		  	 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = us-1;
		
		TIM_TimeBaseStructure.TIM_Prescaler = 71;
		
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);  		

		while(i<1)
		{
			TIM_Cmd(TIM7, ENABLE);			  //���ڲ�Ƕ�׵�Delay����ʱ����ʹ�����Delay�������ʽ�����д��ѭ�����档

			if (TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) == SET)
			{
				i++;
				TIM_ClearFlag(TIM7, TIM_FLAG_Update);
			}
		}

		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		TIM_Cmd(TIM7,DISABLE);
	}
}

void delay_ms(uint16_t ms)	   //0.1ms���ӳ٣�����ƫС
{
	char i = 0;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	if (ms > 1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
	
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		  	 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = ms-1;
		
		TIM_TimeBaseStructure.TIM_Prescaler = 7199;
		
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);  		

		while(i<1)
		{
			TIM_Cmd(TIM7, ENABLE);			  //���ڲ�Ƕ�׵�Delay����ʱ����ʹ�����Delay�������ʽ�����д��ѭ�����档

			if (TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) == SET)
			{
				i++;
				TIM_ClearFlag(TIM7, TIM_FLAG_Update);
			}
				
		}

		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		TIM_Cmd(TIM7,DISABLE);
	}
}

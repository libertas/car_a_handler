#include "misc.h"

#include "clock.h"

static uint16_t fac_us = 0;			   


void systick_config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock / 1000000 / 8;	

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void clock_delay_us(uint32_t us)
{		
	u32 tmp;

	SysTick->LOAD = us * fac_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;

	do {
		tmp = SysTick->CTRL;
	} while((tmp & 0x01) && !(tmp & (1<<16)));

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}

void delay(uint32_t ms)
{
	while(ms--)
		clock_delay_us(1000);
}

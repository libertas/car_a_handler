#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_spi.h"
#include "misc.h"



void rcc_config(void);
void gpio_config(void);
void tim2_config(void);
void TIM4_Configuration(void);
void usart_config(void);
void nvic_config(void);
void spi_config(void);
void system_clk_set(void);

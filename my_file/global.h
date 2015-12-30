#include "configuration.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"


#define CMD_BUFFER_LEN 100


extern int g_tim2_irq_flg;
extern int g_rotary;
extern float g_round;

void myprintf (USART_TypeDef* USARTx, char *fmt, ...);
void delay_us(uint32_t us);
void delay_ms(uint16_t ms);
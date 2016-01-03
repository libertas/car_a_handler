/*****************************************************
*	定时器2中断对手柄进行轮询，轮询得到的数据放在handler.c文件中的data[]数组里，由于手柄的模式不同，所以data[]数组各个数据
*对应手柄的按键需要你自己测试了，你连上串口，在定时器中断里面，给串口发送data[]数据，按上手柄的各个按钮，看看有什么数据不同。
*
*知道了手柄的相应的数据，你就可以做相应的动作了。在一定的范围内，定时器的周期就相当于手柄的响应时间，定时器周期你也可以自己配
*
*/

/************************************************
*手柄使用的是SPI协议，SPI由在文件configuration.c中的spi_config()函数进行初始化，
*而手柄由在文件handle.c中的handle_init()函数进行初始化
*初始化SPI协议的时候，注意波特率不能太高，如果太高了，手柄是不能够响应得了的，其后果是读到的数据都是255
*spi的波特率由configuration.c中函数rcc_config()里的system_clk_set()调用的	RCC_PCLK1Config() 和 configuration.c中的spi_config()对SPI_Structure.SPI_BaudRatePrescaler所设置 
*共同决定
*
*简单的来说，使能好SPI的时钟，配置好手柄相应的GPIO，调用spi_config() 和 handle_init()函数之后，如果没出现异常，调用lunxun()函数，即可以接收到手柄的数据data[]
*如果读不了数据，先看GPIO有没有配置好，然后再看看spi时钟有没有设置好，再看看spi_config()有没有对spi配置好，最后看看handle_init()函数（主要）
**********************2015-12-30*************************/



#include <stdint.h>

#include "debug.h"
#include "global.h"
#include "handler.h"

#define ABS(x) ((x) >= 0? (x): (-1) * (x))
#define HAND_ZERO 0x0f
#define CMD_TIMES 10
#define BUF_SIZE 17

int g_tim2_irq_flg;
uint8_t tmp_buf[BUF_SIZE] = {0};
uint8_t cmd_buf[BUF_SIZE] = {0};
uint16_t cmd_counter = 0;

float roll_rad = 0, kowtow_rad = 0;

uint8_t cmdcmp(const uint8_t *c0, const uint8_t *c1)
{
	uint8_t len = ((c0[0] & 0xf0) >> 4) + 2;
	uint8_t i;
	for(i = 0; i < len; i++) {
		if(c0[i] != c1[i]) {
			return i + 1;
		}
	}
	return 0;
}

void send_control_data(void)
{
	uint8_t cmd;
	int8_t spd_x, spd_y, r_spd;
	const int8_t spd = 100;
	uint8_t tmp, tmp1;
	uint8_t check_sum;
	uint8_t i;


	if(!(data[4] & 0x10)) {
		// left up key
		#ifdef DEBUG
		printf("lu key\n");
		#endif
		
		cmd = 0x22;
		tmp = 0;
		tmp1 = spd;
		check_sum = cmd + tmp + tmp1;
		tmp_buf[0] = cmd;
		tmp_buf[1] = tmp;
		tmp_buf[2] = tmp1;
		tmp_buf[3] = check_sum;
	} else if(!(data[4] & 0x40)) {
		// left down key
		#ifdef DEBUG
		printf("ld key\n");
		#endif
		
		cmd = 0x22;
		tmp = 0;
		tmp1 = (uint8_t)(-spd);
		check_sum = cmd + tmp + tmp1;
		tmp_buf[0] = cmd;
		tmp_buf[1] = tmp;
		tmp_buf[2] = tmp1;
		tmp_buf[3] = check_sum;
	} else if(!(data[4] & 0x80)) {
		// left left key
		#ifdef DEBUG
		printf("ll key\n");
		#endif
		
		cmd = 0x22;
		tmp = (uint8_t)(-spd);
		tmp1 = 0;
		check_sum = cmd + tmp + tmp1;
		tmp_buf[0] = cmd;
		tmp_buf[1] = tmp;
		tmp_buf[2] = tmp1;
		tmp_buf[3] = check_sum;
	} else if(!(data[4] & 0x20)) {
		// left right key
		#ifdef DEBUG
		printf("lr key\n");
		#endif
		
		cmd = 0x22;
		tmp = spd;
		tmp1 = 0;
		check_sum = cmd + tmp + tmp1;
		tmp_buf[0] = cmd;
		tmp_buf[1] = tmp;
		tmp_buf[2] = tmp1;
		tmp_buf[3] = check_sum;
	} else if(!(data[5] & 0x10)) {
		// right up key
		#ifdef DEBUG
		printf("ru key\n");
		#endif

	} else if(!(data[5] & 0x40)) {
		// right down key
		#ifdef DEBUG
		printf("rd key\n");
		#endif

	} else if(!(data[5] & 0x80)) {
		// right left key
		#ifdef DEBUG
		printf("rl key\n");
		#endif

	} else if(!(data[5] & 0x20)) {

		// right right key
		#ifdef DEBUG
		printf("rr key\n");
		#endif

	} else if(!(data[5] & 0x01)) {

		#ifdef DEBUG
		printf("l2 key\n");
		#endif

	} else if(!(data[5] & 0x04)) {

		#ifdef DEBUG
		printf("l1 key\n");
		#endif

	} else if(!(data[5] & 0x08)) {

		#ifdef DEBUG
		printf("r1 key\n");
		#endif	

	} else if(!(data[5] & 0x02)) {
		#ifdef DEBUG
		printf("r2 key\n");
		#endif
		

	} else {
		r_spd = data[8] - 0x80;
		
		if(ABS(r_spd) > HAND_ZERO) {
			cmd = 0x10;
			tmp_buf[0] = cmd;
			tmp_buf[1] = r_spd;
			tmp_buf[2] = cmd + r_spd;
		
			#ifdef DEBUG
			printf("cmd:0x%x\tr_spd:0x%x\n", (uint8_t)cmd, (uint8_t)r_spd);
			#endif
		} else {
			spd_x = data[6] - 0x80;
			spd_y = 0x7f - data[7];

			if(ABS(spd_x) > HAND_ZERO || ABS(spd_y) > HAND_ZERO) {
				/*
					This will be used later
				*/
				/*
				cmd = 0x22;  // move_xy_c(int8_t spd_x, int8_t spd_y)
				snprintf(tmp_buf, 5, "%c%c%c%c", cmd, spd_x, spd_y, cmd + spd_x + spd_y); 

				#ifdef DEBUG
				printf("0x%x\t0x%x\t0x%x\t0x%x\n", (uint8_t)cmd, (uint8_t)spd_x, (uint8_t)spd_y, (uint8_t)(cmd + spd_x + spd_y));
				#endif
				*/
			} else {
				#ifdef DEBUG
				printf("r_spd:%x\tspd_x:%x\tspd_y:%x\n", (uint8_t)r_spd, (uint8_t)spd_x, (uint8_t)spd_y);
				#endif
			}
		}
	}
	
	
	if(cmdcmp(tmp_buf, cmd_buf) == 0) {
		if(CMD_TIMES <= cmd_counter) {
			for(i = 0; i < ((cmd_buf[0] & 0xf0) >> 4) + 2; i++) {
				USART_SendData(USART1, cmd_buf[i]);
				cmd_buf[i] = 0;
			}
			cmd_counter = 0;
			
			#ifdef DEBUG
			for(i = 0; i < ((tmp_buf[0] & 0xf0) >> 4) + 2; i++) {
				printf("0x%x\t", tmp_buf[i]);
			}
			printf("\n");
			#endif
			
		} else {
			cmd_counter++;
		}
	} else {
		for(i = 0; i < ((tmp_buf[0] & 0xf0) >> 4) + 2; i++) {
			cmd_buf[i] = tmp_buf[i];
		}
		cmd_counter = 0;
	}
}


int main(void){
/****************这是一条各种数据定义的起始线**********************/	
		int erro_num;
	
/****************这是一条各种数据定义的结束线**********************/	
	
	
	
/****************这是一条芯片各种配置的起始线**********************/
  	rcc_config();  //配置时钟，，，在函数里边，调用system_clk_set()函数来配置系统时钟
		gpio_config();  //配置GPIO
		tim2_config(); //使用定时器2，它产生的中断用来对手柄数据进行轮询....tim2先配置好，但不使能那么快，初始化手柄之后再使能
		usart_config();   
		nvic_config();  //配置中断
		spi_config();      //配置SPI，用作手柄
/****************这是一条芯片各种配置的结束线*********************/
	
	
		
/*****************这是一条各种外部设备初始化的起始线*************/	
		erro_num = handle_init();  //初始化手柄
		if(erro_num < 0){
				uprintf(USART3,"handle init failure!!");
				while(1);
		}
/*****************这是一条各种外部设备初始化的结束线*************/	
		TIM_Cmd(TIM2, ENABLE);	 //使能tim2
		
		while(1){
			while(g_tim2_irq_flg == 0);

			g_tim2_irq_flg = 0;
			send_control_data();

				//定时器中断对手柄进行轮询来更新数据，轮询之后跳出中断，在main函数的循环里面执行control()函数来对数据作相应的动作
				//control()函数在文件handler.c里面
				//或者你可以自己写其它函数做相应的动作
		}
	
}



//定时器中断函数,该函数本应该放在stm32f10x_it.c文件里面归类在一起统一管理的，现在为了方便程序的阅读，就搁这了
void TIM2_IRQHandler(void){
  	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ){
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//必须清除中断标志位否则一直中断
			g_tim2_irq_flg = 1;
			lunxun();  //轮询手柄，获得手柄数据，数据保存在由handler.c文件定义的data[]数组里面，这是上一届队员写的程序，不过也被我改了下
			// handler_test();
		}	

}



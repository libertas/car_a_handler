/*****************************************************
*	��ʱ��2�ж϶��ֱ�������ѯ����ѯ�õ������ݷ���handler.c�ļ��е�data[]����������ֱ���ģʽ��ͬ������data[]�����������
*��Ӧ�ֱ��İ�����Ҫ���Լ������ˣ������ϴ��ڣ��ڶ�ʱ���ж����棬�����ڷ���data[]���ݣ������ֱ��ĸ�����ť��������ʲô���ݲ�ͬ��
*
*֪�����ֱ�����Ӧ�����ݣ���Ϳ�������Ӧ�Ķ����ˡ���һ���ķ�Χ�ڣ���ʱ�������ھ��൱���ֱ�����Ӧʱ�䣬��ʱ��������Ҳ�����Լ���
*
*/

/************************************************
*�ֱ�ʹ�õ���SPIЭ�飬SPI�����ļ�configuration.c�е�spi_config()�������г�ʼ����
*���ֱ������ļ�handle.c�е�handle_init()�������г�ʼ��
*��ʼ��SPIЭ���ʱ��ע�Ⲩ���ʲ���̫�ߣ����̫���ˣ��ֱ��ǲ��ܹ���Ӧ���˵ģ������Ƕ��������ݶ���255
*spi�Ĳ�������configuration.c�к���rcc_config()���system_clk_set()���õ�	RCC_PCLK1Config() �� configuration.c�е�spi_config()��SPI_Structure.SPI_BaudRatePrescaler������ 
*��ͬ����
*
*�򵥵���˵��ʹ�ܺ�SPI��ʱ�ӣ����ú��ֱ���Ӧ��GPIO������spi_config() �� handle_init()����֮�����û�����쳣������lunxun()�����������Խ��յ��ֱ�������data[]
*������������ݣ��ȿ�GPIO��û�����úã�Ȼ���ٿ���spiʱ����û�����úã��ٿ���spi_config()��û�ж�spi���úã���󿴿�handle_init()��������Ҫ��
**********************2015-12-30*************************/



#include <stdint.h>
#include <string.h>

#include "stm32f10x_usart.h"

#include "clock.h"
#include "debug.h"
#include "global.h"
#include "handler.h"

#define ABS(x) ((x) >= 0? (x): (-1) * (x))
#define HAND_ZERO 0x3f
#define CMD_TIMES 10
#define BUF_SIZE 17

int g_tim2_irq_flg;
uint8_t cmd_buf[BUF_SIZE] = {0};

float roll_rad = 0, kowtow_rad = 0;


void send_cmd(void)
{
	uint8_t i;
	uint8_t cmd_len;

	// we don't use command 0x00
	if(cmd_buf[0] == 0x00)
		return;
	
	cmd_len = ((cmd_buf[0] & 0xf0) >> 4) + 2;

	#ifdef DEBUG
	printf("sending cmd:");
	for(i = 0; i < cmd_len; i++) {
		printf("0x%x\t", cmd_buf[i]);
	}
	printf("\n");
	#endif

	for(i = 0; i < cmd_len; i++) {
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, (uint8_t) cmd_buf[i]);
		cmd_buf[i] = 0;
	}
}

void send_control_data(void)
{
	uint16_t i;
	uint8_t cmd;
	int8_t spd_x, spd_y, r_spd;
	const int8_t spd = 50;
	uint8_t tmp, tmp1;
	uint8_t check_sum;


	for(;!(data[4] & 0x10);) {
		// left up key
		#ifdef DEBUG
		printf("lu key\n");
		#endif
		static uint8_t lu_count;
		lu_count++;
		if(lu_count < CMD_TIMES)
			break;
		
		lu_count = 0;
		
		cmd = 0x22;
		tmp = 0;
		tmp1 = spd;
		check_sum = cmd + tmp + tmp1;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = tmp1;
		cmd_buf[3] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[4] & 0x40);) {
		// left down key
		#ifdef DEBUG
		printf("ld key\n");
		#endif
		static uint8_t ld_count;
		ld_count++;
		if(ld_count < CMD_TIMES)
			break;
		
		ld_count = 0;
		
		cmd = 0x22;
		tmp = 0;
		tmp1 = (uint8_t)(-spd);
		check_sum = cmd + tmp + tmp1;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = tmp1;
		cmd_buf[3] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[4] & 0x80);) {
		// left left key
		#ifdef DEBUG
		printf("ll key\n");
		#endif
		static uint8_t ll_count;
		ll_count++;
		if(ll_count < CMD_TIMES)
			break;
		
		ll_count = 0;
		
		cmd = 0x22;
		tmp = (uint8_t)(-spd);
		tmp1 = 0;
		check_sum = cmd + tmp + tmp1;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = tmp1;
		cmd_buf[3] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[4] & 0x20);) {
		// left right key
		#ifdef DEBUG
		printf("lr key\n");
		#endif
		static uint8_t lr_count;
		lr_count++;
		if(lr_count < CMD_TIMES)
			break;
		
		lr_count = 0;
		
		cmd = 0x22;
		tmp = spd;
		tmp1 = 0;
		check_sum = cmd + tmp + tmp1;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = tmp1;
		cmd_buf[3] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x10);) {
		// right up key
		#ifdef DEBUG
		printf("ru key\n");
		#endif
		static uint8_t ru_count;
		ru_count++;
		if(ru_count < CMD_TIMES)
			break;
		
		ru_count = 0;

		cmd = 0x12;
		tmp = 1;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x40);) {
		// right down key
		#ifdef DEBUG
		printf("rd key\n");
		#endif
		static uint8_t rd_count;
		rd_count++;
		if(rd_count < CMD_TIMES)
			break;
		
		rd_count = 0;

		cmd = 0x12;
		tmp = 0xff;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x80);) {
		// right left key
		#ifdef DEBUG
		printf("rl key\n");
		#endif
		static uint8_t rl_count;
		rl_count++;
		if(rl_count < CMD_TIMES)
			break;
		
		rl_count = 0;

		cmd = 0x11;
		tmp = 0xff;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x20);) {

		// right right key
		#ifdef DEBUG
		printf("rr key\n");
		#endif
		static uint8_t rr_count;
		rr_count++;
		if(rr_count < CMD_TIMES)
			break;
		
		rr_count = 0;

		cmd = 0x11;
		tmp = 1;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x01);) {

		#ifdef DEBUG
		printf("l2 key\n");
		#endif
		static uint8_t l2_count;
		l2_count++;
		if(l2_count < CMD_TIMES)
			break;
		
		l2_count = 0;
		
		cmd = 0x08;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x04);) {

		#ifdef DEBUG
		printf("l1 key\n");
		#endif
		static uint8_t l1_count;
		l1_count++;
		if(l1_count < CMD_TIMES)
			break;
		
		l1_count = 0;
		
		cmd = 0x07;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x08);) {

		#ifdef DEBUG
		printf("r1 key\n");
		#endif	
		static uint8_t r1_count;
		r1_count++;
		if(r1_count < CMD_TIMES)
			break;
		
		r1_count = 0;
		
		// stop_all()
		cmd = 0x01;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
		send_cmd();
		break;
	}
	
	for(;!(data[5] & 0x02);) {
		#ifdef DEBUG
		printf("r2 key\n");
		#endif
		static uint8_t r2_count;
		r2_count++;
		if(r2_count < CMD_TIMES)
			break;
		
		r2_count = 0;
		
		break;
	}
	/*
	{
		r_spd = data[8] - 0x80;
		
		if(ABS(r_spd) > HAND_ZERO) {
			cmd = 0x10;
			cmd_buf[0] = cmd;
			cmd_buf[1] = r_spd;
			cmd_buf[2] = cmd + r_spd;
			send_cmd();
		
			#ifdef DEBUG
			printf("cmd:0x%x\tr_spd:0x%x\n", (uint8_t)cmd, (uint8_t)r_spd);
			#endif
		} else {
			spd_x = data[6] - 0x80;
			spd_y = 0x7f - data[7];

			if(ABS(spd_x) > HAND_ZERO || ABS(spd_y) > HAND_ZERO) {
				static uint32_t roll_kowtow_count = 0;
				roll_kowtow_count++;
				
				// CMD_TIMES + 2 might be a magic number
				if(0 == roll_kowtow_count / (CMD_TIMES + 2) % 2) {
					cmd = 0x42;
					cmd_buf[0] = cmd;
					roll_rad = (float) spd_x * PI / 256;
				
					memcpy(cmd_buf + 1, &roll_rad, 4);

					cmd_buf[5] = 0;
					for(i = 0; i < 5; i++) {
						cmd_buf[5] += cmd_buf[i];
					}
					send_cmd();
				} else {
					cmd = 0x43;
					cmd_buf[0] = cmd;
					kowtow_rad = (float) spd_y * PI / 256;
					memcpy(cmd_buf + 1, &kowtow_rad, 4);
					cmd_buf[5] = 0;
					for(i = 0; i < 5; i++) {
						cmd_buf[5] += cmd_buf[i];
					}
					send_cmd();
				}
				
				#ifdef DEBUG
				printf("roll:%d\tkowtow:%d\n", spd_x, spd_y);
				#endif
				
			} else {
				#ifdef DEBUG
				printf("r_spd:%x\tspd_x:%x\tspd_y:%x\n", (uint8_t)r_spd, (uint8_t)spd_x, (uint8_t)spd_y);
				#endif
			}
		}
	}
	*/
}


int main(void){
/****************����һ���������ݶ������ʼ��**********************/	
		int erro_num;
	
/****************����һ���������ݶ���Ľ�����**********************/	
	
	
	
/****************����һ��оƬ�������õ���ʼ��**********************/
		systick_config();
		rcc_config();  //����ʱ�ӣ������ں�����ߣ�����system_clk_set()����������ϵͳʱ��
		gpio_config();  //����GPIO
		tim2_config(); //ʹ�ö�ʱ��2�����������ж��������ֱ����ݽ�����ѯ....tim2�����úã�����ʹ����ô�죬��ʼ���ֱ�֮����ʹ��
		usart_config();   
		nvic_config();  //�����ж�
		spi_config();      //����SPI�������ֱ�
/****************����һ��оƬ�������õĽ�����*********************/
	
	
		
/*****************����һ�������ⲿ�豸��ʼ������ʼ��*************/	
		erro_num = handle_init();  //��ʼ���ֱ�
		if(erro_num < 0){
				uprintf(USART3,"handle init failure!!");
				while(1);
		}
/*****************����һ�������ⲿ�豸��ʼ���Ľ�����*************/	
		TIM_Cmd(TIM2, ENABLE);	 //ʹ��tim2


		while(1){
			while(g_tim2_irq_flg == 0);

			g_tim2_irq_flg = 0;
			send_control_data();

				//��ʱ���ж϶��ֱ�������ѯ���������ݣ���ѯ֮�������жϣ���main������ѭ������ִ��control()����������������Ӧ�Ķ���
				//control()�������ļ�handler.c����
				//����������Լ�д������������Ӧ�Ķ���
		}
	
}



//��ʱ���жϺ���,�ú�����Ӧ�÷���stm32f10x_it.c�ļ����������һ��ͳһ����ģ�����Ϊ�˷��������Ķ����͸�����
void TIM2_IRQHandler(void){
  	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ){
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
			g_tim2_irq_flg = 1;
			lunxun();  //��ѯ�ֱ�������ֱ����ݣ����ݱ�������handler.c�ļ������data[]�������棬������һ���Աд�ĳ��򣬲���Ҳ���Ҹ�����
			// handler_test();
		}	

}



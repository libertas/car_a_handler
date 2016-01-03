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
/****************����һ���������ݶ������ʼ��**********************/	
		int erro_num;
	
/****************����һ���������ݶ���Ľ�����**********************/	
	
	
	
/****************����һ��оƬ�������õ���ʼ��**********************/
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



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


int g_tim2_irq_flg;


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
			/*
				while(g_tim2_irq_flg == 0);
				g_tim2_irq_flg = 0;
			*/
				//��ʱ���ж϶��ֱ�������ѯ���������ݣ���ѯ֮�������жϣ���main������ѭ������ִ��control()����������������Ӧ�Ķ���
				//control()�������ļ�handler.c����
				//����������Լ�д������������Ӧ�Ķ���
		}
	
}


void send_control_data(void)
{
	uint8_t cmd;
	int8_t spd_x, spd_y;
	int16_t arg_spd = 2000;
	uint8_t tmp, tmp1;
	uint8_t check_sum;
	
	
	
	if(!(data[4] & 0x10)) {
		// left up key
		#ifdef DEBUG
		printf("lu key\n");
		#endif
		
		cmd = 0x20;
		arg_spd *= -1;
		tmp = ((uint16_t) arg_spd) >> 8;
		tmp1 = (uint8_t) arg_spd;
		check_sum = cmd + tmp + tmp1;
		uprintf(USART1, "%c%c%c%c", cmd, tmp, tmp1, check_sum);
		
		return;
	} else if(!(data[4] & 0x40)) {
		// left down key
		#ifdef DEBUG
		printf("ld key\n");
		#endif
		
		cmd = 0x20;
		tmp = ((uint16_t) arg_spd) >> 8;
		tmp1 = (uint8_t) arg_spd;
		check_sum = cmd + tmp + tmp1;
		uprintf(USART1, "%c%c%c%c", cmd, tmp, tmp1, check_sum);
		
		return;
	} else if(!(data[4] & 0x80)) {
		// left left key
		#ifdef DEBUG
		printf("ll key\n");
		#endif
		
		cmd = 0x21;
		arg_spd *= -1;
		tmp = ((uint16_t) arg_spd) >> 8;
		tmp1 = (uint8_t) arg_spd;
		check_sum = cmd + tmp + tmp1;
		uprintf(USART1, "%c%c%c%c", cmd, tmp, tmp1, check_sum);
		
		return;
	} else if(!(data[4] & 0x20)) {
		// left right key
		#ifdef DEBUG
		printf("lr key\n");
		#endif
		
		cmd = 0x21;
		tmp = ((uint16_t) arg_spd) >> 8;
		tmp1 = (uint8_t) arg_spd;
		check_sum = cmd + tmp + tmp1;
		uprintf(USART1, "%c%c%c%c", cmd, tmp, tmp1, check_sum);
		
		return;
	} else if(!(data[5] & 0x10)) {
		// right up key
		#ifdef DEBUG
		printf("ru key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x40)) {
		// right down key
		#ifdef DEBUG
		printf("rd key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x80)) {
		// right left key
		#ifdef DEBUG
		printf("rl key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x20)) {
		// right right key
		#ifdef DEBUG
		printf("rr key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x01)) {
		#ifdef DEBUG
		printf("l2 key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x04)) {
		#ifdef DEBUG
		printf("l1 key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x08)) {
		#ifdef DEBUG
		printf("r1 key\n");
		#endif
		
		return;
	} else if(!(data[5] & 0x02)) {
		#ifdef DEBUG
		printf("r2 key\n");
		#endif
		
		return;
	} else {
		#ifdef DEBUG
		printf("no key\n");
		printf("%x\n", data[4]);
		#endif
	}
	

	spd_x = data[6] - 0x80;
	spd_y = 0x7f - data[7];
	
	cmd = 0x22;  // move_xy_c(int8_t spd_x, int8_t spd_y)
	uprintf(USART1, "%c%c%c%c", cmd, spd_x, spd_y, cmd + spd_x + spd_y); 
	
	#ifdef DEBUG
	uprintf(USART3, "0x%x\t0x%x\t0x%x\t0x%x\n", (uint8_t)cmd, (uint8_t)spd_x, (uint8_t)spd_y, (uint8_t)(cmd + spd_x + spd_y));
	#endif
}



//��ʱ���жϺ���,�ú�����Ӧ�÷���stm32f10x_it.c�ļ����������һ��ͳһ����ģ�����Ϊ�˷��������Ķ����͸�����
void TIM2_IRQHandler(void){
  	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ){
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
			g_tim2_irq_flg = 1;
			lunxun();  //��ѯ�ֱ�������ֱ����ݣ����ݱ�������handler.c�ļ������data[]�������棬������һ���Աд�ĳ��򣬲���Ҳ���Ҹ�����
			// handler_test();
			send_control_data();
		}	

}



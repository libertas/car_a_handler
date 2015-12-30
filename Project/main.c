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
				myprintf(USART1,"handle init failure!!");
				while(1);
		}
/*****************����һ�������ⲿ�豸��ʼ���Ľ�����*************/	
		TIM_Cmd(TIM2, ENABLE);	 //ʹ��tim2
		
		while(1){
				while(g_tim2_irq_flg == 0);
				g_tim2_irq_flg = 0;
				//��ʱ���ж϶��ֱ�������ѯ���������ݣ���ѯ֮�������жϣ���main������ѭ������ִ��control()����������������Ӧ�Ķ���
				//control()�������ļ�handler.c����
				//����������Լ�д������������Ӧ�Ķ���
				control();  
		}
	
}


//��ʱ���жϺ���,�ú�����Ӧ�÷���stm32f10x_it.c�ļ����������һ��ͳһ�����ģ�����Ϊ�˷��������Ķ����͸�����
void TIM2_IRQHandler(void){
	int i,j;
  	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ){
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
			g_tim2_irq_flg = 1;
			lunxun();  //��ѯ�ֱ�������ֱ����ݣ����ݱ�������handler.c�ļ������data[]�������棬������һ���Աд�ĳ��򣬲���Ҳ���Ҹ�����
			handler_test();
		}	

}


/*****************************************************
*	定时器2中断对手柄进行轮询，轮询得到的数据放在handler.c文件中的data[]数组里，由于手柄的模式不同，所以data[]数组各个数据
*对应手柄的按键需要你自己测试了，你连上串口，在定时器中断里面，给串口发送data[]数据，按上手柄的各个按钮，看看有什么数据不同。
*
*知道了手柄的相应的数据，你就可以做相应的动作了。在一定的范围内，定时器的周期就相当于手柄的响应时间，定时器周期你也可以自己配
*
*/





#include "global.h"
#include "handler.h"


int g_tim2_irq_flg;


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
				myprintf(USART1,"handle init failure!!");
				while(1);
		}
/*****************这是一条各种外部设备初始化的结束线*************/	
		TIM_Cmd(TIM2, ENABLE);	 //使能tim2
		
		printf("Entering main loop\n");
		while(1){
				while(g_tim2_irq_flg == 0);
				g_tim2_irq_flg = 0;
				//定时器中断对手柄进行轮询来更新数据，轮询之后跳出中断，在main函数的循环里面执行control()函数来对数据作相应的动作
				//control()函数在文件handler.c里面
				//或者你可以自己写其它函数做相应的动作
				// control();  
		}
	
}


//定时器中断函数,该函数本应该放在stm32f10x_it.c文件里面归类在一起统一管理的，现在为了方便程序的阅读，就搁这了
void TIM2_IRQHandler(void){
	int i,j;
  	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ){
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);//必须清除中断标志位否则一直中断
			g_tim2_irq_flg = 1;
			lunxun();  //轮询手柄，获得手柄数据，数据保存在由handler.c文件定义的data[]数组里面，这是上一届队员写的程序，不过也被我改了下
		}	

}



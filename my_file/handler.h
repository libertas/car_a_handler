#include "configuration.h"


#define CMD_BUFFER_LEN 100
#define ATT GPIO_Pin_12	       //ATT  PB12
#define PI 3.14159265
#define l 19.052               //���̵����ĵ�ľ���
#define dseterror 0.1 
#define MAXv 9000              //�ﵽ������ٶ�
#define MAXw 2000              //�ﵽ�����ת���ٶ�
#define SLOWv 600
#define shouDongMaxv 10
#define shouDongError 20
#define La 8.2
#define Lb 19.5


void lunxun(void);
void press(void);                      //���ݰ���ִֵ��
void shouDongMove(double a,double b,double c);
int  handle_init(void);
int control(void);
void handler_test(void);

extern uint8_t data[10];
extern uint8_t data_old[10];

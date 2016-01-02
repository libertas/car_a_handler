#include "configuration.h"


#define CMD_BUFFER_LEN 100
#define ATT GPIO_Pin_12	       //ATT  PB12
#define PI 3.14159265
#define l 19.052               //码盘到中心点的距离
#define dseterror 0.1 
#define MAXv 9000              //达到的最大速度
#define MAXw 2000              //达到的最大转向速度
#define SLOWv 600
#define shouDongMaxv 10
#define shouDongError 20
#define La 8.2
#define Lb 19.5


void lunxun(void);
void press(void);                      //根据按键值执行
void shouDongMove(double a,double b,double c);
int  handle_init(void);
int control(void);
void handler_test(void);

extern uint8_t data[10];
extern uint8_t data_old[10];

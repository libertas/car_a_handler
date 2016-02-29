#include <stdbool.h>

#define HAND_ZERO 0x3f
#define CMD_TIMES 10
#define BUF_SIZE 17

#define KEYS_NUM 12

#define LU_KEY 0
#define LD_KEY 1
#define LL_KEY 2
#define LR_KEY 3
#define RU_KEY 4
#define RD_KEY 5
#define RL_KEY 6
#define RR_KEY 7
#define L1_KEY 8
#define L2_KEY 9
#define R1_KEY 10
#define R2_KEY 11

struct key_t {
	uint8_t data_pos;
	uint8_t id;
	bool is_pressed;
	uint16_t pressed_times;
};

void sending_config(void);
void send_control_data(void);

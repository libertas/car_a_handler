#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "debug.h"
#include "handler.h"
#include "sending.h"

float roll_rad = 0, kowtow_rad = 0;

struct key_t keys[KEYS_NUM] = {0};

void sending_config(void)
{
	keys[LU_KEY].data_pos = 4;
	keys[LD_KEY].data_pos = 4;
	keys[LL_KEY].data_pos = 4;
	keys[LR_KEY].data_pos = 4;
	keys[RU_KEY].data_pos = 5;
	keys[RD_KEY].data_pos = 5;
	keys[RL_KEY].data_pos = 5;
	keys[RR_KEY].data_pos = 5;
	keys[L1_KEY].data_pos = 5;
	keys[L2_KEY].data_pos = 5;
	keys[R1_KEY].data_pos = 5;
	keys[R2_KEY].data_pos = 5;

	keys[LU_KEY].id = 0x10;
	keys[LD_KEY].id = 0x40;
	keys[LL_KEY].id = 0x80;
	keys[LR_KEY].id = 0x20;
	keys[RU_KEY].id = 0x10;
	keys[RD_KEY].id = 0x40;
	keys[RL_KEY].id = 0x80;
	keys[RR_KEY].id = 0x20;
	keys[L1_KEY].id = 0x04;
	keys[L2_KEY].id = 0x01;
	keys[R1_KEY].id = 0x08;
	keys[R2_KEY].id = 0x02;
}

void send_cmd(uint8_t *cmd_buf)
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
	
	for(i = 0; i < KEYS_NUM; i++) {
		keys[i].pressed_times = 0;
	}
}

void check_keys(void)
{
	for(uint8_t i = 0; i < KEYS_NUM; i++) {
		struct key_t *key = keys + i;

		key->is_pressed = !(data[key->data_pos] & key->id);

		if(key->is_pressed) {
			key->pressed_times++;
		} else {
			key->is_pressed = 0;
		}
	}
}

bool isp(struct key_t *k)
{
	if(k->pressed_times >= CMD_TIMES)
		return true;
	else
		return false;
}

void send_control_data(void)
{
	uint8_t cmd_buf[BUF_SIZE] = {0};
	uint8_t cmd;
	int8_t spd_x, spd_y, r_spd;
	uint8_t tmp;
	uint8_t check_sum;


	check_keys();
	
	if(isp(keys + L1_KEY)&&
		isp(keys + L2_KEY)&&
		isp(keys + R1_KEY)&&
		isp(keys + R2_KEY)) {//fan_toggle(start & stop)
			
			#ifdef DEBUG_KEYS
			printf("l1 l2 r1 r2 pressed\n");
			#endif

			cmd = 0x0a;
			check_sum = cmd;
			cmd_buf[0] = cmd;
			cmd_buf[1] = check_sum;
			send_cmd(cmd_buf);
			
			delay(200);
	} else if(isp(keys + R1_KEY)&&
			isp(keys + R2_KEY)) {//manual_to_auto
			
			cmd = 0x0b;
			check_sum = cmd;
			cmd_buf[0] = cmd;
			cmd_buf[1] = check_sum;
			send_cmd(cmd_buf);
			
	} else if(isp(keys + RR_KEY)) {

			// stop_all()
			cmd = 0x01;
			check_sum = cmd;
			cmd_buf[0] = cmd;
			cmd_buf[1] = check_sum;
			send_cmd(cmd_buf);

	} else if(isp(keys + RU_KEY)) {

		if(isp(keys + LU_KEY)) {
			cmd = 0x02;
			check_sum = cmd;
			cmd_buf[0] = cmd;
			cmd_buf[1] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LD_KEY)) {
			cmd = 0x09;
			check_sum = cmd;
			cmd_buf[0] = cmd;
			cmd_buf[1] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LL_KEY)) {//right_push_rod pull
			cmd = 0x13;
			tmp = 0x00;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LR_KEY)) {//right_push_rod push
			cmd = 0x13;
			tmp = 0x10;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else {
			keys[RU_KEY].pressed_times = 0;
		}

	} else if(isp(keys + RD_KEY)) {//mag

		if(isp(keys + LD_KEY)) {//mag_far
			cmd = 0x14;
			tmp = 0x00;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LU_KEY)) {//mag_near
			cmd = 0x14;
			tmp = 0x03;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LR_KEY)) {//mag_out
			cmd = 0x14;
			tmp = 0x02;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LL_KEY)){//mag_in
			cmd = 0x14;
			tmp = 0x01;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else {
			keys[RD_KEY].pressed_times = 0;
		}

	} else if(isp(keys + RL_KEY)) {
		if(isp(keys + LL_KEY)) {//left_push_rod pull
			cmd = 0x13;
			tmp = 0x01;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LR_KEY)) {//left_push_rod push
			cmd = 0x13;
			tmp = 0x11;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LU_KEY)) {//fan_kowtow
			cmd = 0x12;
			tmp = 1;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else if(isp(keys + LD_KEY)) {//fan_kowtow
			cmd = 0x12;
			tmp = 0xff;
			check_sum = cmd + tmp;
			cmd_buf[0] = cmd;
			cmd_buf[1] = tmp;
			cmd_buf[2] = check_sum;
			send_cmd(cmd_buf);
		} else {
			keys[RL_KEY].pressed_times = 0;
		}
	} else if(isp(keys + L1_KEY)) {//fan up
		cmd = 0x07;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
		send_cmd(cmd_buf);
	} else if(isp(keys + L2_KEY)) {//fan down
		cmd = 0x08;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
		send_cmd(cmd_buf);
	} else if(isp(keys + R1_KEY)) {//fan_roll left
		cmd = 0x11;
		tmp = 0xff;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
		send_cmd(cmd_buf);
	} else if(isp(keys + R2_KEY)){//fan_roll right
		cmd = 0x11;
		tmp = 1;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
		send_cmd(cmd_buf);
	} else {

		static uint8_t loop_times;
		loop_times++;
		if(loop_times > CMD_TIMES) {
			loop_times = 0;
			for(uint8_t i = 0; i < KEYS_NUM; i++) {
				keys[i].pressed_times = 0;
			}
		}

	}

	
	static uint8_t old_spd_x, old_spd_y, old_spd_r;
	r_spd = 0x7f - data[8];
	spd_x = data[6] - 0x80;
	spd_y = 0x7f - data[7];
	
	if(fabsf(r_spd) < HAND_ZERO) {
		r_spd = 0;
	}
	if(fabsf(spd_x) < HAND_ZERO) {
		spd_x = 0;
	}
	if(fabsf(spd_y) < HAND_ZERO) {
		spd_y = 0;
	}
	
	if(r_spd == old_spd_r && spd_x == old_spd_x && spd_y == old_spd_y) {
		return;
	} else {
		cmd_buf[0] = 0x30;
		cmd_buf[1] = spd_x;
		cmd_buf[2] = spd_y;
		cmd_buf[3] = r_spd;
		cmd_buf[4] = cmd_buf[0] + cmd_buf[1] + cmd_buf[2] + cmd_buf[3];
		send_cmd(cmd_buf);
		
		old_spd_x = spd_x;
		old_spd_y = spd_y;
		old_spd_r = r_spd;
	}
}

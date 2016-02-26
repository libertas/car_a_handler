#include "clock.h"
#include "handler.h"
#include "sending.h"

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
		
		cmd = 0x02;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
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
		
		cmd = 0x09;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
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
		
		cmd = 0x13;
		tmp = 0x00;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
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
		
		cmd = 0x13;
		tmp = 0x01;
		check_sum = cmd + tmp;
		cmd_buf[0] = cmd;
		cmd_buf[1] = tmp;
		cmd_buf[2] = check_sum;
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
		static uint32_t r2_count;
		r2_count++;
		if(r2_count < CMD_TIMES)
			break;
		
		r2_count = 0;
		
		cmd = 0x0a;
		check_sum = cmd;
		cmd_buf[0] = cmd;
		cmd_buf[1] = check_sum;
		send_cmd();
		
		delay(2000);
		
		break;
	}

	{
		r_spd = data[8] - 0x80;
		
		if(ABS(r_spd) > HAND_ZERO) {
			static uint8_t rotate_count;
			rotate_count++;
			if(rotate_count < CMD_TIMES)
				return;

			rotate_count = 0;

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
				static uint8_t move_count = 0;
				move_count++;
				if(move_count < CMD_TIMES)
					return;
				
				move_count = 0;
				
				if(ABS(spd_x) > ABS(spd_y)) {
					
					#ifdef DEBUG
					printf("spd_x = %d > spd_y = %d\n", spd_x, spd_y);
					#endif
					
					cmd = 0x22;
					
					if(spd_x > 0)
						tmp = spd;
					else
						tmp = (uint8_t)(-spd);
					
					tmp1 = 0;
					check_sum = cmd + tmp + tmp1;
					cmd_buf[0] = cmd;
					cmd_buf[1] = tmp;
					cmd_buf[2] = tmp1;
					cmd_buf[3] = check_sum;
					send_cmd();
					
				} else {
					
					#ifdef DEBUG
					printf("spd_x = %d <= spd_y = %d\n", spd_x, spd_y);
					#endif
					
					cmd = 0x22;
					tmp = 0;
					
					if(spd_y > 0)
						tmp1 = spd;
					else
						tmp1 = (uint8_t)(-spd);
					
					check_sum = cmd + tmp + tmp1;
					cmd_buf[0] = cmd;
					cmd_buf[1] = tmp;
					cmd_buf[2] = tmp1;
					cmd_buf[3] = check_sum;
					send_cmd();
				}
				
				send_cmd();
				
			} else {
				#ifdef DEBUG
				printf("r_spd:%x\tspd_x:%x\tspd_y:%x\n", (uint8_t)r_spd, (uint8_t)spd_x, (uint8_t)spd_y);
				#endif
			}
		}
	}
}

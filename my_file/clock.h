#include <stdint.h>

void delay_init(uint8_t SYSCLK);
inline void delay(uint32_t ms);
void systick_config(void);

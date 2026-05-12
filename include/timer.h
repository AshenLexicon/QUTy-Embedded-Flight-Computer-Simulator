#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

extern volatile uint8_t one_second_elapsed;

void timer_init(void);

#endif


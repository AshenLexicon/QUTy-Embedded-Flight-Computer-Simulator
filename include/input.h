#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <avr/io.h>

#define INPUT_ACCEL_UP_bm      PIN4_bm
#define INPUT_ACCEL_DOWN_bm    PIN5_bm
#define INPUT_RESET_bm         PIN6_bm
#define INPUT_PAUSE_bm         PIN7_bm

void input_init(void);
uint8_t input_button_event(void);

#endif
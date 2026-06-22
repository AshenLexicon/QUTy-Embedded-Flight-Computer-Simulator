#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

volatile uint8_t one_second_elapsed = 0;
static volatile uint16_t timer_ticks = 0;

void timer_init(void)
{
    cli(); // Disable global interrupts while configuring timer to ensure no interrupts happen halfway through setup 

    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB0 in periodic interrupt mode

    TCB0.CCMP = 33333; // Set interval for roughly 10 ms: 3,333,000 clocks/sec × 0.01 sec = 33,330 clocks

    TCB0.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable / enable timer interrupt 

    TCB0.CTRLA = TCB_ENABLE_bm;      // Turns the timer on

    sei(); // Enable global interrupts 
}

ISR(TCB0_INT_vect) // Runs whenever TCB0 reaches CCMP and its interrupt is enabled
{

    timer_ticks++; // Timer_ticks +1 

    if (timer_ticks >= 100) // if timer ticks greater then or equal to 100
    {
        timer_ticks = 0; // timer ticks reset to 0 
        one_second_elapsed = 1; // one_second_elapsed equal to 1. The value which will cause the change in the main 
    }

    TCB0.INTFLAGS = TCB_CAPT_bm; // clear the TCB interrupt flag 
}

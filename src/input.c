#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>


#include "input.h"

static uint8_t previous_buttons;

void input_init(void) // initialising the pins required for button input 
{

    PORTA.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm; 

    // Enable Pull up resistors letting unpressed be high and pressed be low (Active Low)
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm; // S1
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm; // S2
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm; // S3 
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm; // S4 

    previous_buttons = VPORTA.IN & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);
}


uint8_t input_button_event(void)
{
    uint8_t current_buttons = VPORTA.IN & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);

    uint8_t falling_edges = previous_buttons & ~current_buttons;

    falling_edges &= (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);

    if (falling_edges) // TODO: Add more sophisticated debouncing method 
    {
        _delay_ms(20);

        uint8_t confirmed_buttons = VPORTA.IN & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);

        falling_edges &= ~confirmed_buttons;
    }

    previous_buttons = current_buttons;

    return falling_edges;
}

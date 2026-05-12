#include <stdint.h>
#include <avr/io.h>

#include "display.h"

// This is a number lookup table for the bit patterns that work with the QUTYS  7-segment LED display  (From my Embedded LED Counter)
// Due to QUTY display being active-low, patterns are backward 
static const uint8_t binary_patterns[10] = { 
    0b10001000, // 0
    0b11101011, // 1
    0b11000100, // 2
    0b11000001, // 3
    0b10100011, // 4 
    0b10010001, // 5
    0b10010000, // 6
    0b11001011, // 7
    0b10000000, // 8
    0b10000001  // 9 
};

// Stored display state.
// These are the digits displayed on the left side of the 7-segment LED display and the right 
static uint8_t display_left = 0; // The left side of the 7-segment LED display 
static uint8_t display_right = 0; // The right side of the 7-segment LED display 

// Tracks which side of the 7-segment LED display should be turned on.
static uint8_t display_side = 0; // Important for flickering either side of the 7-segment LED display at a fast enough speed for the human eye to see two numbers 

// Setting up the pins required for the shift register 
void initial_display(void){

    // This pin is the shift-register clock.
    // Every time you pulse this pin, the shift register captures one bit from MOSI.
    PORTC.DIRSET = PIN0_bm; // CLK Output 
    PORTC.OUTCLR = PIN0_bm; // CLK set at low

    // Sends the bit captured by the shift-register clock into the shift register 
    PORTC.DIRSET = PIN2_bm; // MOSI Output 
    PORTC.OUTCLR = PIN2_bm; // MOSI set as low 

    // Once all the bits have been clocked and moved into the shift register, the Latch pin is pulsed letting the shift register update its display 
    PORTA.DIRSET = PIN1_bm; // LATCH Output 
    PORTA.OUTCLR = PIN1_bm; // LATCH set as low 

    // Start by displaying 00. Ensureing no glitchs occur
    display_number(0);

}

static void latch_shift(uint8_t pattern)
{
    for (uint8_t i = 0; i < 8; i++) // looping through the 8 values of the bit 
    {
        // Check if the current most significant bit is 1
        if (pattern & 0b10000000)
        {
            PORTC.OUTSET = PIN2_bm; // MOSI set as high becoming the current bit value 
        }
        else
        {
            PORTC.OUTCLR = PIN2_bm; // MOSI set as low not becoming the current bit value 
        }

        // Pulse clock letting the shift register capture the MOSI value 
        PORTC.OUTSET = PIN0_bm; // CLK set as high
        PORTC.OUTCLR = PIN0_bm; // CLK

        // Move next bit into the MSB position.
        pattern <<= 1;
    }

    // Pulse latch so shifted data appears on the 7-segment LED display
    PORTA.OUTSET = PIN1_bm; // LATCH set as high 
    PORTA.OUTCLR = PIN1_bm; // LATCH set as low 
}

void display_number(uint8_t number)
{
    // Keep number within 0-99.
    number %= 100;

    display_left = number / 10; // division by 10 gives the tens digit for the left side 
    display_right = number % 10; // gives the ones digit for the right side 
}

void display_flicker(void)
{
    uint8_t pattern; // variable for the latch_shift function output 

    // loop made from the if & else statement 
    if (display_side == 0) // left side 
    {
        // Show left digit.
        pattern = binary_patterns[display_left];

        // Pin for designating for which side of the  7-segment LED display lights up
        pattern |= PIN7_bm; // sets bit 7 to 1. lighting up the left side of the 7-segment LED display

        display_side = 1;
    }
    else // right digit 
    {
        // Show right digit.
        pattern = binary_patterns[display_right];

         // Pin for designating for which side of the  7-segment LED display lights up
        pattern &= ~PIN7_bm; // sets bit 7 to 0. lighting up right side of the 7-segment LED display

        display_side = 0; // go back to the begining of the if & else statement 
    }

    latch_shift(pattern); // input the binary pattern into the function  
}
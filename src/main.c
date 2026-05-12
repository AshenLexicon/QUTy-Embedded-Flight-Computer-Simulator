#include <stdint.h>
#include <stdio.h>

#include "qutyserial.h"
#include "flight_state.h"
#include "timer.h"
#include "input.h"
#include "display.h"

typedef enum // Setting up for state system 
{
    FLIGHT_IDLE,
    FLIGHT_ASCENT,
    FLIGHT_DESCENT,
    FLIGHT_LANDED
} flight_mode_t;

int main(void)
{
    // Initisation of variables 
    serial_init();
    timer_init(); 
    input_init();
    initial_display();

    flight_state_t state;
    initial_flight_state(&state);

    int32_t dt = 1;
    uint8_t t = 0;
    uint8_t paused = 0; 

    flight_mode_t mode = FLIGHT_IDLE;

    display_number(0);

    printf("t,position,velocity,acceleration,mode\n");

    while (1)
    {
        display_flicker(); // Ensure both sides of the 7-segment display appear

        uint8_t events = input_button_event(); 

        // S4: pause/resume from any state
        if (events & INPUT_PAUSE_bm) 
        {
            paused = !paused;
        }

        // S3: reset from any state
        if (events & INPUT_RESET_bm)
        {
            initial_flight_state(&state);

            t = 0;
            paused = 0;
            mode = FLIGHT_IDLE;

            display_number(0);
        }

        // S1: launch from IDLE
        if ((events & INPUT_ACCEL_UP_bm) && mode == FLIGHT_IDLE)
        {
            state.position = 0;
            state.velocity = 10;
            state.acceleration = -1;

            t = 0;
            paused = 0;
            mode = FLIGHT_ASCENT;

            display_number(0);
        }

        if (one_second_elapsed)
        {
            one_second_elapsed = 0;

            if (!paused)
            {
                switch (mode)
                {
                    case FLIGHT_IDLE:
                        printf("%d,%ld,%ld,%ld,%d\n",
                               t,
                               state.position,
                               state.velocity,
                               state.acceleration,
                               mode);

                        display_number(0);
                        break;

                    case FLIGHT_ASCENT:
                    {
                        printf("%d,%ld,%ld,%ld,%d\n",
                               t,
                               state.position,
                               state.velocity,
                               state.acceleration,
                               mode);

                        display_number(state.position % 100);

                        int32_t previous_position = state.position;

                        flight_state_update(&state, dt);
                        t++;

                        if (state.position < previous_position)
                        {
                            mode = FLIGHT_DESCENT;
                        }

                        break;
                    }

                    case FLIGHT_DESCENT:
                        printf("%d,%ld,%ld,%ld,%d\n",
                               t,
                               state.position,
                               state.velocity,
                               state.acceleration,
                               mode);

                        display_number(state.position % 100);

                        flight_state_update(&state, dt);
                        t++;

                        if (state.position <= 0)
                        {
                            state.position = 0;
                            state.velocity = 0;
                            state.acceleration = 0;

                            mode = FLIGHT_LANDED;
                            display_number(0);
                        }

                        break;

                    case FLIGHT_LANDED:
                        printf("%d,%ld,%ld,%ld,%d\n",
                               t,
                               state.position,
                               state.velocity,
                               state.acceleration,
                               mode);

                        display_number(0);
                        break;

                    default:
                        initial_flight_state(&state);
                        state.position = 0;
                        state.velocity = 0;
                        state.acceleration = 0;

                        t = 0;
                        paused = 0;
                        mode = FLIGHT_IDLE;

                        display_number(0);
                        break;
                }
            }
        }
    }
}
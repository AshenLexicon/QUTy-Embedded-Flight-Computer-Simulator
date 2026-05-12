#include <stdint.h>
#include <stdio.h>
#include "flight_state.h"



void initial_flight_state(flight_state_t *state) // function for the flights initial state (SUBJECT TO CHANGE when connecting to actual sensors)
{
    // setting up the initial physics values 
    state->position = 0; 
    state->velocity = 0; 
    state->acceleration = 0;
}

void flight_state_update(flight_state_t *state, int32_t dt) // function for the continous changes in flight_state 
{ 
    
    int32_t old_velocity = state->velocity; // store the current velocity before updating it 
    int32_t old_position = state->position; // store the current position before updating it 

    // not storing accerlation as it is unchanging 

    int32_t new_velocity = old_velocity + state->acceleration * dt; // inputing physics formula and setting it to new velocity 
    int32_t new_position = old_position + old_velocity * dt; // inputing physics fomrula and setting it to new positiona

    state->velocity = new_velocity; // Write the updated velocity back into the state
    state->position = new_position; // Write the updated position back into the state
}


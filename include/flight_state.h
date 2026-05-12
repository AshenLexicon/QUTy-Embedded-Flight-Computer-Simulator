#ifndef FLIGHT_STATE_H
#define FLIGHT_STATE_H

#include <stdint.h>

// Struct definition
typedef struct {
    int32_t position;
    int32_t velocity;
    int32_t acceleration;
} flight_state_t;

// Function prototypes
void initial_flight_state(flight_state_t *state);
void flight_state_update(flight_state_t *state, int32_t dt);

#endif
//
// Created by nikesh on 12/9/24.
//

#ifndef PICO_MAIN_QUAD_SUBSTEP_H
#define PICO_MAIN_QUAD_SUBSTEP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "quadrature_substep_pio.pio.h"


// the quadrature encoder code starts here

typedef struct substep_state_t {
    // configuration data:
    uint calibration_data[4]; // relative phase sizes
    uint clocks_per_us;       // save the clk_sys frequency in clocks per us
    uint idle_stop_samples;   // after these samples without transitions, assume the encoder is stopped
    PIO pio;
    uint sm;

    // internal fields to keep track of the previous state:
    uint prev_trans_pos, prev_trans_us;
    uint prev_step_us;
    uint prev_low, prev_high;
    uint idle_stop_sample_count;
    int speed_2_20;
    int stopped;

    // output of the encoder update function:
    int speed;     // estimated speed in substeps per second
    uint position; // estimated position in substeps

    uint raw_step; // raw step count
} substep_state_t;


void substep_init_state(PIO pio, int sm, int pin_a, substep_state_t *state);

void substep_update(substep_state_t *state);

int *substep_calibrate_phases(PIO pio, uint sm);

void substep_set_calibration_data(substep_state_t *state, int step0, int step1, int step2);

#ifdef __cplusplus
}
#endif
#endif //PICO_MAIN_QUAD_SUBSTEP_H

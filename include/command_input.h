/*
 * command_input.h
 *
 *  Created on: 29.02.2016
 *      Author: andreasbehnke
 */

#ifndef INCLUDE_COMMAND_INPUT_H_
#define INCLUDE_COMMAND_INPUT_H_

#include <stdbool.h>
#include <stdint.h>

struct pwm_command {
        uint8_t command_a;
        uint8_t command_b;
        bool is_neutral_position;
        bool has_command_changed;
};

extern void init_command();

extern struct pwm_command read_combo_command(uint8_t channel);

#endif /* INCLUDE_COMMAND_INPUT_H_ */

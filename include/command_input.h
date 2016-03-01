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

struct command {
        uint8_t command_a;
        uint8_t command_b;
        bool is_halt;
        bool is_break;
        bool has_command_changed;
};

extern void init_command();

extern struct command read_command(uint8_t channel);

#endif /* INCLUDE_COMMAND_INPUT_H_ */

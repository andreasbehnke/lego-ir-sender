/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "include/ir_transmitter.h"
#include "include/power_functions.h"
#include "include/command_input.h"

#define DEBUG_BAUD UART_BAUD_SELECT(9600, F_CPU)
#define REPEAT_COMMAND 5  // repeat every command 5 times
#define WAIT_BETWEEN_REPEATS 15
#define PAUSE 255

static uint8_t counter[] = {0, 0};
static uint8_t repeat_counter[] = {0, 0};
static enum state {
    repeat, // repeat command
    pause, // wait between command repeat
    wait // wait without command repeat
    } state[] = {pause, pause};

static void send_channel(uint8_t channel) {
    struct command cmd = read_command(channel);
    if (cmd.has_command_changed) {
        state[channel] = repeat;
        counter[channel] = 0;
        repeat_counter[channel] = REPEAT_COMMAND;
    }

    switch (state[channel]) {
        case repeat:
            if (counter[channel] == 0) {
                pf_combo_pwm_mode(channel, cmd.command_a, cmd.command_b);
                if (--repeat_counter[channel] == 0) {
                    if (cmd.is_halt) {
                        state[channel] = wait;
                    } else {
                        state[channel] = pause;
                        counter[channel] = PAUSE;
                    }
                } else {
                    counter[channel] = WAIT_BETWEEN_REPEATS;
                }
            } else {
                counter[channel]--;
            }
            break;
        case pause:
            if (--counter[channel] == 0) {
                state[channel] = repeat;
                counter[channel] = 0;
                repeat_counter[channel] = REPEAT_COMMAND;
            }
            break;
        case wait:
            //noop
            break;
        default:
            break;
    }
}

int main() {
    init_command();
    ir_sender_init();
    sei();
    while (1) {
        // channel 1
        send_channel(0);
        // channel 2
        send_channel(1);
    }
}

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
#include "include/adc.h"

#define DEBUG_BAUD UART_BAUD_SELECT(9600, F_CPU)
#define REPEAT_COMMAND 5  // repeat every command 5 times
#define WAIT_BETWEEN_REPEATS 15
#define PAUSE 255

static uint8_t adc_to_pwm[] = {
        0b0111, // forward 7
        0b0110, // forward 6
        0b0101, // forward 5
        0b0100, // forward 4
        0b0011, // forward 3
        0b0010, // forward 2
        0b0001, // forward 1
        0b0000, // float
        0b1111, // backward 1
        0b1110, // backward 2
        0b1101, // backward 3
        0b1100, // backward 4
        0b1011, // backward 5
        0b1010, // backward 6
        0b1001  // backward 7
};

static uint8_t pwm_values[] = {255, 255, 255, 255};
static uint8_t counter[] = {0, 0};
static uint8_t repeat_counter[] = {0, 0};
static enum state {
    repeat, // repeat command
    pause, // wait between command repeat
    wait // wait without command repeat
    } state[] = {pause, pause};

static uint8_t adc_to_combo_pwm(uint8_t adc_value) {
    uint8_t index = adc_value / 17;
    if (index > 14) index = 14;
    return adc_to_pwm[index];
}

static void send_channel(uint8_t channel) {
    uint8_t index_a = channel * 2;
    uint8_t index_b = channel * 2 + 1;
    uint8_t output_a = adc_to_combo_pwm(adc_read(index_a));
    uint8_t output_b = adc_to_combo_pwm(adc_read(index_b));

    bool has_value_changed =  (output_a != pwm_values[index_a] || output_b != pwm_values[index_b]);
    bool is_halt = (output_b == 0 && output_a == 0);

    pwm_values[index_a] = output_a;
    pwm_values[index_b] = output_b;

    if (has_value_changed) {
        state[channel] = repeat;
        counter[channel] = 0;
        repeat_counter[channel] = REPEAT_COMMAND;
    }

    switch (state[channel]) {
        case repeat:
            if (counter[channel] == 0) {
                pf_combo_pwm_mode(channel, output_a, output_b);
                if (--repeat_counter[channel] == 0) {
                    if (is_halt) {
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
    ir_sender_init();
    adc_init();
    sei();
    while (1) {
        // channel 1
        send_channel(0);
        // channel 2
        send_channel(1);
    }
}

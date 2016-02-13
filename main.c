/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "include/ir_transmitter.h"
#include "include/power_functions.h"
#include "include/adc.h"

#define DEBUG_BAUD UART_BAUD_SELECT(9600, F_CPU)

static uint8_t adc_to_pwm[] = {
        0b0111, // 0
        0b0110, // 1
        0b0101, // 2
        0b0100, // 3
        0b0011, // 4
        0b0010, // 5
        0b0001, // 6
        0b0000, // 7
        0b1111, // 8
        0b1110, // 9
        0b1101, // 10
        0b1100, // 11
        0b1011, // 12
        0b1010, // 13
        0b1001  // 14
};

static uint8_t adc_to_combo_pwm(uint8_t adc_value) {
    uint8_t index = adc_value / 17;
    if (index > 14) index = 14;
    return adc_to_pwm[index];
}

int main() {
    uint8_t ch_1a, ch_1b,  ch_2a,  ch_2b;
    ir_sender_init();
    adc_init();
    sei();
    while (1) {
        // channel 1
        ch_1a = adc_to_combo_pwm(adc_read(0));
        ch_1b = adc_to_combo_pwm(adc_read(1));
        pf_combo_pwm_mode(0, ch_1a, ch_1b);

        // channel 2
        ch_2a = adc_to_combo_pwm(adc_read(2));
        ch_2b = adc_to_combo_pwm(adc_read(3));
        pf_combo_pwm_mode(1, ch_2a, ch_2b);
    }
}

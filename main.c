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

static void send_channel(uint8_t channel) {
    uint8_t output_a = adc_to_combo_pwm(adc_read(channel * 2));
    uint8_t output_b = adc_to_combo_pwm(adc_read(channel * 2 + 1));
    pf_combo_pwm_mode(channel, output_a, output_b);
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

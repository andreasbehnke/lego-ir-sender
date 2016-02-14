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
static uint8_t channel_timer[] = {0, 0};

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
    if (output_a != pwm_values[index_a] || output_b != pwm_values[index_b] || channel_timer[channel] == 0) {
        channel_timer[channel] = 255;
        PORTB |= _BV(PB0); // debug led
        pwm_values[index_a] = output_a;
        pwm_values[index_b] = output_b;
        pf_combo_pwm_mode(channel, output_a, output_b);
        PORTB &= ~_BV(PB0); // debug led
    } else {
        channel_timer[channel]--;
    }
}

int main() {
    // debug LED
    DDRB |= _BV(PINB0);
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

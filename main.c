/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <avr/interrupt.h>
#include <util/delay.h>

#include "include/ir_transmitter.h"
#include "include/power_functions.h"

int main() {
    ir_sender_init();
    sei();
    while (1) {

        for (int i = 0; i < 8; ++i) {
            pf_combo_pwm_mode(1, i, 7 - i);
            _delay_us(125);
            pf_combo_pwm_mode(2, i, 7 - i);
            _delay_us(125);
        }

        for (int i = 7; i >= 0; --i) {
            pf_combo_pwm_mode(1, i, 7 - i);
            _delay_us(125);
            pf_combo_pwm_mode(2, i, 7 - i);
            _delay_us(125);
        }
    }
}

/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "include/uart.h"
#include "include/ir_transmitter.h"
#include "include/power_functions.h"

#define DEBUG_BAUD UART_BAUD_SELECT(9600, F_CPU)

#ifdef DEBUG_IR
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
#endif

int main() {
    uart_init(0, DEBUG_BAUD);
    FILE *debug_stream = uart_open_stream(0);
    while (1) {
        fputs("TEST 123\n\r", debug_stream);
        _delay_ms(1000);
    }
}

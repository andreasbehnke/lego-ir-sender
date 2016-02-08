/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <avr/interrupt.h>
#include <util/delay.h>

#include "include/ir_transmitter.h"

int main() {
    ir_sender_init();
    sei();
    while (1) {

        // send example code
        // protocol:
        // start TECC aMMM DDDD LLLL stop
        //       0001 0100 0001 1011        Single output mode, output a PWM 1

        ir_send_message(0b0001010000011011);

        _delay_ms(1 * 16);

        // send example code
        // protocol:
        // start TECC aMMM DDDD LLLL stop
        //       1001 0101 0011 0000        Single output mode, output b PWM 3
        ir_send_message(0b1001010100110000);

        _delay_ms(1 * 16);
        // toggle debug led
        PORTB ^= _BV(PB0);
    }
}

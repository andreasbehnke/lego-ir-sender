/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <avr/interrupt.h>
#include <util/delay.h>

#include "include/ir_transmitter.h"

static inline void send_start__stop() {
    ir_pulse(6, 39);
    ir_wait();
}

static inline void send_low() {
    ir_pulse(6, 10);
    ir_wait();
}

static inline void send_high() {
    ir_pulse(6, 21);
    ir_wait();
}

int main() {
    ir_sender_init();
    sei();
    while (1) {

        // send example code
        // protocol:
        // start TECC aMMM DDDD LLLL stop
        //       0001 0001 0101 1010         Combo direct mode, both output forward
//        send_start__stop();
//
//        send_low();
//        send_low();
//        send_low();
//        send_high();
//
//        send_low();
//        send_low();
//        send_low();
//        send_high();
//
//        send_low();
//        send_high();
//        send_low();
//        send_high();
//
//        send_high();
//        send_low();
//        send_high();
//        send_low();
//
//        send_start__stop();

        // send example code
        // protocol:
        // start TECC aMMM DDDD LLLL stop
        //       0001 0100 0001 1011        Single output mode, output a PWM 1
        send_start__stop();

        send_low();
        send_low();
        send_low();
        send_high();

        send_low();
        send_high();
        send_low();
        send_low();

        send_low();
        send_low();
        send_low();
        send_high();

        send_high();
        send_low();
        send_high();
        send_high();

        send_start__stop();

        _delay_ms(1 * 16);

        // send example code
        // protocol:
        // start TECC aMMM DDDD LLLL stop
        //       1001 0101 0011 0000        Single output mode, output b PWM 3
        send_start__stop();

        send_high();
        send_low();
        send_low();
        send_high();

        send_low();
        send_high();
        send_low();
        send_high();

        send_low();
        send_low();
        send_high();
        send_high();

        send_low();
        send_low();
        send_low();
        send_low();

        send_start__stop();

        _delay_ms(1 * 16);
        // toggle debug led
        PORTB ^= _BV(PB0);
    }
}

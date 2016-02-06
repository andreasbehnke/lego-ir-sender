/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <avr/interrupt.h>
#include "include/ir_sender.h"

int main() {
    ir_sender_init();
    sei();
    while (1) {
        ir_pulse(100, 10);
        ir_wait();
    }
}

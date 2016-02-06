/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include "include/ir_sender.h"

int main() {
    ir_sender_init();
    ir_pulse(6);
    while (1) {
    }
}

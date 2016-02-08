/*
 * ir_sender.h
 *
 *  Created on: 10.01.2016
 *      Author: andreasbehnke
 */

#ifndef INCLUDE_IR_TRANSMITTER_H_
#define INCLUDE_IR_TRANSMITTER_H_

#include <inttypes.h>

// PWM signal with period based on prescale
// for 38kHz signal and 50% duty cycle
#define IR_FREQ 38000UL
#define IR_PRESCALE 1
#define IR_PERIOD (F_CPU/IR_PRESCALE/IR_FREQ)
#define IR_DUTY_CYCLE (IR_PERIOD / 2)

// ir module API
extern void ir_sender_init();

extern void ir_pulse(uint8_t pulse_count, uint8_t pause_count);

extern void ir_wait();

#endif /* INCLUDE_IR_TRANSMITTER_H_ */

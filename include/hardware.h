/*
 * hardware.h
 *
 *  Created on: 06.02.2016
 *      Author: andreasbehnke
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>

// hardware PWM generator used for ir signals
#define IR_TCCRxA TCCR2A
#define IR_TCCRxB TCCR2B
#define IR_OCRxA OCR2A
#define IR_OCRxB OCR2B

// PWM signal with period based on prescale
// for 38kHz signal and 50% duty cycle
#define IR_FREQ 38000UL
#define IR_PRESCALE 1
#define IR_PERIOD (F_CPU/IR_PRESCALE/IR_FREQ)
#define IR_DUTY_CYCLE (IR_PERIOD / 2)

#endif /* HARDWARE_H_ */

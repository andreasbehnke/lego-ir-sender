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
// PWM output pin
#define IR_INIT_PIN OC0A_DDR |= _BV(OC0A_BIT)
#define IR_CLEAR_PIN OC0A_PORT |= _BV(OC0A_BIT)

// - CTC Mode
// - toggle OC0A on compare match
#define IR_INIT_TCCRxA TCCR0A = _BV(COM0A0) | _BV(WGM01)
#define IR_INIT_TCCRxB TCCR0B = 0

// do not use prescaler
#define IR_PRESCALER _BV(CS00)
#define IR_START_TIMER TCCR0B |= IR_PRESCALER
#define IR_STOP_TIMER TCCR0B &= ~(IR_PRESCALER)

// PWM signal with period based on prescale
// for 38kHz signal and 50% duty cycle
#define IR_FREQ 38000UL
#define IR_PRESCALE 1
#define IR_PERIOD (F_CPU/IR_PRESCALE/IR_FREQ)
#define IR_DUTY_CYCLE (IR_PERIOD / 2)

#define IR_INIT_OCRxA OCR0A = IR_DUTY_CYCLE

#endif /* HARDWARE_H_ */

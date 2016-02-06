/*
 * ir_sender.h
 *
 *  Created on: 10.01.2016
 *      Author: andreasbehnke
 */

#ifndef INCLUDE_IR_SENDER_H_
#define INCLUDE_IR_SENDER_H_

#include <inttypes.h>

// ir module API
extern void ir_sender_init();

extern void ir_pulse(uint8_t pulse_count);

extern void ir_wait();

#endif /* INCLUDE_IR_SENDER_H_ */

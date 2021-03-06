/*
 * ir_sender.h
 *
 *  Created on: 10.01.2016
 *      Author: andreasbehnke
 */

#ifndef INCLUDE_IR_TRANSMITTER_H_
#define INCLUDE_IR_TRANSMITTER_H_

#include <inttypes.h>

// ir module API
extern void ir_sender_init();

extern void ir_send_message(uint16_t message);

#endif /* INCLUDE_IR_TRANSMITTER_H_ */

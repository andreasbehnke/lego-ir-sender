/*
 * power_functions.h
 *
 *  Created on: 08.02.2016
 *      Author: andreasbehnke
 */

#ifndef INCLUDE_POWER_FUNCTIONS_H_
#define INCLUDE_POWER_FUNCTIONS_H_

#include <inttypes.h>

// power functions API

// calculate longitudinal redundancy check and return complete command
extern uint16_t pf_calculate_lrc(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3);

#endif /* INCLUDE_POWER_FUNCTIONS_H_ */

/*
 * adc.h
 *
 *  Created on: 13.02.2016
 *      Author: andreasbehnke
 */

#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_

#include <stdint.h>

#define CHANNEL_DIRECTION 0b00000001

extern void adc_init();

extern uint8_t adc_read(uint8_t channel);

#endif /* INCLUDE_ADC_H_ */

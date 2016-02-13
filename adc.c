/*
 * adc.c
 *
 *  Created on: 13.02.2016
 *      Author: andreasbehnke
 */

#include "include/adc.h"
#include <avr/io.h>

void adc_init() {
    // left align because we only need top 3 bits
    ADMUX = (1 << ADLAR);
    // set optimal prescale for 3MHz
    ADCSRA = (1 << ADPS2) | (1 << ADPS1);
    // activate adc
    ADCSRA |= (1 << ADEN);

    // test read
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {}
    (void) ADCH;
}

uint8_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {}
    return ADCH;
}

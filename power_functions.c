/*
 * power_functions.c
 *
 *  Created on: 08.02.2016
 *      Author: andreasbehnke
 */

#include "include/power_functions.h"
#include "include/ir_transmitter.h"

// calculate longitudinal redundancy check and return complete command
static uint16_t calculate_lrc(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {
    uint8_t lrc = 0x0F ^ nibble1 ^ nibble2 ^ nibble3;
    return nibble1 << 12 | (nibble2 & 0x0F) << 8 | (nibble3 & 0x0F) << 4 | (lrc & 0x0F );
}

extern void pf_combo_pwm_mode(uint8_t channel, uint8_t output_a, uint8_t output_b) {
    ir_send_message(
            calculate_lrc(
                    (channel & 0x03) | 0b0100, // a 1 C C
                    output_b,                  // b b b b
                    output_a                   // a a a a
                )
            );
}

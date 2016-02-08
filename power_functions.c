/*
 * power_functions.c
 *
 *  Created on: 08.02.2016
 *      Author: andreasbehnke
 */

#include "include/power_functions.h"

extern uint16_t pf_calculate_lrc(uint8_t nibble1, uint8_t nibble2, uint8_t nibble3) {
    uint8_t lrc = 0x0F ^ nibble1 ^ nibble2 ^ nibble3;
    return nibble1 << 12 | nibble2 << 8 | nibble3 << 4 | (lrc & 0x0F );
}

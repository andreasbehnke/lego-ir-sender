/*
 * main.c
 *
 *  Created on: 09.01.2016
 *      Author: andreasbehnke
 */
#include <avr/io.h>
#include <util/delay.h>
#define LED_PIN  (1<<PA1)
#define LED_DDR  DDRA
#define LED_PORT PORTA

inline void led_init() {
    LED_DDR |= LED_PIN;
}

inline void led_on() {
    LED_PORT |= LED_PIN;
}

inline void led_off() {
    LED_PORT &= ~LED_PIN;
}

inline void led_toggle() {
    LED_PORT ^= LED_PIN;
}

int main() {
    led_init();
    while(1) {
        led_on();
        _delay_ms(200);
        led_off();
        _delay_ms(800);
    }
}

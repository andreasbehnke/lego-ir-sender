#include <avr/interrupt.h>
#include <util/atomic.h>
#include "include/ir_sender.h"

#define IR_STATE_STOPPED 0
#define IR_STATE_PULSE 1
#define IR_STATE_PAUSE 2

volatile uint8_t ir_state = IR_STATE_STOPPED;
volatile uint8_t ir_pulse_count;
volatile uint8_t ir_pause_count;

static inline void ir_clear_pin() {
    OC0A_PORT &= ~(_BV(OC0A_BIT));
}

static inline void ir_enable_pwm_output() {
    TCCR0A |= _BV(COM0A0);
}

static inline void ir_disable_pwm_output() {
    TCCR0A &= ~(_BV(COM0A0));
}

inline void ir_sender_init() {
    TCNT0 = 0;
    // PWM output pin
    OC0A_DDR |= _BV(OC0A_BIT);
    ir_clear_pin();
    // Clear Timer on Compare Match (CTC) Mode
    TCCR0A = _BV(WGM01);
    // do not use prescaler
    TCCR0B = _BV(CS00);
    // match value
    OCR0A = IR_DUTY_CYCLE;
    // enable interrupt on compare match
    TIMSK0 |= _BV(OCIE0A);

    // debug LED
    DDRB |= _BV(PINB0);
}

void ir_pulse(uint8_t pulse_count, uint8_t pause_count) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // interrupt is triggered for every half pulse,
        // multiply pulse count by 2
        ir_clear_pin();
        ir_enable_pwm_output();
        ir_pulse_count = pulse_count * 2;
        ir_pause_count = pause_count * 2;
        ir_state = IR_STATE_PULSE;
    }
}

ISR(TIMER0_COMPA_vect)
{
    switch (ir_state) {

        case IR_STATE_PULSE:
            if (ir_pulse_count == 0) {
                ir_disable_pwm_output();
                ir_clear_pin();
                // all pulse send, now wait ir_pause_count cycles
                ir_state = IR_STATE_PAUSE;
            } else {
                ir_pulse_count--;
            }
            break;

        case IR_STATE_PAUSE:
            if (ir_pause_count == 0) {
                // end of pause, signal everyone that we are ready
                // for next bit
                ir_state = IR_STATE_STOPPED;
            } else {
                ir_pause_count--;
            }
            break;

        default:
            ir_state = IR_STATE_STOPPED;
            break;
    }
}

void ir_wait() {
    while(ir_state != IR_STATE_STOPPED) {}
}

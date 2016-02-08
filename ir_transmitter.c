#include <avr/interrupt.h>

#include "include/ir_transmitter.h"

static const uint8_t IR_PULSE_COUNT = 6 * 2;
static const uint8_t IR_START_STOP_COUNT = 39 * 2;
static const uint8_t IR_LOW_COUNT = (10 - 3) * 2;
static const uint8_t IR_HIGH_COUNT = (21 - 3) * 2;

volatile static enum state {pulse, pause, stopped} ir_state;
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

ISR(TIMER0_COMPA_vect)
{
    switch (ir_state) {

        case pulse:
            if (ir_pulse_count == 0) {
                ir_disable_pwm_output();
                ir_clear_pin();
                // all pulse send, now wait ir_pause_count cycles
                ir_state = pause;
            } else {
                ir_pulse_count--;
            }
            break;

        case pause:
            if (ir_pause_count == 0) {
                // end of pause, signal everyone that we are ready
                // for next bit
                ir_state = stopped;
            } else {
                ir_pause_count--;
            }
            break;

        default:
            ir_state = stopped;
            break;
    }
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

static inline void send_start_stop() {
    ir_pulse(IR_PULSE_COUNT, IR_START_STOP_COUNT);
    ir_wait();
}

static inline void send_low() {
    ir_pulse(IR_PULSE_COUNT, IR_LOW_COUNT);
    ir_wait();
}

static inline void send_high() {
    ir_pulse(IR_PULSE_COUNT, IR_HIGH_COUNT);
    ir_wait();
}

void inline ir_pulse(uint8_t pulse_count, uint8_t pause_count) {
    // interrupt is triggered for every half pulse,
    // multiply pulse count by 2
    ir_clear_pin();
    ir_enable_pwm_output();
    ir_pulse_count = pulse_count;
    ir_pause_count = pause_count;
    ir_state = pulse;
}

void ir_send_message(uint16_t message) {
    send_start_stop();
    uint8_t i = 16;
    do {
        i--;
        if (message & (1 << i)) {
            send_high();
        } else {
            send_low();
        }
    } while(i > 0);
    send_start_stop();
}

void inline ir_wait() {
    while(ir_state != stopped) {}
}

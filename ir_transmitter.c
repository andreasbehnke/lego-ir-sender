#include <avr/interrupt.h>

#include "include/ir_transmitter.h"

// PWM signal with period based on prescale
// for 38kHz signal and 50% duty cycle
#define IR_FREQ 38000UL
#define IR_PRESCALE 1
#define IR_PERIOD (F_CPU/IR_PRESCALE/IR_FREQ)
#define IR_DUTY_CYCLE (IR_PERIOD / 2)

#define  IR_PULSE_CORRECTION 3
#define  IR_PULSE_COUNT 6 * 2
#define  IR_START_STOP_COUNT 39 * 2
#define  IR_LOW_COUNT (10 - IR_PULSE_CORRECTION) * 2
#define  IR_HIGH_COUNT (21 - IR_PULSE_CORRECTION) * 2

volatile static enum state {pulse, pause, stopped} state;
uint8_t ir_pulse_count;
uint8_t ir_pause_count;

static inline void clear_output_pin() {
    OC0A_PORT &= ~(_BV(OC0A_BIT));
}

static inline void enable_pwm_output() {
    TCCR0A |= _BV(COM0A0);
}

static inline void disable_pwm_output() {
    TCCR0A &= ~(_BV(COM0A0));
}

ISR(TIMER0_COMPA_vect)
{
    switch (state) {

        case pulse:
            if (ir_pulse_count == 0) {
                disable_pwm_output();
                clear_output_pin();
                // all pulse send, now wait ir_pause_count cycles
                state = pause;
            } else {
                ir_pulse_count--;
            }
            break;

        case pause:
            if (ir_pause_count == 0) {
                // end of pause, signal everyone that we are ready
                // for next bit
                state = stopped;
            } else {
                ir_pause_count--;
            }
            break;

        default:
            state = stopped;
            break;
    }
}

inline void ir_sender_init() {
    TCNT0 = 0;
    // PWM output pin
    OC0A_DDR |= _BV(OC0A_BIT);
    clear_output_pin();
    // Clear Timer on Compare Match (CTC) Mode
    TCCR0A = _BV(WGM01);
    // do not use prescaler
    TCCR0B = _BV(CS00);
    // match value
    OCR0A = IR_DUTY_CYCLE;
    // enable interrupt on compare match
    TIMSK0 |= _BV(OCIE0A);
}

static void send_pulse(uint8_t pulse_count, uint8_t pause_count) {
    clear_output_pin();
    enable_pwm_output();
    ir_pulse_count = pulse_count;
    ir_pause_count = pause_count;
    state = pulse;
}

static inline void wait() {
    while(state != stopped) {}
}

static inline void send_start_stop() {
    send_pulse(IR_PULSE_COUNT, IR_START_STOP_COUNT);
    wait();
}

static inline void send_low() {
    send_pulse(IR_PULSE_COUNT, IR_LOW_COUNT);
    wait();
}

static inline void send_high() {
    send_pulse(IR_PULSE_COUNT, IR_HIGH_COUNT);
    wait();
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


#include "include/hardware.h"
#include "include/ir_sender.h"

inline void ir_sender_init() {
    IR_INIT_PIN;
    IR_INIT_TCCRxA;
    IR_INIT_TCCRxB;
    IR_INIT_OCRxA;
    IR_CLEAR_PIN;
}

void ir_pulse(uint8_t pulse_count) {
    IR_CLEAR_PIN;
    IR_START_TIMER;
}

void ir_wait() {

}

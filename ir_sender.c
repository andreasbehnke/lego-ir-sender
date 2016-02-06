
#include "include/hardware.h"
#include "include/ir_sender.h"

inline void ir_sender_init() {

}

void ir_pulse(uint8_t pulse_count) {
    IR_TCCRxA = 3;
}

void ir_wait() {

}

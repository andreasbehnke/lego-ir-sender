#include "include/command_input.h"
#include <avr/io.h>

extern void init_command() {
    DDRC = 0x00; // all pins are input pins
    PORTC = 0xFF; // activate all pullup resistors
}

extern struct command read_command(uint8_t channel) {
    struct command command;

    return command;
}

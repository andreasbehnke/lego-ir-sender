#include "include/command_input.h"
#include "include/adc.h"
#include <avr/io.h>

static uint8_t adc_to_pwm[] = {
        0b0111, // forward 7
        0b0110, // forward 6
        0b0101, // forward 5
        0b0100, // forward 4
        0b0011, // forward 3
        0b0010, // forward 2
        0b0001, // forward 1
        0b0000, // float
        0b1111, // backward 1
        0b1110, // backward 2
        0b1101, // backward 3
        0b1100, // backward 4
        0b1011, // backward 5
        0b1010, // backward 6
        0b1001  // backward 7
};

static uint8_t last_command[] = {255, 255, 255, 255};
static bool is_break[] = {false, false};

extern void init_command() {
    DDRC = 0x00; // all pins are input pins
    PORTC = 0xFF; // activate all pullup resistors
    adc_init();
}

static uint8_t adc_to_combo_pwm(uint8_t adc_value) {
    uint8_t index = adc_value / 17;
    if (index > 14) index = 14;
    return adc_to_pwm[index];
}

extern struct pwm_command read_combo_command(uint8_t channel) {
    struct pwm_command command;
    uint8_t index_a = channel * 2;
    uint8_t index_b = channel * 2 + 1;
    command.command_a = adc_to_combo_pwm(adc_read(index_a));
    command.command_b = adc_to_combo_pwm(adc_read(index_b));

    if (!(PINC & (1 << channel))) {
        // break switch pressed
        command.command_a = 0b1000;
        command.command_b = 0b1000;
        is_break[channel] = true;
    }

    command.is_halt = (command.command_b == 0 && command.command_a == 0);

    if (command.is_halt && is_break[channel]) {
        // release break, if all joysticks are in neutral halt position
        is_break[channel] = false;
    }
    if (is_break[channel]) {
        // only release break after reaching neutral position of joystick,
        // override command.
        command.command_a = 0b1000;
        command.command_b = 0b1000;
    }

    command.has_command_changed =  (command.command_a != last_command[index_a] || command.command_b != last_command[index_b]);

    last_command[index_a] = command.command_a;
    last_command[index_b] = command.command_b;

    return command;
}

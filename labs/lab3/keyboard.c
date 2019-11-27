#include <lcom/lcf.h>
#include "keyboard.h"
#include "macros/keyboard.h"
#include "macros/error.h"

/// --------------------------------------------------------------------------------
///                                      KBC
/// --------------------------------------------------------------------------------

static uint8_t status;

int kbc_get_status() {
    uint32_t aux;
    error(sys_inb(STATUS_REGISTER, &aux), "Error reading status from keyboard");
    status = (uint8_t) aux;
    error(status & PARITY_ERROR, "Parity error when reading status");
    error(status & TIMEOUT_ERROR, "Timeout error when reading status");
    return 0;
}

int kbc_write_command(uint32_t command) {
    for (char i = 0; i < MAX_TRIES; ++i) {
        kbc_get_status();
        if (status & FULL_IN_BUFFER) {
            error(sys_outb(COMMAND_REGISTER, command), "Error writing command to register");
            return 0;
        }
        delay(DELAY_US);
    }
    error(true, "Max number of tries reached when trying to write command");
}

int kbc_read_return(uint16_t *data) {
    for (char i = 0; i < MAX_TRIES; ++i) {
        kbc_get_status();
        if (status & FULL_OUT_BUFFER) {
            uint32_t aux;
            error(sys_inb(OUT_BUFFER, &aux), "Error reading output buffer");
            *data = (uint16_t) aux;
            return 0;
        }
        delay(DELAY_US);
    }
    error(true, "Max number of tries reached when trying to read output buffer");
}

/// --------------------------------------------------------------------------------
///                                  Interruptions
/// --------------------------------------------------------------------------------

struct scancode {
    bool mode;
    uint8_t size;
    uint8_t bytes[2];
};

int keyboard_hook_id = KEYBOARD_IRQ_LINE;

int keyboard_subscribe_int(uint8_t *bit_no) {
    *bit_no = BIT(keyboard_hook_id);
    error(sys_irqsetpolicy(KEYBOARD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id),
          "Error on keyboard interruptions subscription");
    return 0;
}

int keyboard_unsubscribe_int() {
    error(sys_irqrmpolicy(&keyboard_hook_id), "Error on timer interruptions unsubscription");
    return 0;
}

struct scancode last_scancode;

uint16_t keyboard_get_last_scancode() {
    return (last_scancode.bytes[1] << 8) | last_scancode.bytes[0];
}

void keyboard_int_handler() {
    if (kbc_get_status())
        return;
    uint16_t scancode;
    if(kbc_read_return(&scancode))
        return;
    last_scancode.mode = (scancode & BREAKCODE) ? false : true;
    last_scancode.bytes[0] = scancode;
    last_scancode.bytes[1] = scancode >> 8;
    last_scancode.size = (last_scancode.bytes[1] == LONG_SCANCODE) ? 2 : 1;
}

/// --------------------------------------------------------------------------------
///                            Functions only for the lab
/// --------------------------------------------------------------------------------

void keyboard_print_last_scancode() {
    kbd_print_scancode(last_scancode.mode, last_scancode.size, last_scancode.bytes);
}

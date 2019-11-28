#include <lcom/lcf.h>
#include "keyboard.h"
#include "kbc.h"
#include "macros/error.h"

/// --------------------------------------------------------------------------------
///                                  Interruptions
/// --------------------------------------------------------------------------------

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

struct scancode {
    bool mode;
    uint8_t size;
    uint8_t bytes[2];
};

struct scancode last_scancode;

uint16_t keyboard_get_last_scancode() {
    return (last_scancode.bytes[1] << 8) | last_scancode.bytes[0];
}

void keyboard_int_handler() {
    uint16_t scancode;
    if (kbc_read_return(&scancode))
        return;
    last_scancode.mode = (scancode & BREAKCODE) ? false : true;
    last_scancode.bytes[0] = scancode;
    last_scancode.bytes[1] = scancode >> 8;
    last_scancode.size = (last_scancode.bytes[1] == LONG_SCANCODE) ? 2 : 1;
    keyboard_print_last_scancode();
}

/// --------------------------------------------------------------------------------
///                            Functions only for the lab
/// --------------------------------------------------------------------------------

void keyboard_print_last_scancode() {
    kbd_print_scancode(last_scancode.mode, last_scancode.size, last_scancode.bytes);
}

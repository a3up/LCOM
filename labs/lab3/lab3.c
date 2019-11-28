#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>

#include "interrupts.h"
#include "timer.h"
#include "keyboard.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    // lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    // lcf_log_output("/home/lcom/labs/lab3/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (kbd_test_scan)() {
    subscribe_ints(KEYBOARD);
    do {
        if (request_message())
            continue;
        if (received_message(KEYBOARD)) {
            keyboard_int_handler();
        }
    } while (keyboard_get_last_scancode() != ESC_BREAKCODE);
    unsubscribe_ints(KEYBOARD);
    return 0;
}

int (kbd_test_poll)() {
    do {
        keyboard_int_handler();
    } while (keyboard_get_last_scancode() != ESC_BREAKCODE);
    return 1;
}

int (kbd_test_timed_scan)(uint8_t n) {
    subscribe_ints(TIMER | KEYBOARD);
    uint8_t frequency = 60;
    if(timer_set_frequency(0, frequency))
        return 1;
    do {
        if (request_message())
            continue;
        if (received_message(TIMER)) {
            timer_int_handler();
        }
        if (received_message(KEYBOARD)) {
            keyboard_int_handler();
            timer_reset_counter();
        }
    } while (keyboard_get_last_scancode() != ESC_BREAKCODE && timer_get_counter() < frequency * n);
    unsubscribe_ints(TIMER | KEYBOARD);
    return 1;
}

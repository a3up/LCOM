#include <lcom/lcf.h>
#include "macros/error.h"

#include "interrupts.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"

/// Variables for interruption receiving
int ipc_status;
message msg;

/// IRQ sets of dispositives
uint8_t timer_irq_set;
uint8_t keyboard_irq_set;
uint8_t mouse_irq_set;

/// Requests message from driver
int request_message() {
    static int r;
    error((r = driver_receive(ANY, &msg, &ipc_status)), "Driver receive failed");
    return 0;
}

/// Verifies if an interruption from a specific dispositive happened
bool received_message(enum dispositive dispositive) {
    if (!(is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE))
        return false;
    switch (dispositive) {
        case TIMER:
            return msg.m_notify.interrupts & timer_irq_set;
        case KEYBOARD:
            return msg.m_notify.interrupts & keyboard_irq_set;
        case MOUSE:
            return msg.m_notify.interrupts & mouse_irq_set;
        default:
            return false;
    }
}

/// Subscribe interruptions of devices
int subscribe_ints(uint8_t dispositives) {
    if (dispositives & BIT(0))
        error(timer_subscribe_int(&timer_irq_set), "Error subscribing timer interruptions");
    if (dispositives & BIT(1))
        error(keyboard_subscribe_int(&keyboard_irq_set), "Error subscribing keyboard interruptions");
    if (dispositives & BIT(2))
        error(mouse_subscribe_int(&mouse_irq_set), "Error subscribing mouse interruptions");
    return 0;
}

/// Unsubscribe interruptions from devices
int unsubscribe_ints(uint8_t dispositives) {
    if (dispositives & BIT(1))
        error(keyboard_unsubscribe_int(&keyboard_irq_set), "Error unsubscribing keyboard interruptions");
    if (dispositives & BIT(0))
        error(timer_unsubscribe_int(), "Error unsubscribing timer interruptions");
    if (dispositives & BIT(2))
        error(mouse_unsubscribe_int(), "Error unsubscribing mouse interruptions");
    return 0;
}

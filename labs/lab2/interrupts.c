#include <lcom/lcf.h>
#include "interrupts.h"
#include "macros/error.h"

/// Variables for interruption receiving
int ipc_status;
message msg;

/// IRQ sets of dispositives
uint8_t timer_irq_set;

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
        default:
            return false;
    }
}

/// Subscribe interruptions of devices
int subscribe_ints() {
    error(timer_subscribe_int(&timer_irq_set), "Error subscribing timer interruptions");
    return 0;
}

/// Unsubscribe interruptions from devices
int unsubscribe_ints() {
    error(timer_unsubscribe_int(), "Error unsubscribing timer interruptions");
    return 0;
}

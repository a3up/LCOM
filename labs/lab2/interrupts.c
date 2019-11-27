#include <lcom/lcf.h>
#include "interrupts.h"
#include "macros/error.h"

int r;
int ipc_status;
message msg;

uint8_t timer_irq_set;

int request_message() {
    error((r = driver_receive(ANY, &msg, &ipc_status)), "Driver receive failed");
    return 0;
}

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

int subscribe_ints(){
    return timer_subscribe_int(&timer_irq_set);
}

int unsubscribe_ints(){
    return timer_unsubscribe_int();
}

#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <stdbool.h>
#include <stdint.h>

#include "interrupts.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    // lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    // lcf_log_output("/home/lcom/labs/lab2/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
    uint8_t status;
    timer_get_status(timer, &status);
    timer_display_conf(timer, status, field);
    return 0;
}

int (timer_test_time_base)(uint8_t timer, uint32_t freq) {
    return timer_set_frequency(timer, freq);
}

int (timer_test_int)(uint8_t time) {
    subscribe_ints();
    uint8_t frequency = 60;
    timer_set_frequency(0, frequency);
    while (timer_get_counter() < frequency * time) {
        if (request_message())
            continue;
        if (received_message(TIMER)) {
            timer_int_handler();
            if(timer_get_counter() % frequency == 0)
                timer_print_elapsed_time();
        }
    }
    unsubscribe_ints();
    return 0;
}

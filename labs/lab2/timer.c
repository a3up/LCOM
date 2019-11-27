#include <lcom/lcf.h>
#include <stdbool.h>
#include "timer.h"
#include "macros/timer.h"
#include "macros/error.h"

uint8_t byte(void *pointer, uint8_t b) {
    uint8_t *initial = pointer;
    return *(initial + b);
}

int read_back_command(uint8_t command) {
    error(sys_outb(TIMER_CTRL, RB_COMMAND | command), "Error writing read-back command in timer control");
    return 0;
}

int control_word_command(uint8_t timer, uint8_t command) {
    error(sys_outb(TIMER_CTRL, (timer << 6) | command), "Error writing control word to timer control");
    return 0;
}

/// --------------------------------------------------------------------------------
///                                   Information
/// --------------------------------------------------------------------------------

enum timer_init_mode {
    INVALID_ACCESS_TYPE, LSB, MSB, LSB_followed_by_MSB
};

enum timer_base {
    BINARY, DECIMAL
};

struct timer_info {
    bool output;
    bool null_count;
    enum timer_init_mode init_mode;
    uint8_t prog_mode;
    enum timer_base base;
    uint8_t status;
};

int timer_get_status(uint8_t timer, uint8_t *status) {
    read_back_command(RB_COUNT | RB_TIMER(timer));
    uint32_t aux;
    error(sys_inb(TIMER_0 + timer, &aux), "Error reading status byte from timer");
    *status = (uint8_t) aux;
    return 0;
}

int timer_get_info(uint8_t timer, struct timer_info *info) {
    uint8_t status_byte;
    if (timer_get_status(timer, &status_byte))
        return 1;
    info->output = status_byte & ST_OUTPUT;
    info->null_count = status_byte & ST_NULL_COUNT;
    info->init_mode = (status_byte & ST_INIT_MODE) >> 4;
    info->prog_mode = (status_byte & ST_PROG_MODE) >> 1;
    if (info->prog_mode > 5)
        info->prog_mode -= 4;
    info->base = status_byte & ST_BCD;
    info->status = status_byte;
    return 0;
}

void timer_print_info(struct timer_info *info) {
    printf("output     : %s\n", (info->output) ? "TRUE" : "FALSE");
    printf("null_count : %s\n", (info->null_count) ? "TRUE" : "FALSE");
    printf("init_mode  : ");
    switch (info->init_mode) {
        case INVALID_ACCESS_TYPE:
            printf("INVALID\n");
            break;
        case LSB:
            printf("LSB\n");
            break;
        case MSB:
            printf("MSB\n");
            break;
        case LSB_followed_by_MSB:
            printf("LSB_MSB\n");
            break;
    }
    printf("prog_mode  : 0x%x\n", info->prog_mode);
    printf("base       : %s\n", (info->base) ? "DECIMAL" : "BINARY");
    printf("status_byte: 0x%x\n", info->status);
}

/// --------------------------------------------------------------------------------
///                                     Settings
/// --------------------------------------------------------------------------------

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
    error(freq > TIMER_FREQ || freq == 0, "Invalid frequency");
    uint8_t status;
    if (timer_get_status(timer, &status))
        return 1;
    status &= ST_PROG_MODE | ST_BCD;
    if (control_word_command(timer, LSB_MSB_MODE | status))
        return 1;
    uint32_t ini_val = TIMER_FREQ / freq;
    error(ini_val > 0xffff, "Invalid initial value");
    error(sys_outb(TIMER_0 + timer, byte(&ini_val, 0)), "Error writing LSB to timer");
    error(sys_outb(TIMER_0 + timer, byte(&ini_val, 1)), "Error writing MSB to timer");
    return 0;
}

/// --------------------------------------------------------------------------------
///                                  Interruptions
/// --------------------------------------------------------------------------------

static int hook_id = TIMER_IRQ;

int (timer_subscribe_int)(uint8_t *bit_no) {
    *bit_no = BIT(hook_id);
    error(sys_irqsetpolicy(TIMER_IRQ, IRQ_REENABLE, &hook_id), "Error on timer interruptions subscription");
    return 0;
}

int (timer_unsubscribe_int)() {
    error(sys_irqrmpolicy(&hook_id), "Error on timer interruptions unsubscription");
    return 0;
}

static unsigned timer_counter = 0;

unsigned timer_get_counter() {
    return timer_counter;
}

void (timer_int_handler)() {
    timer_counter++;
}

/// --------------------------------------------------------------------------------
///                            Functions only for the lab
/// --------------------------------------------------------------------------------

union timer_status_field_val timer_isolate_info(struct timer_info *info, enum timer_status_field field) {
    union timer_status_field_val ret;
    switch (field) {
        case tsf_all:
            ret.byte = info->status;
            break;
        case tsf_initial:
            ret.in_mode = (enum timer_init) info->init_mode;
            break;
        case tsf_mode:
            ret.count_mode = info->prog_mode;
            break;
        case tsf_base:
            ret.bcd = info->base;
            break;
    }
    return ret;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
    struct timer_info info;
    timer_get_info(timer, &info);
    union timer_status_field_val val = timer_isolate_info(&info, field);
    timer_print_config(timer, field, val);
    return 0;
}
